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

// Current Flash write block
typedef enum {
	Flash_Currently_Writing_Timestamp,
	Flash_Currently_Writing_Imu,
	Flash_Currently_Writing_Magnetometer,
	Flash_Currently_Writing_Pressure,
	Flash_Currently_Writing_Temp_Pressure,
	Flash_Currently_Writing_Temp1,
	Flash_Currently_Writing_Temp2,
	Flash_Currently_Writing_Temp3,
	Flash_Currently_Writing_Temp4,
	Flash_Currently_Writing_Temp_MCU,
	Flash_Currently_Writing_Test_Data,
	Flash_Currently_Writing_Nothing
} Flash_Enum_Current_Write_Block;

Flash_Enum_Current_Write_Block Flash_Current_Write_Block;

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
uint8_t FlashRead[260];
uint8_t FlashReadID[4];
uint8_t *Flash_data_to_write;
// Add a few dummy arrays that can be written to flash via CLI command


/* void Flash_Initialize()
 * Parameters: None
 * Return: void
 * Description: Flash initialize.  At every power-up, read first byte of every page.
 * Populate current_offsets accordingly.
 */
void Flash_Initialize()
{

	Flash_Current_Write_Block = Flash_Currently_Writing_Nothing;

	Flash_Offset_Timestamp = FLASH_PAGE0_TIMESTAMP;
	Flash_Offset_Imu = FLASH_PAGE0_IMU;
	Flash_Offset_Magnetometer = FLASH_PAGE0_MAGNETOMETER;
	Flash_Offset_Pressure = FLASH_PAGE0_PRESSURE;
	Flash_Offset_Temp_Pressure = FLASH_PAGE0_TEMP_PRESSURE;
	Flash_Offset_Temp1 = FLASH_PAGE0_TEMP1;
	Flash_Offset_Temp2 = FLASH_PAGE0_TEMP2;
	Flash_Offset_Temp3 = FLASH_PAGE0_TEMP3;
	Flash_Offset_Temp4 = FLASH_PAGE0_TEMP4;
	Flash_Offset_Temp_MCU = FLASH_PAGE0_TEMP_MCU;

	for (int k = FLASH_PAGE0_TIMESTAMP; k < FLASH_PAGE0_IMU; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Timestamp = k - FLASH_PAGE0_TIMESTAMP + 1;}
	}
	for (int k = FLASH_PAGE0_IMU; k < FLASH_PAGE0_MAGNETOMETER; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Imu = k - FLASH_PAGE0_IMU + 1;}
	}
	for (int k = FLASH_PAGE0_MAGNETOMETER; k < FLASH_PAGE0_PRESSURE; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Magnetometer = k - FLASH_PAGE0_MAGNETOMETER + 1;}
	}
	for (int k = FLASH_PAGE0_PRESSURE; k < FLASH_PAGE0_TEMP_PRESSURE; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Pressure = k - FLASH_PAGE0_PRESSURE + 1;}
	}
	for (int k = FLASH_PAGE0_TEMP_PRESSURE; k < FLASH_PAGE0_TEMP1; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Temp_Pressure = k - FLASH_PAGE0_TEMP_PRESSURE + 1;}
	}
	for (int k = FLASH_PAGE0_TEMP1; k < FLASH_PAGE0_TEMP2; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Temp1 = k - FLASH_PAGE0_TEMP1 + 1;}
	}
	for (int k = FLASH_PAGE0_TEMP2; k < FLASH_PAGE0_TEMP3; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Temp2 = k - FLASH_PAGE0_TEMP2 + 1;}
	}
	for (int k = FLASH_PAGE0_TEMP3; k < FLASH_PAGE0_TEMP4; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Temp3 = k - FLASH_PAGE0_TEMP3 + 1;}
	}
	for (int k = FLASH_PAGE0_TEMP4; k < FLASH_PAGE0_TEMP_MCU; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Temp4 = k - FLASH_PAGE0_TEMP4 + 1;}
	}
	for (int k = FLASH_PAGE0_TEMP_MCU; k < 32767; k++)
	{
		if (Flash_Read_FirstByte(k) == 0) {Flash_Offset_Temp_MCU = k - FLASH_PAGE0_TEMP_MCU + 1;}
	}
}

/* void Flash_Run_Periodic()
 * Parameters: None
 * Return: void
 * Description: Flash manager.  Checks to see if flash is in the middle of write.
 * If not in the middle of a write, then it goes through the list of Flash_Write_xxxxx()
 * functions, where xxxxx is the appropriate sensor.
 * Also keeps track of last module flash invoked so appropriate flag can be cleared.
 */
