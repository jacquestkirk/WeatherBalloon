/*
 * magnetometer.h
 *
 *  Created on: Jun 5, 2016
 *      Author: Summer
 */

#ifndef MAGNETOMETER_H_
#define MAGNETOMETER_H_

#define MAG_DATA_SIZE_BYTES 6
#include <stdint.h>
#include "Sensors/lsm9ds_defs.h"
typedef struct Mag_Data {
	int16_t x_mag;
	int16_t y_mag;
	int16_t z_mag;
} Mag_Data;

Mag_Data Mag_Read(void);//Read and return
void Mag_Read_Tsk(void);//Read and store in local ram

void Mag_WriteRegister1Byte(uint8_t reg, uint8_t data);
uint8_t Mag_QueryRegister1Byte(uint8_t reg);
Mag_Data QueryAllMagValues();
void Mag_Initialize_OneShot();

uint8_t Mag_QueryReadyToWriteFlashFlag(void);
void Mag_ClearReadyToWriteFlashFlag(void);
uint8_t* Mag_GetBufferAddress(void);

#endif /* MAGNETOMETER_H_ */
