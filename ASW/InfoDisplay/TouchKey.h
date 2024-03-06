/*
 * TouchKey.h
 *
 *  Created on: 2024Äê2ÔÂ4ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_ASW_INFODISPLAY_TOUCHKEY_H_
#define SRC_ASW_INFODISPLAY_TOUCHKEY_H_

#include "RTE.h"

#include "ZYNQ_BasicType.h"

#include "LCD.h"

typedef struct
{
	uint8 prevPressState;
	uint8 currentPressState;
	uint8 pressDown;
	uint8 pressUp;
}TouchStateType;

typedef struct TouchKeyType_Tag
{
	uint16 x;
	uint16 y;
	uint16 ex;
	uint16 ey;
	uint16 width;
	uint16 height;
	TouchStateType touchState;
	const uint8 * name;
	void * handler;
	void (*displaySelf)(struct TouchKeyType_Tag * self);
	void (*clearSelf)(struct TouchKeyType_Tag * self);
	void (*touchDetect)(struct TouchKeyType_Tag * self);
	void (*function)(struct TouchKeyType_Tag * self);
}TouchKeyType;

typedef struct
{
	uint16 keyNum;
	uint16 keyPage;
	TouchKeyType * key;
}TouchKeyManagerType;

extern void TouchKey_DisplaySelf(TouchKeyType* key);

#endif /* SRC_ASW_INFODISPLAY_TOUCHKEY_H_ */
