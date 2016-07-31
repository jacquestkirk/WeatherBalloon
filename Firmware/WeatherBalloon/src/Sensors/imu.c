/*
 * imu.c
 *
 *  Created on: Jun 19, 2016
 *      Author: Summer
 */


#include "Sensors/imu.h"
#include "descriptors.h"
#include "flash.h"
#include "em_i2c.h"
#include "gpiointerrupt.h"
#include "I2CBubbl.h"
#include "em_assert.h"
#include "em_gpio.h"


STATIC_UBUF(imu_data_buff,  FLASH_PAGE_SIZE_BYTES);   /* Allocate USB receive buffer.   */
Imu_Data fifo_data[IMU_FIFO_SIZE];


Imu_Data QueryAllImuValues(void);
void ConfigInterrupt(void);
void Int1_a_g_Callback(void);
void ClearFifo(void);
void WriteFifoOverUsb();

#define WHO_AM_I_RESPONSE 0x68

#define ADDR_BUBBL		0xD4
#define ADDR_BREAKOUT	0xD6

#define ADDR_IMU	ADDR_BREAKOUT	//7 bit address stored in 7MSB





Imu_Data Imu_Read(void)
{

	Imu_Data imudata = QueryAllImuValues();

	//Todo: actually read this data
	//imudata.x_accel = 0x1234;
	//imudata.y_accel = 0x5678;
	//imudata.z_accel = 0x9abc;
	//imudata.x_gyro  = 0xdeff;
	//imudata.y_gyro  = 0xedcb;
	//imudata.z_gyro  = 0xa987;


	return imudata;
}
void Imu_Read_Tsk(void)
{
	//Read
	//Write to tem_dat_buff
	//If address gets to FLASH_PAGE_SIZE_BYTES
	//then write temp_data_buff to flash
}

