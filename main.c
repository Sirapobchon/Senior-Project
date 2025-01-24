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
#include <string.h>
#include <stdint.h>

// Function Prototypes
// Blink LED
void vBlinkInit(void);
void vBlinkToggle(void);

// UART USB
void vUARTInit(void);
void vUARTTask(void *pvParameters);
void vUARTTransmit(char data);
char vUARTReceive(void);
void vUARTMorseLED(char data);
void vBlinkLED(portTickType duration);
char vUARTBufferGet(void);

#define mainLED_TASK_PRIORITY			(tskIDLE_PRIORITY)
#define mainUART_TASK_PRIORITY 			(tskIDLE_PRIORITY)
// #define mainNEXT_TASK_2				(tskIDLE_PRIORITY+2)
// #define mainNEXT_TASK_3				(tskIDLE_PRIORITY+3)

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

// UART Communication Task

#define DOT_DURATION pdMS_TO_TICKS(200)  // 200 ms
#define DASH_DURATION pdMS_TO_TICKS(600) // 600 ms
#define SYMBOL_SPACE pdMS_TO_TICKS(300)  // Space between dot/dash
#define LETTER_SPACE pdMS_TO_TICKS(600)  // Space between letters
#define WORD_SPACE pdMS_TO_TICKS(1400)  // Space between words
// Buffer size for received UART data
#define UART_BUFFER_SIZE 64

// Circular buffer for UART data
static char uartBuffer[UART_BUFFER_SIZE];
static volatile uint8_t bufferHead = 0;
static volatile uint8_t bufferTail = 0;

// Morse Code Mapping Table
const char *morseTable[36] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
    "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",         // S-Z
    "-----", ".----", "..---", "...--", "....-", ".....", "-....",    // 0-5
    "--...", "---..", "----."                                       // 6-9
};

// UART Initialization
void vUARTInit(void)
{
    DDRB |= _BV(PB2); // Configure PB2 as output
    UBRR0H = 0;     // Set baud rate to 9600 (for 8 MHz clock)
    UBRR0L = 51;    // Baud rate for 9600 at 8 MHz
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Enable RX and TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

// Add data to the UART buffer
void vUARTBufferAdd(char data) {
    uartBuffer[bufferHead] = data;
    bufferHead = (bufferHead + 1) % UART_BUFFER_SIZE;
    // Optional: Handle buffer overflow (overwrite or discard data)
}

// Retrieve data from the UART buffer
char vUARTBufferGet(void) {
    char data = uartBuffer[bufferTail];
    bufferTail = (bufferTail + 1) % UART_BUFFER_SIZE;
    return data;
}

// UART Receive Interrupt Service Routine (ISR)
ISR(USART_RX_vect) {
    char receivedData = UDR0; // Read received data
    vUARTBufferAdd(receivedData); // Add data to buffer
}

void vUARTTask(void *pvParameters) 
{
    char receivedData;

    for (;;) {
        // Check if there is data in the buffer
        if (bufferHead != bufferTail) {
            receivedData = vUARTBufferGet(); // Get next character
            vUARTTransmit(receivedData);    // Echo back the character
            vUARTMorseLED(receivedData);    // Blink Morse code
        } else {
            vTaskDelay(pdMS_TO_TICKS(10)); // Short delay to prevent busy-waiting
            vUARTMorseLED('T');
        }
    }
}

// UART Transmit Function
void vUARTTransmit(char data)
{
    // Wait for the transmit buffer to be empty
    while (!(UCSR0A & (1 << UDRE0))) {
        taskYIELD(); // Allow other tasks to run
    }
    UDR0 = data; // Load data into transmit register
}

// UART Receive Function
char vUARTReceive(void)
{
    // Wait for data to be received
    while (!(UCSR0A & (1 << RXC0))) {
        taskYIELD(); // Allow other tasks to run
    }
    return UDR0; // Return received data
}

// Function to convert character to Morse code and blink LED
void vUARTMorseLED(char data) {
    static const char *morseCode = NULL;
    static size_t morseIndex = 0;
    static portTickType nextBlinkTime = 0;

    // Initialize Morse code for the current character
    if (!morseCode) {
        // Convert to uppercase
        if (data >= 'a' && data <= 'z') data -= 32;

        // Handle spaces
        if (data == ' ') {
            nextBlinkTime = xTaskGetTickCount() + WORD_SPACE;
            return;
        }

        // Get Morse code for the character
        if (data >= 'A' && data <= 'Z') {
            morseCode = morseTable[data - 'A'];
        } else if (data >= '0' && data <= '9') {
            morseCode = morseTable[data - '0' + 26];
        }
    }

    // Blink Morse code symbols
    if (morseCode && xTaskGetTickCount() >= nextBlinkTime) {
        if (morseCode[morseIndex] == '.') {
            vBlinkLED(DOT_DURATION);
            nextBlinkTime = xTaskGetTickCount() + DOT_DURATION + SYMBOL_SPACE;
        } else if (morseCode[morseIndex] == '-') {
            vBlinkLED(DASH_DURATION);
            nextBlinkTime = xTaskGetTickCount() + DASH_DURATION + SYMBOL_SPACE;
        }

        morseIndex++;
        if (morseIndex >= strlen(morseCode)) {
            // Reset for the next character
            morseCode = NULL;
            morseIndex = 0;
            nextBlinkTime = xTaskGetTickCount() + LETTER_SPACE;
        }
    }
}

// Function to blink LED for a dot or dash
void vBlinkLED(portTickType duration)
{
    PORTB |= _BV(PB2);          // Turn LED on
    vTaskDelay(duration);       // Wait
    PORTB &= ~_BV(PB2);         // Turn LED off
    vTaskDelay(SYMBOL_SPACE);   // Wait between symbols
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
