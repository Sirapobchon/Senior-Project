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

// ---------------- Task Status Report ----------------

void list_running_tasks() {
    struct TaskHeader task;
    
    for (uint8_t i = 0; i < 10; i++) {
        uint16_t addr = i * (sizeof(struct TaskHeader) + 256);
        eeprom_read_block((void *)&task, (const void *)addr, sizeof(struct TaskHeader));

        if (task.taskID != 0xFF) {
            uart_transmit('T');  // Debug: Task Found
            uart_transmit(task.taskID);
            uart_transmit(task.taskPriority);
            uart_transmit(task.taskType);
        }
    }
}

void delete_task_eeprom(uint8_t taskID) {
    struct TaskHeader emptyTask = {0xFF}; // Mark as empty
    uint8_t emptyData[256] = {0xFF}; // Maximum task size assumed
    uint16_t addr = taskID * (sizeof(struct TaskHeader) + 256); // Adjust for binary size

    eeprom_update_block((const void *)&emptyTask, (void *)addr, sizeof(struct TaskHeader));
    for (uint16_t i = 0; i < 256; i++) {
        eeprom_update_byte((void *)(addr + sizeof(struct TaskHeader) + i), 0xFF);
    }

    uart_transmit('D');  // Debug: Task Deleted
}


// ---------------- Task Reception & Storage ----------------

void receive_task_data(char *buffer, int max_length) {
    int index = 0;
    unsigned char incoming;
    
    //uart_transmit('R'); // Received Data

    while (index < max_length - 1) {
        incoming = uart_receive();
        uart_transmit(incoming); // Debug: Show Received Data
        
        if (incoming == END_MARKER) { 
            buffer[index] = '\0';
            //uart_transmit('E'); // Debug: End of Data Reached
            return;
        }

        buffer[index++] = incoming;
    }

    buffer[index] = '\0'; // Null-terminate
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

        if (strncmp(buffer, "<LIST_TASKS>", 12) == 0) {
            list_running_tasks();
        } else if (strncmp(buffer, "<DELETE:", 8) == 0) {
            uint8_t taskID = buffer[8] - '0';
            delete_task_eeprom(taskID);
        } else {
            uart_transmit('U');     // Debug: Unknown Command

            // Handle task reception normally
            struct TaskHeader newTask;
            newTask.taskID = buffer[0];
            newTask.taskType = buffer[1];
            newTask.taskPriority = buffer[2];

            union {
                uint16_t size;
                uint8_t bytes[2];
            } sizeConverter;

            sizeConverter.bytes[0] = buffer[4];
            sizeConverter.bytes[1] = buffer[3];

            newTask.binarySize = sizeConverter.size;

            store_task_eeprom(&newTask, (uint8_t *)(buffer + 5));
            uart_transmit('D');  // Debug: Task Received
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

