/*
    FreeRTOS V8.2.0 - Copyright (C) 2015 Real Time Engineers Ltd.
    All rights reserved

    VISIT http://www.FreeRTOS.org TO ENSURE YOU ARE USING THE LATEST VERSION.

    This file is part of the FreeRTOS distribution.

    FreeRTOS is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public License (version 2) as published by the
    Free Software Foundation >>!AND MODIFIED BY!<< the FreeRTOS exception.

	***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes FreeRTOS without being   !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of the FreeRTOS kernel.                                   !<<
	***************************************************************************

    FreeRTOS is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE.  Full license text is available on the following
    link: http://www.freertos.org/a00114.html

    ***************************************************************************
     *                                                                       *
     *    FreeRTOS provides completely free yet professionally developed,    *
     *    robust, strictly quality controlled, supported, and cross          *
     *    platform software that is more than just the market leader, it     *
     *    is the industry's de facto standard.                               *
     *                                                                       *
     *    Help yourself get started quickly while simultaneously helping     *
     *    to support the FreeRTOS project by purchasing a FreeRTOS           *
     *    tutorial book, reference manual, or both:                          *
     *    http://www.FreeRTOS.org/Documentation                              *
     *                                                                       *
    ***************************************************************************

    http://www.FreeRTOS.org/FAQHelp.html - Having a problem?  Start by reading
	the FAQ page "My application does not run, what could be wrong?".  Have you
	defined configASSERT()?

	http://www.FreeRTOS.org/support - In return for receiving this top quality
	embedded software for free we request you assist our global community by
	participating in the support forum.

	http://www.FreeRTOS.org/training - Investing in training allows your team to
	be as productive as possible as early as possible.  Now you can receive
	FreeRTOS training directly from Richard Barry, CEO of Real Time Engineers
	Ltd, and the world's leading authority on the world's leading RTOS.

    http://www.FreeRTOS.org/plus - A selection of FreeRTOS ecosystem products,
    including FreeRTOS+Trace - an indispensable productivity tool, a DOS
    compatible FAT file system, and our tiny thread aware UDP/IP stack.

    http://www.FreeRTOS.org/labs - Where new FreeRTOS products go to incubate.
    Come and try FreeRTOS+TCP, our new open source TCP/IP stack for FreeRTOS.

    http://www.OpenRTOS.com - Real Time Engineers ltd. license FreeRTOS to High
    Integrity Systems ltd. to sell under the OpenRTOS brand.  Low cost OpenRTOS
    licenses offer ticketed support, indemnification and commercial middleware.

    http://www.SafeRTOS.com - High Integrity Systems also provide a safety
    engineered and independently SIL3 certified version for use in safety and
    mission critical applications that require provable dependability.

    1 tab == 4 spaces!
*/

/* 

Changes from V2.6.0

	+ AVR port - Replaced the inb() and outb() functions with direct memory
	  access.  This allows the port to be built with the 20050414 build of
	  WinAVR.
*/

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "FreeRTOS.h"
#include "task.h"


/*-----------------------------------------------------------
 * Implementation of functions defined in portable.h for the AVR port.
 *----------------------------------------------------------*/

/* Start tasks with interrupts enables. */
#define portFLAGS_INT_ENABLED					( ( StackType_t ) 0x80 )

/* Hardware constants for timer 1. */
 /*
#define portCLEAR_COUNTER_ON_MATCH				( ( uint8_t ) 0x08 )
#define portPRESCALE_64							( ( uint8_t ) 0x03 )
#define portCLOCK_PRESCALER						( ( uint32_t ) 64 )
#define portCOMPARE_MATCH_A_INTERRUPT_ENABLE	( ( uint8_t ) 0x10 )
*/

#define portCLEAR_COUNTER_ON_MATCH 				((unsigned char) _BV(WGM12))
#define portPRESCALE_64 						((unsigned char)_BV(CS11) | _BV(CS10))
#define portCLOCK_PRESCALER						(( uint32_t) 64)
#define portCOMPARE_MATCH_A_INTERRUPT_ENABLE	((unsigned char) _BV(OCIE1A))


