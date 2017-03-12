/*
 * ErrorHandler.c
 *
 *  Created on: Feb 6, 2017
 *      Author: Summer
 */


#include "ErrorHandler.h"
#include "flash.h"
#include "HelperFunctions.h"
#include "descriptors.h"
#include "timestamp.h"


STATIC_UBUF(errorHandler_data_buff1,  ERRORHANDLER_MAX_ERRORS_PER_CYCLE * ERRORHANDLER_ERROR_SIZE_BYTES);   /* Allocate USB receive buffer.   */
STATIC_UBUF(errorHandler_data_buff2,  ERRORHANDLER_MAX_ERRORS_PER_CYCLE * ERRORHANDLER_ERROR_SIZE_BYTES);   /* Allocate USB receive buffer.   */
uint8_t *errorHandler_data_buffs [2] = {errorHandler_data_buff1, errorHandler_data_buff2};
uint8_t *errorHandler_current_data_buffer = errorHandler_data_buff1;
uint8_t *errorHandler_data_buffer_to_write = errorHandler_data_buff1;

uint8_t _errorHandlerActiveBuffer = 0;
uint8_t _errorHandlerNumberOfErrors = 0;


void NewCycle(void);
void ClearNumberOfErrors(void);

void ErrorHandler_Throw(uint16_t ErrorEnum)
{
	if(_errorHandlerNumberOfErrors >= ERRORHANDLER_MAX_ERRORS_PER_CYCLE)
	{
		return;
	}


	int location_in_buffer = ERRORHANDLER_ERROR_SIZE_BYTES * _errorHandlerNumberOfErrors;

	location_in_buffer = Helper_Write_16bit_To_Buffer(errorHandler_current_data_buffer, location_in_buffer, Flash_Get_ResetCount());
	location_in_buffer = Helper_Write_32bit_To_Buffer(errorHandler_current_data_buffer, location_in_buffer, Time_Get_TimeStamp());
	location_in_buffer = Helper_Write_16bit_To_Buffer(errorHandler_current_data_buffer, location_in_buffer, ErrorEnum);

	_errorHandlerNumberOfErrors += 1;
}


uint8_t ErrorHandler_QueryNumberOfErrors(void)
{
	return _errorHandlerNumberOfErrors;
}



void ClearNumberOfErrors(void)
{
	_errorHandlerNumberOfErrors = 0;
}

void NewCycle(void)
{
	//Call this function when you start a new scheduler cycle

	//Swap buffers
	if(_errorHandlerActiveBuffer)
	{
		errorHandler_current_data_buffer = errorHandler_data_buffs[0];
		_errorHandlerActiveBuffer = 0;
	}
	else
	{
		errorHandler_current_data_buffer = errorHandler_data_buffs[1];;
		_errorHandlerActiveBuffer = 1;
	}

	//Clear number of errors
	ClearNumberOfErrors();
}

ErrorHandler_BufferInfo ErrorHandler_GetBufferAddress(void)
{
	ErrorHandler_BufferInfo bufferInfo;

	if(_errorHandlerActiveBuffer == 1)
	{
		bufferInfo.BufferAddress = errorHandler_data_buff1;
	}
	else
	{
		bufferInfo.BufferAddress = errorHandler_data_buff2;
	}

	bufferInfo.NumberOfErrors = _errorHandlerNumberOfErrors;

	return bufferInfo;
}

ErrorHandler_BufferInfo ErrorHandler_GetBufferAddressAndClearBuffer(void)
{
	ErrorHandler_BufferInfo bufferInfo = ErrorHandler_GetBufferAddress();



	return bufferInfo;
}

