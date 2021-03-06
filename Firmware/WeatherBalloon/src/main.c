/**************************************************************************//**
 * @file
 * @brief Empty Project
 * @author Energy Micro AS
 * @version 3.20.2
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2014 Silicon Labs, http://www.silabs.com</b>
 *******************************************************************************
 *
 * This file is licensed under the Silicon Labs Software License Agreement. See 
 * "http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt"  
 * for details. Before using this software for any purpose, you must agree to the 
 * terms of that agreement.
 *
 ******************************************************************************/
//Silabs modules
#include "em_device.h"
#include "em_chip.h"

//my modules
#include "Scheduler.h"
#include "Cli.h"
#include "InitDevice.h"
#include "timestamp.h"

//Eventually remove these
#include "Sensors/imu.h"
#include "Sensors/magnetometer.h"
/**************************************************************************//**
 * @brief  Main function
 *****************************************************************************/
int main(void)
{
  /* Chip errata */
  CHIP_Init();

  enter_DefaultMode_from_RESET();


  Imu_Initialize();
  //Imu_Initialize_OneShot();
  Mag_Initialize_OneShot();
  Cli_Initialize_Cli();
  Time_Initilize_TimeStamp();
  Sch_Initilize_Scheduler();
  /* Infinite loop */
  Sch_Run_Scheduler();




  while(1)
  {
	  //Temporary while loop
	  //Imu_TestFunction();
	  //Imu_Read();
	  Imu_WriteStreamFifo();
  }

}


void InitializeMain(void)
{
	Sch_Initilize_Scheduler();
	Cli_Initialize_Cli();
}
