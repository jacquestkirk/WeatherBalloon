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

/*******************************************************
 * API EXAMPLE CODE
 *******************************************************

#include "spidrv.h"

SPIDRV_HandleData_t handleData;
SPIDRV_Handle_t handle = &handleData;

void TransferComplete( SPIDRV_Handle_t handle,
                       Ecode_t transferStatus,
                       int itemsTransferred )
{
  if ( transferStatus == ECODE_EMDRV_SPIDRV_OK )
  {
    // Success !
  }
}

int main( void )
{
  uint8_t buffer[10];
  SPIDRV_Init_t initData = SPIDRV_MASTER_USART2;

  // Initialize a SPI driver instance
  SPIDRV_Init( handle, &initData );

  // Transmit data using a blocking transmit function
  SPIDRV_MTransmitB( handle, buffer, 10 );

  // Transmit data using a callback to catch transfer completion.
  SPIDRV_MTransmit( handle, buffer, 10, TransferComplete );
}

 *******************************************************
 *******************************************************
********************************************************/
