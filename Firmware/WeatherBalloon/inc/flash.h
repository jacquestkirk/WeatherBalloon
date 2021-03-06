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

#define FLASH_PAGE_SIZE_BYTES 256

// Flash data page0 per dataset
#define FLASH_PAGE0_TIMESTAMP 0
#define FLASH_PAGE0_IMU 490
#define FLASH_PAGE0_MAGNETOMETER 31412
#define FLASH_PAGE0_PRESSURE 31441
#define FLASH_PAGE0_TEMP_PRESSURE 31931
#define FLASH_PAGE0_TEMP1 32421
#define FLASH_PAGE0_TEMP2 32445
#define FLASH_PAGE0_TEMP3 32469
#define FLASH_PAGE0_TEMP4 32493
#define FLASH_PAGE0_TEMP_MCU 32517
#define FLASH_PAGE0_ERR_CODES 32766

typedef enum {
    ALLZEROS,
    ALLONES,
    RANDOM
} Flash_Enum_Test_Data_type;

void Flash_Initilize();
void Flash_Run_Periodic();
void Flash_Populate_Write_Buffer();
void Flash_Write_Time();
void Flash_Write_Imu();
void Flash_Write_Mag();
void Flash_Write_Pressure();
void Flash_Write_Temp();
void Flash_Write_Page0Test();
void Flash_Write_Page(int pagenum);
void Flash_Write_Page_Test_Data(int pagenum, Flash_Enum_Test_Data_type write_dataset);
uint8_t Flash_Read_FirstByte(int pagenum);
uint8_t * Flash_Read_Page(int pagenum);
uint8_t * Flash_Read_ID();
uint8_t * Flash_GetWriteBufferAddress();

#endif /* FLASH_H_ */
