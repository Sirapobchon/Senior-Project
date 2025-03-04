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

struct TaskHeader {
    uint8_t taskID;
    uint8_t taskType;
    uint8_t taskPriority;
    uint16_t binarySize;
    // uint32_t flashAddress;
};

// Task function prototypes
void uart_init(void);
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

portSHORT main(void) {
    // Initialize UART
    uart_init();
    
    // Load stored tasks from EEPROM
    load_tasks_from_eeprom();
    
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

// ---------------- Task Status Report ----------------

void list_running_tasks() {
    struct TaskHeader task;
    uint8_t task_found = 0; // Flag to check if any task exists
    
    uart_transmit_string("List of Tasks: ");

    for (uint8_t i = 0; i < 10; i++) {
        uint16_t addr = i * (sizeof(struct TaskHeader) + 256);
        eeprom_read_block((void *)&task, (const void *)addr, sizeof(struct TaskHeader));

        if (task.taskID != 0xFF) {  // If valid task exists
            uart_transmit_string("\nSlot ");
            uart_transmit_hex(i);
            uart_transmit_string(": ");
            uart_transmit_string("T ID: ");
            uart_transmit_hex(task.taskID);
            uart_transmit_string(" | Priority: ");
            uart_transmit_hex(task.taskPriority);
            uart_transmit_string(" | Type: ");
            uart_transmit_hex(task.taskType);
            task_found = 1;
        }
    }

    if (!task_found) {
        uart_transmit_string("\nNo stored tasks found.");
    }
}

void delete_task_eeprom(uint8_t taskID) {
    struct TaskHeader task;
    uint16_t addr = taskID * (sizeof(struct TaskHeader) + 256); // Adjust for binary size

    // Read the task from EEPROM
    eeprom_read_block((void *)&task, (const void *)addr, sizeof(struct TaskHeader));

    // Check if the task exists
    if (task.taskID == 0xFF) {
        uart_transmit_string("No Task ID: ");
        uart_transmit_hex(taskID);
        return;
    }

    // Task exists, proceed with deletion
    struct TaskHeader emptyTask = {0xFF}; // Mark as empty
    eeprom_update_block((const void *)&emptyTask, (void *)addr, sizeof(struct TaskHeader));

    for (uint16_t i = 0; i < 256; i++) {
        eeprom_update_byte((void *)(addr + sizeof(struct TaskHeader) + i), 0xFF);
    }

    uart_transmit_string("Task Deleted ID: ");
    uart_transmit_hex(taskID);
}


void delete_all_tasks() {
    struct TaskHeader emptyTask = {0xFF}; // Empty task header

    for (uint8_t i = 0; i < 10; i++) {
        uint16_t addr = i * (sizeof(struct TaskHeader) + 256);
        eeprom_update_block((const void *)&emptyTask, (void *)addr, sizeof(struct TaskHeader));

        for (uint16_t j = 0; j < 256; j++) {
            eeprom_update_byte((void *)(addr + sizeof(struct TaskHeader) + j), 0xFF);
        }
    }

    uart_transmit_string("All Tasks Deleted");
}


// ---------------- Task Reception & Storage ----------------

void receive_task_data(char *buffer, int max_length) {
    int index = 0;
    unsigned char incoming;
    uint8_t inside_command = 0;  // Flag to track if inside '< >'

    while (index < max_length - 1) {
        incoming = uart_receive();

        // Check if it's a command (inside < >)
        if (incoming == START_MARKER) {
            inside_command = 1;  // We are inside a command
            index = 0;  // Reset index for command buffer
        } 
        else if (incoming == END_MARKER) {
            inside_command = 0;  // End of command
            buffer[index] = '\0'; // Null-terminate buffer
            return;
        } 
        else if (inside_command) {
            // Store command data
            buffer[index++] = incoming;
        } 
        else {
            // Not inside < >, echo back to Serial Monitor
            uart_transmit(incoming);
        }
    }

    buffer[index] = '\0'; // Null-terminate in case of overflow
    uart_transmit_string("Buffer Overflow\n");  // Debug: Buffer Overflow (Should Not Happen)
}

#define TASK_STORAGE_SIZE 256  // Fixed storage size per task

void store_task_eeprom(struct TaskHeader *header, uint8_t *binaryData) {
    uint16_t addr = header->taskID * (sizeof(struct TaskHeader) + header->binarySize);

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

    for (uint8_t i = 0; i < 10; i++) {
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

    for (uint8_t i = 0; i < 10; i++) {
        uint16_t addr = i * (sizeof(struct TaskHeader) + 256);
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
        char buffer[32];  
        receive_task_data(buffer, sizeof(buffer));

        uart_transmit_string("Received: ");
        for (uint8_t i = 0; i < strlen(buffer); i++) {
            uart_transmit_hex(buffer[i]);  // Print in HEX format
            uart_transmit(' ');
        }
        uart_transmit('\n');

        if (buffer[0] != '\0') { // Ensure buffer is not empty
            if (strcmp(buffer, "LIST") == 0) {
                list_running_tasks();
            } else if (strcmp(buffer, "DELETE") == 0) {
                delete_all_tasks();
            } else if (strncmp(buffer, "DELETE:", 7) == 0) {
                uint8_t taskID = buffer[7] - '0';
                delete_task_eeprom(taskID);
            } else if (strcmp(buffer, "DEBUG") == 0) {
                debug_eeprom_tasks();
            } else if (strncmp(buffer, "TASK:", 5) == 0) {
                uart_transmit_string("Task Command Detected\n");

                if (strlen(buffer) < 10) {
                    uart_transmit_string("Error: Task Data Too Short!");
                    return;
                }

                struct TaskHeader newTask;
                newTask.taskID = buffer[5];  // Byte 1: Task ID
                newTask.taskType = buffer[6]; // Byte 2: Task Type
                newTask.taskPriority = buffer[7]; // Byte 3: Priority

                // Extract Binary Size (16-bit value, little endian)
                union {
                    uint16_t size;
                    uint8_t bytes[2];
                } sizeConverter;
                sizeConverter.bytes[0] = buffer[8];  // Low byte
                sizeConverter.bytes[1] = buffer[9];  // High byte
                newTask.binarySize = sizeConverter.size;

                uart_transmit_string("Parsed Task - ID: ");
                uart_transmit_hex(newTask.taskID);
                uart_transmit_string(", Type: ");
                uart_transmit_hex(newTask.taskType);
                uart_transmit_string(", Priority: ");
                uart_transmit_hex(newTask.taskPriority);
                uart_transmit_string(", Size: ");
                uart_transmit_hex(newTask.binarySize);
                uart_transmit('\n');

                // Store in EEPROM
                store_task_eeprom(&newTask, (uint8_t *)(buffer + 10));

                // Verify EEPROM Write
                struct TaskHeader checkTask;
                uint16_t addr = newTask.taskID * (sizeof(struct TaskHeader) + newTask.binarySize);
                eeprom_read_block((void *)&checkTask, (const void *)addr, sizeof(struct TaskHeader));

                uart_transmit_string("EEPROM Write Check - ID: ");
                uart_transmit_hex(checkTask.taskID);
                uart_transmit_string(", Type: ");
                uart_transmit_hex(checkTask.taskType);
                uart_transmit_string(", Priority: ");
                uart_transmit_hex(checkTask.taskPriority);
                uart_transmit_string(", Size: ");
                uart_transmit_hex(checkTask.binarySize);
                uart_transmit('\n');

                if (checkTask.taskID != newTask.taskID) {
                    uart_transmit_string("Task Store Error!\n");
                } else {
                    uart_transmit_string("Task Stored\n");
                }
            } else {
                uart_transmit_string("Unknown Command");
            }
        }
        else{
            uart_transmit_string("Unknown Command");
        }
    }
}

void vTaskExecution(void *pvParameters) {
    struct TaskHeader *task = (struct TaskHeader *)pvParameters;
    uint8_t *taskBinary = (uint8_t *)pvPortMalloc(task->binarySize);

    if (!taskBinary) return; // Handle memory allocation failure

    uint16_t addr = task->taskID * (sizeof(struct TaskHeader) + task->binarySize);
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

