/*
 * Cli.c
 *
 *  Created on: May 22, 2016
 *      Author: Summer
 */


#include <stdio.h>
#include <string.h>
#include "em_usb.h"
#include "em_cmu.h"
#include "em_usb.h"
//#include "cdc.h"
#include "descriptors.h"


#include "Cli.h"
#include "Sensors\imu.h"
#include "Sensors\led.h"
#include "Sensors\magnetometer.h"
#include "Sensors\pressure.h"
#include "Sensors\temperature.h"
#include "timestamp.h"
#include "flash.h"

#define USB_TIMEOUT 100
#define USB_TX_BUFFER_SIZE 300

//CLI commands
enum Commands{
	Cmd_ReadPress,
	Cmd_ReadTemp,
	Cmd_ReadImu,
	Cmd_ReadMag,
	Cmd_BlinkLed1,
	Cmd_BlinkLed2,
	Cmd_ReadTimestamp,
	Cmd_EraseFlash,
	Cmd_ReadFlashLoc,
	Cmd_ReadLastData,
	Cmd_ReadStartAddr,
	Cmd_ReadImuRegister,
	Cmd_WriteImuRegister,
	Cmd_StartRecording,
	Cmd_StopRecording,
	Cmd_StartImuStream,
	Cmd_StopImuStream,
	Cmd_ReadMagRegister,
	Cmd_WriteMagRegister,
	Cmd_ReadPress2Byte,
	Cmd_ReadPress3Byte,
	Cmd_WritePressRegister,
	Cmd_ReadFlashPage,
	Cmd_WriteFlashPage,

};

#define ERROR_MESSAGE_ENUM  255

/*
#define CMD_READ_PRESSURE 		"readpressure"
#define CMD_READ_TEMPERATURE 	"readtemperature"
#define CMD_READ_IMU 			"readimu"
#define CMD_READ_MAGNETOMETER	"readmag"
#define CMD_BLINK_LED1			"blinkled1"
#define CMD_BLINK_LED2			"blinkled2"
#define CMD_READ_TIMESTAMP		"readtimestamp"
#define CMD_ERASE_FLASH			"eraseflash"
#define CMD_READ_FLASH_LOCATIONS	"readflashlocations"
#define CMD_READ_LAST_DATA		"readlastdata"
#define CMD_READ_START_ADDR		"readstartaddr"
*/

//States
enum States{
	State_Home,
	State_Confirm_Erase,

};



static void StateChangeCallback( USBD_State_TypeDef oldState, USBD_State_TypeDef newState );
static int  UsbDataReceived(USB_Status_TypeDef status, uint32_t xferred,
                            uint32_t remaining);
static int UsbDataTransmitted(USB_Status_TypeDef status,
                           uint32_t xferred,
                           uint32_t remaining);
void RunStateMachine(char* usbRxBuff);
int ParseCommand(char* usbRxBuff);

int Add8bitIntToTxBuff(int8_t data, int offset);
int Add16bitIntToTxBuff(int16_t data, int offset);
int Add32bitIntToTxBuff(int32_t data, int offset);

int WriteDebugMessage(char* message, int offset);
void WriteInvalidCommandMessage(void);
void WriteTempData(void);
void WritePressData(void);
void WriteImuData(void);
void WriteMagData(void);
void BlinkLed1(void);
void BlinkLed2(void);

void WriteTimestamp(void);
void EraseFlash(void);
void ReadFlashLoc(void);
void ReadLastData(void);
void ReadStartAddr(void);

void ReadImuRegister(void);
void WriteImuRegister(void);
void StopImuStream(void);
void StartImuStream(void);
void ReadMagRegister(void);
void WriteMagRegister(void);
void ReadPress2Byte(void);
void ReadPress3Byte(void);
void WritePressRegister(void);
void ReadFlashPage(void);
void WriteFlashPage(void);

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
STATIC_UBUF(usbTxBuff,  USB_TX_BUFFER_SIZE);   /* Allocate USB transmit buffer.   */

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


static int UsbDataTransmitted(USB_Status_TypeDef status,
                           uint32_t xferred,
                           uint32_t remaining)
{
  (void) remaining;            /* Unused parameter. */
  (void) xferred;
  (void) status;
  return USB_STATUS_OK;
}

static int UsbDataReceived(USB_Status_TypeDef status,
                           uint32_t xferred,
                           uint32_t remaining)
{
  (void) remaining;            /* Unused parameter. */
  //char writeData[100];

  if ((status == USB_STATUS_OK) && (xferred > 0))
  {

	  //Debug: Write something back to show that we received something
	  //sprintf(writeData, "From Device: %s", (char*)usbRxBuff);
	  //Cli_WriteUSB((void*)writeData, xferred + 13);


	  RunStateMachine((char*) usbRxBuff);

      /* Start a new USB receive transfer. */
      USBD_Read(CDC_EP_DATA_OUT, (void*) usbRxBuff, USB_FS_BULK_EP_MAXSIZE, UsbDataReceived);
  }
  return USB_STATUS_OK;
}