void Flash_Run_Periodic()
{

	if (Flash_Current_Write_Block != Flash_Currently_Writing_Nothing) // as in it's writing something
	{
		/*Check if the flash writing is done.  If so, then
		 *       1. Set Flash_Current_Write_Block = Flash_Currently_Writing_Nothing
			// increase current offset and check for overflow
			// then clearreadytowrite in IMU module
		 *
		 *    else break out of Flash_Run()... is "return;" okay for this??
		*/
	}


	if (Imu_QueryReadyToWriteFlashFlag())
	{
		if (FLASH_PAGE0_IMU + Flash_Offset_Imu < FLASH_PAGE0_MAGNETOMETER)
		{
			// Record Error code ().. then...
			Imu_ClearReadyToWriteFlashFlag();
		}
		else
			Flash_Current_Write_Block = Flash_Currently_Writing_Imu;
			Flash_Write_Imu();
			Flash_Offset_Imu++;
		// Break out of Flash_Run() function... is "return;" a valid statement for this??
	}

	if (Mag_QueryReadyToWriteFlashFlag())
		{
			if (FLASH_PAGE0_MAGNETOMETER + Flash_Offset_Magnetometer < FLASH_PAGE0_PRESSURE)
			{
				// Record Error code ().. then...
				Mag_ClearReadyToWriteFlashFlag();
			}
			else
				Flash_Current_Write_Block = Flash_Currently_Writing_Magnetometer;
				Flash_Write_Mag();
				Flash_Offset_Magnetometer++;
			// Break out of Flash_Run() function... is "return;" a valid statement for this??
		}

}

/* void Flash_Populate_Write_Buffer()
 * Parameters: None
 * Return: void
 * Description: Populates SPIWrite buffer with current data buffer.
 */

void Flash_Write_Time()
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

void Flash_Write_Imu()
{
	Flash_data_to_write = Imu_GetBufferAddress();
	Flash_Populate_Write_Buffer();
	Flash_Write_Page(FLASH_PAGE0_IMU + Flash_Offset_Imu);
	//  SHOULD THIS BE IN FLASH_RUN() ??? Imu_ClearReadyToWriteFlashFlag();
}

void Flash_Write_Mag()
{
	Flash_data_to_write = Mag_GetBufferAddress();
	Flash_Populate_Write_Buffer();
	Flash_Write_Page(FLASH_PAGE0_MAGNETOMETER + Flash_Offset_Magnetometer);
	//  SHOULD THIS BE IN FLASH_RUN() ??? Mag_ClearReadyToWriteFlashFlag();
}

void Flash_Write_Pressure()
{
	//Flash_data_to_write = Press_GetBufferAddress();
	//Flash_Populate_Write_Buffer();
	//Flash_Write_Page(FLASH_PAGE0_PRESSURE + Flash_Offset_Pressure);
	//  SHOULD THIS BE IN FLASH_RUN() ??? Press_ClearReadyToWriteFlashFlag();
}

void Flash_Write_Temp()
{
	//Flash_data_to_write = Temp_GetBufferAddress();
	//Flash_Populate_Write_Buffer();
	//Flash_Write_Page(FLASH_PAGE0_TEMP_PRESSURE + Flash_Offset_Temp_Pressure);
	//  SHOULD THIS BE IN FLASH_RUN() ??? ..._ClearReadyToWriteFlashFlag();
	// Should also do for TEMP1,2,3,4, and MCU
}

void Flash_Populate_Write_Buffer()
{
	SPIWrite[4] = 0x00;
	for(int k = 5; k < (FLASH_PAGE_SIZE_BYTES+4); k++)
	{
		SPIWrite[k] = Flash_data_to_write[k-5];
	}
}

// Sets up and invoke SPI command to write a full page in flash
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
	SPIBubbl_Transmit(SPIWrite, 260);
}

// Writes test data of a type defined in Flash_Enum_Test_Data_type in page # pagenum
void Flash_Write_Page_Test_Data(int pagenum, Flash_Enum_Test_Data_type write_dataset)
{

	// Fill SPIWrite array with data to be written to page pagenum
	switch (write_dataset)
	{
	case ALLZEROS:
		for(int k = 5; k < (FLASH_PAGE_SIZE_BYTES+4); k++)
				SPIWrite[k] = 0;
		break;
	case ALLONES:
		for(int k = 5; k < (FLASH_PAGE_SIZE_BYTES+4); k++)
						SPIWrite[k] = 1;
		break;
	case RANDOM:
		for(int k = 5; k < (FLASH_PAGE_SIZE_BYTES+4); k++)
						SPIWrite[k] = (uint8_t) (rand() % 256);
		break;
	}

	// Write page
	Flash_Write_Page(pagenum);
}


// Returns byte .. Used in normal mode of operation
uint8_t Flash_Read_FirstByte(int pagenum)
{
	uint8_t readbyte;
	// Read byte 0 at pagenum
	readbyte = 0x00;
	return readbyte;
}

// Only used for CLI mode of operation
uint8_t * Flash_Read_Page(int pagenum)
{
	// Do the reading at pagenum and store into FlashRead[]
	// Then copy FlashRead[4..259] to page_read[0..255]

	for(int i=0; i<=255; i++)
	{
		page_read[i] = FlashRead[i+4];
	}
	return page_read;
}

// Returns first 3 bytes of Flash Identification
// 0x20 20 17
uint8_t * Flash_Read_ID()
{
	// Do the reading at pagenum and store into FlashRead[]
	// Then copy FlashRead[4..6] to page_read[0..2]

	Flash_Initialize();

	SPIBubbl_Read_ID(FlashReadID);

	return &FlashReadID[1];
}
