/*
 * I2CBubbl.c
 *
 *  Created on: Jul 2, 2016
 *      Author: Summer
 *
 *  A lot of this code was stolen from i2cspm.c which is part of the weather station tutorial in Simplicity Studio
 */





#include "I2CBubbl.h"

#include <stddef.h>
#include "em_cmu.h"
#include "em_gpio.h"
//#include "i2cspmconfig.h"
//#include "i2cspm.h"
#include "em_assert.h"

#include "InitDevice.h"


I2C_Init_TypeDef initStruct = {
		.clhr = i2cClockHLRAsymetric,
		.enable = true,
		.freq = I2C_FREQ_FAST_MAX,
		.master = true,
		.refFreq = 0				//Use currently configured reference clock
};



/*******************************************************************************
 **************************   GLOBAL FUNCTIONS   *******************************
 ******************************************************************************/

//I2C is initialized as part of InitDevice.c. I mapped the function over here incase it's needed, but you don't really have to call it.
void I2CBubbl_Initalize(void)
{
	I2C0_enter_DefaultMode_from_RESET();
	//I2C_Init(I2C0, &initStruct);
}

I2C_TransferReturn_TypeDef I2CBubbl_Transfer(I2C_TypeDef *i2c, I2C_TransferSeq_TypeDef *seq)
{
	I2C_TransferReturn_TypeDef ret;
	uint32_t timeout = I2C_TRANSFER_TIMEOUT;


	ret = I2C_TransferInit(I2C0, seq);
	while (ret == i2cTransferInProgress && timeout--)
	{
		ret = I2C_Transfer(I2C0);
	}

	return ret;
}
