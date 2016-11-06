/*
 * timestamp.c
 *
 *  Created on: Nov 6, 2016
 *      Author: Summer
 */


#include "timestamp.h"
#include "em_cmu.h"
#include "em_rtc.h"
#include "rtcdriver.h"

void RtcCallback( RTCDRV_TimerID_t, void*);

RTCDRV_TimerID_t id;

void Time_Initilize_TimeStamp(void)
{
	/* Setup RTC with selected clock source and prescaler. */
	RTCDRV_Init();
	/*RTCDRV_AllocateTimer( &id );
	// Start a oneshot timer with 100 millisecond timeout
	// Todo: I should move this somewhere else
	RTCDRV_StartTimer( id, rtcdrvTimerTypeOneshot, SCH_SCHEDULERPERIOD_MS, RtcCallback,(void*)junk);
	*/

	//tbd
}


uint32_t Time_Get_TimeStamp(void)
{
	return RTCDRV_GetWallClockTicks32();
}

/*void RtcCallback( RTCDRV_TimerID_t ignore_me, void* ignore_me_too)
{
  // Timer has elapsed !

  // Restart timer
  RTCDRV_StartTimer( id, rtcdrvTimerTypePeriodic, SCH_SCHEDULERPERIOD_MS, RtcCallback, (void*)junk);
}*/
