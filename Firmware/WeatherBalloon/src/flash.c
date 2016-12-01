/*
 * flash.c
 *
 *  Created on: Jul 2, 2016
 *      Author: manofwar
 */


#include "flash.h"
#include "timestamp.h"
#include "SPIBubbl.h"
#include "Sensors/imu.h"
#include "Sensors/magnetometer.h"
#include "Sensors/pressure.h"
#include "Sensors/temperature.h"

// Current offsets in Flash
int Flash_Offset_Timestamp = 0;
int Flash_Offset_Imu = 0;
int Flash_Offset_Magnetometer = 0;
int Flash_Offset_Pressure = 0;
int Flash_Offset_Temp_Pressure = 0;
int Flash_Offset_Temp1 = 0;
int Flash_Offset_Temp2 = 0;
int Flash_Offset_Temp3 = 0;
int Flash_Offset_Temp4 = 0;
int Flash_Offset_Temp_MCU = 0;


uint8_t page_read[256];
uint8_t SPIWrite[260];
uint8_t FlashRead[256];
// Add a few dummy arrays that can be written to flash via CLI command


void Flash_Initilize()
{
	/*
	 * Flash initialize... every power-up, read first byte of every page.
	 * Populate current_offsets accordingly.
	 */

	for (int k = FLASH_PAGE0_TIMESTAMP; k < FLASH_PAGE0_IMU; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Timestamp = k - FLASH_PAGE0_TIMESTAMP;}
	}
	for (int k = FLASH_PAGE0_IMU; k < FLASH_PAGE0_MAGNETOMETER; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Imu = k - FLASH_PAGE0_IMU;}
	}
	for (int k = FLASH_PAGE0_MAGNETOMETER; k < FLASH_PAGE0_PRESSURE; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Timestamp = k - FLASH_PAGE0_TIMESTAMP;}
	}
	for (int k = FLASH_PAGE0_PRESSURE; k < FLASH_PAGE0_TEMP_PRESSURE; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Pressure = k - FLASH_PAGE0_PRESSURE;}
	}
	for (int k = FLASH_PAGE0_TEMP_PRESSURE; k < FLASH_PAGE0_TEMP1; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Temp_Pressure = k - FLASH_PAGE0_TEMP_PRESSURE;}
	}
	for (int k = FLASH_PAGE0_TEMP1; k < FLASH_PAGE0_TEMP2; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Temp1 = k - FLASH_PAGE0_TEMP1;}
	}
	for (int k = FLASH_PAGE0_TEMP2; k < FLASH_PAGE0_TEMP3; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Temp2 = k - FLASH_PAGE0_TEMP2;}
	}
	for (int k = FLASH_PAGE0_TEMP3; k < FLASH_PAGE0_TEMP4; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Temp3 = k - FLASH_PAGE0_TEMP3;}
	}
	for (int k = FLASH_PAGE0_TEMP4; k < FLASH_PAGE0_TEMP_MCU; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Temp4 = k - FLASH_PAGE0_TEMP4;}
	}
	for (int k = FLASH_PAGE0_TEMP_MCU; k < 32767; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Temp_MCU = k - FLASH_PAGE0_TEMP_MCU;}
	}
}

void Flash_Run()
{
	// Add function here manage the whole module
	// Check to see if flash is in the middle of write
	//    also keep track of last module flash invoked so appropriate flag can be cleared
	//	If not middle of write, then go through the list of Flash_Write_Imu, Flash_Write_Mag, etc
}

void Flash_Write_Time(int offset)
{
	// Ping Query... If ready... then
	//if (Time_QueryReadyToWriteFlashFlag())
	{
		// getbufferaddress... ask IMU block which of the two buffers to get data from
		// Write the data by invoking FlashWritePage()
		// ... Then when all good, clearreadytowrite in IMU module

		//Time_GetBufferAddress(void)

	}

	//Time_ClearReadyToWriteFlashFlag(void) // This should prob be in the Flash management function

}

