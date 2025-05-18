# RTOS for ATMega328P – Senior Project

This is my Senior Project focused on embedded systems using the ATMega328P. It implements a lightweight RTOS using FreeRTOS Kernel v11.1.0 with support for task scheduling, multitasking, and real-time I/O interaction. Task upload via UART and EEPROM/Flash-based task storage are available in dedicated branches, but the primary version used for testing and presentation is now in `RTOS_NoUpload`.

# RTOS_NoUpload – Current Testing Build

## 📌 Overview

This project is a **custom RTOS for the ATMega328P** based on **FreeRTOS v11.1.0**.
- Tick timer uses **TIMER1_COMPA_vect**
- **heap_4.c** is used for dynamic allocation and task buffer management

`RTOS_NoUpload.X` is a FreeRTOS-based firmware that runs predefined tasks:
* Blink LED on PB2 (1s cycle)
* Fade PWM output on PB3 (OC2A) via Timer2
* Read 4x4 numpad input using PC0–PC3 and PB6/PB7/PD5/PD6
* Send pressed key via UART
* Blink the Morse code of the pressed key on PB0
* UART Monitor on USART to log data

`RTOS.X` and `Bootloader.X` plans to adds the ability to:
- Accept precompiled **binary tasks** via UART.
- **Store and load tasks from Flash**, currently test with only EEPROM.
- Execute tasks dynamically using **FreeRTOS task scheduling**.
- Communicate with a **Python-based PC tool** for real-time task management.

## 📁 Project Structure

```
📁 RTOS-ATMega328P
├──📂 No_RTOS             # Minimal version without FreeRTOS (for baseline comparison)
├──📂 RTOS_NoUpload.X     # [Current Focus] Core RTOS with hardcoded tasks, no UART upload
│   ├── FreeRTOS/
│   │   ├── include/                 # FreeRTOS kernel headers
│   │   ├── portable/
│   │   │   ├── GCC/ATMega328/      # AVR port (port.c, portmacro.h)
│   │   │   └── MemMang/            # heap_4.c used for dynamic memory
│   │   ├── FreeRTOSConfig.h        # config file for RTOS
│   │   └── *.c                     # Core FreeRTOS kernel files
│   ├── nbproject/                  # MPLAB X project metadata
│   ├── build/                      # Auto-generated build directory
│   ├── dist/                       # Compiled .hex outputs
│   ├── main.c                      # Project entry point
│   ├── LICENSE
│   └── Makefile
├──📂 RTOS.X            # Full RTOS with UART upload and EEPROM+Flash task storage
│   └── (Similar to the RTOS_NoUpload.X)
├──📂 Bootloader.X      # Handles Flash storage and UART task upload
│   ├── main.c                   # Custom bootloader for receiving .tsk files
│   ├── flash_write.c            # Handles SPM Flash write logic
│   ├── serial_input.c           # Handles UART reception for bootloader
│   └── bootloader_upload.bat    # Batch file to flash the bootloader using USBasp
└── README.md                       # This file
```

## 🧠 Features

* FreeRTOS preemptive multitasking on ATmega328P
* Custom PWM fade task
* Numpad scan task + Morse LED response
* UART logging and monitoring
* Real-time cooperative task execution using queues

## ⚙️ Hardware Pinout

| Function       | ATmega328P Pin     | Port/Bit     |
| -------------- | ------------------ | ------------ |
| Blink LED      | Digital Pin 2      | PB2          |
| PWM Output     | Digital Pin 11     | PB3 (OC2A)   |
| Morse LED      | Digital Pin 8      | PB0          |
| UART TX        | Digital Pin 1      | PD1          |
| UART RX        | Digital Pin 0      | PD0          |
| Numpad Rows    | PB6, PB7, PD5, PD6 | Out          |
| Numpad Columns | PC0–PC3            | In (pull-up) |

## 📎 Tools

* USBasp programmer (flashing)
* FTDI232 for UART testing
* MPLAB X + XC8 (optional, for code edits)
* Optional Python script for logging (not included here)

## 🔧 Getting Started

1. Requirements

