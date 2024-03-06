/*
 * TouchKey.c
 *
 *  Created on: 2024Äê2ÔÂ4ÈÕ
 *      Author: Administrator
 */

#include "TouchKey.h"



void TouchKey_ConnectFunction(TouchKeyType *key, void (*function)(void * par))
{
	key->function = function;
}

void TouchKey_DisplaySelf(TouchKeyType* key)
{
	LCD_DrawLine(key->x,  key->y,  key->x,  key->ey);
	LCD_DrawLine(key->ex,  key->y,  key->ex,  key->ey);
	LCD_DrawLine(key->x,  key->y,  key->ex,  key->y);
	LCD_DrawLine(key->x,  key->ey,  key->ex,  key->ey);

	uint16 namex = key->x + 8;
	uint16 namey = key->y + 4;
	LCD_DrawString(namex, namey, key->name);
}

void TouchKey_ClearSelf(TouchKeyType* key)
{
	LCD_ClearArea(key->x,  key->y,  key->ex,  key->ey);
}

void TouchKey_Detect(TouchKeyType* key)
{
	uint8 touchState;
	RTE_Read_LCDTouchState(&touchState);
	sint16 xy[2];
	RTE_Read_LCDTouchCoordinate(xy);
	if(TRUE == touchState)
	{
		if(xy[0] < key->ex && xy[0] > key->x && xy[1] < key->ey && xy[1] > key->y)
		{
			key->touchState.currentPressState = TRUE;
		}
	}
	else
	{
		key->touchState.currentPressState = FALSE;
	}

	if(key->touchState.currentPressState && !key->touchState.prevPressState)
	{
		key->touchState.pressDown = TRUE;
	}

	if(key->touchState.prevPressState && !key->touchState.currentPressState)
	{
		key->touchState.pressUp = TRUE;
	}

	key->touchState.prevPressState = key->touchState.currentPressState;

	if(key->function != NULL)
	{
		key->function(key);
	}
}

void TouchKey_Init(TouchKeyType *key, uint16 x, uint16 y, uint16 width, uint16 height, uint8 * name, void (*function)(TouchKeyType * self))
{
	key->x = x;
	key->y = y;
	key->width = width;
	key->height = height;
	key->ex = key->x + key->width;
	key->ey = key->y + key->height;
	key->name = name;
	key->displaySelf = &TouchKey_DisplaySelf;
	key->clearSelf = &TouchKey_ClearSelf;
	key->touchDetect = &TouchKey_Detect;
	key->function = function;
}
