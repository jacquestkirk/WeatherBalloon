/*
 * Cli.c
 *
 *  Created on: May 22, 2016
 *      Author: Summer
 */


#include "em_usb.h"
#include "em_cmu.h"

#include "em_usb.h"
#include "cdc.h"
#include "descriptors.h"

#include "Cli.h"


static void StateChangeCallback( USBD_State_TypeDef oldState,
                         USBD_State_TypeDef newState );


/*static const USBD_Callbacks_TypeDef callbacks =
{
  .usbReset        = NULL,
  .usbStateChange  = StateChangeCallback,
  .setupCmd        = CDC_SetupCmd,
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
};*/


void Cli_Initialize_Cli(void)
{

}


static void StateChangeCallback( USBD_State_TypeDef oldState, USBD_State_TypeDef newState )
{

}
