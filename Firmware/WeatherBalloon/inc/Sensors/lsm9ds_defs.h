/*
 * lsm9ds_defs.h
 *
 *  Created on: Jul 3, 2016
 *      Author: Summer
 */

#ifndef LSM9DS_DEFS_H_
#define LSM9DS_DEFS_H_

enum register_map //Register Map for IMU
{
	reg_act_ths				= 0x04,
	reg_act_dur				= 0x05,
	reg_int_gen_cfg_xl		= 0x06,
	reg_int_gen_ths_x_xl	= 0x07,
	reg_int_gen_ths_y_xl	= 0x08,
	reg_int_gen_ths_z_xl	= 0x09,
	reg_int_gen_dur_xl		= 0x0a,
	reg_reference_g			= 0x0b,
	reg_int1_ctrl			= 0x0c,
	reg_int2_ctrl			= 0x0d,

	reg_who_am_i			= 0x0f,
	reg_ctrl_reg1_g			= 0x10,
	reg_ctrl_reg2_g			= 0x11,
	reg_ctrl_reg3_g			= 0x12,
	reg_orient_cfg_g		= 0x13,
	reg_int_gen_src_g		= 0x14,
	reg_out_temp_l			= 0x15,
	reg_out_temp_h			= 0x16,
	reg_status_reg			= 0x17,
	reg_out_x_l_g			= 0x18,
	reg_out_x_h_g			= 0x19,
	reg_out_y_l_g			= 0x1A,
	reg_out_y_h_g			= 0x1B,
	reg_out_z_l_g			= 0x1C,
	reg_out_z_h_g			= 0x1D,
	reg_ctrl_reg4			= 0x1E,
	reg_ctrl_reg5_xl		= 0x1F,
	reg_ctrl_reg6_xl		= 0x20,
	reg_ctrl_reg7_xl		= 0x21,
	reg_ctrl_reg8			= 0x22,
	reg_ctrl_reg9			= 0x23,
	reg_ctrl_reg10			= 0x24,

	res_int_gen_src_xl		= 0x26,
	res_status_reg			= 0x27,
	reg_out_x_l_xl			= 0x28,
	reg_out_x_h_xl			= 0x29,
	reg_out_y_l_xl			= 0x2A,
	reg_out_y_h_xl			= 0x2B,
	reg_out_z_l_xl			= 0x2C,
	reg_out_z_h_xl			= 0x2D,
	reg_fifo_ctrl			= 0x2E,
	reg_fifo_src			= 0x2F,
	reg_int_gen_cfg_g		= 0x30,
	reg_int_gen_ths_xh_g	= 0x31,
	reg_int_gen_ths_xl_g	= 0x32,
	reg_int_gen_ths_yh_g	= 0x33,
	reg_int_gen_ths_yl_g	= 0x34,
	reg_int_gen_ths_zh_g	= 0x35,
	reg_int_gen_ths_zl_g	= 0x36,
	reg_int_gen_dur_g		= 0x37
};

///////////////////////////////////////////////////////////////
//CTRL_REG_1_G
///////////////////////////////////////////////////////////////

	//Gyro Data Rate
	#define CTRL_REG_1_G_ODR_G_SHIFT	5
	#define CTRL_REG_1_G_ODR_G_MASK 0b11100000
	enum gyro_odr
	{
		CTRL_REG_1_G_ODR_G_PD,	// Power down (0)
		CTRL_REG_1_G_ODR_G_14p9,	// 14.9 Hz (1)
		CTRL_REG_1_G_ODR_G_59p5,	// 59.5 Hz (2)
		CTRL_REG_1_G_ODR_G_119,	// 119 Hz (3)
		CTRL_REG_1_G_ODR_G_238,	// 238 Hz (4)
		CTRL_REG_1_G_ODR_G_476,	// 476 Hz (5)
		CTRL_REG_1_G_ODR_G_952	// 952 Hz (6)
	};

	#define CTRL_REG_1_G_BW_G1_SHIFT 0
	#define CTRL_REG_1_G_BW_G1_MASK 0b00011000
	//No Enum, use numbers


	#define CTRL_REG_1_G_FS_SHIFT 3
	#define CTRL_REG_1_G_FS_MASK 0b00000011
	enum gyro_fs
	{
		CTRL_REG_1_G_FS_245DPS = 0,
		CTRL_REG_1_G_FS_500DPS = 1,
		CTRL_REG_1_G_FS_2000DPS = 3
	};


///////////////////////////////////////////////////////////////
//CTRL_REG_2_G
///////////////////////////////////////////////////////////////

	#define CTRL_REG_2_G_INT_SEL_SHIFT 3
	#define CTRL_REG_2_G_INT_SEL_MASK 0b00001100
	enum gyro_int_select
	{
		CTRL_REG_2_G_INT_SEL_LPF1,
		CTRL_REG_2_G_INT_SEL_HPF,
		CTRL_REG_2_G_INT_SEL_LPF2
	};

	#define CTRL_REG_2_G_OUT_SEL_SHIFT 3
	#define CTRL_REG_2_G_OUT_SEL_MASK 0b00001100
	enum gyro_out_select
	{
		CTRL_REG_2_G_OUT_SEL_LPF1,
		CTRL_REG_2_G_OUT_SEL_HPF,
		CTRL_REG_2_G_OUT_SEL_LPF2
	};

