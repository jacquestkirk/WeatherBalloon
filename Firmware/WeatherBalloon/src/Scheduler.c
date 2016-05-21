/*
 * Scheduler.c
 *
 *  Created on: May 19, 2016
 *      Author: Summer
 */



// Functions
void RunTasks(void);
void DecrementTaskTimer(void);

// Task Functions
// (should these live in the the other modules? I'll just leave these here and call into other modules in case these are needed. can refactor later)
void TskReadPressure(void);
void TskReadTemps(void);
void TskReadAccel(void);
void TskReadGyro(void);
void TskReadMagnetometer(void);
void TskTurnOnLed1(void);
void TskTurnOnLed2(void);
void TskTurnOffLed1(void);
void TskTurnOffLed2(void);
void TskPollBtn1(void);
void TskPollBtn2(void);
void TskTimeStanp(void);
void TskWriteToFlash(void);
void TskCheckCli(void);

//defines
#define READ_PRESSURE_RESET			1
#define READ_TEMPS_RESET			1
#define READ_ACCEL_RESET			1
#define READ_GYRO_RESET				1
#define READ_MAGNETOMETER_RESET		1
#define TURN_ON_LED1_RESET			1
#define TURN_ON_LED2_RESET			1
#define TURN_OFF_LED1_RESET			1
#define TURN_OFF_LED2_RESET			1
#define POLL_BTN1_RESET				1
#define POLL_BTN2_RESET				1
#define TIMESTAMP_RESET				1
#define WRITE_TO_FLASH_RESET		1
#define CHECK_CLI_RESET				1


/*To add a new task...
 * 1) Add a new enum to Tasks
 * 2) Add a new #def reset value
 * 3) Add a new Tsk function
 * 4) Add a new if statement to RunTasks
 */

enum Tasks
{
	ReadPressure,
	ReadTemps,
	ReadAccel,
	ReadGyro,
	ReadMagnetometer,
	TurnOnLed1,
	TurnOnLed2,
	TurnOffLed1,
	TurnOffLed2,
	PollBtn1,
	PollBtn2,
	TimeStanp,
	WriteToFlash,
	CheckCli,
	TotalNumOfTasks //A place holder to make the compiler keep track of the last task
};


int TaskTimer[TotalNumOfTasks]; //count down to 0 when it's time to run a task. Index of the timer corresponds to Tasks enum.
char _continue_running_scheduler = 0;


//public functions
void Sch_Initilize_Scheduler(void)
{
	_continue_running_scheduler = 1;
	//tbd
}

void Sch_Run_Scheduler(void)
{
	while(_continue_running_scheduler)
	{
		RunTasks();
		//Add a sleep function here.
	}
}

void Sch_Stop_Scheduler (void)
{
	//tbd
}


//private functions
void RunTasks(void)
{
	//ReadPressure
	if (TaskTimer[ReadPressure] == 0)							//If it is time to run the task
	{
		TskReadPressure();										//Run the task
		TaskTimer[ReadPressure] = READ_PRESSURE_RESET;			//Reset the task timer
	}

	//ReadTemps
	if (TaskTimer[ReadTemps] == 0)
	{
		TskReadTemps();
		TaskTimer[ReadTemps] = READ_TEMPS_RESET;
	}

	//ReadAccel
	if (TaskTimer[ReadAccel] == 0)
	{
		TskReadAccel();
		TaskTimer[ReadAccel] = READ_ACCEL_RESET;
	}
	//ReadGyro
	if (TaskTimer[ReadGyro] == 0)
	{
		TskReadGyro();
		TaskTimer[ReadGyro] = READ_GYRO_RESET;
	}
	//ReadMagnetometer
	if (TaskTimer[ReadMagnetometer] == 0)
	{
		TskReadMagnetometer();
		TaskTimer[ReadMagnetometer] = READ_MAGNETOMETER_RESET;
	}
	//TurnOnLed1
	if (TaskTimer[TurnOnLed1] == 0)
	{
		TskTurnOnLed1();
		TaskTimer[TurnOnLed1] = TURN_ON_LED1_RESET;
	}
	//TurnOnLed2
	if (TaskTimer[TurnOnLed2] == 0)
	{
		TskTurnOnLed2();
		TaskTimer[TurnOnLed2] = TURN_ON_LED2_RESET;
	}
	//TurnOffLed1
	if (TaskTimer[TurnOffLed1] == 0)
	{
		TskTurnOffLed1();
		TaskTimer[TurnOffLed1] = TURN_OFF_LED1_RESET;
	}
	//TurnOffLed2
	if (TaskTimer[TurnOffLed2] == 0)
	{
		TskTurnOffLed2();
		TaskTimer[TurnOffLed2] = TURN_OFF_LED2_RESET;
	}
	//PollBtn1
	if (TaskTimer[PollBtn1] == 0)
	{
		TskPollBtn1();
		TaskTimer[PollBtn1] = POLL_BTN1_RESET;
	}
	//PollBtn2
	if (TaskTimer[PollBtn2] == 0)
	{
		TskPollBtn1();
		TaskTimer[PollBtn2] = POLL_BTN1_RESET;
	}
	//TimeStanp
	if (TaskTimer[TimeStanp] == 0)
	{
		TskTimeStanp();
		TaskTimer[TimeStanp] = TIMESTAMP_RESET;
	}
	//WriteToFlash
	if (TaskTimer[WriteToFlash] == 0)
	{
		TskWriteToFlash();
		TaskTimer[WriteToFlash] = WRITE_TO_FLASH_RESET;
	}
	//CheckCli
	if (TaskTimer[CheckCli] == 0)
	{
		TskCheckCli();
		TaskTimer[CheckCli] = CHECK_CLI_RESET;
	}

}


void DecrementTaskTimer()
{
	for( int i = 0; i<TotalNumOfTasks; i++ )
	{
		TaskTimer[i] -=1;
	}
}


///
///Tasks/////////////////////////////////////////////////////////////////////////////////////////////////////////
///

void TskReadPressure(void)
{
	//TBD
}
void TskReadTemps(void)
{
	//TBD
}
void TskReadAccel(void)
{
	//TBD
}
void TskReadGyro(void)
{
	//TBD
}
void TskReadMagnetometer(void)
{
	//TBD
}
void TskTurnOnLed1(void)
{
	//TBD
}
void TskTurnOnLed2(void)
{
	//TBD
}
void TskTurnOffLed1(void)
{
	//TBD
}
void TskTurnOffLed2(void)
{
	//TBD
}
void TskPollBtn1(void)
{
	//TBD
}
void TskPollBtn2(void)
{
	//TBD
}
void TskTimeStanp(void)
{
	//TBD
}
void TskWriteToFlash(void)
{
	//TBD
}
void TskCheckCli(void)
{
	//TBD
}




