/*
 * File:   morsetask.c
 * Author: Sirapob-ASUSTUF
 *
 * Created on January 28, 2025, 10:01 AM
 */

#include "morsetask.h"
#include <avr/io.h>
#include <string.h>

// Morse Code Mapping Table
static const char *morseTable[36] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..",    // A-I
    ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.",  // J-R
    "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",         // S-Z
    "-----", ".----", "..---", "...--", "....-", ".....", "-....",    // 0-5
    "--...", "---..", "----."                                        // 6-9
};

// Queue handle for Morse
static QueueHandle_t xMorseQueue;

// UART initialization
void vUARTInit(void) {
    // Set baud rate and UART settings (9600 baud, 8N1)
    DDRB |= _BV(PB2); // Configure PB2 as output
    UBRR0H = 0;     // Set baud rate to 9600 (for 8 MHz clock)
    UBRR0L = 51;    // Baud rate for 9600 at 8 MHz
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);  // Enable RX and TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

// UART transmit function
void vUARTTransmit(char data) {
    while (!(UCSR0A & _BV(UDRE0))); // Wait for empty transmit buffer
    UDR0 = data;
}

// UART receive function
char vUARTReceive(void) {
    while (!(UCSR0A & _BV(RXC0))); // Wait for data to be received
    return UDR0;
}

// UART buffer get (non-blocking)
char vUARTBufferGet(void) {
    if (UCSR0A & _BV(RXC0)) {
        return UDR0;
    }
    return '\0'; // No data available
}

// UART task
void vUARTTask(void *pvParameters) {
    char receivedChar;

    for (;;) {
        receivedChar = vUARTReceive(); // Receive data
        if (xQueueSend(xMorseQueue, &receivedChar, 0) == pdPASS) {
            // Successfully sent to Morse queue
            vUARTTransmit(receivedChar); // Echo back
        }
    }
}

// Function to blink LED for a dot or dash
static void vBlinkLED(portTickType duration) {
    PORTB |= _BV(PB2);          // Turn LED on
    vTaskDelay(duration);       // Wait
    PORTB &= ~_BV(PB2);         // Turn LED off
    vTaskDelay(SYMBOL_SPACE);   // Wait between symbols
}

// Morse Task
void vMorseTask(void *pvParameters) {
    char data;
    const char *morseCode;
    size_t morseIndex;

    for (;;) {
        // Wait for data from the queue
        if (xQueueReceive(xMorseQueue, &data, portMAX_DELAY) == pdPASS) {
            // Convert to uppercase
            if (data >= 'a' && data <= 'z') data -= 32;

            // Handle spaces
            if (data == ' ') {
                vTaskDelay(WORD_SPACE);
                continue;
            }

            // Get Morse code for the character
            if (data >= 'A' && data <= 'Z') {
                morseCode = morseTable[data - 'A'];
            } else if (data >= '0' && data <= '9') {
                morseCode = morseTable[data - '0' + 26];
            } else {
                continue; // Ignore unsupported characters
            }

            // Blink Morse code
            morseIndex = 0;
            while (morseCode[morseIndex] != '\0') {
                if (morseCode[morseIndex] == '.') {
                    vBlinkLED(DOT_DURATION);
                } else if (morseCode[morseIndex] == '-') {
                    vBlinkLED(DASH_DURATION);
                }
                morseIndex++;
            }
            vTaskDelay(LETTER_SPACE); // Delay between letters
        }
    }
}