void Cli_WriteUSB(void* message, int dataLen)
{

	USBD_Write(CDC_EP_DATA_IN, message, dataLen, UsbDataTransmitted);
}


void RunStateMachine(char* usbRxBuff)
{
	static int current_state = State_Home;
	static int next_state = State_Home;

	int command = ParseCommand(usbRxBuff);

	switch (command)
	{

		case Cmd_ReadPress:
			WritePressData();
			next_state = State_Home;
			return;
		case Cmd_ReadTemp:
			WriteTempData();
			next_state = State_Home;
			return;
		case Cmd_ReadImu:
			WriteImuData();
			next_state = State_Home;
			return;
		case Cmd_ReadMag:
			WriteMagData();
			next_state = State_Home;
			return;
		case Cmd_BlinkLed1:
			BlinkLed1();
			next_state = State_Home;
			return;
		case Cmd_BlinkLed2:
			BlinkLed2();
			next_state = State_Home;
			return;
		case Cmd_ReadTimestamp:
			WriteTimestamp();
			next_state = State_Home;
			return;
		case Cmd_EraseFlash:
			EraseFlash();
			next_state = State_Home;
			return;
		case Cmd_ReadFlashLoc:
			ReadFlashLoc();
			next_state = State_Home;
			return;
		case Cmd_ReadLastData:
			ReadLastData();
			next_state = State_Home;
			return;
		case Cmd_ReadStartAddr:
			ReadStartAddr();
			next_state = State_Home;
			return;
		case Cmd_ReadImuRegister:
			ReadImuRegister();
			next_state = State_Home;
			return;
		case Cmd_WriteImuRegister:
			WriteImuRegister();
			next_state = State_Home;
			return;
		case Cmd_StartImuStream:
			StartImuStream();
			next_state = State_Home;
			return;
		case Cmd_StopImuStream:
			StopImuStream();
			next_state = State_Home;
			return;
		case Cmd_ReadMagRegister:
			ReadMagRegister();
			return;
		case Cmd_WriteMagRegister:
			WriteMagRegister();
			return;
		case Cmd_ReadPress2Byte:
			ReadPress2Byte();
			return;
		case Cmd_ReadPress3Byte:
			ReadPress3Byte();
			return;
		case Cmd_WritePressRegister:
			WritePressRegister();
			return;
		case Cmd_ReadFlashPage:
			ReadFlashPage();
			return;
		case Cmd_WriteFlashPage:
			WriteFlashPage();
			return;
		default:
			WriteInvalidCommandMessage();
			return;

	}

}

void WritePressData(void)
{
	Press_Data  pressData;
	int startIndex = 0;

	pressData = Press_Read();

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_ReadPress, startIndex);
	startIndex = Add32bitIntToTxBuff(pressData.pressure, startIndex);
	startIndex = Add32bitIntToTxBuff(pressData.temperature, startIndex);


	//Write debug message to Tx Buff
	char message[] = "ThisIsDebugData";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);
}

void WriteTempData(void)
{
	Temp_Data tempData;
	int startIndex = 0;

	tempData = Temp_Read();

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_ReadTemp, startIndex);
	startIndex = Add16bitIntToTxBuff(tempData.therm1, startIndex);
	startIndex = Add16bitIntToTxBuff(tempData.therm2, startIndex);
	startIndex = Add16bitIntToTxBuff(tempData.therm3, startIndex);
	startIndex = Add16bitIntToTxBuff(tempData.therm4, startIndex);

	//Write debug message to Tx Buff
	char message[] = "ThisIsDebugData";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);

}

void WriteImuData(void)
{
	Imu_Data imuData;
	int startIndex = 0;

	imuData = Imu_Read();

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_ReadImu, startIndex);
	startIndex = Add16bitIntToTxBuff(imuData.x_accel, startIndex);
	startIndex = Add16bitIntToTxBuff(imuData.y_accel, startIndex);
	startIndex = Add16bitIntToTxBuff(imuData.z_accel, startIndex);
	startIndex = Add16bitIntToTxBuff(imuData.x_gyro, startIndex);
	startIndex = Add16bitIntToTxBuff(imuData.y_gyro, startIndex);
	startIndex = Add16bitIntToTxBuff(imuData.z_gyro, startIndex);

	//Write debug message to Tx Buff
	char message[] = "ThisIsDebugData";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);

}