void Flash_Write_Imu(int offset)
{
	// Ping Query... If ready... then
	if (Imu_QueryReadyToWriteFlashFlag())
	{
			// getbufferaddress... ask IMU block which of the two buffers to get data from
		//Imu_GetBufferAddress(void)

		// Write the data by invoking FlashWritePage( ... )

		// ... Then when all good, clearreadytowrite in IMU module
	}

	//Imu_ClearReadyToWriteFlashFlag(void) // This should prob be in the Flash management function

/*
 * uint8_t Imu_QueryReadyToWriteFlashFlag(void) // stored in IMU module
{
	return _imuReadyToWriteFlash;
}
uint8_t* Imu_GetBufferAddress(void) // stored in IMU module
{
	return imu_data_buffer_to_write;
}

void Imu_ClearReadyToWriteFlashFlag(void) // stored in IMU module
{
	_imuReadyToWriteFlash = 0;
}

 *
 *
 */


}

void Flash_Write_Mag(int offset)
{
	// Ping Query... If ready... then
	//if (Mag_QueryReadyToWriteFlashFlag())
	{
		// getbufferaddress... ask IMU block which of the two buffers to get data from
		// Write the data by invoking FlashWritePage()
		// ... Then when all good, clearreadytowrite in IMU module

		//Mag_GetBufferAddress(void)

	}

	//Mag_ClearReadyToWriteFlashFlag(void) // This should prob be in the Flash management function

}

void Flash_Write_Pressure(int offset)
{

	// Ping Query... If ready... then
	//if (Press_QueryReadyToWriteFlashFlag())
	{
		// getbufferaddress... ask IMU block which of the two buffers to get data from
		// Write the data by invoking FlashWritePage()
		// ... Then when all good, clearreadytowrite in IMU module

		//Press_GetBufferAddress(void)

	}

	//Press_ClearReadyToWriteFlashFlag(void) // This should prob be in the Flash management function

}

void Flash_Write_Temp(int offset)
{
	// Ping Query... If ready... then
	//if (Temp_QueryReadyToWriteFlashFlag())
	{
		// getbufferaddress... ask IMU block which of the two buffers to get data from
		// Write the data by invoking FlashWritePage()
		// ... Then when all good, clearreadytowrite in IMU module

		//Temp_GetBufferAddress(void)

	}

	//Temp_ClearReadyToWriteFlashFlag(void) // This should prob be in the Flash management function

}

// Used for both CLI debug and normal operation.
void Flash_Write_Page(int pagenum)
{
	// Flash command: Byte0 = Write Instruction = 0x02.
	SPIWrite[0] = 0x02;

	// Bytes3,2,1 = Flash Page Address = pagenum.
	SPIWrite[3] = pagenum && 0xFF;
	SPIWrite[2] = (pagenum >> 8) && 0xFF;
	SPIWrite[1] = (pagenum >> 16) && 0xFF;

	// Byte4 = DATA0 = 0x00 (DATA0 byte is Page written status byte).
	SPIWrite[4] = 0x00;

	// Invoke SPI write with *SPIWrite
	//		SPIBubbl_Transmit(SPIWrite, 260);
}
// Have an overloaded version of Flash_Write_Page() with an enum for debug-writing one
// of the dummy arrays... overloaded version will set up the SPIWrite array and
// then call the original Flash_Write_Page() function.
// Passing
void Flash_Write_Page(int pagenum, Flash_Enum_Test_Data_type write_dataset)
{
	if (write_dataset == FLASH_ENUM_ALLZEROS)
	{
		for(int k = 5; k < (FLASH_PAGE_SIZE_BYTES+4); k++)
		SPIWrite[k] = 0;
	}
	if (write_dataset == FLASH_ENUM_ALLONES)
		{
			for(int k = 5; k < (FLASH_PAGE_SIZE_BYTES+4); k++)
			SPIWrite[k] = 1;
		}

    //FLASH_ENUM_PRIMES
    //FLASH_ENUM_RANDOM
}


// Returns byte .. Used in normal mode of operation
uint8_t Flash_Read_FirstByte(int pagenum)
{
	int readbyte;
	// Read byte 0 at pagenum
	return readbyte;
}

// Only used for CLI mode of operation
uint8_t * Flash_Read_Page(int pagenum)
{
	// Do the reading at pagenum and store into FlashRead[]
	return FlashRead;
}

