# RTOS for ATMega328P Senior-Project
This is my Senior Project on Embedded system (WIP)

## Overview  
This project is a **custom RTOS (Real-Time Operating System) for the ATMega328P**, built using **FreeRTOS v8.2.0** and integrated with **EEPROM and Flash storage** for dynamic task management. The RTOS enables the execution of **precompiled binary task files** received over UART and provides a **task manager** interface for managing stored tasks.  

## Features  
✅ **FreeRTOS-based** task scheduling for ATMega328P  
✅ **EEPROM task storage** with plans for Flash-based execution  
✅ **Serial communication via UART (9600 baud)**  
✅ **PC-side tool** for sending task files and monitoring RTOS state  
✅ **USBasp compatibility** for flashing firmware updates  
✅ **Task management commands** via UART  

## Project Structure  

```
📂 RTOS-ATMega328P
│── 📁 src
│   ├── main.c          # Main RTOS implementation
│   ├── port.c          # FreeRTOS v8.2.0 ATMega328P port
│   ├── portmacro.h     # Macros for AVR FreeRTOS port
│   ├── eeprom_tasks.c  # Task storage functions
│   ├── uart.c          # UART communication functions
│── 📁 include
│   ├── FreeRTOSConfig.h  # RTOS configuration
│   ├── task.h           # Task structures and management
│── 📁 pc_tool
│   ├── serial_tool.py   # Python script for task communication
│── 📁 docs
│   ├── RTOS_Implementation_Report.md
│── README.md
```

## Getting Started  

### **1. Setting Up the Development Environment**  
- Install **MPLAB X IDE** and **XC8 Compiler**  
- Install **AVRDUDE** for flashing firmware  
- Connect an **ATMega328P** via **USBasp**  

### **2. Flashing the RTOS Firmware**  
```sh
avrdude -c usbasp -p m328p -B 10 -U flash:w:RTOS.hex:i
```

### **3. Serial Communication**  
Use a **serial monitor** (e.g., Arduino Serial Monitor, PuTTY) to send commands:  
**Might still be a bit buggy**

| Command | Description |
|---------|-------------|
| `<LIST>` | List stored tasks |
| `<DELETE:x>` | Delete task ID x |
| `<DELETE>` | Delete all tasks |
| `<TASK:...>` | Upload task binary |

### **4. PC Tool for Task Management**  
A Python-based tool for **uploading tasks and monitoring RTOS state**:  
https://github.com/Sirapobchon/Senior-Project-PC (WIP)

## Technical Details  

### **RTOS Core Implementation**  
- **FreeRTOS v8.2.0** adapted for ATMega328P  
- Uses **Timer1** for task switching  
- Supports **dynamic task creation** via UART  

### **Task Storage System**  
- Initially designed for **EEPROM** (1 KB storage)  
- Investigating **Flash storage (32 KB) for larger tasks**  
- Task binaries can be **loaded into RAM for execution**  

### **EEPROM Task Format**  
Each task consists of a **TaskHeader** stored in EEPROM:  
```c
struct TaskHeader {
    uint8_t taskID;
    uint8_t taskType;
    uint8_t taskPriority;
    uint16_t binarySize;
};
```

### **Flash vs EEPROM Discussion**  (Transtion In-progress)
| Feature  | EEPROM | Flash |
|----------|--------|--------|
| Capacity | 1 KB | 32 KB |
| Task Size Limit | 256 bytes | Up to 22 KB |
| Read Speed | Slow | Fast |
| Write Flexibility | Per byte | Requires block erase |
| Suitable for | Metadata | Full task binaries |

## Issues & Future Plans  

### **Current Challenges**  
🚧 **Task storage bugs** – EEPROM storing incorrect task IDs  
🚧 **Flash storage feasibility** – Requires proper sector handling  
🚧 **Live task monitoring** – Implementing dynamic status updates  

### **Future Enhancements**  
🔹 **Full Flash-based task execution**  
🔹 **GUI-based task manager for PC**  
🔹 **Better error handling & debugging tools**  
🔹 **Optimized memory usage & task scheduling**  

## License  
This project is licensed under the **MIT License**. Feel free to help and contribute!  

---

**Contributors:**  
@Sirapobchon
@pppcyd
