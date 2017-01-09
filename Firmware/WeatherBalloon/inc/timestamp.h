/*
 * timestamp.h
 *
 *  Created on: Nov 6, 2016
 *      Author: Summer
 */

#ifndef TIMESTAMP_H_
#define TIMESTAMP_H_

#include <stdint.h>

void Time_Initilize_TimeStamp(void);

uint32_t Time_Get_TimeStamp(void);

uint8_t TimeStamp_QueryReadyToWriteFlashFlag(void);
void TimeStamp_ClearReadyToWriteFlashFlag(void);
uint8_t* TimeStamp_GetBufferAddress(void);

#endif /* TIMESTAMP_H_ */