/*-----------------------------------------------------------*/

/* We require the address of the pxCurrentTCB variable, but don't want to know
any details of its type. */
typedef void TCB_t;
extern volatile TCB_t * volatile pxCurrentTCB;

/*-----------------------------------------------------------*/

/* 
 * Macro to save all the general purpose registers, the save the stack pointer
 * into the TCB.  
 * 
 * The first thing we do is save the flags then disable interrupts.  This is to 
 * guard our stack against having a context switch interrupt after we have already 
 * pushed the registers onto the stack - causing the 32 registers to be on the 
 * stack twice. 
 * 
 * r1 is set to zero as the compiler expects it to be thus, however some
 * of the math routines make use of R1. 
 * 
 * The interrupts will have been disabled during the call to portSAVE_CONTEXT()
 * so we need not worry about reading/writing to the stack pointer. 
 */

#define portSAVE_CONTEXT()									\
	asm volatile (	"push	r0						\n\t"	\
					"in		r0, __SREG__			\n\t"	\
					"cli							\n\t"	\
					"push	r0						\n\t"	\
					"push	r1						\n\t"	\
					"clr	r1						\n\t"	\
					"push	r2						\n\t"	\
					"push	r3						\n\t"	\
					"push	r4						\n\t"	\
					"push	r5						\n\t"	\
					"push	r6						\n\t"	\
					"push	r7						\n\t"	\
					"push	r8						\n\t"	\
					"push	r9						\n\t"	\
					"push	r10						\n\t"	\
					"push	r11						\n\t"	\
					"push	r12						\n\t"	\
					"push	r13						\n\t"	\
					"push	r14						\n\t"	\
					"push	r15						\n\t"	\
					"push	r16						\n\t"	\
					"push	r17						\n\t"	\
					"push	r18						\n\t"	\
					"push	r19						\n\t"	\
					"push	r20						\n\t"	\
					"push	r21						\n\t"	\
					"push	r22						\n\t"	\
					"push	r23						\n\t"	\
					"push	r24						\n\t"	\
					"push	r25						\n\t"	\
					"push	r26						\n\t"	\
					"push	r27						\n\t"	\
					"push	r28						\n\t"	\
					"push	r29						\n\t"	\
					"push	r30						\n\t"	\
					"push	r31						\n\t"	\
					"lds	r26, pxCurrentTCB		\n\t"	\
					"lds	r27, pxCurrentTCB + 1	\n\t"	\
					"in		r0, 0x3d				\n\t"	\
					"st		x+, r0					\n\t"	\
					"in		r0, 0x3e				\n\t"	\
					"st		x+, r0					\n\t"	\
				);

/* 
 * Opposite to portSAVE_CONTEXT().  Interrupts will have been disabled during
 * the context save so we can write to the stack pointer. 
 */

#define portRESTORE_CONTEXT()								\
	asm volatile (	"lds	r26, pxCurrentTCB		\n\t"	\
					"lds	r27, pxCurrentTCB + 1	\n\t"	\
					"ld		r28, x+					\n\t"	\
					"out	__SP_L__, r28			\n\t"	\
					"ld		r29, x+					\n\t"	\
					"out	__SP_H__, r29			\n\t"	\
					"pop	r31						\n\t"	\
					"pop	r30						\n\t"	\
					"pop	r29						\n\t"	\
					"pop	r28						\n\t"	\
					"pop	r27						\n\t"	\
					"pop	r26						\n\t"	\
					"pop	r25						\n\t"	\
					"pop	r24						\n\t"	\
					"pop	r23						\n\t"	\
					"pop	r22						\n\t"	\
					"pop	r21						\n\t"	\
					"pop	r20						\n\t"	\
					"pop	r19						\n\t"	\
					"pop	r18						\n\t"	\
					"pop	r17						\n\t"	\
					"pop	r16						\n\t"	\
					"pop	r15						\n\t"	\
					"pop	r14						\n\t"	\
					"pop	r13						\n\t"	\
					"pop	r12						\n\t"	\
					"pop	r11						\n\t"	\
					"pop	r10						\n\t"	\
					"pop	r9						\n\t"	\
					"pop	r8						\n\t"	\
					"pop	r7						\n\t"	\
					"pop	r6						\n\t"	\
					"pop	r5						\n\t"	\
					"pop	r4						\n\t"	\
					"pop	r3						\n\t"	\
					"pop	r2						\n\t"	\
					"pop	r1						\n\t"	\
					"pop	r0						\n\t"	\
					"out	__SREG__, r0			\n\t"	\
					"pop	r0						\n\t"	\
				);

