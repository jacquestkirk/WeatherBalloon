/*
 * SPIBubbl.c
 *
 *  Created on: Nov 3, 2016
 *      Author: Mubashir
 */


#include "SPIBubbl.h"

SPIDRV_HandleData_t handleData;
SPIDRV_Handle_t handle = &handleData;


// Initilize SPI
void SPIBubbl_Initalize(void)
{
	// Set up the necessary peripheral clocks
	CMU_ClockEnable(cmuClock_GPIO, true);

	GPIO_DriveModeSet(gpioPortA, gpioDriveModeLow);

	// Enable the GPIO pins for the misc signals, leave pulled high
	GPIO_PinModeSet(gpioPortA, 9, gpioModePushPullDrive, 1);          // WP#
	GPIO_PinModeSet(gpioPortA, 10, gpioModePushPullDrive, 1);          // HOLD#

    // Initialize and enable the SPIDRV
	SPIDRV_Init_t initData = SPIDRV_MASTER_USART1;
	initData.clockMode = spidrvClockMode0;  // Flash can operate in mode 0 or 1
	initData.csControl = spidrvCsControlApplication;

	// Initialize a SPI driver instance
	SPIDRV_Init( handle, &initData );
}

void SPIBubbl_Transmit(void *buffer, int size)
{
	// Transmit data using a blocking transmit function
	SPIDRV_MTransmitB(handle, buffer, size);

}

void SPIBubbl_Receive_Page(void *page_data, int page)
{
	// Receive data using a blocking transmit function
	SPIDRV_MReceiveB(handle, page_data, 256);

}

void SPIBubbl_Read_ID(void *readID)
{
	uint8_t tx_data[4];

	tx_data[0] = 0x40; //JEDEC_ID_CMD

	SPIDRV_MTransferB( handle, &tx_data, readID, 4);

}