void Imu_Initialize_OneShot()
{
	//No need to initialize I2C, this should be done in InitDevice module

	//Throw away first query, some startup glitches
	uint8_t id = QueryRegister1Byte(reg_who_am_i);
	id = QueryRegister1Byte(reg_who_am_i);
	EFM_ASSERT(id == WHO_AM_I_RESPONSE); //todo: How should I handle this? Should I just continue since there is nothing we can do anyway? Does this line actually do anyting?

	//Write the CTRL REGISTERS
	//todo: clean this up somehow so that all the settings are in a better place. A single structure?
	uint8_t odr_g = CTRL_REG_1_G_ODR_G_14p9;
	uint8_t fs_g = CTRL_REG_1_G_FS_500DPS;
	uint8_t bw_g = 0;//N/A for 14.9Hz data rate
	uint8_t ctrl_reg_1_g = odr_g << CTRL_REG_1_G_ODR_G_SHIFT | fs_g << CTRL_REG_1_G_FS_SHIFT | bw_g << CTRL_REG_1_G_BW_G1_SHIFT;
	WriteRegister1Byte(reg_ctrl_reg1_g, ctrl_reg_1_g);

	uint8_t int_sel = CTRL_REG_2_G_INT_SEL_LPF1;
	uint8_t out_sel = CTRL_REG_2_G_OUT_SEL_LPF1;
	uint8_t ctrl_reg_2_g = int_sel << CTRL_REG_2_G_INT_SEL_SHIFT | out_sel << CTRL_REG_2_G_OUT_SEL_SHIFT;
	WriteRegister1Byte(reg_ctrl_reg2_g, ctrl_reg_2_g);

	uint8_t lp_mode = 1;
	uint8_t high_pass_en = 0;
	uint8_t high_pass_cutoff = 0;
	uint8_t ctrl_reg_3_g = lp_mode << CTRL_REG_3_G_LPMODE_SHIFT | high_pass_en << CTRL_REG_3_G_HP_EN_SHIFT | high_pass_cutoff << CTRL_REG_3_G_HPCF_SHIFT;
	WriteRegister1Byte(reg_ctrl_reg3_g, ctrl_reg_3_g);

	uint8_t zen_g = 1;
	uint8_t yen_g = 1;
	uint8_t xen_g = 1;
	uint8_t lir_xl = 1;
	uint8_t ctrl_reg_4 = zen_g << CTRL_REG_4_ZEN_G_SHIFT | yen_g << CTRL_REG_4_YEN_G_SHIFT | xen_g << CTRL_REG_4_XEN_G_SHIFT | lir_xl << CTRL_REG_4_LIR_XL1_SHIFT;
	WriteRegister1Byte(reg_ctrl_reg4, ctrl_reg_4);

	uint8_t dec = CTRL_REG_5_XL_DEC_NONE;
	uint8_t zen_xl = 1;
	uint8_t yen_xl = 1;
	uint8_t xen_xl = 1;
	uint8_t ctrl_reg_5 = zen_xl << CTRL_REG_5_XL_ZEN_XL_SHIFT | yen_xl << CTRL_REG_5_XL_YEN_XL_SHIFT | xen_xl << CTRL_REG_5_XL_XEN_XL_SHIFT | dec << CTRL_REG_5_XL_DEC_SHIFT;
	WriteRegister1Byte(reg_ctrl_reg5_xl, ctrl_reg_5);

	uint8_t odr_xl = CTRL_REG_6_XL_ODR_XL_10HZ; //Not sure if this does anything if you're not using accel only mode, but I think you need to have it not in power down mode
	uint8_t fs_xl = CTRL_REG_6_XL_FS_XL_8g;
	uint8_t bw_scale_with_odr = 0;
	uint8_t bw_xl = CTRL_REG_6_XL_BW_XL_50Hz; //Todo: Can't get anti aliasing filter closer? Is output data rate sampling rate?
	uint8_t ctrl_reg_6_xl = odr_xl << CTRL_REG_6_XL_ODR_XL_SHIFT | fs_xl << CTRL_REG_6_XL_FS_XL_SHIFT | bw_scale_with_odr << CTRL_REG_6_XL_BW_SCAL_ODR_SHIFT | bw_xl << CTRL_REG_6_XL_BW_XL_SHIFT;
	WriteRegister1Byte(reg_ctrl_reg6_xl, ctrl_reg_6_xl);

	uint8_t high_res_mode = 1;
	uint8_t dig_filt = 0; //Does not do anything with filter disabled?
	uint8_t filt_dat_en = 0; //don't use filter
	uint8_t high_pass_xl_en = 0;
	uint8_t ctrl_reg7_xl = high_res_mode << CTRL_REG_7_XL_HR_EN_SHIFT| dig_filt << CTRL_REG_7_XL_DCF_SHIFT | filt_dat_en << CTRL_REG_7_XL_FDS_EN_SHIFT | high_pass_xl_en << CTRL_REG_7_XL_HPIS_EN_SHIFT;
	WriteRegister1Byte(reg_ctrl_reg7_xl, ctrl_reg7_xl);

	//todo: add fifo configs
	//uint8_t

}

