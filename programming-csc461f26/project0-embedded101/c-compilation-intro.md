# Introduction to Compiling C (for Java/Python Programmers)

If you're coming from Java or Python, C's build process will feel unfamiliar at first. Java compiles to bytecode that runs on the JVM, and Python doesn't compile at all in the way you're used to thinking about it — it's interpreted line by line as the program runs. C is different: it compiles directly to machine code your processor executes natively, with no runtime environment sitting underneath it. That difference is why C requires more explicit organization from you than Java or Python do.

This guide walks through the pieces you'll need to understand: how a C program is organized into files, what the compiler actually does to those files, and the tools (avr-gcc and Makefiles) you'll use to build programs for AVR microcontrollers.

---

## 1. Program Structure: Header and Source Files

### Source files (`.c`)

A `.c` file contains actual C code — function definitions, variable definitions, the logic of your program. This is roughly analogous to a `.java` file or a `.py` module: it's where the "real work" lives.

### Header files (`.h`)

A header file typically contains **declarations**, not full definitions: function prototypes, `#define` constants, and type definitions that other files need to know about. Think of a header as a "table of contents" or interface — it tells other files *what exists* without providing the actual implementation.

**Why does C split things this way, when Java doesn't?**

In Java, the compiler can look at any class file in your project and figure out what's available. C's compiler is much simpler and only looks at one file at a time — it has no built-in concept of a "project." If `main.c` wants to call a function defined in `math_utils.c`, the compiler processing `main.c` has no way to know that function exists unless something tells it first. Header files are that "something" — they let you declare a function's existence in one place and share that declaration across multiple `.c` files.

**Typical pattern:**

```c
// math_utils.h  (declaration only)
int add(int a, int b);
```

```c
// math_utils.c  (the actual definition)
#include "math_utils.h"

int add(int a, int b) {
    return a + b;
}
```

```c
// main.c
#include "math_utils.h"

int main(void) {
    int result = add(2, 3);
    return 0;
}
```

Notice `main.c` never sees `math_utils.c` directly — it only includes the header. The connection between the *declaration* the compiler sees and the *definition* that actually exists somewhere is resolved later, during linking (more on that below).

---

## 2. Preprocessing Directives

Before your code is compiled, it passes through the **preprocessor** — a separate step that does simple text substitution and file inclusion, with no real understanding of C syntax. Anything starting with `#` is a preprocessor directive, not a normal C statement (notice: no semicolon at the end of these lines).

Common directives you'll encounter:

- **`#include`** — literally pastes the contents of another file in at that location. `#include "myheader.h"` (quotes) looks in your project folder first; `#include <stdio.h>` (angle brackets) looks in the standard system library locations.
- **`#define`** — defines a constant or macro via text substitution. `#define MAX_SIZE 100` replaces every later occurrence of `MAX_SIZE` with `100` before compilation even starts.
- **`#ifdef` / `#ifndef` / `#endif`** — conditionally includes or excludes code, often used for platform-specific code or debug builds.

A very common pattern you'll see at the top of nearly every header file is an **include guard**:

```c
#ifndef MATH_UTILS_H
#define MATH_UTILS_H

// declarations here

#endif
```

This prevents a header from being pasted into the same file twice (which can happen if multiple included files each include the same header), which would otherwise cause "duplicate definition" errors.

---

## 3. Order Matters

This is one of the biggest adjustments coming from Java. In Java, you can call a method defined later in the same class, or in a class defined later in your project — the compiler resolves everything regardless of order. **C reads top to bottom, and generally will not let you use something before it has been declared.**

```c
int main(void) {
    greet();   // ERROR: greet() hasn't been declared yet
    return 0;
}

void greet(void) {
    printf("hello\n");
}
```

This is why C programs rely heavily on **declarations** near the top of a file (or in a header) even when the actual **definition** comes later:

```c
void greet(void);   // declaration — "this function exists, trust me"

int main(void) {
    greet();         // fine now — compiler already knows greet() exists
    return 0;
}

void greet(void) {   // definition — the actual implementation
    printf("hello\n");
}
```

