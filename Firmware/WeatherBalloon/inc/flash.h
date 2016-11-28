/*
 * flash.h
 *
 *  Created on: Jun 5, 2016
 *      Author: Mubashir
 */

#ifndef FLASH_H_
#define FLASH_H_

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

void Flash_Initilize();
void Flash_Run();
void Flash_Write_Time(int offset);
void Flash_Write_Imu(int offset);
void Flash_Write_Mag(int offset);
void Flash_Write_Pressure(int offset);
void Flash_Write_Temp(int offset);
void Flash_Write_Page0Test();
void Flash_Write_Page(int pagenum);
uint8_t Flash_Read_FirstByte(int pagenum);
uint8_t * Flash_Read_Page(int pagenum);
unit8_t * Flash_GetWriteBufferAddress();

#endif /* FLASH_H_ */