void Imu_Initialize()
{
	//No need to initialize I2C, this should be done in InitDevice module

	ConfigInterrupt();
	ClearFifo();



	//Throw away first query, some startup glitches
	uint8_t id = Imu_QueryRegister1Byte(reg_who_am_i);

	//Reset IMU
	uint8_t reset_cmd = 1 << CTRL_REG_8_SW_RESET_SHIFT;
	Imu_WriteRegister1Byte(reg_ctrl_reg8, reset_cmd);

	id = Imu_QueryRegister1Byte(reg_who_am_i);
	EFM_ASSERT(id == WHO_AM_I_RESPONSE); //todo: How should I handle this? Should I just continue since there is nothing we can do anyway? Does this line actually do anyting?

	//Write the CTRL REGISTERS
	//todo: clean this up somehow so that all the settings are in a better place. A single structure?
	uint8_t odr_g = CTRL_REG_1_G_ODR_G_14p9;
	uint8_t fs_g = CTRL_REG_1_G_FS_500DPS;
	uint8_t bw_g = 0;//N/A for 14.9Hz data rate
	uint8_t ctrl_reg_1_g = odr_g << CTRL_REG_1_G_ODR_G_SHIFT | fs_g << CTRL_REG_1_G_FS_SHIFT | bw_g << CTRL_REG_1_G_BW_G1_SHIFT;
	Imu_WriteRegister1Byte(reg_ctrl_reg1_g, ctrl_reg_1_g);

	uint8_t int_sel = CTRL_REG_2_G_INT_SEL_LPF1;
	uint8_t out_sel = CTRL_REG_2_G_OUT_SEL_LPF1;
	uint8_t ctrl_reg_2_g = int_sel << CTRL_REG_2_G_INT_SEL_SHIFT | out_sel << CTRL_REG_2_G_OUT_SEL_SHIFT;
	Imu_WriteRegister1Byte(reg_ctrl_reg2_g, ctrl_reg_2_g);

	uint8_t lp_mode = 1;
	uint8_t high_pass_en = 0;
	uint8_t high_pass_cutoff = 0;
	uint8_t ctrl_reg_3_g = lp_mode << CTRL_REG_3_G_LPMODE_SHIFT | high_pass_en << CTRL_REG_3_G_HP_EN_SHIFT | high_pass_cutoff << CTRL_REG_3_G_HPCF_SHIFT;
	Imu_WriteRegister1Byte(reg_ctrl_reg3_g, ctrl_reg_3_g);

	uint8_t zen_g = 1;
	uint8_t yen_g = 1;
	uint8_t xen_g = 1;
	uint8_t lir_xl = 1;
	uint8_t ctrl_reg_4 = zen_g << CTRL_REG_4_ZEN_G_SHIFT | yen_g << CTRL_REG_4_YEN_G_SHIFT | xen_g << CTRL_REG_4_XEN_G_SHIFT | lir_xl << CTRL_REG_4_LIR_XL1_SHIFT;
	Imu_WriteRegister1Byte(reg_ctrl_reg4, ctrl_reg_4);

	uint8_t dec = CTRL_REG_5_XL_DEC_NONE;
	uint8_t zen_xl = 1;
	uint8_t yen_xl = 1;
	uint8_t xen_xl = 1;
	uint8_t ctrl_reg_5 = zen_xl << CTRL_REG_5_XL_ZEN_XL_SHIFT | yen_xl << CTRL_REG_5_XL_YEN_XL_SHIFT | xen_xl << CTRL_REG_5_XL_XEN_XL_SHIFT | dec << CTRL_REG_5_XL_DEC_SHIFT;
	Imu_WriteRegister1Byte(reg_ctrl_reg5_xl, ctrl_reg_5);

	uint8_t odr_xl = CTRL_REG_6_XL_ODR_XL_10HZ; //Not sure if this does anything if you're not using accel only mode, but I think you need to have it not in power down mode
	uint8_t fs_xl = CTRL_REG_6_XL_FS_XL_8g;
	uint8_t bw_scale_with_odr = 0;
	uint8_t bw_xl = CTRL_REG_6_XL_BW_XL_50Hz; //Todo: Can't get anti aliasing filter closer? Is output data rate sampling rate?
	uint8_t ctrl_reg_6_xl = odr_xl << CTRL_REG_6_XL_ODR_XL_SHIFT | fs_xl << CTRL_REG_6_XL_FS_XL_SHIFT | bw_scale_with_odr << CTRL_REG_6_XL_BW_SCAL_ODR_SHIFT | bw_xl << CTRL_REG_6_XL_BW_XL_SHIFT;
	Imu_WriteRegister1Byte(reg_ctrl_reg6_xl, ctrl_reg_6_xl);

	uint8_t high_res_mode = 1;
	uint8_t dig_filt = 0; //Does not do anything with filter disabled?
	uint8_t filt_dat_en = 0; //don't use filter
	uint8_t high_pass_xl_en = 0;
	uint8_t ctrl_reg7_xl = high_res_mode << CTRL_REG_7_XL_HR_EN_SHIFT| dig_filt << CTRL_REG_7_XL_DCF_SHIFT | filt_dat_en << CTRL_REG_7_XL_FDS_EN_SHIFT | high_pass_xl_en << CTRL_REG_7_XL_HPIS_EN_SHIFT;
	Imu_WriteRegister1Byte(reg_ctrl_reg7_xl, ctrl_reg7_xl);

	//FIFO and interrupt configs

	uint8_t int1_fth = 1;
	uint8_t int1_ctrl =  int1_fth << INT1_CTRL_FTH_SHIFT;
	Imu_WriteRegister1Byte(reg_int1_ctrl, int1_ctrl);

	uint8_t fmode = FIFO_CTRL_FMODE_CONT_OVERWRITE;
	uint8_t fth = 31;
	uint8_t fifo_ctrl = fmode << FIFO_CTRL_FMODE_SHIFT | fth << FIFO_CTRL_FTH_SHIFT;
	Imu_WriteRegister1Byte(reg_fifo_ctrl, fifo_ctrl);

	uint8_t fifo_en = 1;
	uint8_t ctrl_reg_9 = fifo_en << CTRL_REG_9_FIFO_EN_SHIFT;
	Imu_WriteRegister1Byte(reg_ctrl_reg9, ctrl_reg_9);


}

