/*
 * HelperFunctions.c
 *
 *  Created on: Dec 4, 2016
 *      Author: Summer
 */

#include "HelperFunctions.h"
#define DEBUG_BUFFER_SIZE 256

uint8_t debug_buffer[DEBUG_BUFFER_SIZE];

int Helper_Write_16bit_To_Buffer(uint8_t *buffer, int starting_location, int value_to_write)
{
	int location = starting_location;

	buffer [location] = (uint8_t) (value_to_write>>8);
	buffer [location+1] = (uint8_t) (value_to_write);

	location += 2;
	return location;
}

int Helper_Write_32bit_To_Buffer(uint8_t *buffer, int starting_location, int value_to_write)
{
	int location = starting_location;

	buffer [location] = (uint8_t) (value_to_write>>24);
	buffer [location+1] = (uint8_t) (value_to_write>>16);
	buffer [location+2] = (uint8_t) (value_to_write>>8);
	buffer [location+3] = (uint8_t) (value_to_write);

	location += 4;
	return location;
}

void Helper_Debug_Buffer_Append(uint8_t data_to_append)
{
	static int index = 0;

	//If the index is larger than the buffer size, loop around.
	if (index >= DEBUG_BUFFER_SIZE)
	{
		index =0;
	}

	debug_buffer[index] = data_to_append;

	index ++;
}
