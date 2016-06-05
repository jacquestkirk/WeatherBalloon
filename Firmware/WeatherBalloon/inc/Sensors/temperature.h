/*
 * temp.h
 *
 *  Created on: Jun 5, 2016
 *      Author: Summer
 */

#ifndef TEMP_H_
#define TEMP_H_

#define TEMP_DATA_SIZE 8
#include <stdint.h>
typedef struct Temp_Data{
	int16_t therm1;
	int16_t therm2;
	int16_t therm3;
	int16_t therm4;
}Temp_Data;

Temp_Data Temp_Read(void);//Read and return
void Temp_Read_Tsk(void);//Read and store in local ram

#endif /* TEMP_H_ */