A **declaration** tells the compiler a function or variable's name and type exist somewhere. A **definition** actually provides the implementation (for a function) or allocates storage (for a variable). Organizing your file with declarations up top is less about strict necessity and more about readability and flexibility — it lets you write `main` near the top of a file (where a reader expects to find the entry point) even though it depends on functions defined further down.

---

## 4. The Necessity of `main`

Every C program (running under an operating system, which is the case for anything on your laptop, though not necessarily for embedded/AVR code — see below) must have exactly one function named `main`. This is the required entry point — the C runtime startup code specifically looks for and calls a function with this exact name. Unlike Java, where the entry point is `public static void main(String[] args)` inside *some* class you specify when running the program, C's `main` isn't attached to a class and there's no ambiguity about which one runs — there can only be one in the whole program.

Standard forms:

```c
int main(void) { ... }
int main(int argc, char *argv[]) { ... }
```

Note that `main` returns an `int`, conventionally `0` for success and nonzero for an error — this is a convention your Java background doesn't have a direct equivalent for, since Java programs don't return an exit code from `main` in the same way (though the JVM does have `System.exit(code)`).

**A note for AVR/embedded work:** microcontroller programs are "freestanding," not "hosted" — there's no operating system to hand control to `main`. The C standard allows freestanding environments to define their own entry point conventions. In the Arduino ecosystem specifically, there actually is still a hidden `main()` under the hood (inside the Arduino core library) that calls your `setup()` once and then `loop()` repeatedly — so even though you never type `main`, the requirement is still being satisfied for you behind the scenes.

---

## 5. Inline Function Definitions

You may see functions marked `inline`:

```c
inline int square(int x) {
    return x * x;
}
```

This is a *hint* to the compiler suggesting it should insert the function's actual code directly at each call site, rather than performing a normal function call (which has a small overhead: jumping to the function, pushing arguments, jumping back). This can make small, frequently-called functions faster, at the cost of a slightly larger compiled program (since the code gets duplicated at every call site instead of existing once).

A few things worth knowing:
- The compiler is free to ignore the `inline` hint if it decides not to honor it — it's a suggestion, not a command.
- This matters more in C than you might expect from Java, since the JVM's just-in-time compiler makes these kinds of decisions dynamically at runtime; in C, this decision is made once, at compile time, and can't adapt later.
- On resource-constrained AVR chips, judicious use of `inline` (or its opposite — avoiding it) can matter for program size in a way it rarely does on a laptop-class program.

### An older alternative: `#define` as a one-line "function"

Before `inline` existed in C, and still commonly seen in embedded code today, simple one-line operations are often written as preprocessor macros instead of functions:

```c
#define SQUARE(x) ((x) * (x))
```

Because `#define` is handled entirely by the preprocessor (see Section 2), this is pure text substitution, done *before* the compiler ever sees real C syntax. Every occurrence of `SQUARE(x)` in your code is replaced, literally, with `((x) * (x))`. There's no function call at all — not even the possibility of one, unlike `inline`, where a real function still exists and the compiler merely *chooses* to expand it inline.

```c
int result = SQUARE(5);
// after preprocessing, this line becomes:
int result = ((5) * (5));
```

**Why the extra parentheses everywhere?** Since this is blind text substitution, not real code, forgetting them causes surprising bugs from normal operator precedence:

```c
#define SQUARE(x) x * x        // missing parens -- dangerous

int result = SQUARE(2 + 3);
// becomes:  2 + 3 * 2 + 3   =  11, NOT 25!
```

**A second pitfall with macros that `inline` functions don't have:** because the substitution is textual, an argument used more than once gets *evaluated* more than once:

```c
#define SQUARE(x) ((x) * (x))

int i = 5;
int result = SQUARE(i++);
// becomes:  ((i++) * (i++))  -- i gets incremented twice, not once!
```

An `inline` (or ordinary) function doesn't have this problem, since `i++` would be evaluated once, its result passed in as a normal parameter.

