# RTOS for ATMega328P – Senior Project

This is my Senior Project focused on embedded systems using the ATMega328P. It implements a lightweight RTOS using FreeRTOS v8.2.0 with support for dynamic task management via UART and persistent storage through EEPROM (and planned Flash integration).

## 📌 Overview

This project is a **custom RTOS for the ATMega328P** based on **FreeRTOS v8.2.0**. It adds the ability to:
- Accept precompiled **binary tasks** via UART.
- **Store and load tasks from EEPROM**, and later Flash.
- Execute tasks dynamically using **FreeRTOS task scheduling**.
- Communicate with a **Python-based PC tool** for real-time task management.

## ✅ Features

- ✅ **FreeRTOS-based preemptive multitasking**
- ✅ **EEPROM task storage** system
- ✅ **UART serial interface** for command & task uploads
- ✅ **Task execution from stored binary payloads**
- ✅ **Compatible with USBasp + FTDI**
- 🔜 **Flash-based task binary storage**
- 🔜 **Pause/Resume support for tasks**
- 🔜 **Custom PC-side task compiler and manager**

## 📁 Project Structure

```
📁 RTOS-ATMega328P
├── FreeRTOS/
│   ├── include/                 # FreeRTOS kernel headers
│   ├── portable/
│   │   ├── GCC/ATMega328/      # AVR port (port.c, portmacro.h)
│   │   └── MemMang/            # heap_4.c used for dynamic memory
│   ├── *.c                     # Core FreeRTOS kernel files
├── nbproject/                  # MPLAB X project metadata
├── build/                      # Auto-generated build directory
├── dist/                       # Compiled .hex outputs
├── main.c                      # Project entry point
├── LICENSE
├── Makefile
└── README.md                   # This file
```

## 🔧 Getting Started

### 1. Requirements
- MPLAB X IDE with XC8 v2.50
- AVRDUDE (for flashing firmware)
- USBasp programmer (for flashing ATMega328P)
- FTDI/USB-Serial converter (for UART communication)

### 2. Flash the RTOS firmware

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
| `<DEBUG>`     | Dumps raw EEPROM contents           |
| `<TASK:...>`  | Uploads a binary task to store+run  |

## 💾 Task Storage

### EEPROM Task Format

```c
struct TaskHeader {
    uint8_t  taskID;
    uint8_t  taskType;
    uint8_t  taskPriority;
    uint16_t binarySize;
    // uint32_t flashAddress (future)
};
```

- Each task is stored in EEPROM with `TaskHeader + BinaryPayload` (max 256 bytes per task).
- Up to **10 tasks** can be stored.

### Flash Storage (Planned)

| Feature      | EEPROM        | Flash           |
|--------------|---------------|------------------|
| Size         | 1 KB          | 32 KB total      |
| Max Tasks    | ~10           | Up to ~100       |
| Use          | Metadata+Code | Code Only        |
| Execution    | From RAM      | From RAM         |
| Write Type   | Byte-Level     | Requires erase   |

## 🧠 RTOS Core

- Uses **FreeRTOS v8.2.0** with port for AVR
- Tick timer uses **TIMER1_COMPA_vect**
- **heap_4.c** is used for dynamic allocation and task buffer management
- All tasks run via a common executor (`vTaskExecution()`), based on the task’s `taskType`

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

## 🚧 Known Issues

- ❌ EEPROM data sometimes misparsed due to binary vs ASCII issues
- ❌ Flash writing not yet implemented
- ❌ Manual reset needed after task upload
- 🔜 Dynamic live task monitoring under development

## 📈 Future Enhancements

- [ ] Flash-based task upload with sector management
- [ ] Add task pause/resume without deletion
- [ ] CRC checks and file verification
- [ ] GUI-based PC utility with task builder
- [ ] Custom bootloader for dynamic updates

## 📄 License

Licensed under the **MIT License**. Contributions welcome!

**Contributors:**  
- @Sirapobchon  
- @pppcyd
