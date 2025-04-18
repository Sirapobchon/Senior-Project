#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void vNumpadTask(void *pvParameters) {
    (void)pvParameters;
    // Rows: PB4-PB7 (output)
    // Cols: PC0-PC4 (input w/ pullup)
    DDRB |= (1 << PB4) | (1 << PB5) | (1 << PB6) | (1 << PB7); // Rows as output
    DDRC &= ~((1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3)); // Cols as input
    PORTC |= (1 << PC0) | (1 << PC1) | (1 << PC2) | (1 << PC3); // Enable pull-ups

    while (1) {
        for (uint8_t row = 0; row < 4; row++) {
            PORTC = ~(1 << row); // Drive one row low at a time
            for (uint8_t col = 0; col < 4; col++) {
                if (!(PINC & (1 << (col + 4)))) {
                    // Key (row,col) pressed
                    // Do something with it (for now just delay)
                    vTaskDelay(pdMS_TO_TICKS(200));
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