///////////////////////////////////////////////////////////////
//CTRL_REG_3_G
///////////////////////////////////////////////////////////////
	#define CTRL_REG_3_G_LPMODE_SHIFT 7
	#define CTRL_REG_3_G_LPMODE_MASK 0b10000000
	enum gyrolpmode
	{
		CTRL_REG_3_G_LPMODE_DISABLED,
		CTRL_REG_3_G_LPMODE_ENABLED
	};

	#define CTRL_REG_3_G_HP_EN_SHIFT 6
	#define CTRL_REG_3_G_HP_EN_MASK 0b01000000
	//No Enum, enable and disable are self expanitory

	#define CTRL_REG_3_G_HPCF_SHIFT 4
	#define CTRL_REG_3_G_HPCF_MASK 0b00001111
	//No enum just use numbers

///////////////////////////////////////////////////////////////
//CTRL_REG_4_G
///////////////////////////////////////////////////////////////
	#define CTRL_REG_4_ZEN_G_SHIFT 5
	#define CTRL_REG_4_ZEN_G_MASK 0b00100000

	#define CTRL_REG_4_YEN_G_SHIFT 4
	#define CTRL_REG_4_YEN_G_MASK 0b00010000

	#define CTRL_REG_4_XEN_G_SHIFT 3
	#define CTRL_REG_4_XEN_G_MASK 0b00001000

	#define CTRL_REG_4_LIR_XL1_SHIFT 1
	#define CTRL_REG_4_LIR_XL1_MASK 0b00000010

	//not supporting 4DXL I'm not really sure what they do so I probably won't use them


///////////////////////////////////////////////////////////////
//CTRL_REG_5_XL
///////////////////////////////////////////////////////////////
	#define CTRL_REG_5_XL_DEC_SHIFT 6
	#define CTRL_REG_5_XL_DEC_MASK 0b11000000
	enum accel_dec
	{
		CTRL_REG_5_XL_DEC_NONE,
		CTRL_REG_5_XL_DEC_2x,
		CTRL_REG_5_XL_DEC_4x,
		CTRL_REG_5_XL_DEC_8x
	};

	#define CTRL_REG_5_XL_ZEN_XL_SHIFT 5
	#define CTRL_REG_5_XL_ZEN_XL_MASK 0b00100000

	#define CTRL_REG_5_XL_YEN_XL_SHIFT 4
	#define CTRL_REG_5_XL_YEN_XL_MASK 0b00010000

	#define CTRL_REG_5_XL_XEN_XL_SHIFT 3
	#define CTRL_REG_5_XL_XEN_XL_MASK 0b00001000

///////////////////////////////////////////////////////////////
//CTRL_REG_6_XL
///////////////////////////////////////////////////////////////
#define CTRL_REG_6_XL_ODR_XL_SHIFT 5
#define CTRL_REG_6_XL_ODR_XL_MASK 0b11100000
enum accel_data_rate
{
	CTRL_REG_6_XL_ODR_XL_PD,
	CTRL_REG_6_XL_ODR_XL_10HZ,
	CTRL_REG_6_XL_ODR_XL_50HZ,
	CTRL_REG_6_XL_ODR_XL_119HZ,
	CTRL_REG_6_XL_ODR_XL_238_HZ,
	CTRL_REG_6_XL_ODR_XL_476_HZ,
	CTRL_REG_6_XL_ODR_XL_952_HZ
};

#define CTRL_REG_6_XL_FS_XL_SHIFT 3
#define CTRL_REG_6_XL_FS_XL_MASK 0b00011000
enum accel_full_scale
{
	CTRL_REG_6_XL_FS_XL_2g,
	CTRL_REG_6_XL_FS_XL_16g,
	CTRL_REG_6_XL_FS_XL_4g,
	CTRL_REG_6_XL_FS_XL_8g,
};

#define CTRL_REG_6_XL_BW_SCAL_ODR_SHIFT 2
#define CTRL_REG_6_XL_BW_SCAL_ODR_MASK	0b00000100

#define CTRL_REG_6_XL_BW_XL_SHIFT 0
#define CTRL_REG_6_XL_BW_XL_MASK 0b00000011
enum accel_anti_aliasing_bandwidth
{
	CTRL_REG_6_XL_BW_XL_408Hz,
	CTRL_REG_6_XL_BW_XL_211Hz,
	CTRL_REG_6_XL_BW_XL_105Hz,
	CTRL_REG_6_XL_BW_XL_50Hz
};

