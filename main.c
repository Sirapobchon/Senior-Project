/*
 * File:   main.c
 * Author: Sirapob-ASUSTUF
 *
 * Created on November 28, 2024, 3:21 PM
 */

#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "apptasks.h"
#include <xc.h>

// Function Prototypes
// Blink LED
void vLEDInit(void);
void vLEDToggle(void);

// UART USB
void vUARTInit(void);
void vUARTTask(void *pvParameters);
void vUARTTransmit(char data);
char vUARTReceive(void);

#define mainLED_TASK_PRIORITY			(tskIDLE_PRIORITY)
#define mainUART_TASK_PRIORITY 			(tskIDLE_PRIORITY)
// #define mainNEXT_TASK_2				(tskIDLE_PRIORITY+2)
// #define mainNEXT_TASK_3				(tskIDLE_PRIORITY+3)


// TODO: Research moving vLEDFlashTask method to apptasks.c
void vLEDFlashTask(void *pvParms)
{
	vLEDInit();
	portTickType xLastWakeTime;
	const portTickType xFrequency = 100;
	xLastWakeTime = xTaskGetTickCount();

	for(;;) {
		vLEDToggle();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

// UART Communication Task
void vUARTTask(void *pvParameters)
{
    vUARTInit(); // Initialize UART
    char receivedData;

    for (;;) {
        // Wait for incoming data
        receivedData = vUARTReceive();

        // Echo received data
        vUARTTransmit(receivedData);

        // Optional: Toggle LED for each received byte
        vLEDToggle();
    }
}

void init(void);

// FreeRTOS Application Idle Hook
void vApplicationIdleHook(void);

portSHORT main(void)
{
    // Create LED Blink Task
    xTaskCreate(vLEDFlashTask, "LED", configMINIMAL_STACK_SIZE, NULL, mainLED_TASK_PRIORITY, NULL);

    // Create UART Communication Task
    xTaskCreate(vUARTTask, "UART", configMINIMAL_STACK_SIZE, NULL, mainUART_TASK_PRIORITY, NULL);

    vTaskStartScheduler();
    
	return 0;
}

void vApplicationIdleHook( void )
{
	//vCoRoutineSchedule();
}

void vLEDInit(void)
{
    DDRB |= _BV(PB1); // Configure PB1 as output
}

void vLEDToggle(void)
{
    PORTB ^= _BV(PB1); // Toggle PB1
}

// UART Initialization
void vUARTInit(void)
{
    UBRR0H = 0;     // Set baud rate to 9600 (for 8 MHz clock)
    UBRR0L = 51;    // Baud rate for 9600 at 8 MHz
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Enable RX and TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

// UART Transmit Function
void vUARTTransmit(char data)
{
    while (!(UCSR0A & (1 << UDRE0))); // Wait for transmit buffer to be empty
    UDR0 = data; // Send data
}

// UART Receive Function
char vUARTReceive(void)
{
    while (!(UCSR0A & (1 << RXC0))); // Wait for data to be received
    return UDR0; // Return received data
}