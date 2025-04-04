/*
 * File:   main.c
 * Author: Sirapob-ASUSTUF
 *
 * Created on November 28, 2024, 3:21 PM
 */

#include <avr/io.h>
#include <avr/eeprom.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <xc.h>
#include <string.h>
#include <stdint.h>
#include <util/delay.h>

// #define mainNEXT_TASK_1      (tskIDLE_PRIORITY)
// #define mainNEXT_TASK_2		(tskIDLE_PRIORITY+2)
// #define mainNEXT_TASK_3		(tskIDLE_PRIORITY+3)

#define START_MARKER '<'
#define END_MARKER '>'

#define BAUD 9600   // Define Baud Rate
#define UBRR 51   // Define UBRR Value


#define MAX_TASK_BINARY_SIZE 512  // Adjust as needed
#define HEADER_SIZE sizeof(struct TaskHeader)

#define EEPROM_SIZE 1024 
#define TASK_STORAGE_SIZE 128  // If each task needs 128 bytes total
#define MAX_TASK_SLOTS (EEPROM_SIZE / TASK_STORAGE_SIZE)  // EEPROM_SIZE = 1024

struct TaskHeader {
    uint8_t taskID;        // Unique task identifier
    uint8_t taskType;      // Defines task behavior (GPIO, PWM, etc.)
    uint8_t taskPriority;  // Defines execution priority
    uint16_t binarySize;   // Size of the task binary stored
    uint8_t status;        // (planned: running/paused)
    uint32_t flashAddress; // (only if Flash is used)
};

// Task function prototypes
void uart_init(void);
void uart_transmit(unsigned char data);
void uart_transmit_string(const char *str);
void uart_transmit_number(uint16_t num);
void list_running_tasks(void);
void delete_task_eeprom(uint8_t taskID);
void vTaskExecution(void *pvParameters);
void vTaskReceive(void *pvParameters);
void load_tasks_from_eeprom();
void store_task_eeprom(struct TaskHeader *header, uint8_t *binaryData);

// FreeRTOS Application Idle Hook
void vApplicationIdleHook(void) {
    // Idle Hook (Optional)
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    uart_transmit_string("\n[ERROR] Stack overflow in task: ");
    uart_transmit_string(pcTaskName);
    uart_transmit('\n');
    while (1); // Halt system for safety
}

portSHORT main(void) {
    // Initialize UART
    uart_init();
    uart_transmit_string("Free heap: ");
    uart_transmit_number(xPortGetFreeHeapSize());
    uart_transmit('\n');
    
    // Load stored tasks from EEPROM
    //load_tasks_from_eeprom();
    
    // Create a task for receiving new task files over UART
    xTaskCreate(vTaskReceive, "ReceiveTask", 192, NULL, 2, NULL);
    
    // Start RTOS Scheduler
    vTaskStartScheduler();
    
    return 0;
}

// ---------------- UART Communication ----------------

