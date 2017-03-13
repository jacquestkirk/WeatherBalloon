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
void Helper_Debug_Buffer_Append(uint8_t data_to_append);

#endif /* HELPERFUNCTIONS_H_ */
