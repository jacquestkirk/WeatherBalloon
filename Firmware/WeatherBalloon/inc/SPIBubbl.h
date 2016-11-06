/*
 * SPIBubbl.h
 *
 *  Created on: Nov 3, 2016
 *      Author: Mubashir
 */

#ifndef SPIBUBBL_H_
#define SPIBUBBL_H_

#include "spidrv.h"

void SPIBubbl_Initalize(void);
// I2C_TransferReturn_TypeDef I2CBubbl_Transfer(I2C_TypeDef *i2c, I2C_TransferSeq_TypeDef *seq);

void SPIBubbl_Transmit(uint8_t buffer, int size);



#endif /* SPIBUBBL_H_ */
