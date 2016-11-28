/*
 * flash.c
 *
 *  Created on: Jul 2, 2016
 *      Author: manofwar
 */


#include "flash.h"
// #include "TIMESTAMPERDUDENONAMEYETYO.h"
#include "SPIBubbl.h"
#include "Sensors/imu.h"
#include "Sensors/magnetometer.h"
#include "Sensors/pressure.h"
#include "Sensors/temperature.h"

uint8_t page_read[256];

/*
 * Flash initialize... every power-up, read first byte of every page.
 * Populate current_offsets accordingly.
 */


//
uint8_t SPIWrite[260];
uint8_t FlashRead[256];

void Flash_Write_Imu(int offset)
{
	// Ping Query... If ready... then
		// getbufferaddress... ask IMU block which of the two buffers to get data from
		// Write the data by invoking FlashWritePage()
	// ... Then when all good, clearreadytowrite in IMU module

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

}

void Flash_Write_Pressure(int offset)
{

}

void Flash_Write_Temp(int offset)
{

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

		// Byte4 = DATA0 = 0x00 (Page written byte).
		SPIWrite[4] = 0x00;

		// Invoke SPI write with *SPIWrite
		//		SPIBubbl_Transmit(SPIWrite, 260);
}

// Only used for CLI mode of operation
void * Flash_Read_Page(int pagenum)
{
	// Do the reading at pagenum and store into FlashRead[]
	return FlashRead;
}

// Only used for CLI mode of operation
void * Flash_GetWriteBufferAddress()
{
	return &SPIWrite[5];
}
