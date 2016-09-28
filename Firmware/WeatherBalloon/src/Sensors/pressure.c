/*
 * pressure.c
 *
 *  Created on: Jun 19, 2016
 *      Author: Summer
 */


#include "Sensors/pressure.h"
#include "descriptors.h"
#include "flash.h"
#include "flash.h"
#include "em_i2c.h"
#include "I2CBubbl.h"
#include "em_assert.h"


STATIC_UBUF(press_data_buff,  FLASH_PAGE_SIZE_BYTES);   /* Allocate USB receive buffer.   */

#define ADDR_PRESS	0xEE	//7 bit address stored in 7MSB (0xEE?)

Press_Data Press_Read(void)
{
	Press_Data pressdata;

	//Todo: actually read this data
	pressdata.pressure = 0xFFFFAAAA;
	pressdata.temperature = 0xABCD1234;


	return pressdata;
}
void Press_Read_Tsk(void)
{
	//Read
	//Write to tem_dat_buff
	//If address gets to FLASH_PAGE_SIZE_BYTES
	//then write temp_data_buff to flash
}

uint16_t Press_QueryRegister2Byte(uint8_t command)
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;

	//uint8_t reg = reg_who_am_i;
	uint16_t data=0;
	uint8_t buff[2] = {0,0}; //just a default value

	seq.addr  = ADDR_PRESS;
	seq.flags = I2C_FLAG_WRITE_READ;
	/* Select register to start reading from */
	seq.buf[0].data = &command;
	seq.buf[0].len  = 1;
	/* Select length of data to be read */
	seq.buf[1].data = buff;
	seq.buf[1].len  = 2;

	ret = I2CBubbl_Transfer(I2C0, &seq);
	if (ret != i2cTransferDone)
	{
	  data = 0xffff;
	}
	else
	{
		data = (buff[0]<<8) | buff[1];
	}

	return data;
}

uint32_t Press_QueryRegister3Byte(uint8_t command)
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;

	//uint8_t reg = reg_who_am_i;
	uint32_t data = 0;
	uint8_t buff[3] = {0,0,0}; //just a default value

	seq.addr  = ADDR_PRESS;
	seq.flags = I2C_FLAG_WRITE_READ;
	/* Select register to start reading from */
	seq.buf[0].data = &command;
	seq.buf[0].len  = 1;
	/* Select length of data to be read */
	seq.buf[1].data = buff;
	seq.buf[1].len  = 2;

	ret = I2CBubbl_Transfer(I2C0, &seq);
	if (ret != i2cTransferDone)
	{
	  data = 0xffff;
	}
	else
	{
		data = (buff[0]<<16) | (buff[1] << 8) | buff[2];
	}

	return data;
}


void Press_WriteCommandByte(uint8_t command)
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;

	//uint8_t reg = reg_who_am_i;
	//uint8_t data = 0; //just a default value

	seq.addr  = ADDR_PRESS;
	seq.flags = I2C_FLAG_WRITE_WRITE;
	/* Select register to start reading from */
	seq.buf[0].data = &command;
	seq.buf[0].len  = 1;


	ret = I2CBubbl_Transfer(I2C0, &seq);
}