///////////////////////////////////////////////////////////////
//CTRL_REG_7_XL
///////////////////////////////////////////////////////////////
//todo: Do we need to upsample and use some of these filters?
#define CTRL_REG_7_XL_HR_EN_SHIFT 7
#define CTRL_REG_7_XL_HR_EN_MASK 0b10000000

#define CTRL_REG_7_XL_DCF_SHIFT 5
#define CTRL_REG_7_XL_DCF_MASK 0b01100000
enum accel_digital_filt_cutoff
{
	CTRL_REG_7_XL_DCF_DIV_50,
	CTRL_REG_7_XL_DCF_DIV_100,
	CTRL_REG_7_XL_DCF_DIV_9,
	CTRL_REG_7_XL_DCF_DIV400
};

#define CTRL_REG_7_XL_FDS_EN_SHIFT 2
#define CTRL_REG_7_XL_FDS_EN_MASK 0b00000100

#define CTRL_REG_7_XL_HPIS_EN_SHIFT 0
#define CTRL_REG_7_XL_HPIS_EN_MASK 0b00000001

///////////////////////////////////////////////////////////////
//CTRL_REG_8
///////////////////////////////////////////////////////////////
#define CTRL_REG_8_IF_ADD_INC_SHIFT 2
#define CTRL_REG_8_IF_ADD_INC_MASK 0b00000100

#define CTRL_REG_8_SW_RESET_SHIFT 0
#define CTRL_REG_8_SW_RESET_MASK 0b00000001


///////////////////////////////////////////////////////////////
//CTRL_REG_9
///////////////////////////////////////////////////////////////
#define CTRL_REG_9_SLEEP_G_SHIFT 6
#define CTRL_REG_9_SLEEP_G_MASK 0b01000000

#define CTRL_REG_9_FIFO_TEMP_EN_SHIFT 4
#define CTRL_REG_9_FIFO_TEMP_EN_MASK 0b00010000

#define CTRL_REG_9_DRDY_MASK_BIT_SHIFT 3
#define CTRL_REG_9_DRDY_MASK_BIT_MASK 0b00001000

#define CTRL_REG_9_I2C_DISABLE_SHIFT 2
#define CTRL_REG_9_I2C_DISABLE_MASK 0b00000100

#define CTRL_REG_9_FIFO_EN_SHIFT 1
#define CTRL_REG_9_FIFO_EN_MASK 0b00000010

#define CTRL_REG_9_STOP_ON_FTH 0
#define CTRL_REG_9_STOP_ON_MASK 0b00000001


///////////////////////////////////////////////////////////////
//FIFO_CTRL
///////////////////////////////////////////////////////////////

#define FIFO_CTRL_FMODE_SHIFT 5
#define FIFO_CTRL_FMODE_MASK 0b11100000
enum fifo_ctrl_fmode
{
	FIFO_CTRL_FMODE_BYPASS=0,
	FIFO_CTRL_FMODE_FIFO=1,
	FIFO_CTRL_FMODE_CONTINUOUS = 3,
	FIFO_CTRL_FMODE_BYPASS_2_CONT = 4,
	FIFO_CTRL_FMODE_CONT_OVERWRITE = 6
};

#define FIFO_CTRL_FTH_SHIFT 0
#define FIFO_CTRL_FTH_MASK 0b00011111

///////////////////////////////////////////////////////////////
//FIFO_SRC
///////////////////////////////////////////////////////////////
#define FIFO_SRC_FTH_SHIFT 7
#define FIFO_SRC_FTH_MASK 0b10000000

#define FIFO_SRC_OVRN_SHIFT 6
#define FIFO_SRC_OVRN_MASK 0b01000000

#define FIFO_SRC_FSS_SHIFT 0
#define FIFO_SRC_FSS_MASK 0b00111111


///////////////////////////////////////////////////////////////
//INT1_CTRL
///////////////////////////////////////////////////////////////
#define INT1_CTRL_1G_G_SHIFT 7
#define INT1_CTRL_1G_G_MASK 0b10000000

#define INT1_CTRL_1G_XL_SHIFT 6
#define INT1_CTRL_1G_XL_MASK 0b01000000

#define INT1_CTRL_FSS5_SHIFT 5
#define INT1_CTRL_FSS5_MASK 0b00100000

#define INT1_CTRL_OVR_SHIFT 4
#define INT1_CTRL_OVR_MASK 0b000010000

#define INT1_CTRL_FTH_SHIFT 3
#define INT1_CTRL_FTH_MASK 0b00001000

#define INT1_CTRL_BOOT_SHIFT 2
#define INT1_CTRL_BOOT_MASK 0b00000100

#define INT1_CTRL_DRDY_G_SHIFT 1
#define INT1_CTRL_DRDY_G_MASK 0b00000010

#define INT1_CTRL_DRDY_XL_SHIFT 0
#define INT1_CTRL_DRDY_XL_MASK 0b00000001

#endif /* LSM9DS_DEFS_H_ */
