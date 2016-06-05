/*
 * temperature.c
 *
 *  Created on: Jun 5, 2016
 *      Author: Summer
 */

#include "Sensors/temperature.h"
#include "descriptors.h"
#include "flash.h"

STATIC_UBUF(temp_data_buff,  FLASH_PAGE_SIZE_BYTES);   /* Allocate USB receive buffer.   */

Temp_Data Temp_Read(void)
{
	Temp_Data tempdata;

	//Todo: actually read this data
	tempdata.therm1 = 0xFFFF;
	tempdata.therm2 = 0x0000;
	tempdata.therm3 = 0xF0A0;
	tempdata.therm4 = 0x1234;

	return tempdata;
}
void Temp_Read_Tsk(void)
{
	//Read
	//Write to tem_dat_buff
	//If address gets to FLASH_PAGE_SIZE_BYTES
	//then write temp_data_buff to flash
}
