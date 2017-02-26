/*
 * HelperFunctions.h
 *
 *  Created on: Dec 4, 2016
 *      Author: Summer
 */

#ifndef HELPERFUNCTIONS_H_
#define HELPERFUNCTIONS_H_
#include <stdint.h>

int Helper_Write_16bit_To_Buffer(uint8_t *buffer, int starting_location, int value_to_write);
int Helper_Write_32bit_To_Buffer(uint8_t *buffer, int starting_location, int value_to_write);

#endif /* HELPERFUNCTIONS_H_ */