void uart_init(void) {
    // Set Baud Rate
    UBRR0H = (unsigned char)(UBRR >> 8);
    UBRR0L = (unsigned char)UBRR;
    
    // Enable Receiver & Transmitter
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    
    // Set frame format: 8 data bits, 1 stop bit
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_transmit(unsigned char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Wait for empty transmit buffer
    UDR0 = data;
}

unsigned char uart_receive(void) {
    while (!(UCSR0A & (1 << RXC0))); // Wait for data to be received
    return UDR0;
}

void uart_transmit_string(const char *str) {
    while (*str) {
        uart_transmit(*str++);
    }
}

void uart_transmit_hex(uint8_t value) {
    char hex[3];
    hex[0] = "0123456789ABCDEF"[value >> 4];  // Extract high nibble
    hex[1] = "0123456789ABCDEF"[value & 0x0F]; // Extract low nibble
    hex[2] = '\0';  // Null terminator
    uart_transmit_string(hex);
}

void uart_transmit_number(uint16_t num) {
    if (num == 0) {
        uart_transmit('0');
        return;
    }

    if (num < 0) {
        uart_transmit('-');
        num = -num;
    }
    
    char buf[6];
    uint8_t i = 0;

    // Convert to decimal (reverse order)
    do {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    } while (num > 0);

    // Send in correct order
    while (i > 0) {
        uart_transmit(buf[--i]);
    }
}

// ---------------- Task Status Report ----------------

void list_running_tasks() {
    struct TaskHeader task;
    uint8_t task_found = 0; // Flag to check if any task exists

    uart_transmit_string("List of Tasks:");

    for (uint8_t i = 0; i < MAX_TASK_SLOTS; i++) {
        uint16_t addr = i * TASK_STORAGE_SIZE;
        eeprom_read_block((void *)&task, (const void *)addr, sizeof(struct TaskHeader));

        if (task.taskID != 0xFF) {  // If valid task exists
            uart_transmit_string("\nSlot ");
            uart_transmit_hex(i);
            uart_transmit_string(": ");

            uart_transmit_string("ID=");
            uart_transmit_hex(task.taskID);

            uart_transmit_string(" Type: ");
            uart_transmit_hex(task.taskType);

            switch (task.taskType) {
                case 0:
                    uart_transmit_string(" (GPIO)");
                    break; 
                case 1:
                    uart_transmit_string(" (PWM)");
                    break;
                case 2:
                    uart_transmit_string(" (Serial)");
                    break;
                default:
                    uart_transmit_string(" (Custom)");
            }

            uart_transmit_string(", Priority=");
            uart_transmit_hex(task.taskPriority);

            uart_transmit_string(", Size: ");
            uart_transmit_hex(task.binarySize);
            uart_transmit_string(" (");
            uart_transmit_number(task.binarySize);
            uart_transmit_string(" byte)");

            uart_transmit_string(" Status: ");
            uart_transmit_hex(task.status);

            if (task.status == 1)
                uart_transmit_string(" (Running)");
            else if (task.status == 0)
                uart_transmit_string(" (Paused)");
            else
                uart_transmit_string(" (Unknown)");

            uart_transmit_string(", FlashAddr=0x");

            // Flash address is 32-bit, print each byte
            uart_transmit_hex((task.flashAddress >> 24) & 0xFF);
            uart_transmit_hex((task.flashAddress >> 16) & 0xFF);
            uart_transmit_hex((task.flashAddress >> 8) & 0xFF);
            uart_transmit_hex(task.flashAddress & 0xFF);

            task_found = 1;
        }
    }

    if (!task_found) {
        uart_transmit_string("\nNo stored tasks found.");
    }
}


void delete_task_eeprom(uint8_t taskID) {
    struct TaskHeader task;
    uint16_t addr = taskID * TASK_STORAGE_SIZE;

    // Read the task from EEPROM
    eeprom_read_block((void *)&task, (const void *)addr, sizeof(struct TaskHeader));

    // Check if the task exists
    if (task.taskID == 0xFF) {
        uart_transmit_string("\nNo Task ID: ");
        uart_transmit_hex(taskID);
        return;
    }

    // Task exists, proceed with deletion
    struct TaskHeader emptyTask = {0xFF}; // Mark as empty
    eeprom_update_block((const void *)&emptyTask, (void *)addr, sizeof(struct TaskHeader));

    for (uint16_t i = 0; i < (TASK_STORAGE_SIZE - sizeof(struct TaskHeader)); i++) {
        eeprom_update_byte((void *)(addr + sizeof(struct TaskHeader) + i), 0xFF);
    }

    uart_transmit_string("\nTask Deleted ID: ");
    uart_transmit_hex(taskID);
}


void delete_all_tasks() {
    struct TaskHeader emptyTask = {0xFF}; // Empty task header

    for (uint8_t i = 0; i < MAX_TASK_SLOTS; i++) {
        uint16_t addr = i * TASK_STORAGE_SIZE;
        eeprom_update_block((const void *)&emptyTask, (void *)addr, sizeof(struct TaskHeader));

        for (uint16_t i = 0; i < (TASK_STORAGE_SIZE - sizeof(struct TaskHeader)); i++) {
            eeprom_update_byte((void *)(addr + sizeof(struct TaskHeader) + i), 0xFF);
        }
    }

    uart_transmit_string("\nAll Tasks Deleted");
}


// ---------------- Task Reception & Storage ----------------

void store_task_eeprom(struct TaskHeader *header, uint8_t *binaryData) {
    uint16_t addr = header->taskID * TASK_STORAGE_SIZE;

    struct TaskHeader existingTask;
    eeprom_read_block((void *)&existingTask, (const void *)addr, sizeof(struct TaskHeader));

    if (existingTask.taskID != 0xFF) {
        delete_task_eeprom(header->taskID); // Remove old task before writing
    }

    eeprom_update_block((const void *)header, (void *)addr, sizeof(struct TaskHeader));
    eeprom_update_block((const void *)binaryData, (void *)(addr + sizeof(struct TaskHeader)), header->binarySize);

    // **Verify EEPROM**
    struct TaskHeader verifyTask;
    eeprom_read_block((void *)&verifyTask, (const void *)addr, sizeof(struct TaskHeader));

    uart_transmit_string("EEPROM Write Check - ID: ");
    uart_transmit_hex(verifyTask.taskID);
    uart_transmit_string(", Type: ");
    uart_transmit_hex(verifyTask.taskType);
    uart_transmit_string(", Priority: ");
    uart_transmit_hex(verifyTask.taskPriority);
    uart_transmit_string(", Size: ");
    uart_transmit_hex(verifyTask.binarySize);
    uart_transmit('\n');

    if (verifyTask.taskID == header->taskID) {
        uart_transmit_string("Task Stored Successfully\n");
    } else {
        uart_transmit_string("Task Store Error!\n");
    }
}

// ---------------- Task Loading & Execution ----------------

void load_tasks_from_eeprom() {
    struct TaskHeader task;
    uint8_t found = 0;  // Track if any task is found

    for (uint8_t i = 0; i < MAX_TASK_SLOTS; i++) {
        uint16_t addr = i * TASK_STORAGE_SIZE;
        eeprom_read_block((void *)&task, (const void *)addr, sizeof(struct TaskHeader));

        if (task.taskID != 0xFF) {  // If task exists
            found = 1;
            uart_transmit_string("Loaded Task ID: ");
            uart_transmit_hex(task.taskID);
            uart_transmit('\n');

            // Create a new task in FreeRTOS
            xTaskCreate(vTaskExecution, "TaskExec", 128, (void *)&task, task.taskPriority, NULL);
        }
    }

    if (!found) {
        uart_transmit_string("No stored tasks found.\n");
    }
}

void debug_eeprom_tasks() {
    struct TaskHeader task;

    uart_transmit_string("EEPROM Debug:");

    for (uint8_t i = 0; i < MAX_TASK_SLOTS; i++) {
        uint16_t addr = i * TASK_STORAGE_SIZE;
        eeprom_read_block((void *)&task, (const void *)addr, sizeof(struct TaskHeader));
        
        uart_transmit('\n');
        uart_transmit_string("Slot ");
        uart_transmit_hex(i);
        uart_transmit_string(": ");

        if (task.taskID == 0xFF) {
            uart_transmit_string("Empty");
        } else {
            uart_transmit_string("Task ID: ");
            uart_transmit_hex(task.taskID);
            uart_transmit_string(" Type: ");
            uart_transmit_hex(task.taskType);
            uart_transmit_string(" Priority: ");
            uart_transmit_hex(task.taskPriority);
            uart_transmit_string(" Size: ");
            uart_transmit_hex(task.binarySize);
        }
    }
}

// ---------------- FreeRTOS Task Definitions ----------------

void vTaskReceive(void *pvParameters) {
    while (1) {
        uint16_t index = 0;
        uint8_t byte;
        uint8_t *rxBuffer = (uint8_t *)pvPortMalloc(HEADER_SIZE + MAX_TASK_BINARY_SIZE);

        if (!rxBuffer) {
            uart_transmit_string("Memory alloc failed\n");
            vTaskDelay(pdMS_TO_TICKS(1000));
            continue;
        }

        while (UCSR0A & (1 << RXC0)) {
            (void)UDR0;
        }

        // Wait for start marker
        do {
            byte = uart_receive();
        } while (byte != START_MARKER);

        // Check for <LIST>, <DELETE>, etc. by reading text-only first
        char tempBuffer[16] = {0};
        uint8_t tempIndex = 0;
        while ((byte = uart_receive()) != END_MARKER && tempIndex < sizeof(tempBuffer) - 1) {
            if (byte == '\n' || byte == '\r') break;
            tempBuffer[tempIndex++] = byte;
        }
        tempBuffer[tempIndex] = '\0';

         // Check if it's a known ASCII command
        if (strcmp(tempBuffer, "LIST") == 0) {
            list_running_tasks();
        } else if (strncmp(tempBuffer, "DELETE:", 7) == 0) {
            uint8_t taskID = tempBuffer[7] - '0';
            delete_task_eeprom(taskID);
        } else if (strcmp(tempBuffer, "DELETE") == 0) {
            delete_all_tasks();
        } else if (strcmp(tempBuffer, "DEBUG") == 0) {
            debug_eeprom_tasks();
        } else if (strncmp(tempBuffer, "TASK:", 5) == 0) {
            // Copy leftover data after "TASK:" from tempBuffer
            index = 0;
            for (uint8_t i = 5; i < tempIndex; i++) {
                rxBuffer[index++] = tempBuffer[i];
            }

            // Continue receiving the rest of the binary data
            while ((byte = uart_receive()) != END_MARKER && index < HEADER_SIZE + MAX_TASK_BINARY_SIZE) {
                rxBuffer[index++] = byte;
            }

            if (index >= HEADER_SIZE) {
                struct TaskHeader newTask;
                newTask.taskID       = rxBuffer[0];
                newTask.taskType     = rxBuffer[1];
                newTask.taskPriority = rxBuffer[2];

                // Binary size (little endian)
                newTask.binarySize   = rxBuffer[3] | (rxBuffer[4] << 8);
                newTask.status       = rxBuffer[5];
                newTask.flashAddress = 0;

                uart_transmit_string("Parsed Task - ID: ");
                uart_transmit_hex(newTask.taskID);
                uart_transmit_string(", Type: ");
                uart_transmit_hex(newTask.taskType);
                uart_transmit_string(", Priority: ");
                uart_transmit_hex(newTask.taskPriority);
                uart_transmit_string(", Size: ");
                uart_transmit_hex(newTask.binarySize);
                uart_transmit('\n');

                if (newTask.binarySize > 0 && newTask.binarySize <= MAX_TASK_BINARY_SIZE) {
                    store_task_eeprom(&newTask, &rxBuffer[HEADER_SIZE]);
                    uart_transmit_string("Task Stored\n");
                } else {
                    uart_transmit_string("Invalid Binary Size\n");
                }
            } else {
                uart_transmit_string("Corrupted or Too Short Payload\n");
            }
        } else {
            uart_transmit_string("Unknown Command\n");
        }

        vPortFree(rxBuffer);
    }
}

void vTaskExecution(void *pvParameters) {
    struct TaskHeader *task = (struct TaskHeader *)pvParameters;
    uint8_t *taskBinary = (uint8_t *)pvPortMalloc(task->binarySize);

    if (!taskBinary) return; // Handle memory allocation failure

    uint16_t addr = task->taskID * TASK_STORAGE_SIZE;  // ? consistent with store/load/delete
    eeprom_read_block((void *)taskBinary, (const void *)(addr + sizeof(struct TaskHeader)), task->binarySize);

    while (1) {
        if (task->taskType == 0) { // GPIO Example
            DDRB |= (1 << taskBinary[0]);  // Set pin as output
            PORTB ^= (1 << taskBinary[0]); // Toggle pin
            uart_transmit('T');  // Debug: Task Executed
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Adjust delay as needed
    }

    vPortFree(taskBinary); // Free memory when done
}

