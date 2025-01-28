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
#include "morsetask.h"
#include <xc.h>
#include <string.h>
#include <stdint.h>
#include <util/delay.h>

// Function Prototypes
// Blink LED
void vBlinkInit(void);
void vBlinkToggle(void);

#define mainLED_TASK_PRIORITY			(tskIDLE_PRIORITY)
#define mainUART_TASK_PRIORITY 			(tskIDLE_PRIORITY)
// #define mainNEXT_TASK_2				(tskIDLE_PRIORITY+2)
// #define mainNEXT_TASK_3				(tskIDLE_PRIORITY+3)

QueueHandle_t xMorseQueue; // Add this declaration

// TODO: Research moving vLEDFlashTask method to apptasks.c
void vLEDFlashTask(void *pvParms)
{
	vBlinkInit();
	portTickType xLastWakeTime;
	const portTickType xFrequency = 100;
	xLastWakeTime = xTaskGetTickCount();

	for(;;) {
		vBlinkToggle();
		vTaskDelayUntil(&xLastWakeTime, xFrequency);
	}
}

void vBlinkInit(void)
{
    DDRB |= _BV(PB1); // Configure PB1 as output
}

void vBlinkToggle(void)
{
    PORTB ^= _BV(PB1); // Toggle PB1
}

void init(void);

// FreeRTOS Application Idle Hook
void vApplicationIdleHook(void);

portSHORT main(void)
{
    // Create Morse queue
    xMorseQueue = xQueueCreate(10, sizeof(char));
    if (xMorseQueue == NULL) {
        // Handle queue creation failure
        for (;;) {
            PORTB ^= _BV(PB2); // Blink error LED
            _delay_ms(200);
        }
    }
    
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
