/*
 * flash.h
 *
 *  Created on: Jun 5, 2016
 *      Author: Summer
 */

#ifndef FLASH_H_
#define FLASH_H_

#define FLASH_PAGE_SIZE_BYTES 256

void Flash_Write_Imu(int offset);
void Flash_Write_Mag(int offset);
void Flash_Write_Press(int offset);
void Flash_Write_Temp(int offset);

#endif /* FLASH_H_ */
