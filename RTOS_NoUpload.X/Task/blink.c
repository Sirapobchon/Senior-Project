#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"

void vBlinkTask(void *pvParameters) {
    (void)pvParameters;
    DDRB |= (1 << PB2);      // Set PB1 as output
    while (1) {
        PORTB ^= (1 << PB2); // Toggle PB1
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

