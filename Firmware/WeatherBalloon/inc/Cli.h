/*
 * Cli.h
 *
 *  Created on: May 22, 2016
 *      Author: Summer
 */

#ifndef CLI_H_
#define CLI_H_
#include "Sensors\imu.h"


void Cli_Initialize_Cli(void);
void Cli_WriteUSB(void* message, int dataLen);
void Cli_ReadPressure(void);
void Cli_ReadTemperature(void);
void Cli_ReadImu(void);
void Cli_Read_Magnetometer(void);
void Cli_BlinkLed1(void);
void Cli_BlinkLed2(void);
void Cli_ReadTimeStamp(void);
void Cli_EraseFlash(void);
void Cli_StartRecording(void);
void Cli_StopRecording(void);
void Cli_ReadFromFlash(void);
void Cli_ReadFlashLocations(void);
void Cli_ReadLastFewDataPoints(void);
void Cli_ReadModuleStartAddresses (void);
void Cli_Stream_Imu_Data(Imu_Data imuData);


#endif /* CLI_H_ */
