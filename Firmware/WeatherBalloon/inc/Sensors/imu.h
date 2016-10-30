/*
 * imu.h
 *
 *  Created on: Jun 5, 2016
 *      Author: Summer
 */

#ifndef IMU_H_
#define IMU_H_

#include <stdint.h>
#include "Sensors/lsm9ds_defs.h"

#define IMU_DATA_SIZE_BYTES 12
#define IMU_FIFO_SIZE 32


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
void Imu_Initialize(void); //Initialize Imu streaming
void Imu_Initialize_OneShot();//Initialize Imu in one shot mode
uint8_t Imu_TestFunction();
uint8_t Imu_QueryRegister1Byte(uint8_t reg);
void Imu_WriteRegister1Byte(uint8_t reg, uint8_t data);
void Imu_StartFifo(void);
void Imu_StopFifo(void);
void Imu_WriteStreamFifo(void);

uint8_t Imu_QueryReadyToWriteFlashFlag(void);
void Imu_ClearReadyToWriteFlashFlag(void);
uint8_t* Imu_GetBufferAddress(void);

//IMU Settings

#endif /* IMU_H_ */