/*-----------------------------------------------------------*/

/*
 * Perform hardware setup to enable ticks from timer 1, compare match A.
 */
static void prvSetupTimerInterrupt( void );
/*-----------------------------------------------------------*/

/* 
 * See header file for description. 
 */
StackType_t *pxPortInitialiseStack( StackType_t *pxTopOfStack, TaskFunction_t pxCode, void *pvParameters )
{
uint16_t usAddress;

	/* Place a few bytes of known values on the bottom of the stack. 
	This is just useful for debugging. */

	*pxTopOfStack = 0x11;
	pxTopOfStack--;
	*pxTopOfStack = 0x22;
	pxTopOfStack--;
	*pxTopOfStack = 0x33;
	pxTopOfStack--;

	/* Simulate how the stack would look after a call to vPortYield() generated by 
	the compiler. */

	/*lint -e950 -e611 -e923 Lint doesn't like this much - but nothing I can do about it. */

	/* The start of the task code will be popped off the stack last, so place
	it on first. */
	usAddress = ( uint16_t ) pxCode;
	*pxTopOfStack = ( StackType_t ) ( usAddress & ( uint16_t ) 0x00ff );
	pxTopOfStack--;

	usAddress >>= 8;
	*pxTopOfStack = ( StackType_t ) ( usAddress & ( uint16_t ) 0x00ff );
	pxTopOfStack--;

	/* Next simulate the stack as if after a call to portSAVE_CONTEXT().  
	portSAVE_CONTEXT places the flags on the stack immediately after r0
	to ensure the interrupts get disabled as soon as possible, and so ensuring
	the stack use is minimal should a context switch interrupt occur. */
	*pxTopOfStack = ( StackType_t ) 0x00;	/* R0 */
	pxTopOfStack--;
	*pxTopOfStack = portFLAGS_INT_ENABLED;
	pxTopOfStack--;


	/* Now the remaining registers.   The compiler expects R1 to be 0. */
	*pxTopOfStack = ( StackType_t ) 0x00;	/* R1 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x02;	/* R2 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x03;	/* R3 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x04;	/* R4 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x05;	/* R5 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x06;	/* R6 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x07;	/* R7 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x08;	/* R8 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x09;	/* R9 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x10;	/* R10 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x11;	/* R11 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x12;	/* R12 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x13;	/* R13 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x14;	/* R14 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x15;	/* R15 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x16;	/* R16 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x17;	/* R17 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x18;	/* R18 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x19;	/* R19 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x20;	/* R20 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x21;	/* R21 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x22;	/* R22 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x23;	/* R23 */
	pxTopOfStack--;

	/* Place the parameter on the stack in the expected location. */
	usAddress = ( uint16_t ) pvParameters;
	*pxTopOfStack = ( StackType_t ) ( usAddress & ( uint16_t ) 0x00ff );
	pxTopOfStack--;

	usAddress >>= 8;
	*pxTopOfStack = ( StackType_t ) ( usAddress & ( uint16_t ) 0x00ff );
	pxTopOfStack--;

	*pxTopOfStack = ( StackType_t ) 0x26;	/* R26 X */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x27;	/* R27 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x28;	/* R28 Y */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x29;	/* R29 */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x30;	/* R30 Z */
	pxTopOfStack--;
	*pxTopOfStack = ( StackType_t ) 0x031;	/* R31 */
	pxTopOfStack--;

	/*lint +e950 +e611 +e923 */

	return pxTopOfStack;
}
/*-----------------------------------------------------------*/

