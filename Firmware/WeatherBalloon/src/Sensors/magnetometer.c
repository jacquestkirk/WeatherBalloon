/*
 * magnetometer.c
 *
 *  Created on: Jun 19, 2016
 *      Author: Summer
 */


#include "Sensors/magnetometer.h"
#include "descriptors.h"
#include "flash.h"
#include "em_i2c.h"
#include "I2CBubbl.h"
#include "em_assert.h"
#include "HelperFunctions.h"

STATIC_UBUF(mag_data_buff1,  FLASH_PAGE_SIZE_BYTES);   /* Allocate USB receive buffer.   */
STATIC_UBUF(mag_data_buff2,  FLASH_PAGE_SIZE_BYTES);   /* Allocate USB receive buffer.   */
uint8_t *mag_data_buffs [2] = {mag_data_buff1, mag_data_buff2};
uint8_t *mag_curent_data_buffer = mag_data_buff1;
uint8_t *mag_data_buffer_to_write = mag_data_buff1;

uint8_t _magActiveBuffer = 0;
uint8_t _magReadyToWriteFlash = 0;

#define WHO_AM_I_RESPONSE 0b00111101

#define ADDR_BUBBL		0x38
#define ADDR_BREAKOUT	0x3C

#define ADDR_MAG	ADDR_BUBBL	//7 bit address stored in 7MSB


void TriggerNewMagReading();
Mag_Data QueryAllMagValues();
void WriteMagDataToFlashBuffer(Mag_Data dataToWrite);

Mag_Data Mag_Read(void)
{
	TriggerNewMagReading();
	return QueryAllMagValues();
}
void Mag_Read_Tsk(void)
{
	//Read
	TriggerNewMagReading();
	Mag_Data Mag_Data = QueryAllMagValues();

	//Buffer data in ram array that can be read by flash
	WriteMagDataToFlashBuffer(Mag_Data);


}

void WriteMagDataToFlashBuffer(Mag_Data dataToWrite)
{
	static int location_in_buffer = 0;

	//check if this write will overflow the current buffer.
	//If it does, move to the next buffer and signal that the old buffer is ready to be written to flash

	if(location_in_buffer + MAG_DATA_SIZE_BYTES > FLASH_PAGE_SIZE_BYTES - 1)
	{
		mag_data_buffer_to_write = mag_curent_data_buffer;
		_magReadyToWriteFlash = 1;

		if(_magActiveBuffer)
		{
			mag_curent_data_buffer = mag_data_buffs[0];
			_magActiveBuffer = 0;
		}
		else
		{
			mag_curent_data_buffer = mag_data_buffs[1];;
			_magActiveBuffer = 1;
		}
		location_in_buffer = 0;

	}
	location_in_buffer = Helper_Write_16bit_To_Buffer(mag_curent_data_buffer, location_in_buffer, dataToWrite.x_mag);
	location_in_buffer = Helper_Write_16bit_To_Buffer(mag_curent_data_buffer, location_in_buffer, dataToWrite.y_mag);
	location_in_buffer = Helper_Write_16bit_To_Buffer(mag_curent_data_buffer, location_in_buffer, dataToWrite.z_mag);

}

void Mag_Initialize_OneShot()
{
	//No need to initialize I2C, this should be done in InitDevice module

	//Throw away first query, some startup glitches
	uint8_t id = Mag_QueryRegister1Byte(regm_who_am_i);
	id = Mag_QueryRegister1Byte(regm_who_am_i);
	EFM_ASSERT(id == WHO_AM_I_RESPONSE); //todo: How should I handle this? Should I just continue since there is nothing we can do anyway? Does this line actually do anyting?

	//Write the CTRL REGISTERS
	//todo: clean this up somehow so that all the settings are in a better place. A single structure?
	uint8_t temp_comp = 1;
	uint8_t om = ctrl1_reg1_m_om_ultraHighPowerMode;
	uint8_t doM = ctrl_reg1_m_do_0p625_Hz;
	uint8_t fast_odr = 0;
	uint8_t st = 0;

	uint8_t ctrl_reg_1_m = temp_comp << CTRL_REG1_M_TEMP_COMP_SHIFT | om << CTRL_REG1_M_OM_SHIFT | doM << CTRL_REG1_M_DO_SHIFT | fast_odr << CTRL_REG1_M_FAST_ODR_SHIFT | st << CTRL_REG1_M_ST_SHIFT;
	Mag_WriteRegister1Byte(regm_ctrl_reg1, ctrl_reg_1_m);


	uint8_t fs = ctrl_reg2_m_fs_4gauss;
	uint8_t reboot = 0;
	uint8_t soft_reset = 0;

	uint8_t ctrl_reg_2_m = fs << CTRL_REG2_M_FS_SHIFT | reboot << CTRL_REG2_M_REBOOT_SHIFT | soft_reset << CTRL_REG2_M_SOFT_RST_SHIFT;
	Mag_WriteRegister1Byte(regm_ctrl_reg2, ctrl_reg_2_m);


	uint8_t lp = 0;
	uint8_t md = ctrl_reg3_m_md_single_conversion;

	uint8_t ctrl_reg_3_m = lp << CTRL_REG3_M_LP_SHIFT | md << CTRL_REG3_M_MD_SHIFT;
	Mag_WriteRegister1Byte(regm_ctrl_reg3, ctrl_reg_3_m);


	uint8_t omz = ctrl_reg4_m_omz_ultraHighPower;
	uint8_t ble = ctrl_reg4_m_ble_lsbLower;

	uint8_t ctrl_reg_4_m = omz << CTRL_REG4_M_OMZ_SHIFT | ble << CTRL_REG4_M_BLE_SHIFT;
	Mag_WriteRegister1Byte(regm_ctrl_reg4, ctrl_reg_4_m);

}

