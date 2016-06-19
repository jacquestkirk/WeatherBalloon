/*
 * imu.c
 *
 *  Created on: Jun 19, 2016
 *      Author: Summer
 */


#include "Sensors/imu.h"
#include "descriptors.h"
#include "flash.h"

STATIC_UBUF(imu_data_buff,  FLASH_PAGE_SIZE_BYTES);   /* Allocate USB receive buffer.   */

Imu_Data Imu_Read(void)
{
	Imu_Data imudata;

	//Todo: actually read this data
	imudata.x_accel = 0x1234;
	imudata.y_accel = 0x5678;
	imudata.z_accel = 0x9abc;
	imudata.x_gyro  = 0xdeff;
	imudata.y_gyro  = 0xedcb;
	imudata.z_gyro  = 0xa987;


	return imudata;
}
void Imu_Read_Tsk(void)
{
	//Read
	//Write to tem_dat_buff
	//If address gets to FLASH_PAGE_SIZE_BYTES
	//then write temp_data_buff to flash
}