**When you'll see each one used in practice:** `#define` macros remain common in embedded/AVR code for very small, chip-specific bit manipulations (e.g. setting a single register bit) where you want to guarantee zero function-call overhead even without optimization enabled. `inline` functions are generally preferred when you have the choice, since they get real type checking from the compiler (macros get none — `SQUARE("hello")` would "compile," nonsensically, since the preprocessor doesn't know what a string is) and don't suffer from the double-evaluation problem above.

---

## 6. Global Variables

A global variable is declared outside of any function, making it accessible from anywhere in that file (and, with the right declarations, from other files too):

```c
int counter = 0;   // global — visible to every function below it in this file

void increment(void) {
    counter++;      // no need to pass counter as a parameter
}
```

This is a bigger deal in C than in Java, for a specific reason: **Java doesn't really have true global variables** — anything similar is a `static` field on some class, which is still namespaced and accessed through that class. C's globals are genuinely unscoped within the file (or program) they're visible in, with no enclosing class or object required.

A few practical notes:
- Globals are convenient for state that many functions need (like a running counter, or a hardware register your whole program interacts with), but overusing them makes programs harder to reason about, since any function can silently change global state — there's no equivalent of Java's access modifiers protecting it by default.
- To share a global variable across multiple `.c` files, you declare it as `extern` in a header and define it (without `extern`) in exactly one `.c` file:

```c
// shared.h
extern int counter;   // declaration: "this exists somewhere"
```

```c
// shared.c
int counter = 0;       // definition: "here it actually lives"
```

- In embedded/AVR programming specifically, global variables are common for representing hardware state (like a flag set inside an interrupt handler) precisely because functions triggered asynchronously (interrupts) need a way to communicate with the rest of the program without formal parameters/return values.

---

## 7. avr-gcc and Common Flags

`avr-gcc` is a cross-compiler: it runs on your laptop but produces machine code for the AVR microcontroller's instruction set, not your laptop's own processor. This is different from a normal `gcc` invocation on your Mac or Windows machine, which compiles code *for* the machine it's running *on*.

A typical compile command looks like:

```bash
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -o program.elf main.c
```

**Common flags, explained:**

| Flag | Meaning |
|---|---|
| `-mmcu=atmega328p` | Tells the compiler which specific AVR chip you're targeting, since instruction details and available registers vary by chip. |
| `-DF_CPU=16000000UL` | Defines a preprocessor macro (`F_CPU`) specifying the clock speed (16 MHz here), which timing-related library code needs to calculate delays correctly. |
| `-Os` | Optimize for size rather than speed — important on AVR chips, which have very limited flash memory (often 32 KB or less). |
| `-o program.elf` | Sets the name of the output file. `.elf` is a common intermediate output format for embedded targets, which then gets converted to `.hex` for actual flashing. |
| `-Wall` | Enables most common compiler warnings — worth adding to every command, since it catches many beginner mistakes (like comparing signed and unsigned values, or unused variables). |
| `-c` | Compile only — produces a `.o` object file without attempting to link it into a complete program. Used when compiling multiple `.c` files separately before linking them together. |

**A typical two-step process for multiple source files:**

```bash
avr-gcc -mmcu=atmega328p -DF_CPU=16000000UL -Os -c main.c -o main.o
avr-gcc -mmcu=atmega328p main.o -o program.elf
```

The first command compiles `main.c` into an object file without linking. The second command **links** — this is the step that resolves all those declarations you saw earlier into actual addresses, matching every function/variable that was only *declared* in one file to where it's actually *defined* in another. Linking is also where you'd get an error if you declared something (in a header) but never actually defined it anywhere.

Finally, converting the compiled `.elf` to a `.hex` file suitable for flashing onto the chip via `avrdude`:

```bash
avr-objcopy -O ihex -R .eeprom program.elf program.hex
```

---

## 8. Makefiles

