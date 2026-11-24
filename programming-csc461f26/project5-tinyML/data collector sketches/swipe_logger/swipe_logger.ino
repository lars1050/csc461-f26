// swipe_logger.ino (v3) -- Metro 328, photocells on A0 (left) and A1 (right).
// Records labeled windows straight from the Arduino IDE Serial Monitor.
//
// Serial Monitor settings: 115200 baud, line ending "New Line", timestamps OFF.
//
// 1. Type:  set <username> <station> <label>      e.g.  set jsmith S2 right
//    (label is left, right, hover or other)
// 2. Make the gesture. The board describes the window it saw;
//    type k to keep it or d to drop it.
// 3. After 10 kept windows the block is done. Click the copy icon, paste at the
//    end of your <username>.csv file, clear the monitor, and send the next set command.
//
// Kept windows print as CSV rows:
//   username,station,label,trial,baseL,baseR,<50 L samples>,<50 R samples>
// Every other line starts with '#', and the class tools skip those lines.
// Do not change the settings below: every station must record with the same code.

#include <ctype.h>
#include <string.h>

// photocell pins
const uint8_t PIN_L = A0; 
const uint8_t PIN_R = A1;

/*
WINDOW 
1 second of data from each photocell. At 50 Hz, this is 50 samples per cell.
These 100 samples are displayed in the serial monitor on a single row.

BLOCK
10 samples (windows) of the same gesture.
Copy these 10 rows (using Arduino copy icon in serial monitor) and pasted into csv.

User is prompted when ready for a gesture.
User is prompted after the gesture to confirm it was a good sample.

After performing 10 gestures in a block, reset monitor and collect for another gesture.
*/

// sampling settings. 50 Hz (20ms). 1 second (50 (N) samples).
// constant sampling into a ring buffer that keeps most recent samples
const uint16_t PERIOD_MS = 20;
const uint8_t  N = 50;    
     
// ring buffer saves 10 pre-samples prior to trigger and collects 40 samples after
const uint8_t  PRE   = 10;

// system is triggered for sampling when one cell gets shaded
// shaded = MAX(10% below normal light, cell reading 15 below normal)
const float    DROP_FRAC = 0.10; 
const int      MIN_DROP  = 15;
     
const uint8_t  QUIET = 10;      // samples of normal light needed to re-arm (0.2 s)

// block is number of samples per gesture
const uint8_t  BLOCK = 10;
 
// ring buffer for left, right photocell. 200 bytes total. head is next write position
int bufL[N];
int bufR[N];
uint8_t head = 0;

// base line readings for normal lighting conditions
float baseL;
float baseR;

/* recording status. system is triggered when one sensor shaded
- active: in the middle of a block (collecting 10 samples) 
- capturing: collecting a window of data for a single gesture
- waiting: waiting for user to respond with keep or discard gesture data
- armed: sensors reading normal and ready to be triggered.
*/
bool active = false;
bool capturing = false; 
bool waiting = false;
bool armed = false;

// counters for remaining samples to collect in a window
uint8_t remaining = 0;

// counter for number of readings of normal light (to know when it is armed)
uint8_t quiet = 0;

// counter for how many of 10 samples recorded so far
uint8_t kept = 0;

// timer expiration
unsigned long nextSample;

// information about user and station
char user[21] = "", station[6] = "", label[6] = "";
char line[48];
uint8_t lineLen = 0;
unsigned long lastCharMs = 0;


bool isDark(int v, float base) {
	// v: current reading
	// base: normal lighting
	
	// shadow detected if 10% drop OR MIN_DROP in current reading if 10% too small
	float need = base * DROP_FRAC;
	if (need < MIN_DROP) need = MIN_DROP;
	return (base - v) > need;
}

bool validWord(const char *s, uint8_t maxLen) {
  uint8_t n = strlen(s);
  if (n == 0 || n > maxLen) return false;
  for (uint8_t i = 0; i < n; i++)
    if (!isalnum(s[i]) && s[i] != '.' && s[i] != '_' && s[i] != '-') return false;
  return true;
}

void help() {
  Serial.println(F("# Type: set <username> <station> <label>   e.g. set jsmith S2 right"));
  Serial.println(F("# Labels: left, right, hover, other"));
}

void prompt() {
  Serial.print(F("# ")); Serial.print(user);
  Serial.print(F(" at ")); Serial.print(station);
  Serial.print(F(": ")); Serial.print(label);
  Serial.print(' '); Serial.print(kept + 1); Serial.print('/'); Serial.print(BLOCK);
  Serial.println(F(" - go"));
}

void describeWindow() {
  uint8_t iL = 0, iR = 0;
  int minL = 1023, minR = 1023;
  for (uint8_t i = 0; i < N; i++) {
    int l = bufL[(head + i) % N], r = bufR[(head + i) % N];
    if (l < minL) { minL = l; iL = i; }
    if (r < minR) { minR = r; iR = i; }
  }
  Serial.print(F("# window: dip L=")); Serial.print((int)baseL - minL);
  Serial.print(F(" R=")); Serial.print((int)baseR - minR);
  Serial.print(F(", darkest first: "));
  if (abs((int)iL - (int)iR) <= 1) Serial.print(F("both"));   // hover: both go dark together
  else Serial.print(iL < iR ? 'L' : 'R');
  Serial.println(F("   -> type k (keep) or d (drop)"));
}

