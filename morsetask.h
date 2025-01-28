/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   morsetask.h
 * Author: Sirapob-ASUSTUF
 * Comments: 
 * Revision history: January 28, 2024, 10:03 AM
 */

#ifndef MORSETASK_H
#define MORSETASK_H

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdint.h>
#include <xc.h> // include processor files - each processor file is guarded.  

// Define Morse durations
#define DOT_DURATION pdMS_TO_TICKS(200)  // 200 ms
#define DASH_DURATION pdMS_TO_TICKS(600) // 600 ms
#define SYMBOL_SPACE pdMS_TO_TICKS(300)  // Space between dot/dash
#define LETTER_SPACE pdMS_TO_TICKS(600)  // Space between letters
#define WORD_SPACE pdMS_TO_TICKS(1400)   // Space between words

// Morse Task
void vMorseTask(void *pvParameters);

// UART USB Functions
void vUARTInit(void);
void vUARTTask(void *pvParameters);
void vUARTTransmit(char data);
char vUARTReceive(void);
char vUARTBufferGet(void);

//void vBlinkLED(portTickType duration);  // LED blink helper

#endif // BLINKTASK_H

