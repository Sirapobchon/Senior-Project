#ifndef GLOBALS_H
#define GLOBALS_H

#include <stdint.h>

#define PWM_AUTO 256  // Special marker value (beyond valid 0?255)

extern volatile uint16_t pwm_brightness;  // Use uint16_t to allow 256

#endif