void ConfigInterrupt(void)
{
	//Config PC10 as an interrupt(IMU_INT1_A/G)

	GPIO_IntConfig(gpioPortC, 10, 1, 0, 1);
	GPIOINT_Init();
	GPIOINT_CallbackRegister(10, (GPIOINT_IrqCallbackPtr_t)Int1_a_g_Callback);
	GPIO_IntEnable(1<<10);

}



uint8_t Imu_TestFunction()
{
	return QueryRegister1Byte(reg_who_am_i);
}

Imu_Data QueryAllImuValues()
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;

	uint8_t reg = reg_out_x_l_g;
	uint8_t data[12] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff}; //just a default value

	seq.addr  = ADDR_IMU;
	seq.flags = I2C_FLAG_WRITE_READ;
	/* Select register to start reading from */
	seq.buf[0].data = &reg;
	seq.buf[0].len  = 1;
	/* Select length of data to be read */
	seq.buf[1].data = data;
	seq.buf[1].len  = 12;

	ret = I2CBubbl_Transfer(I2C0, &seq);
	if (ret != i2cTransferDone)
	{
		//if there is an error data won't be overwritten? Will 0xff pass through?
	}

	Imu_Data imu_data;
	imu_data.x_gyro = data[1] << 8 | data[0];
	imu_data.y_gyro = data[3] << 8 | data[2];
	imu_data.z_gyro = data[5] << 8 | data[4];
	imu_data.x_accel = data[7] << 8 | data[6];
	imu_data.y_accel = data[9] << 8 | data[8];
	imu_data.z_accel = data[11] << 8 | data[10];


	return imu_data;
}

uint8_t Imu_QueryRegister1Byte(uint8_t reg)
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;

	//uint8_t reg = reg_who_am_i;
	uint8_t data = 0; //just a default value

	seq.addr  = ADDR_IMU;
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

void Imu_WriteRegister1Byte(uint8_t reg, uint8_t data)
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;

	//uint8_t reg = reg_who_am_i;
	//uint8_t data = 0; //just a default value

	seq.addr  = ADDR_IMU;
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

void ClearFifo(void)
{
	for( int i = 0; i < IMU_FIFO_SIZE; i++)
	{
		fifo_data[i].x_accel = 0;
		fifo_data[i].y_accel = 0;
		fifo_data[i].z_accel = 0;
		fifo_data[i].x_gyro = 0;
		fifo_data[i].y_gyro = 0;
		fifo_data[i].z_gyro = 0;
	}
}

void ReadFifo(void)
{
	for( int i = 0; i < IMU_FIFO_SIZE; i++)
	{
		fifo_data[i] = QueryAllImuValues();
	}
}

void Int1_a_g_Callback(void)
{
	ReadFifo();
	WriteFifoOverUsb();
	ClearFifo();
}

void WriteFifoOverUsb(void)
{
	//todo: Should this live here?
	uint8_t dataToWrite[IMU_FIFO_SIZE * IMU_DATA_SIZE_BYTES];

	//package data into 8 byte chunks
	for( int i = 0; i< IMU_FIFO_SIZE; i++)
	{

	}

	//write the data
	//Cli_WriteUSB((void*) *dataToWrite, IMU_FIFO_SIZE * IMU_DATA_SIZE_BYTES);

}
