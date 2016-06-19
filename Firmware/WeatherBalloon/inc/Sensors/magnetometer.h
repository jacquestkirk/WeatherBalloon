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
typedef struct Mag_Data {
	int16_t x_mag;
	int16_t y_mag;
	int16_t z_mag;
} Mag_Data;

Mag_Data Mag_Read(void);//Read and return
void Mag_Read_Tsk(void);//Read and store in local ram

#endif /* MAGNETOMETER_H_ */
