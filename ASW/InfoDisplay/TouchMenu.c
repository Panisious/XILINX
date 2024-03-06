/*
 * TouchMenu.c
 *
 *  Created on: 2024Äê2ÔÂ4ÈÕ
 *      Author: Administrator
 */

#include "TouchMenu.h"

TouchKeyType debugKey;

void TouchMenu_Init(void)
{
	//TouchKey_Init(&debugKey, 400, 440, 80, 24, "TestKey", NULL);
}

void TouchMenu_Run200ms(void)
{
	static uint8 debug = 0;
	if(debug % 2 )
	{
		//debugKey.displaySelf(&debugKey);
	}
	else
	{
		//debugKey.clearSelf(&debugKey);
	}
	debug++ ;
}
