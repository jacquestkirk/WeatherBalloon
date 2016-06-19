/*
 * magnetometer.c
 *
 *  Created on: Jun 19, 2016
 *      Author: Summer
 */


#include "Sensors/magnetometer.h"
#include "descriptors.h"
#include "flash.h"

STATIC_UBUF(mag_data_buff,  FLASH_PAGE_SIZE_BYTES);   /* Allocate USB receive buffer.   */

Mag_Data Mag_Read(void)
{
	Mag_Data magdata;

	//Todo: actually read this data
	magdata.x_mag  = 0x1111;
	magdata.y_mag  = 0x2222;
	magdata.z_mag  = 0x3333;



	return magdata;
}
void Mag_Read_Tsk(void)
{
	//Read
	//Write to tem_dat_buff
	//If address gets to FLASH_PAGE_SIZE_BYTES
	//then write temp_data_buff to flash
}