Typing out multi-flag `avr-gcc` commands by hand every time you change one file gets old fast — and it's error-prone, since it's easy to forget a flag or mistype a filename. A **Makefile** automates this: it's a script that describes how to build your program, and lets you rebuild everything (or just what changed) by typing a single command: `make`.

Here's the Makefile used for this course:

```makefile
# --- Configuration ---
# Serial port the board shows up on (Mac-style path w/ wildcard for the
# suffix, since it varies by cable). Override on the command line for a
# different OS/port, e.g.:  make flash PORT=COM3
PORT=/dev/tty.SLAB_USBtoUART*

MCU=atmega328p

# -g:               include debug symbols
# -Wall:            enable common compiler warnings
# -mcall-prologues: use library routines for function entry/exit (saves space)
# -mmcu:            target chip, needed for both compiling and linking
# -Os:              optimize for size (important on limited flash memory)
CFLAGS=-g -Wall -mcall-prologues -mmcu=$(MCU) -Os

# Strip unused sections and allow the linker to relax certain jumps to
# smaller instructions where possible -- both reduce final program size.
LDFLAGS=-Wl,-gc-sections -Wl,-relax

CC=avr-gcc
TARGET=main
OBJECT_FILES=main.o

# Targets that don't correspond to real output files
.PHONY: all clean flash

all: $(TARGET).hex

clean:
	rm -f *.o *.hex *.obj

# Compile: source (.c) -> object file (.o)
main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o

# Link: object file(s) (.o) -> linked binary (.obj)
%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@

# Convert: linked binary (.obj) -> flashable Intel HEX (.hex)
%.hex: %.obj
	avr-objcopy -R .eeprom -O ihex $< $@

flash: $(TARGET).hex
	avrdude -p $(MCU) -c arduino -P $(PORT) -U flash:w:$(TARGET).hex
```

**How to read this:**

- **Variables** (`PORT`, `MCU`, `CFLAGS`, `LDFLAGS`, `CC`, `TARGET`, `OBJECT_FILES`) work like constants — defined once at the top, referenced elsewhere with `$(...)`. This means changing chips, for example, only requires editing the `MCU=` line once, rather than hunting down every place it's used. Notice `F_CPU` isn't defined here at all — in this course's setup, clock speed is instead `#define`d directly inside `main.c`, so the Makefile doesn't need to know or pass it along.
- **`.PHONY`** tells `make` that `all`, `clean`, and `flash` are actions to run, not files it should expect to find on disk — without this, `make` could get confused if a file happened to be named `clean`.
- **Rules** follow the pattern:
  ```makefile
  target: dependencies
  	command
  ```
  A target is what you're trying to build; dependencies are the files it needs first; the command (indented with an actual **tab character**, not spaces — this trips up almost everyone at least once) is what actually builds it.
- This Makefile shows all **three build stages** explicitly, matching the compile/link/convert steps from Section 7:
  1. **`main.o: main.c`** — compiles your source file into an object file.
  2. **`%.obj: $(OBJECT_FILES)`** — links the object file(s) into `main.obj`. The `%` is a pattern rule, meaning "anything ending in `.obj`, built from whatever `OBJECT_FILES` lists."
  3. **`%.hex: %.obj`** — converts the linked binary into the Intel HEX format `avrdude` needs to flash onto the chip.
- **`$@`** and **`$<`** are automatic variables: `$@` means "the target of this rule" and `$<` means "the first dependency." In the hex rule, `$<` is `main.obj` and `$@` is `main.hex` — this lets one pattern rule work no matter what the actual filename is, instead of hardcoding `main` everywhere.
- `make` is smart about **only rebuilding what's changed** — if `main.c` hasn't been modified since `main.hex` was last built, running `make` again does nothing, saving time as a project grows. This is a meaningful difference from re-running a full compile command every time.
- `flash` is the upload step — it depends on `main.hex` (so `make flash` will build everything it needs first, automatically) and then calls `avrdude` to upload it, using the `PORT` and `MCU` variables from the top of the file.

**Running it:**

