#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"

void vBlinkTask(void) {
    DDRB |= (1 << PB0);      // Set PB0 as output
    while (1) {
        PORTB ^= (1 << PB0); // Toggle PB0
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

