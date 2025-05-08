#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "..\globals.h"

void vPWMTask(void *pvParameters) {
    (void)pvParameters;
    // Set PB1 (OC1A) as output
    DDRB |= (1 << PB1);

    // Timer1 Fast PWM, 8-bit, non-inverting mode
    TCCR1A = (1 << COM1A1) | (1 << WGM10);  // Fast PWM 8-bit
    TCCR1B = (1 << WGM12) | (1 << CS11);    // Prescaler = 8

    uint8_t brightness = 0;
    int8_t direction = 5;
    uint16_t last_value = PWM_AUTO + 1;  // Ensure initial mismatch

    for (;;) {
        if (pwm_brightness == PWM_AUTO) {
            OCR1A = brightness;
            brightness += direction;
            if (brightness == 0 || brightness == 255) {
                direction = -direction;
            }
        } else if (pwm_brightness != last_value) {
            last_value = pwm_brightness;
            OCR1A = (uint8_t)pwm_brightness;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
