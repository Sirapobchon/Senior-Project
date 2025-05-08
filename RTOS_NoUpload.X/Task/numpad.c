#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "../globals.h"
#include <string.h> 
#include <stdbool.h>

QueueHandle_t morseQueue; // Queue to pass keypresses

// External UART functions declared
extern void uart_transmit(char data);
extern void uart_transmit_string(const char *str);

// Morse code table
typedef struct {
    char key;
    const char *morse;
} MorseEntry;

const MorseEntry morse_table[] = {
    {'0', "-----"},
    {'1', ".----"},
    {'2', "..---"},
    {'3', "...--"},
    {'4', "....-"},
    {'5', "....."},
    {'6', "-...."},
    {'7', "--..."},
    {'8', "---.."},
    {'9', "----."},
    {'A', ".-"},
    {'B', "-..."},
    {'C', "-.-."},
    {'D', "-.."},
    {'*', "."},   // * = short blink
    {'#', "-"}    // # = long blink
};
const uint8_t morse_table_size = sizeof(morse_table) / sizeof(morse_table[0]);

// Lookup Morse code pattern by key
const char* getMorseCode(char key) {
    for (uint8_t i = 0; i < morse_table_size; i++) {
        if (morse_table[i].key == key) {
            return morse_table[i].morse;
        }
    }
    return NULL;
}

// Blink Morse code on LED
void blinkMorse(const char *morse) {
    while (*morse) {
        PORTB |= (1 << PB0); // LED ON
        
        if (*morse == '.') {
            vTaskDelay(pdMS_TO_TICKS(200)); // Short blink
        } else if (*morse == '-') {
            vTaskDelay(pdMS_TO_TICKS(600)); // Long blink
        }
        
        PORTB &= ~(1 << PB0); // LED OFF
        vTaskDelay(pdMS_TO_TICKS(200)); // Gap between dot/dash
        
        morse++;
    }
    vTaskDelay(pdMS_TO_TICKS(800)); // Slightly shorter gap after full letter
}

void handlePWMInput(const char *buffer, uint8_t length) {
    if (length == 0) {
        pwm_brightness = PWM_AUTO;
        uart_transmit_string("PWM set to AUTO\n");
    } else {
        uint16_t value = atoi(buffer);
        if (value <= 255) {
            pwm_brightness = value;

            // UART confirmation
            uart_transmit_string("PWM set to ");
            uart_transmit((value / 100) + '0');
            uart_transmit(((value / 10) % 10) + '0');
            uart_transmit((value % 10) + '0');
            uart_transmit('\n');

            // Blink LED 3x to confirm
            for (int i = 0; i < 3; i++) {
                blinkMorse(".");
            }
        } else {
            uart_transmit_string("Invalid PWM value\n");
        }
    }
}

void vNumpadTask(void *pvParameters) {
    (void)pvParameters;
    // Rows: PB4-PB7 (output)
    DDRB |= (1 << PB6) | (1 << PB7);        // PB6 and PB7 as output
    DDRD |= (1 << PD5) | (1 << PD6);         // PD5 and PD6 as output
    
    // Columns: PC0-PC3 (input with pull-up)
    DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3)); 
    PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3); 

    // LED pin for feedback
    DDRB |= (1 << PB0); // PB0 output (LED)

    const char keymap[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };
    
    char input_buffer[4] = {0};
    uint8_t buffer_index = 0;
    bool tracking_pwm_input = false;
    
    while (1) {
        for (uint8_t row = 0; row < 4; row++) {
            // First, drive all rows HIGH
            PORTB |= (1 << PB6) | (1 << PB7);
            PORTD |= (1 << PD5) | (1 << PD6);

            // Now drive the selected row LOW
            switch (row) {
                case 0: PORTB &= ~(1 << PB6); break;
                case 1: PORTB &= ~(1 << PB7); break;
                case 2: PORTD &= ~(1 << PD5); break;
                case 3: PORTD &= ~(1 << PD6); break;
            }
            
             vTaskDelay(pdMS_TO_TICKS(1)); // Small settle delay

            for (uint8_t col = 0; col < 4; col++) {
                if (!(PINC & (1 << col))) {
                    // Key detected!
                    char key = keymap[row][col];

                    // Send key over UART
                    uart_transmit_string("Row ");
                    uart_transmit('1' + row);   // row is 0-indexed, so add '1'
                    uart_transmit_string(", Col ");
                    uart_transmit('1' + col);   // col is 0-indexed, so add '1'
                    uart_transmit_string(": ");
                    uart_transmit(key);
                    uart_transmit('\n');
                    
                    // Send key to Morse Task via Queue
                    xQueueSend(morseQueue, &key, portMAX_DELAY);
                    
                    // PWM input tracking state machine
                    if (key == '*') {
                        // Start or cancel PWM tracking
                        tracking_pwm_input = true;
                        buffer_index = 0;
                        memset(input_buffer, 0, sizeof(input_buffer));
                    } 
                    else if (tracking_pwm_input) {
                        if (key >= '0' && key <= '9' && buffer_index < 3) {
                            input_buffer[buffer_index++] = key;
                        } 
                        else if (key == '#') {
                            handlePWMInput(input_buffer, buffer_index);
                            tracking_pwm_input = false;
                        } 
                        else {
                            // Any non-digit key during tracking cancels input
                            tracking_pwm_input = false;
                        }
                    }
                    
                    // Small debounce delay
                    vTaskDelay(pdMS_TO_TICKS(1000));
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void vMorseTask(void *pvParameters) {
    char key;
    while (1) {
        if (xQueueReceive(morseQueue, &key, portMAX_DELAY) == pdPASS) {
            // When a key received, blink its Morse
            const char *morse = getMorseCode(key);
            if (morse) {
                blinkMorse(morse);
            }
        }
    }
}