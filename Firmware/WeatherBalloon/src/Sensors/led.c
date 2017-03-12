/*
 * led.c
 *
 *  Created on: Jun 19, 2016
 *      Author: Summer
 */


#include "Sensors/led.h"
#include "em_gpio.h"

uint8_t _led1_state = 0;
uint8_t _led2_state = 1;

void Led_Blink_1(void)
{
	//Todo: It would be nice if we actually did something here.
}
void Led_Blink_2(void)
{
	//Todo: It would be nice if we actually did something here.
}

void Led_1_On_Tsk(void)
{
	//Todo: It would be nice if we actually did something here.
}

void Led_2_On_Tsk(void)
{
	//Todo: It would be nice if we actually did something here.
}

void Led_1_Off_Tsk(void)
{
	//Todo: It would be nice if we actually did something here.
}

void Led_2_Off_Tsk(void)
{
	//Todo: It would be nice if we actually did something here.
}

void Led_On_1(void)
{
	GPIO_PinOutClear(gpioPortC, 2);
	_led1_state = 1;
}

void Led_Off_1(void)
{
	GPIO_PinOutSet(gpioPortC,2);
	_led1_state = 0;
}
void Led_On_2(void)
{
	GPIO_PinOutClear(gpioPortC, 3);
	_led2_state = 1;
}

void Led_Off_2(void)
{
	GPIO_PinOutSet(gpioPortC,3);
	_led2_state = 0;
}

void Led_Toggle_1(void)
{
	if(_led1_state)
	{
		Led_Off_1();
	}
	else
	{
		Led_On_1();
	}
}

void Led_Toggle_2(void)
{
	if(_led2_state)
	{
		Led_Off_2();
	}
	else
	{
		Led_On_2();
	}
}
