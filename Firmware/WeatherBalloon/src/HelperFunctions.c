/*
 * HelperFunctions.c
 *
 *  Created on: Dec 4, 2016
 *      Author: Summer
 */

#include "HelperFunctions.h"


int Helper_Write_16bit_To_Buffer(uint8_t *buffer, int starting_location, int value_to_write)
{
	int location = starting_location;

	buffer [location] = (uint8_t) (value_to_write>>8);
	buffer [location+1] = (uint8_t) (value_to_write);

	location += 2;
	return location;
}
