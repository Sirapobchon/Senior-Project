/*
 * File:   main.c
 * Author: Sirapob-ASUSTUF
 *
 * Created on November 28, 2024, 3:21 PM
 */

#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <xc.h>
#include <string.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "globals.h"

volatile uint16_t pwm_brightness = PWM_AUTO;

extern QueueHandle_t morseQueue;

// #define mainNEXT_TASK_1      (tskIDLE_PRIORITY)
// #define mainNEXT_TASK_2		(tskIDLE_PRIORITY+2)
// #define mainNEXT_TASK_3		(tskIDLE_PRIORITY+3)

#define BAUD 9600   // Define Baud Rate
#define UBRR 51   // Define UBRR Value

// Task function prototypes
void uart_init(void);
void uart_transmit(unsigned char data);
void uart_transmit_string(const char *str);
void uart_transmit_number(uint16_t num);
void vUARTMonitor(void *pvParameters);

// Declare tasks from .c files
void vBlinkTask(void *pvParameters);
void vPWMTask(void *pvParameters);
void vNumpadTask(void *pvParameters);
void vMorseTask(void *pvParameters);
// Just make sure they're added to the project as source files.

// FreeRTOS Application Idle Hook
void vApplicationIdleHook(void) {
    // Idle Hook (Optional)
}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char *pcTaskName) {
    uart_transmit_string("[ERROR] Stack overflow in task: ");
    uart_transmit_string(pcTaskName);
    uart_transmit('\n');
    while (1); // Halt system for safety
}

portSHORT main(void) {
    // Disable prescaler
    CLKPR = (1 << CLKPCE);
    CLKPR = 0;
    
    // Initialize UART
    uart_init();
    
    uart_transmit_string("RTOS is running...\n");

    // Create a task for receiving new task files over UART
    morseQueue = xQueueCreate(10, sizeof(char));
    xTaskCreate(vBlinkTask, "BlinkTask", 64, NULL, 1, NULL);
    xTaskCreate(vPWMTask, "PWMTask", 64, NULL, 1, NULL);
    xTaskCreate(vNumpadTask, "NumpadTask", 128, NULL, 1, NULL);
    xTaskCreate(vMorseTask, "Morse", 96, NULL, 1, NULL);
    xTaskCreate(vUARTMonitor, "UARTMonitor", 96, NULL, 1, NULL);

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

    char buf[6];
    uint8_t i = 0;

    do {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    } while (num > 0);

    while (i > 0) {
        uart_transmit(buf[--i]);
    }
}

void vUARTMonitor(void *pvParameters) {
    (void)pvParameters;
    uart_transmit_string("UART Monitor Started...\n");

    while (1) {
        if (UCSR0A & (1 << RXC0)) { // Check if data received
            char received = UDR0;
            uart_transmit(received);
        }
    }
}

