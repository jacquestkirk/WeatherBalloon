/*
 * BackupTimer.c
 *
 *  Created on: Mar 11, 2017
 *      Author: Summer
 */

#include "BackupTimer.h"
#include "em_timer.h"
#include "sensors/led.h"
void TIMER1_IRQHandler(void)
{
  TIMER_IntClear(TIMER1, TIMER_IF_OF);      // Clear overflow flag
  Led_Toggle_2();
}

void BackupTimer_Init(void)
{
	TIMER_TopSet(TIMER1, 1000);               // Set timer TOP value
	TIMER_Init_TypeDef timerInit =            // Setup Timer initialization
	{
	.enable     = true,                     // Start timer upon configuration
	.debugRun   = true,                     // Keep timer running even on debug halt
	.prescale   = timerPrescale1,           // Use /1 prescaler...timer clock = HF clock = 1 MHz
	.clkSel     = timerClkSelHFPerClk,      // Set HF peripheral clock as clock source
	.fallAction = timerInputActionNone,     // No action on falling edge
	.riseAction = timerInputActionNone,     // No action on rising edge
	.mode       = timerModeUp,              // Use up-count mode
	.dmaClrAct  = false,                    // Not using DMA
	.quadModeX4 = false,                    // Not using quad decoder
	.oneShot    = false,                    // Using continuous, not one-shot
	.sync       = false,                    // Not synchronizing timer operation off of other timers
	};
	TIMER_IntEnable(TIMER1, TIMER_IF_OF);     // Enable Timer1 overflow interrupt
	NVIC_EnableIRQ(TIMER1_IRQn);              // Enable TIMER1 interrupt vector in NVIC
	TIMER_Init(TIMER1, &timerInit);           // Configure and start Timer1
}


