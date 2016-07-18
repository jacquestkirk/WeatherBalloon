/*
 * I2CBubbl.h
 *
 *  Created on: Jul 2, 2016
 *      Author: Summer
 */

#ifndef I2CBUBBL_H_
#define I2CBUBBL_H_

#include "em_i2c.h"

#define I2C_TRANSFER_TIMEOUT 300000

void I2CBubbl_Initalize(void);
I2C_TransferReturn_TypeDef I2CBubbl_Transfer(I2C_TypeDef *i2c, I2C_TransferSeq_TypeDef *seq);




#endif /* I2CBUBBL_H_ */
