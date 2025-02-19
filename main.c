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

#define START_MARKER "<"
#define END_MARKER ">"

#define BAUD 9600         // Define Baud Rate
#define MYUBRR F_CPU/16/BAUD-1

struct TaskHeader {
    uint8_t taskID;
    uint8_t taskType;
    uint8_t taskPriority;
    uint16_t binarySize;
    uint32_t flashAddress;
};

// Task function prototypes
void vTaskExecution(void *pvParameters);
void vTaskReceive(void *pvParameters);
void load_tasks_from_eeprom();

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
    xTaskCreate(vTaskReceive, "ReceiveTask", 128, NULL, 1, NULL);
    
    // Start RTOS Scheduler
    vTaskStartScheduler();
    
    return 0;
}

// ---------------- UART Communication ----------------

void uart_init(void) {
    // Set Baud Rate
    UBRR0H = (unsigned char)(MYUBRR >> 8);
    UBRR0L = (unsigned char)MYUBRR;
    
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

// ---------------- Task Reception & Storage ----------------

void receive_task_data(char *buffer, int max_length) {
    int index = 0;
    unsigned char incoming;

    while (index < max_length - 1) {
        incoming = uart_receive();

        if (incoming == END_MARKER) { // End of file marker
            buffer[index] = '\0';
            return;
        }

        buffer[index++] = incoming;
    }

    buffer[index] = '\0'; // Null-terminate
}

void store_task_eeprom(TaskHeader *header, uint8_t *binaryData) {
    uint16_t addr = header->taskID * sizeof(TaskHeader);

    eeprom_update_block((const void *)header, (void *)addr, sizeof(TaskHeader));
    eeprom_update_block((const void *)binaryData, (void *)(addr + sizeof(TaskHeader)), header->binarySize);
}

// ---------------- Task Loading & Execution ----------------

void load_tasks_from_eeprom() {
    TaskHeader task;
    
    for (uint8_t i = 0; i < 10; i++) { // Assume max 10 tasks
        uint16_t addr = i * sizeof(TaskHeader);
        eeprom_read_block((void *)&task, (const void *)addr, sizeof(TaskHeader));

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
    char buffer[256];  // Buffer for incoming task data
    TaskHeader newTask;
    
    while (1) {
        receive_task_data(buffer, sizeof(buffer));

        // Parse the received buffer into TaskHeader (Placeholder)
        // In real implementation, extract actual values
        newTask.taskID = buffer[0];  // Example: First byte = Task ID
        newTask.taskType = buffer[1];
        newTask.taskPriority = buffer[2];
        newTask.binarySize = (buffer[3] << 8) | buffer[4];

        // Store task in EEPROM
        store_task_eeprom(&newTask, (uint8_t *)(buffer + 5));

        // Confirm task reception
        uart_transmit('R');  // Debug: Task Received
    }
}

void vTaskExecution(void *pvParameters) {
    TaskHeader *task = (TaskHeader *)pvParameters;

    while (1) {
        // Execute task based on type
        if (task->taskType == 0) { // GPIO Example
            DDRB |= (1 << task->flashAddress);  // Set pin as output
            PORTB ^= (1 << task->flashAddress); // Toggle pin
            uart_transmit('T');  // Debug: Task Executed
        }

        vTaskDelay(pdMS_TO_TICKS(1000)); // Adjust delay as needed
    }
}