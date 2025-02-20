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
    snprintf(hex, sizeof(hex), "%02X", value);  // Convert byte to hex
    uart_transmit_string(hex);
}

// ---------------- Task Status Report ----------------

void list_running_tasks() {
    struct TaskHeader task;
    uint8_t task_found = 0; // Flag to check if any task exists

    for (uint8_t i = 0; i < 10; i++) {
        uint16_t addr = i * (sizeof(struct TaskHeader) + 256);
        eeprom_read_block((void *)&task, (const void *)addr, sizeof(struct TaskHeader));

        if (task.taskID != 0xFF) {  // If valid task exists
            uart_transmit_string("T ID:");
            uart_transmit_hex(task.taskID);
            uart_transmit_string(" Priority:");
            uart_transmit_hex(task.taskPriority);
            uart_transmit_string(" Type:");
            uart_transmit_hex(task.taskType);
            uart_transmit('\n');  // Newline for better readability
            task_found = 1;
        }
    }

    if (!task_found) {
        uart_transmit_string("No tasks");
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
        uart_transmit('\n');
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
    uart_transmit('\n');
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
    uart_transmit('X');  // Debug: Buffer Overflow (Should Not Happen)
}


void store_task_eeprom(struct TaskHeader *header, uint8_t *binaryData) {
    uint16_t addr = header->taskID * (sizeof(struct TaskHeader) + header->binarySize);

    struct TaskHeader existingTask;
    eeprom_read_block((void *)&existingTask, (const void *)addr, sizeof(struct TaskHeader));

    if (existingTask.taskID != 0xFF) {
        delete_task_eeprom(header->taskID); // Remove old task before writing
    }

    eeprom_update_block((const void *)header, (void *)addr, sizeof(struct TaskHeader));
    eeprom_update_block((const void *)binaryData, (void *)(addr + sizeof(struct TaskHeader)), header->binarySize);

    uart_transmit_string(" Stored Task ID: ");
    uart_transmit_hex(header->taskID);
    uart_transmit('\n');
}


// ---------------- Task Loading & Execution ----------------

void load_tasks_from_eeprom() {
    struct TaskHeader task;
    
    for (uint8_t i = 0; i < 10; i++) { // Assume max 10 tasks
        uint16_t addr = i * (sizeof(struct TaskHeader) + 256);
        eeprom_read_block((void *)&task, (const void *)addr, sizeof(struct TaskHeader));

        if (task.taskID != 0xFF) { // If task exists
            uart_transmit('L'); // Debug: Task Loaded
            uart_transmit(task.taskID);
            
            // Create a new task in FreeRTOS to execute this binary
            xTaskCreate(vTaskExecution, "TaskExec", 128, (void *)&task, task.taskPriority, NULL);
        }
    }
}

// ---------------- FreeRTOS Task Definitions ----------------

void vTaskReceive(void *pvParameters) {
    while (1) {        
        char buffer[32];  
        receive_task_data(buffer, sizeof(buffer));

        if (buffer[0] != '\0') { // Ensure buffer is not empty
            if (strcmp(buffer, "LIST") == 0) {
                list_running_tasks();
            } else if (strcmp(buffer, "DELETE") == 0) {  // Delete All Tasks
                delete_all_tasks();
            } else if (strncmp(buffer, "DELETE:", 7) == 0) {
                uint8_t taskID = buffer[7] - '0'; // Extract task ID
                delete_task_eeprom(taskID);
            } else if (strncmp(buffer, "TASK:", 5) == 0) {
                // Handle task creation normally
                struct TaskHeader newTask;
                newTask.taskID = buffer[5];
                uart_transmit_string(" Task ID: ");
                uart_transmit_hex(newTask.taskID);
                newTask.taskType = buffer[6];
                newTask.taskPriority = buffer[7];
                union {
                    uint16_t size;
                    uint8_t bytes[2];
                } sizeConverter;
                sizeConverter.bytes[0] = buffer[9];
                sizeConverter.bytes[1] = buffer[8];
                newTask.binarySize = sizeConverter.size;
                store_task_eeprom(&newTask, (uint8_t *)(buffer + 10));
                uart_transmit_string("Task Stored\n");
            } else {
                uart_transmit_string("Unknown Command\n");
            }
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

