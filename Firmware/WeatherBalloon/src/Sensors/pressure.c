/*
 * pressure.c
 *
 *  Created on: Jun 19, 2016
 *      Author: Summer
 */


#include "Sensors/pressure.h"
#include "descriptors.h"
#include "flash.h"

STATIC_UBUF(press_data_buff,  FLASH_PAGE_SIZE_BYTES);   /* Allocate USB receive buffer.   */

Press_Data Press_Read(void)
{
	Press_Data pressdata;

	//Todo: actually read this data
	pressdata.pressure = 0xFFFFAAAA;
	pressdata.temperature = 0xABCD1234;


	return pressdata;
}
void Press_Read_Tsk(void)
{
	//Read
	//Write to tem_dat_buff
	//If address gets to FLASH_PAGE_SIZE_BYTES
	//then write temp_data_buff to flash
}
