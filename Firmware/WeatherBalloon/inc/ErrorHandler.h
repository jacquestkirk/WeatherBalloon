/*
 * ErrorHandler.h
 *
 *  Created on: Feb 6, 2017
 *      Author: Summer
 */

#ifndef ERRORHANDLER_H_
#define ERRORHANDLER_H_

#include <stdint.h>
#define ERRORHANDLER_ERROR_SIZE_BYTES 8
#define ERRORHANDLER_MAX_ERRORS_PER_CYCLE 10


typedef enum ErrorHandler_Enum_Error{
	ErrorHandler_Enum_Error_TestError,
	ErrorHandler_Enum_Error_SchedulerNotComplete,
}ErrorHandler_Enum_Error;

typedef struct ErrorHandler_Error{
	uint16_t resetNumber;
	uint32_t timestamp;
	uint16_t errorEnum;
}ErrorHandler_Error;


void ErrorHandler_Throw(uint16_t ErrorEnum);
uint8_t ErrorHandler_QueryNumberOfErrors(void);
void ErrorHandler_ClearNumberOfErrors(void);
void ErrorHandler_NewCycle(void);
uint8_t* ErrorHandler_GetBufferAddress(void);
#endif /* ERRORHANDLER_H_ */