void TriggerNewMagReading()
{
	uint8_t ctrl_reg_3_m = Mag_QueryRegister1Byte(regm_ctrl_reg3);

	uint8_t md = ctrl_reg3_m_md_single_conversion;

	// clear out the mode bits then or the single conversion bits back in
	ctrl_reg_3_m = ctrl_reg_3_m & ~CTRL_REG3_M_MD_MASK;
	ctrl_reg_3_m = ctrl_reg_3_m | md << CTRL_REG3_M_MD_SHIFT;
	Mag_WriteRegister1Byte(regm_ctrl_reg3, ctrl_reg_3_m);
}

Mag_Data QueryAllMagValues()
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;

	uint8_t reg = regm_out_x_l;
	uint8_t data[12] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; //just a default value

	seq.addr  = ADDR_MAG;
	seq.flags = I2C_FLAG_WRITE_READ;
	/* Select register to start reading from */
	seq.buf[0].data = &reg;
	seq.buf[0].len  = 1;
	/* Select length of data to be read */
	seq.buf[1].data = data;
	seq.buf[1].len  = 6;

	ret = I2CBubbl_Transfer(I2C0, &seq);
	if (ret != i2cTransferDone)
	{
		//if there is an error data won't be overwritten? Will 0xff pass through?
	}

	Mag_Data mag_data;
	mag_data.x_mag = data[1] << 8 | data[0];
	mag_data.y_mag = data[3] << 8 | data[2];
	mag_data.z_mag = data[5] << 8 | data[4];


	return mag_data;
}

uint8_t Mag_QueryRegister1Byte(uint8_t reg)
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;

	//uint8_t reg = reg_who_am_i;
	uint8_t data = 0; //just a default value

	seq.addr  = ADDR_MAG;
	seq.flags = I2C_FLAG_WRITE_READ;
	/* Select register to start reading from */
	seq.buf[0].data = &reg;
	seq.buf[0].len  = 1;
	/* Select length of data to be read */
	seq.buf[1].data = &data;
	seq.buf[1].len  = 1;

	ret = I2CBubbl_Transfer(I2C0, &seq);
	if (ret != i2cTransferDone)
	{
	  data = 0xff;
	}

	return data;
}

void Mag_WriteRegister1Byte(uint8_t reg, uint8_t data)
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;

	//uint8_t reg = reg_who_am_i;
	//uint8_t data = 0; //just a default value

	seq.addr  = ADDR_MAG;
	seq.flags = I2C_FLAG_WRITE_WRITE;
	/* Select register to start reading from */
	seq.buf[0].data = &reg;
	seq.buf[0].len  = 1;
	/* Select length of data to be read */
	seq.buf[1].data = &data;
	seq.buf[1].len  = 1;

	ret = I2CBubbl_Transfer(I2C0, &seq);
	if (ret != i2cTransferDone)
	{
	  data = 0xff;
	}
}
uint8_t Mag_QueryReadyToWriteFlashFlag(void)
{
	return _magReadyToWriteFlash;
}
void Mag_ClearReadyToWriteFlashFlag(void)
{
	_magReadyToWriteFlash = 0;
}
uint8_t* Mag_GetBufferAddress(void)
{
	if(_magActiveBuffer == 1)
	{
		return mag_data_buff1;
	}
	else
	{
		return mag_data_buff2;
	}
}

