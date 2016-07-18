/*
 * magnetometer.c
 *
 *  Created on: Jun 19, 2016
 *      Author: Summer
 */


#include "Sensors/magnetometer.h"
#include "descriptors.h"
#include "flash.h"

STATIC_UBUF(mag_data_buff,  FLASH_PAGE_SIZE_BYTES);   /* Allocate USB receive buffer.   */

#define ADDR_MAG_READ	0x39
#define ADDR_IMU_WRITE	0x38

enum register_map //Register Map for Magnetometer
{
	reg_offset_x_reg_l_m 	= 0x05,
	reg_offset_x_reg_h_m 	= 0x06,
	reg_offset_y_reg_l_m 	= 0x07,
	reg_offset_y_reg_h_m 	= 0x08,
	reg_offset_z_reg_l_m 	= 0x09,
	reg_offset_z_reg_h_m 	= 0x0A,

	reg_who_am_i 			= 0x0F,

	reg_ctrl_reg1_m			= 0x20,
	reg_ctrl_reg2_m			= 0x21,
	reg_ctrl_reg3_m			= 0x22,
	reg_ctrl_reg4_m			= 0x23,
	reg_ctrl_reg5_m			= 0x24,

	reg_status_reg_m		= 0x27,

	reg_out_x_l_m			= 0x28,
	reg_out_x_h_m			= 0x20,
	reg_out_y_l_m			= 0x2A,
	reg_out_y_h_m			= 0x2B,
	reg_out_z_l_m			= 0x2C,
	reg_out_z_h_m			= 0x2D,

	reg_int_cfg_m			= 0x30,
	reg_int_src_m			= 0x31,
	reg_int_ths_l_m			= 0x32,
	reg_int_ths_h_m			= 0x33

};

Mag_Data Mag_Read(void)
{
	Mag_Data magdata;

	//Todo: actually read this data
	magdata.x_mag  = 0x1111;
	magdata.y_mag  = 0x2222;
	magdata.z_mag  = 0x3333;



	return magdata;
}
void Mag_Read_Tsk(void)
{
	//Read
	//Write to tem_dat_buff
	//If address gets to FLASH_PAGE_SIZE_BYTES
	//then write temp_data_buff to flash
}
