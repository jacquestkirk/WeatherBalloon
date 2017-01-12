/*
 * SPIBubbl.h
 *
 *  Created on: Nov 3, 2016
 *      Author: Mubashir
 */

#ifndef SPIBUBBL_H_
#define SPIBUBBL_H_

#include "spidrv.h"
#include "em_gpio.h"

void SPIBubbl_Initalize(void);
// I2C_TransferReturn_TypeDef I2CBubbl_Transfer(I2C_TypeDef *i2c, I2C_TransferSeq_TypeDef *seq);

void SPIBubbl_Transmit(void *buffer, int size);
void SPIBubbl_Receive_Page(void *page_data, int page);

#endif /* SPIBUBBL_H_ */
