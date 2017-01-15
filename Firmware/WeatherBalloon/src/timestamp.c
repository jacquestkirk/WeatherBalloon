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
#include "descriptors.h"
#include "flash.h"
#include "HelperFunctions.h"

void RtcCallback( RTCDRV_TimerID_t, void*);

RTCDRV_TimerID_t id;

STATIC_UBUF(timestamp_data_buff1,  FLASH_PAGE_SIZE_BYTES);   /* Allocate USB receive buffer.   */
STATIC_UBUF(timestamp_data_buff2,  FLASH_PAGE_SIZE_BYTES);   /* Allocate USB receive buffer.   */
uint8_t *timestamp_data_buffs [2] = {timestamp_data_buff1, timestamp_data_buff2};
uint8_t *timestamp_current_data_buffer = timestamp_data_buff1;
uint8_t *timestamp_data_buffer_to_write = timestamp_data_buff1;


uint8_t _timestampActiveBuffer = 0;
uint8_t _timestampReadyToWriteFlash = 0;

uint32_t current_time;

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

void Time_Record_TimeStamp(void)
{
	current_time = RTCDRV_GetWallClockTicks32();
}

void Time_Read_Tsk(void)
{
	WriteTimestampToFlashBuffer(current_time);
}


void WriteTimestampToFlashBuffer(uint32_t dataToWrite)
{
	static int location_in_buffer = 0;

	//check if this write will overflow the current buffer.
	//If it does, move to the next buffer and signal that the old buffer is ready to be written to flash

	if(location_in_buffer + TIMESTAMP_SIZE_BYTES > FLASH_PAGE_SIZE_BYTES - 1)
	{
		timestamp_data_buffer_to_write = timestamp_current_data_buffer;
		_timestampReadyToWriteFlash = 1;

		if(_timestampActiveBuffer)
		{
			timestamp_current_data_buffer = timestamp_data_buffs[0];
			_timestampActiveBuffer = 0;
		}
		else
		{
			timestamp_current_data_buffer = timestamp_data_buffs[1];;
			_timestampActiveBuffer = 1;
		}
		location_in_buffer = 0;

	}
	location_in_buffer = Helper_Write_32bit_To_Buffer(timestamp_current_data_buffer, location_in_buffer, dataToWrite);

}

/*void RtcCallback( RTCDRV_TimerID_t ignore_me, void* ignore_me_too)
{
  // Timer has elapsed !

  // Restart timer
  RTCDRV_StartTimer( id, rtcdrvTimerTypePeriodic, SCH_SCHEDULERPERIOD_MS, RtcCallback, (void*)junk);
}*/

uint8_t TimeStamp_QueryReadyToWriteFlashFlag(void)
{
	return _timestampReadyToWriteFlash;
}

void TimeStamp_ClearReadyToWriteFlashFlag(void)
{
	_timestampReadyToWriteFlash = 0;
}

uint8_t* TimeStamp_GetBufferAddress(void)
{
	if(_timestampActiveBuffer == 1)
	{
		return timestamp_data_buff1;
	}
	else
	{
		return timestamp_data_buff2;
	}
}
