/*
 * imu.h
 *
 *  Created on: Jun 5, 2016
 *      Author: Summer
 */

#ifndef IMU_H_
#define IMU_H_

#define IMU_DATA_SIZE_BYTES 12
#include <stdint.h>
#include "Sensors/lsm9ds_defs.h"


typedef struct Imu_Data {
	int16_t x_accel;
	int16_t y_accel;
	int16_t z_accel;
	int16_t x_gyro;
	int16_t y_gyro;
	int16_t z_gyro;
} Imu_Data;


Imu_Data Imu_Read(void); //Read and return
void Imu_Read_Tsk(void); //Read and store in local ram
void Imu_Initialize(void);
uint8_t Imu_TestFunction();


//IMU Settings

#endif /* IMU_H_ */