void Cli_Stream_Imu_Data(Imu_Data imuData)
{
	int startIndex = 0;

	//Write data to Tx buff
	startIndex = Add16bitIntToTxBuff(imuData.x_accel, startIndex);
	startIndex = Add16bitIntToTxBuff(imuData.y_accel, startIndex);
	startIndex = Add16bitIntToTxBuff(imuData.z_accel, startIndex);
	startIndex = Add16bitIntToTxBuff(imuData.x_gyro, startIndex);
	startIndex = Add16bitIntToTxBuff(imuData.y_gyro, startIndex);
	startIndex = Add16bitIntToTxBuff(imuData.z_gyro, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);
}

void WriteMagData(void)
{
	Mag_Data magData;
	int startIndex = 0;

	magData = Mag_Read();

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_ReadMag, startIndex);
	startIndex = Add16bitIntToTxBuff(magData.x_mag, startIndex);
	startIndex = Add16bitIntToTxBuff(magData.y_mag, startIndex);
	startIndex = Add16bitIntToTxBuff(magData.z_mag, startIndex);

	//Write debug message to Tx Buff
	char message[] = "ThisIsDebugData";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);

}

void BlinkLed1(void)
{
	int startIndex = 0;

	//Execute the command
	Led_Blink_1();

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_BlinkLed1, startIndex);

	//Write debug message to Tx Buff
	char message[] = "This command doesn't do anything yet";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);
}

void BlinkLed2(void)
{
	int startIndex = 0;

	//Execute the command
	Led_Blink_2();

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_BlinkLed2, startIndex);

	//Write debug message to Tx Buff
	char message[] = "This command doesn't do anything yet";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);
}

void WriteTimestamp(void)
{
	int startIndex = 0;

	//Get timestamp
	uint32_t timestamp = Time_Get_TimeStamp();

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_ReadTimestamp, startIndex);
	startIndex = Add32bitIntToTxBuff(timestamp, startIndex);

	//Write debug message to Tx Buff
	char message[] = "See above line for time since reset";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);
}
void EraseFlash(void)
{
	int startIndex = 0;

	//Execute the command
	//Todo: Place command here

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_EraseFlash, startIndex);

	//Write debug message to Tx Buff
	char message[] = "This command doesn't do anything yet";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);
}
void ReadFlashLoc(void)
{
	int startIndex = 0;

	//Execute the command
	//Todo: Place command here

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_ReadFlashLoc, startIndex);

	//Write debug message to Tx Buff
	char message[] = "This command doesn't do anything yet";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);
}
void ReadLastData(void)
{
	int startIndex = 0;

	//Execute the command
	//Todo: Place command here

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_ReadLastData, startIndex);

	//Write debug message to Tx Buff
	char message[] = "This command doesn't do anything yet";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);
}
void ReadStartAddr(void)
{
	int startIndex = 0;

	//Execute the command
	//Todo: Place command here

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_ReadStartAddr, startIndex);

	//Write debug message to Tx Buff
	char message[] = "This command doesn't do anything yet";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);
}

void ReadPress2Byte(void)
{
	uint8_t regToRead = usbRxBuff[1];
	uint16_t regContents = Press_QueryRegister2Byte(regToRead);

	int startIndex = 0;

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_ReadPress2Byte, startIndex);
	startIndex = Add16bitIntToTxBuff(regContents, startIndex);

	//Write debug message to Tx Buff
	char message[] = "Barometer Register Contents";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);

}

void ReadPress3Byte(void)
{
	uint8_t regToRead = usbRxBuff[1];
	uint32_t regContents = Press_QueryRegister3Byte(regToRead);

	int startIndex = 0;

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_ReadPress3Byte, startIndex);
	startIndex = Add32bitIntToTxBuff(regContents, startIndex);

	//Write debug message to Tx Buff
	char message[] = "Barometer Register Contents";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);

}

void WritePressRegister(void)
{
	//Todo: Do we need to save a second buffer for Rx so that we are not reading inputs for a following command. Probably
	uint8_t command = usbRxBuff[1];

	Press_WriteCommandByte(command);

	int startIndex = 0;

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_WritePressRegister, startIndex);

	//Write debug message to Tx Buff
	char message[] = "Barometer Register Written";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);

}

void ReadMagRegister(void)
{
	uint8_t regToRead = usbRxBuff[1];
	uint8_t regContents = Mag_QueryRegister1Byte(regToRead);

	int startIndex = 0;

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_ReadMagRegister, startIndex);
	startIndex = Add8bitIntToTxBuff(regContents, startIndex);

	//Write debug message to Tx Buff
	char message[] = "Magnetometer Register Contents";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);

}

