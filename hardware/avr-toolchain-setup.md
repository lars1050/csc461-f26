# Installing avr-gcc and avrdude (Standalone, No Arduino IDE Required)

This sets up the AVR compiler toolchain (avr-gcc + avr-libc) and avrdude (the upload/flashing tool) independently of the Arduino IDE. This avoids the Arduino IDE's internal version-numbered folder paths, which can change silently on IDE updates.

---

## macOS

The simplest path is Homebrew, which packages avr-gcc, avr-libc, and avrdude together.

```bash
brew tap osx-cross/avr
brew install avr-gcc avrdude
```

Homebrew installs avr-libc automatically as a dependency of avr-gcc — no separate library step needed.

**Verify the install:**

```bash
avr-gcc --version
avrdude -v
```

**No PATH editing needed** — Homebrew's bin directory is already on your PATH if Homebrew is installed:
- Apple Silicon: `/opt/homebrew/bin`
- Intel: `/usr/local/bin`

**Headers / avr-libc location:**
- Apple Silicon: `/opt/homebrew/avr/include/avr/io.h`
- Intel: `/usr/local/avr/include/avr/io.h`

---

## Windows

### Step 1 — Install the compiler + avr-libc

Microchip's official "AVR 8-Bit Toolchain for Windows" bundles avr-gcc, binutils, and avr-libc together in one zip.

1. Download the "AVR 8-Bit Toolchain for Windows" from Microchip's website (https://www.microchip.com/en-us/tools-resources/develop/microchip-studio/gcc-compilers). It's a zip file, not an installer.

2. Extract it to a simple path with no spaces. Do NOT place it on the cloud.
   ```
   C:\avr8-gnu-toolchain
   ```

### Step 2 — Install avrdude separately

Microchip's toolchain does not include avrdude, which is the app that flashes the program onto the Metro board, so install it on its own:

1. Go to the official avrdude GitHub releases page (https://github.com/avrdudes/avrdude/releases).
2. Scroll down to the Assets list and download avrdude-v8.2-windows-x64.zip
3. Extract it to a folder, e.g.:
   ```
   C:\avrdude
   ```

### Step 3 — Add both to PATH

1. Press Start, search **"Environment Variables"**, open **"Edit the system environment variables"**.
2. Click **Environment Variables...**
3. Under **User variables**, select `Path` → **Edit** → **New**, and add both (this is an example -- make sure you check the path on your machine):
   ```
   C:\avr8-gnu-toolchain\bin
   C:\avrdude
   ```
4. Click OK to close all dialogs.
5. **Restart any open terminal or Git Bash windows** — PATH changes don't apply to already-open shells.

### Step 4 — Verify the install

In Git Bash or PowerShell:

```bash
avr-gcc --version
avrdude -v
```

### avrdude config file note

The Windows avrdude zip usually includes `avrdude.conf` in the same folder as `avrdude.exe`, and it should be found automatically. If you get a "config file not found" error, point to it explicitly:

```bash
avrdude -C "C:\avrdude\avrdude.conf" -c arduino -p atmega328p -P COM3 -b 115200 -U flash:w:program.hex:i
```

## Install make on Windows (it should already be installed on Mac)

`winget install GnuWin32.Make` in your PowerShell.

Add the path `C:\Program Files (x86)\GnuWin32\bin` to the environment variable.

Reopen a shell and make sure it works: `make --version`