```bash
make            # builds main.hex (the "all" target, first in the file, so it's the default)
make flash      # builds main.hex if needed, then flashes it to the board
make clean      # removes all build artifacts (.o, .obj, .hex)
```

**One thing to watch for on Windows:** the `PORT` variable above uses a Mac-style path. On Windows (Git Bash), your board will show up as `COMx` instead, so override it directly on the command line rather than editing the file:

```bash
make flash PORT=COM3
```

---

### Looking ahead: linking multiple source files

Everything above assumes one source file (`main.c`). You won't need this yet, but it's worth seeing now as a preview: real programs are usually split across several `.c` files (recall the `math_utils.c` / `main.c` example from Section 1), and the Makefile needs small, predictable changes to handle that.

The version below (three files: `main.c`, `math_utils.c`, `uart.c` — say, a small helper module for serial communication) shows the pattern most real Makefiles actually use. Rather than writing out a separate rule for every single file by hand (which gets tedious and error-prone as a project grows to dozens of files), it uses a **pattern rule** plus a bit of variable substitution to generate the list of object files automatically from the list of source files:

```makefile
# List source files once, here.
SOURCES=main.c math_utils.c uart.c

# Automatically derive the object file list from SOURCES by
# swapping the .c extension for .o -- e.g. main.c -> main.o
OBJECT_FILES=$(SOURCES:.c=.o)

# Pattern rule: compiles ANY .c file into a matching .o file.
# $< = the source file that triggered this rule (e.g. main.c)
# $@ = the target being built (e.g. main.o)
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link: combine every object file listed in OBJECT_FILES into one binary
%.obj: $(OBJECT_FILES)
	$(CC) $(CFLAGS) $(OBJECT_FILES) $(LDFLAGS) -o $@
```

**What actually changed, and what didn't:**

- **`SOURCES`** is the one place you now list your `.c` files. Add a new file to the project, and this is the only line you touch.
- **`OBJECT_FILES=$(SOURCES:.c=.o)`** is a *substitution reference* — Make-speak for "take the `SOURCES` list and replace every `.c` with `.o`." This automatically produces `main.o math_utils.o uart.o` without you typing any of those three names out. Add a fourth source file to `SOURCES`, and its `.o` is included here automatically too.
- **The single pattern rule `%.o: %.c`** replaces what would otherwise be one hand-written rule per file (`main.o: main.c`, `math_utils.o: math_utils.c`, `uart.o: uart.c`, ...). The `%` matches any filename stem, so this one rule handles all three files — and every file you add later — without needing a new rule at all. This is the idiomatic Make pattern, and the reason our earlier explicit `main.o: main.c` rule was slightly atypical: it was written that way specifically so the compile step wasn't hidden while there was only one file to look at.
- **The link rule (`%.obj: $(OBJECT_FILES)`) needed zero edits**, because it already referenced the `OBJECT_FILES` variable rather than hardcoding `main.o` — exactly why the original Makefile was written that way.
- **Everything else** — `flash`, `clean`, the `.hex` conversion rule, `CFLAGS`, `MCU`, `PORT` — stays completely unchanged, since those steps only care about the final linked binary, not how many source files went into it.

**The trade-off worth knowing about:** the generic pattern rule above doesn't know that `main.c` depends on `math_utils.h` and `uart.h` (via `#include`). That means if you edit a header file only — without touching any `.c` file — `make` won't realize anything needs recompiling, since as far as it can see, no `.c` file changed. Larger C projects typically solve this with **automatic dependency generation** (compiler flags like `-MMD -MP` that make `gcc`/`avr-gcc` write out a `.d` file recording each source's header dependencies, which the Makefile then includes). That's a level of polish beyond what you need for now — the practical workaround, if you edit a header and something seems stale, is simply `make clean` before rebuilding.

The broader lesson: a Makefile scales to bigger projects primarily by adding filenames to a variable, not by rewriting rules — which is exactly the kind of repetitive-but-mechanical work `make`'s pattern rules are meant to save you from doing by hand.
