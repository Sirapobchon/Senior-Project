/*
 * File:   main.c
 * Author: Sirapob-ASUSTUF
 *
 * Created on April 28, 2025, 8:00 PM
 */

#define F_CPU 8000000UL
#include <xc.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdbool.h>

// --- Function Prototypes ---
void init(void);
void uart_transmit(char data);
void uart_transmit_string(const char *str);

void blink_led_handler(void);
void pwm_handler(void);
void numpad_handler(void);
void morse_handler(void);
void uart_handler(void);

// --- Initialization ---

void init(void) {
    // UART
    UBRR0H = 0;
    UBRR0L = 51; // 9600 baud at 8MHz
    UCSR0B = (1 << RXEN0) | (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    
    // LED: PB0 = Morse LED, PB3 = PWM LED, PB2 = Blink LED
    DDRB |= (1 << PB0) | (1 << PB3) | (1 << PB2); 
    
    // PWM on PB1 (OC1A) using Timer1 Fast PWM
    TCCR2A = (1 << COM2A1) | (1 << WGM20) | (1 << WGM21); // WGM21 + WGM20 = Mode 3
    TCCR2B = (1 << CS21);  // Prescaler 8, WGM22 = 0
    OCR2A = 0; // Start at 0 brightness
    
    // Numpad
    // Rows: PB6, PB7, PD5, PD6 (outputs)
    DDRB |= (1 << PB6) | (1 << PB7);
    DDRD |= (1 << PD5) | (1 << PD6);

    // Columns: PC0-PC3 (inputs with pull-up)
    DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3));
    PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3);
}

void uart_transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

void uart_transmit_string(const char *str) {
    while (*str) {
        uart_transmit(*str++);
    }
}

void blink_led_handler(void) {
    static uint8_t state = 0;
    _delay_ms(1000); // Delay 1000ms

    if (state == 0) {
        PORTB |= (1 << PB2); // LED ON
        state = 1;
    } else {
        PORTB &= ~(1 << PB2); // LED OFF
        state = 0;
    }
}

void pwm_handler(void) {
    static uint8_t step = 0;
    static int8_t direction = 1; // 1 = increasing, -1 = decreasing

    OCR2A = (step * 255) / 50; // Set PWM duty cycle

    _delay_ms(10); // Smooth fade

    step += direction;

    if (step >= 50) {
        step = 50;
        direction = -1;
    } else if (step == 0) {
        direction = 1;
    }
}

void numpad_handler(void) {
    static const char keymap[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    static const struct {
        char key;
        const char *morse;
    } morse_table[] = {
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
        {'*', "."},  // We define * as .
        {'#', "-"}   // # as -
    };
    const uint8_t morse_table_size = sizeof(morse_table) / sizeof(morse_table[0]);

    for (uint8_t row = 0; row < 4; row++) {
        // Drive all rows HIGH first
        PORTB |= (1 << PB6) | (1 << PB7);
        PORTD |= (1 << PD5) | (1 << PD6);

        // Drive one selected row LOW
        switch (row) {
            case 0: PORTB &= ~(1 << PB6); break;
            case 1: PORTB &= ~(1 << PB7); break;
            case 2: PORTD &= ~(1 << PD5); break;
            case 3: PORTD &= ~(1 << PD6); break;
        }

        _delay_ms(1); // settle time

        for (uint8_t col = 0; col < 4; col++) {
            if (!(PINC & (1 << col))) {
                // Key pressed!
                char key = keymap[row][col];

                uart_transmit_string("Key Pressed: ");
                uart_transmit(key);
                uart_transmit('\n');

                // Lookup Morse code
                for (uint8_t i = 0; i < morse_table_size; i++) {
                    if (morse_table[i].key == key) {
                        const char *morse = morse_table[i].morse;

                        // Blink the Morse code
                        while (*morse) {
                            PORTB |= (1 << PB0); // LED ON

                            if (*morse == '.') {
                                _delay_ms(200); // short blink
                            } else if (*morse == '-') {
                                _delay_ms(600); // long blink
                            }

                            PORTB &= ~(1 << PB0); // LED OFF
                            _delay_ms(200); // gap between symbols
                            morse++;
                        }
                        //_delay_ms(800); // gap after full letter
                        break;
                    }
                }

                // Simple debounce
                _delay_ms(200);
            }
        }
    }
}


void uart_handler(void) {
    if (UCSR0A & (1 << RXC0)) { // If data is available
        char received = UDR0;   // Read received byte
        uart_transmit(received); // Echo it back
    }
}

// --- Main Program ---

int main(void) {
    // System setup
    init();

    uart_transmit_string("Non-RTOS Setup Start\n");

    while (1) {
        blink_led_handler();
        pwm_handler();
        numpad_handler();
        uart_handler();
        
        _delay_ms(1);
    }
}

