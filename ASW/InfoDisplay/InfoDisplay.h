/*
 * InfoDisplay.h
 *
 *  Created on: 2024Äê1ÔÂ27ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_APPLICATION_INFODISPLAY_INFODISPLAY_H_
#define SRC_APPLICATION_INFODISPLAY_INFODISPLAY_H_

#include "AfeCalibration.h"
#include "AfeDeviceSim.h"
#include "BatterySim.h"
#include "lcd.h"

#include "TouchKey.h"
#include "TouchMenu.h"

typedef void (*InfoDisplay_ScreenType)(void * par);
typedef void (*InfoDisplay_ServiceType)(void * par);
typedef void* InfoDisplay_ServiceHandler;
typedef struct
{
	InfoDisplay_ScreenType screen[32];
	InfoDisplay_ServiceType serviceFunction[32];
	InfoDisplay_ServiceHandler serviceHandler[32];
	u32 screenMaxNum;
	s32 currentScreen;
	s32 switchScreenLock;
	s32 switchScreenCmd;
}InfoDisplay_ManagerType;

extern void InfoDisplay_Init(void);
extern void InfoDisplay_Run50ms(void);
extern void InfoDisplay_Run100ms(void);

#endif /* SRC_APPLICATION_INFODISPLAY_INFODISPLAY_H_ */
