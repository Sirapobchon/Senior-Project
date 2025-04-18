#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"

void vPWMTask(void *pvParameters) {
    (void)pvParameters;
    // Set PB1 (OC1A) as output
    DDRB |= (1 << PB1);

    // Timer1 Fast PWM, 8-bit, non-inverting mode
    TCCR1A = (1 << COM1A1) | (1 << WGM10);  // Fast PWM 8-bit
    TCCR1B = (1 << WGM12) | (1 << CS11);    // Prescaler = 8

    uint8_t brightness = 0;
    int8_t direction = 5;

    while (1) {
        OCR1A = brightness; // PWM duty
        brightness += direction;
        if (brightness == 0 || brightness == 255) {
            direction = -direction;
        }
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
