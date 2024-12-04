/*
 * File:   main.c
 * Author: Sirapob-ASUSTUF
 *
 * Created on September 22, 2024, 8:21 PM
 */

#define F_CPU 8000000L

#include <avr/io.h>
#include <util/delay.h>

void blink(void) {
    DDRB |= (1 << DDB1);
    
    while (1) {
        PORTB ^= (1 << PORTB1);
        _delay_ms(1000);
    }
}