void printRow() {
  Serial.print(user);    Serial.print(',');
  Serial.print(station); Serial.print(',');
  Serial.print(label);   Serial.print(',');
  Serial.print(kept);
  Serial.print(','); Serial.print((int)baseL);
  Serial.print(','); Serial.print((int)baseR);
  for (uint8_t i = 0; i < N; i++) { Serial.print(','); Serial.print(bufL[(head + i) % N]); }
  for (uint8_t i = 0; i < N; i++) { Serial.print(','); Serial.print(bufR[(head + i) % N]); }
  Serial.println();
}

void finishWindow() {
  waiting = false;
  armed = false;                   // wait for the hand to go away again
  quiet = 0;
  if (kept >= BLOCK) {
    active = false;
    Serial.print(F("# block done: ")); Serial.print(kept); Serial.print(' ');
    Serial.print(label); Serial.println(F(" windows kept."));
    Serial.println(F("# Copy everything into your file, clear the monitor, then send the next set command."));
  } else {
    prompt();
  }
}

void handleCommand(char *cmd) {
  for (char *p = cmd; *p; p++) *p = tolower(*p);
  Serial.print(F("# > ")); Serial.println(cmd);      // echo, so the pasted log shows what was typed

  if (waiting) {
    if (strcmp(cmd, "k") == 0) { kept++; printRow(); finishWindow(); }
    else if (strcmp(cmd, "d") == 0) { Serial.println(F("# dropped")); finishWindow(); }
    else Serial.println(F("# type k (keep) or d (drop)"));
    return;
  }

  if (strncmp(cmd, "set ", 4) == 0) {
    char *u = strtok(cmd + 4, " ");
    char *s = strtok(NULL, " ");
    char *l = strtok(NULL, " ");
    char *extra = strtok(NULL, " ");
    if (!u || !s || !l || extra) {
      Serial.println(F("# need exactly 3 words after set: set <username> <station> <label>"));
      return;
    }
    if (!validWord(u, 20)) { Serial.println(F("# username: letters, digits, . _ - only (max 20)")); return; }
    if (!validWord(s, 5))  { Serial.println(F("# station should look like S2")); return; }
    if (strcmp(l, "left") && strcmp(l, "right") && strcmp(l, "hover") && strcmp(l, "other")) {
      Serial.println(F("# label must be left, right, hover or other"));
      return;
    }
    strcpy(user, u);
    strcpy(station, s);
    station[0] = toupper(station[0]);                // S2, S2a, ...
    strcpy(label, l);
    kept = 0;
    active = true;
    capturing = false;
    armed = false;
    quiet = 0;
    prompt();
    return;
  }

  if (strcmp(cmd, "k") == 0 || strcmp(cmd, "d") == 0) {
    if (active) Serial.println(F("# nothing to keep or drop yet: make the gesture first"));
    else Serial.println(F("# no block running: send a set command first"));
    return;
  }
  if (strcmp(cmd, "?") == 0 || strcmp(cmd, "help") == 0) { help(); return; }
  Serial.println(F("# unknown command"));
  help();
}

void readSerial() {
  while (Serial.available()) {
    char c = Serial.read();
    lastCharMs = millis();
    if (c == '\r') continue;
    if (c == '\n') {
      line[lineLen] = '\0';
      if (lineLen) handleCommand(line);
      lineLen = 0;
    } else if (lineLen < sizeof(line) - 1) {
      line[lineLen++] = c;
    }
  }
  // Also accept a command when the monitor sends no line ending.
  if (lineLen && millis() - lastCharMs > 200) {
    line[lineLen] = '\0';
    handleCommand(line);
    lineLen = 0;
  }
}

void setup() {
  Serial.begin(9600);
  baseL = analogRead(PIN_L);
  baseR = analogRead(PIN_R);
  nextSample = millis();
  Serial.println(F("# swipe_logger v3"));
  help();
}

void loop() {

	readSerial();
	if (waiting) return;           // hold the window until k or d
	
	// get ready for next sample
	unsigned long now = millis();

	// continue to next loop iteration
	if ((long)(now - nextSample) < 0) return;
	
	// ready for a sample
	nextSample += PERIOD_MS;
	if ((long)(now - nextSample) > 0) 
		nextSample = now;   // don't race to catch up after a pause

	// get a sample
	int l = analogRead(PIN_L);
	int r = analogRead(PIN_R);
	bufL[head] = l;
	bufR[head] = r;
	head = (head + 1) % N;

	if (!capturing) {
		// haven't started a gesture yet. is there a shadow?
		bool dark = isDark(l, baseL) || isDark(r, baseR);
		
		// no sign of a gesture yet
		if (!dark) {
			baseL += 0.02 * (l - baseL);  // update baselines only in normal light
			baseR += 0.02 * (r - baseR);
			if (++quiet >= QUIET) { quiet = QUIET; armed = true; }
		} else {
			quiet = 0;      // hand still there
		}
		
		// starting a gesture
		if (dark && armed && active) {
			capturing = true;
			armed = false;
			remaining = N - PRE;            // record the rest of the window
		}
		
	} else if (--remaining == 0) {
		// capturing. are we done recording for this window (single gesture)??
		capturing = false;
		waiting = true;
		describeWindow();
	}
}