- **Flashing Utility:** AVRDUDE (for flashing firmware)
- **Compiler:** AVR-GCC 14.1.0 (If updated please change ver. in .bat)
- **Programmer:** USBasp programmer (for flashing ATMega328P)
- **Driver note:** For USBasp to function reliably, install the **libusbK** driver using [Zadig](https://zadig.akeo.ie/)
- **Task communication:** 
    - FTDI/USB-Serial converter hardware (FTDI232) (for UART communication)
    - PC Side Utility []
- **Optional** MPLAB X IDE with XC8 v2.50
    - Probably could be use with later version but didn't test
    - If you want to edit and rebuild the RTOS

2. The Fuse Settings

To configure the ATMega328P to run with the bootloader section and support 8 MHz internal clock:

```bash
avrdude -c usbasp -p m328p -U lfuse:w:0xe2:m -U hfuse:w:0xd8:m -U efuse:w:0xff:m
```

- `lfuse = 0xE2` → 8 MHz internal clock
- `hfuse = 0xD8` → Enable BOOTRST, 2 KB bootloader size
- `efuse = 0xFF` → Default

Check current fuse settings with

```bash
avrdude -c usbasp -p m328p -U lfuse:r:-:h -U hfuse:r:-:h -U efuse:r:-:h
```

3. **Compile and Upload**

   Option 1: Easy Setup
   * Open `RTOS_NoUpload.X` folder
   * Run `post_build_noup.bat`
   
   Option 2: Ready to Develop
   * Open `RTOS_NoUpload.X` in MPLAB X IDE
   * Ensure compiler is **XC8 v2.50**
   * Build and upload `.hex` using:
     ```bash
     avrdude -c usbasp -p m328p -B 10 -U flash:w:dist/default/production/RTOS_NoUpload.X.production.hex:i
     ```
   or
   * Setting the execute this line after setup
     ```bash
     call post_build_noup.bat 
     ```

5. **UART Monitor**

   * Baud: `9600`
   * Monitor all system output via FTDI232 or any serial tool

## 🔬 Current RTOS Tasks

| Task           | Description                       |
| -------------- | --------------------------------- |
| `vBlinkTask`   | LED blinking on PB2 at 1s         |
| `vPWMTask`     | Fading PWM using Timer2 on PB3    |
| `vNumpadTask`  | Matrix scan + queue send to Morse |
| `vMorseTask`   | LED PB0 blinks Morse of last key  |
| `vUARTMonitor` | Logs serial data for debugging    |

## 🔤 Key Input Logic

* Pressing `*` on the numpad starts **PWM Set Mode**
* Then enter `0–255` followed by `#` to update PWM brightness
* Invalid input cancels the attempt; PWM resumes auto mode
* Morse code blinks 3 short flashes to confirm valid set

# 🛠 Flash Upload (Legacy)

The `Bootloader.X` and `RTOS.X` folders contain features for Flash-based task storage and UART-based uploads. These are **paused** but ready for future development. All current testing uses `RTOS_NoUpload.X`.

## 🔧 Why a Bootloader?

Originally, the RTOS stored all task binaries in EEPROM. This was severely limited (1 KB total). To enable dynamic storage of larger, more complex precompiled task files, we developed a custom bootloader that:

- Executes before the RTOS (at 0x7000, the boot section)
- Receives `.tsk` task binaries over UART using a `<TASK:...>` format
- Writes them into the internal Flash beyond the RTOS (e.g., 0x2800–0x6FFF)
- Then jumps to the RTOS starting at 0x0000

This makes use of the ATMega328P's full 32 KB of Flash, separating RTOS (first 10 KB) and task storage (last ~20 KB), while retaining a small 2 KB bootloader section.

## 🧠 How Flash Upload Works

1. **Bootloader Phase**
   - Runs on reset due to `BOOTRST` fuse
   - Listens for UART packets with `<TASK:...>`
   - Stores each task binary to a defined Flash offset (aligned, e.g. 0x2800, 0x2A00...)
   - Jumps to RTOS after upload

2. **RTOS Phase**
   - Begins execution from 0x0000
   - Reads stored task metadata from EEPROM
   - Jumps to appropriate Flash address to run preloaded `.tsk` tasks

Tasks are compiled externally with a custom PC-side Python tool that supports `.c` or `.bin` task files and prepares them for Flash storage.

## ✅ Features

- ✅ **FreeRTOS-based preemptive multitasking**
- ✅ **EEPROM task storage** system
- ✅ **UART serial interface** for command & task uploads
- ✅ **Task execution from stored binary payloads**
- ✅ **Compatible with USBasp + FTDI**
- 🔜 **Flash-based task binary storage**
- 🔜 **Pause/Resume support for tasks**
- 🔜 **Custom PC-side task compiler and manager**

### To upload the Bootloader and RTOS Task Upload code

Use the provided `.bat` or using the command for RTOS code

But be warn. Very incomplete!

```bash
avrdude -c usbasp -p m328p -B 10 -U flash:w:dist/default/production/RTOS.hex:i
```

> 💡 Use `-B 10` for slower SCK if you get sync errors.

## 🖧 Serial Interface & Commands

Connect via UART at **9600 baud** using your preferred serial terminal or the PC-side tool.

| Command       | Description                         |
|---------------|-------------------------------------|
| `<LIST>`      | Lists stored tasks from EEPROM      |
| `<DELETE:x>`  | Deletes task with ID `x`            |
| `<DELETE>`    | Clears all stored tasks             |
| `<EDIT:...>`  | Edit Metadata of stored tasks       |
| `<DEBUG>`     | Dev use (probably not staying)      |
| `<TASK:...>`  | Uploads a binary task to store+run  |

### Flash Storage

| Feature      | EEPROM        | Flash            |
|--------------|---------------|------------------|
| Size         | 1 KB          | 32 KB total      |
| Max Tasks    | none (was 8)  | ~20 KB of space  |
| Use          | Metadata      | Code Only        |
| Execution    | From RAM      | From RAM         |
| Write Type   | Byte-Level    | Requires erase   |

## 🧰 Debugging Tools

- UART printing of task execution (`'T'`)
- EEPROM verification after writing
- Fuses fixed via `avrdude` (`lfuse: 0xE2` for 8 MHz)
- USBasp firmware flashing supported (`JP1` shorting to enter boot mode)

## 🧪 PC-Side Utility

A Python-based utility is being developed to:

- Detect and select COM ports
- Send task management commands
- Upload task binaries using `<TASK:...>`
- Future: Compile AVR C code into binary tasks via `avr-gcc`

GitHub Repo:  
➡️ [Senior-Project-PC (Python Utility)](https://github.com/Sirapobchon/Senior-Project-PC)

## 📄 License

Licensed under the **MIT License**. Contributions welcome!

**Contributors:**  
- @Sirapobchon  
- @pppcyd
