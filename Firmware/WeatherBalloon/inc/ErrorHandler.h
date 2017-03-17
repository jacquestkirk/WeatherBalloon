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
	ErrorHandler_Enum_Error_Imu_Buffer_Timeout,
	ErrorHandler_Enum_Error_Imu_Buffer_Overflow,
}ErrorHandler_Enum_Error;

typedef struct ErrorHandler_Error{
	uint16_t resetNumber;
	uint32_t timestamp;
	uint16_t errorEnum;
}ErrorHandler_Error;

typedef struct ErrorHandler_BufferInfo{
	uint8_t* BufferAddress;
	uint8_t NumberOfErrors;
}ErrorHandler_BufferInfo;

void ErrorHandler_Throw(uint16_t ErrorEnum);


ErrorHandler_BufferInfo ErrorHandler_GetBufferAddress(void);
ErrorHandler_BufferInfo ErrorHandler_GetBufferAddressAndClearBuffer(void);
#endif /* ERRORHANDLER_H_ */