void WriteMagRegister(void)
{
	//Todo: Do we need to save a second buffer for Rx so that we are not reading inputs for a following command. Probably
	uint8_t regToWrite = usbRxBuff[1];
	uint8_t byteToWrite = usbRxBuff[2];

	Mag_WriteRegister1Byte(regToWrite, byteToWrite);

	int startIndex = 0;

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_WriteMagRegister, startIndex);

	//Write debug message to Tx Buff
	char message[] = "Magnetometer Register Written";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);

}

void ReadImuRegister(void)
{
	uint8_t regToRead = usbRxBuff[1];
	uint8_t regContents = Imu_QueryRegister1Byte(regToRead);

	int startIndex = 0;

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_ReadImuRegister, startIndex);
	startIndex = Add8bitIntToTxBuff(regContents, startIndex);

	//Write debug message to Tx Buff
	char message[] = "IMU Register Contents";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);

}

void WriteImuRegister(void)
{
	//Todo: Do we need to save a second buffer for Rx so that we are not reading inputs for a following command. Probably
	uint8_t regToWrite = usbRxBuff[1];
	uint8_t byteToWrite = usbRxBuff[2];

	Imu_WriteRegister1Byte(regToWrite, byteToWrite);

	int startIndex = 0;

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_WriteImuRegister, startIndex);

	//Write debug message to Tx Buff
	char message[] = "IMU Register Written";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);

}

void ReadFlashPage(void)
{
	uint8_t *flashBuffer;

	//Todo: Do we need to save a second buffer for Rx so that we are not reading inputs for a following command. Probably
	uint8_t pageNum_high = usbRxBuff[1];
	uint8_t pageNum_low = usbRxBuff[2];

	int pageNum = ((int)pageNum_high << 8) + pageNum_low;

	flashBuffer = (uint8_t *)Flash_Read_Page(pageNum);

	int startIndex = 0;

	//Echo command
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_ReadFlashPage, startIndex);

	//Write data to tx buffer
	for( int i = 0; i < FLASH_PAGE_SIZE_BYTES; i++) //The size of the data should be the flash size
	{
		//if ( startIndex >= USB_FS_BULK_EP_MAXSIZE+38 ) //check if we are overflowing, then we need to move to a new buffer
		//{
		//	//Todo: make this use a new buffer instead of breaking
		//	break;
		//}

		startIndex = Add8bitIntToTxBuff(flashBuffer[i] , startIndex);
	}

	//Todo: add back message once we get multiple tx buffers working
	//Write debug message to Tx Buff
	//char message[] = "IMU Register Written";
	//startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);
}

void WriteFlashPage(void)
{
	//Todo: fill this out
	return;
}

void StartImuStream(void)
{
	Imu_StartFifo();

	int startIndex = 0;

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_StartImuStream, startIndex);

	//Write debug message to Tx Buff
	char message[] = "IMU Stream Started";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);
}

void StopImuStream(void)
{
	Imu_StopFifo();

	int startIndex = 0;

	//Write data to Tx buff
	startIndex = Add8bitIntToTxBuff((uint8_t) Cmd_StartImuStream, startIndex);

	//Write debug message to Tx Buff
	char message[] = "IMU Stream Started";
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);
}

void WriteInvalidCommandMessage(void)
{
	//Write debug message to Tx Buff
	char message[] = "Invalid Command Sent";
	int startIndex = 0;
	startIndex = Add8bitIntToTxBuff(ERROR_MESSAGE_ENUM, startIndex);
	startIndex = WriteDebugMessage((char*)&message, startIndex);

	//Write the TxBuff over USB
	 Cli_WriteUSB((void*)usbTxBuff, startIndex);
}

int Add8bitIntToTxBuff(int8_t data, int offset)
{
	usbTxBuff [offset] = data;

	offset += 1;
	return offset;
}

int Add16bitIntToTxBuff(int16_t data, int offset)
{
	usbTxBuff [offset] = (uint8_t) (data>>8);
	usbTxBuff [offset+1] = (uint8_t) (data);

	offset += 2;
	return offset;
}

int Add32bitIntToTxBuff(int32_t data, int offset)
{
	usbTxBuff [offset]   = (uint8_t) (data >> 24);
	usbTxBuff [offset+1] = (uint8_t) (data >> 16);
	usbTxBuff [offset+2] = (uint8_t) (data >> 8);
	usbTxBuff [offset+3] = (uint8_t) (data);

	offset += 4;
	return offset;
}

int WriteDebugMessage(char* message, int offset)
{
	int length = strlen(message);

	for( int i = 0; i<length; i++)
	{
		usbTxBuff[offset + i ] = message[i];
	}

	return offset + length;
}

int ParseCommand(char* usbRxBuff)
{
	//To parse return the first and only byte
	//Limited to 256 commands, but I tink that that should be fine.

	return (int) usbRxBuff[0];
}
