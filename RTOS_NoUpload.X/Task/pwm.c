#include <avr/io.h>
#include "FreeRTOS.h"
#include "task.h"
#include "..\globals.h"

void vPWMTask(void *pvParameters) {
    (void)pvParameters;
    // Set PB1 (OC1A) as output
    DDRB |= (1 << PB3);  // Set PB3 (OC2A) as output

    TCCR2A = (1 << COM2A1) | (1 << WGM20) | (1 << WGM21); // WGM21 + WGM20 = Mode 3
    TCCR2B = (1 << CS21);  // Prescaler 8, WGM22 = 0

    uint8_t brightness = 0;
    int8_t direction = 5;
    uint16_t last_value = PWM_AUTO + 1;  // Ensure initial mismatch

    for (;;) {
        if (pwm_brightness == PWM_AUTO) {
            OCR2A = brightness;
            brightness += direction;
            if (brightness == 0 || brightness == 255) {
                direction = -direction;
            }
        } else if (pwm_brightness != last_value) {
            last_value = pwm_brightness;
            OCR2A = (uint8_t)pwm_brightness;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }
}
