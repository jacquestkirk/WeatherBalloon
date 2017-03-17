/*
 * timestamp.h
 *
 *  Created on: Nov 6, 2016
 *      Author: Summer
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <stdint.h>

#define TIMESTAMP_SIZE_BYTES 4


void Time_Initilize_TimeStamp(void);
void WriteTimestampToFlashBuffer(uint32_t dataToWrite);

uint32_t Time_Get_TimeStamp(void);
void Time_Record_TimeStamp(void);
void Time_Read_Tsk(void);

uint8_t TimeStamp_QueryReadyToWriteFlashFlag(void);
void TimeStamp_ClearReadyToWriteFlashFlag(void);
uint8_t* TimeStamp_GetBufferAddress(void);

void Time_ReArm_Timeout(void);


#endif /* TIMESTAMP_H_ */
