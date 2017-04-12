/*
 * flash.h
 *
 *  Created on: Jun 5, 2016
 *      Author: Mubashir
 */

#ifndef FLASH_H_
#define FLASH_H_

#include <stdint.h>
#include <stdlib.h>
#include "spidrv.h"
#include "em_gpio.h"


#define FLASH_PAGE_SIZE_BYTES 256

// Flash data page0 per dataset
#define FLASH_PAGE0_BOOT_COUNT 0
#define FLASH_PAGELAST_BOOT_COUNT 1
#define FLASH_PAGE0_ERR_CODES 2
#define FLASH_PAGELAST_ERROR 255
#define FLASH_PAGE0_TIMESTAMP 256
#define FLASH_PAGELAST_TIMESTAMP 741
#define FLASH_PAGE0_IMU 742
#define FLASH_PAGELAST_IMU 31422
#define FLASH_PAGE0_MAGNETOMETER 31423
#define FLASH_PAGELAST_MAGNETOMETER 31450
#define FLASH_PAGE0_PRESSURE 31451
#define FLASH_PAGELAST_PRESSURE 31937
#define FLASH_PAGE0_TEMP_PRESSURE 31938
#define FLASH_PAGELAST_TEMP_PRESSURE 32423
#define FLASH_PAGE0_TEMP1 32424
#define FLASH_PAGELAST_TEMP1 32447
#define FLASH_PAGE0_TEMP2 32448
#define FLASH_PAGELAST_TEMP2 32470
#define FLASH_PAGE0_TEMP3 32471
#define FLASH_PAGELAST_TEMP3 32494
#define FLASH_PAGE0_TEMP4 32495
#define FLASH_PAGELAST_TEMP4 32517
#define FLASH_PAGE0_TEMP_MCU 32518
#define FLASH_PAGELAST_TEMP_MCU 32767

typedef enum {
    ALLZEROS,
    ALLONES,
    RANDOM
} Flash_Enum_Test_Data_type;

void Flash_SPI_Initalize();
void Flash_Initialize();
void Flash_Run_Periodic_Task();
void Flash_Populate_Write_Buffer();
void Flash_Write_Time();
void Flash_Write_Imu();
void Flash_Write_Mag();
void Flash_Write_Pressure();
void Flash_Write_Temp();
void Flash_Populate_Write_Buffer();
void Flash_Write_Page(int pagenum);
void Flash_Write_Page_Test_Data(int pagenum, Flash_Enum_Test_Data_type write_dataset);
uint8_t * Flash_Read_FirstByte(int pagenum);
uint8_t * Flash_Read_Page(int pagenum);
uint8_t * Flash_Read_ID();
uint8_t * Flash_Read_Status();
uint16_t Flash_Get_ResetCount();


#endif /* FLASH_H_ */
