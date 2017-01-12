/*
 * pressure.h
 *
 *  Created on: Jun 5, 2016
 *      Author: Summer
 */

#ifndef PRESSURE_H_
#define PRESSURE_H_

#define PRESS_DATA_SIZE_BYTES 8
#include <stdint.h>
typedef struct Press_Data{
	int32_t pressure;
	int32_t temperature;
}Press_Data;

Press_Data Press_Read(void);//Read and return
void Press_Read_Tsk(void);//Read and store in local ram

uint32_t Press_QueryRegister3Byte(uint8_t command);
uint16_t Press_QueryRegister2Byte(uint8_t command);
void Press_WriteCommandByte(uint8_t command);

uint8_t Press_QueryReadyToWriteFlashFlag(void);
void Press_ClearReadyToWriteFlashFlag(void);
uint8_t* Press_GetBufferAddress(void);
uint16_t Press_ReadCalibrations(int calibration_num);

#endif /* PRESSURE_H_ */