BaseType_t xPortStartScheduler( void )
{
	/* Setup the hardware to generate the tick. */
	prvSetupTimerInterrupt();

	/* Restore the context of the first task that is going to run. */
	portRESTORE_CONTEXT();

	/* Simulate a function call end as generated by the compiler.  We will now
	jump to the start of the task the context of which we have just restored. */
	asm volatile ( "ret" );

	/* Should not get here. */
	return pdTRUE;
}
/*-----------------------------------------------------------*/

void vPortEndScheduler( void )
{
	/* It is unlikely that the AVR port will get stopped.  If required simply
	disable the tick interrupt here. */
}
/*-----------------------------------------------------------*/

/*
 * Manual context switch.  The first thing we do is save the registers so we
 * can use a naked attribute.
 */
void vPortYield( void ) __attribute__ ( ( naked ) );
void vPortYield( void )
{
	portSAVE_CONTEXT();
	vTaskSwitchContext();
	portRESTORE_CONTEXT();

	asm volatile ( "ret" );
}
/*-----------------------------------------------------------*/

/*
 * Context switch function used by the tick.  This must be identical to 
 * vPortYield() from the call to vTaskSwitchContext() onwards.  The only
 * difference from vPortYield() is the tick count is incremented as the
 * call comes from the tick ISR.
 */
void vPortYieldFromTick( void ) __attribute__ ( ( naked ) );
void vPortYieldFromTick( void )
{
	portSAVE_CONTEXT();
	if( xTaskIncrementTick() != pdFALSE )
	{
		vTaskSwitchContext();
	}
	portRESTORE_CONTEXT();

	asm volatile ( "ret" );
}
/*-----------------------------------------------------------*/

/*
 * Setup timer 1 compare match A to generate a tick interrupt.
 */
static void prvSetupTimerInterrupt( void )
{
	uint32_t ulCompareMatch;
	// uint8_t ucHighByte, 
    uint8_t ucLowByte;


	/* Using 16bit timer 1 to generate the tick.  Correct fuses must be
	selected for the configCPU_CLOCK_HZ clock. */

	ulCompareMatch = configCPU_CLOCK_HZ / configTICK_RATE_HZ;

	/* We only have 16 bits so have to scale to get our required tick rate. */
	ulCompareMatch /= portCLOCK_PRESCALER;

	/* Adjust for correct value. */
	ulCompareMatch -= ( uint32_t ) 1;

	/* Setup compare match value for compare match A.  Interrupts are disabled 
	before this is called so we need not worry here. */
	OCR1A = ulCompareMatch;

	/* Setup clock source and compare match behaviour. */
	TCCR1A &= ~(_BV(WGM11) | _BV(WGM10));
	ucLowByte = portCLEAR_COUNTER_ON_MATCH | portPRESCALE_64;
	TCCR1B = ucLowByte;

	/* Enable the interrupt - this is okay as interrupt are currently globally
	disabled. */
	ucLowByte = TIMSK1;
	ucLowByte |= portCOMPARE_MATCH_A_INTERRUPT_ENABLE;
	TIMSK1 = ucLowByte;
}
/*-----------------------------------------------------------*/

#if configUSE_PREEMPTION == 1

	/*
	 * Tick ISR for preemptive scheduler.  We can use a naked attribute as
	 * the context is saved at the start of vPortYieldFromTick().  The tick
	 * count is incremented after the context is saved.
	 */
	void TIMER1_COMPA_vect( void ) __attribute__ ( ( signal, naked ) );
	void TIMER1_COMPA_vect( void )
	{
		vPortYieldFromTick();
		asm volatile ( "reti" );
	}
#else

	/*
	 * Tick ISR for the cooperative scheduler.  All this does is increment the
	 * tick count.  We don't need to switch context, this can only be done by
	 * manual calls to taskYIELD();
	 */
	void TIMER1_COMPA_vect( void ) __attribute__ ( ( signal ) );
	void TIMER1_COMPA_vect( void )
	{
		xTaskIncrementTick();
	}
#endif


	
