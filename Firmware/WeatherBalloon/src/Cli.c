/*
 * Cli.c
 *
 *  Created on: May 22, 2016
 *      Author: Summer
 */


#include <stdio.h>
#include "em_usb.h"
#include "em_cmu.h"
#include "em_usb.h"
//#include "cdc.h"
#include "descriptors.h"

#include "Cli.h"

#define USB_TIMEOUT 100


static void StateChangeCallback( USBD_State_TypeDef oldState, USBD_State_TypeDef newState );
static int  UsbDataReceived(USB_Status_TypeDef status, uint32_t xferred,
                            uint32_t remaining);
static int  LineCodingReceived(USB_Status_TypeDef status,
                               uint32_t xferred,
                               uint32_t remaining);
int Usb_SetupCmd(const USB_Setup_TypeDef *setup);


SL_PACK_START(1)
typedef struct
{
  uint32_t dwDTERate;               /** Baudrate                            */
  uint8_t  bCharFormat;             /** Stop bits, 0=1 1=1.5 2=2            */
  uint8_t  bParityType;             /** 0=None 1=Odd 2=Even 3=Mark 4=Space  */
  uint8_t  bDataBits;               /** 5, 6, 7, 8 or 16                    */
  uint8_t  dummy;                   /** To ensure size is a multiple of 4 bytes */
} SL_ATTRIBUTE_PACKED cdcLineCoding_TypeDef;
SL_PACK_END()


SL_ALIGN(4)
STATIC_UBUF(usbRxBuff,  USB_FS_BULK_EP_MAXSIZE);   /* Allocate USB receive buffer.   */


static const USBD_Callbacks_TypeDef callbacks =
{
  .usbReset        = NULL,
  .usbStateChange  = StateChangeCallback,
  .setupCmd        = NULL,
  .isSelfPowered   = NULL,
  .sofInt          = NULL
};

static const USBD_Init_TypeDef usbInitStruct =
{
  .deviceDescriptor    = &USBDESC_deviceDesc,
  .configDescriptor    = USBDESC_configDesc,
  .stringDescriptors   = USBDESC_strings,
  .numberOfStrings     = sizeof(USBDESC_strings)/sizeof(void*),
  .callbacks           = &callbacks,
  .bufferingMultiplier = USBDESC_bufferingMultiplier,
  .reserved            = 0
};


void Cli_Initialize_Cli(void)
{
	USBD_Init(&usbInitStruct);


}




static void StateChangeCallback( USBD_State_TypeDef oldState, USBD_State_TypeDef newState )
{
  if (newState == USBD_STATE_CONFIGURED)
  {
	/* We have been configured, start CDC functionality ! */

	if (oldState == USBD_STATE_SUSPENDED)   /* Resume ?   */
	{
	}

	/* Start receiving data from USB host. */
	USBD_Read(CDC_EP_DATA_OUT, (void*) usbRxBuff, USB_FS_BULK_EP_MAXSIZE, UsbDataReceived);


	USBTIMER_Start(CDC_TIMER_ID, USB_TIMEOUT, NULL);
  }

  else if ((oldState == USBD_STATE_CONFIGURED) &&
		   (newState != USBD_STATE_SUSPENDED))
  {
	/* We have been de-configured, stop CDC functionality. */
  }

  else if (newState == USBD_STATE_SUSPENDED)
  {
	/* We have been suspended, stop CDC functionality. */
	/* Reduce current consumption to below 2.5 mA.     */
	USBTIMER_Stop(CDC_TIMER_ID);
  }
}


static int UsbDataReceived(USB_Status_TypeDef status,
                           uint32_t xferred,
                           uint32_t remaining)
{
  (void) remaining;            /* Unused parameter. */
  char writeData[100];

  if ((status == USB_STATUS_OK) && (xferred > 0))
  {

	  //Debug: Write something back to show that we received something
	  sprintf(writeData, "From Device: %s", (char*)usbRxBuff);
	  Cli_WriteUSB((void*)writeData, xferred + 13);

      /* Start a new USB receive transfer. */
      USBD_Read(CDC_EP_DATA_OUT, (void*) usbRxBuff, USB_FS_BULK_EP_MAXSIZE, UsbDataReceived);
  }
  return USB_STATUS_OK;
}

void Cli_WriteUSB(void* message, int dataLen)
{

	USBD_Write(CDC_EP_DATA_IN, message, dataLen, NULL);
}


