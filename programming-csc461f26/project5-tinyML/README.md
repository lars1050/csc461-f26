## TinyML Swipe Classifier: class data collection

Two photocells on an Adafruit Metro 328 record a hand gesture as a 1-second
window of readings. The class collects **swipe left**, **swipe right** and
**hover** windows at several lighting stations, then trains a decision tree and a
tiny neural network on the combined dataset.

Data is collected entirely in the **Arduino IDE Serial Monitor**. Every student
keeps one plain-text file, `<username>.csv` (your Augsburg username), pastes each
block of recordings into it, and pushes it to the shared gitlab repo. All data will be  combined into a single dataset used to train the decision tree and ANN.

## Recording at a station (students)

1. **Run sensor_test/sensor_test.ino** to ensure the photocells are functional.
   
3. **Upload the sketch.** Open `swipe_logger/swipe_logger.ino` in the Arduino IDE, choose board **Arduino Uno** and the station's port, and upload.

4. **Set up the Serial Monitor:** 9600 baud, line ending **New Line**, and timestamps **off** (clock icon).
   You should see `# swipe_logger v3`.
   
5. **Start a block** by typing, then pressing Enter:

   ```
   set larsonam S2 right
   ```

   That's your username, the station ID, and the gesture (`left`, `right`, or `hover`).
   
6. **Make the gesture.** The board describes the window it saw, for example
   `# window: dip L=212 R=198, darkest first: L`. Type `k` to keep it or `d` to drop it
   (drop only if you made a mistake). Repeat until the board says `# block done`.
   
7. **Save the block.** Click the copy icon, paste at the **end** of your `jsmith.csv`, save, then
   **clear the Serial Monitor** so the next copy doesn't repeat this block.
   
8. Send the next `set` command for the next block.

**Your file must be plain text.**

- Windows Notepad: Save as type **All files**, name it `jsmith.csv`.
- Mac TextEdit: **Format > Make Plain Text** before saving.

The `#` lines can stay in the file; the tools skip them.

## Submitting

Once you have a sample csv file with 1 block per gesture (i.e. 10 rows of sensor readings per gesture), push to the shared gitlab repo https://gitlab.com/augsburg/csc461f26/proj5-tinyml-data.

