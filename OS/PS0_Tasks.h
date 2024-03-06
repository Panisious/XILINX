/*
 * PS0_Tasks.h
 *
 *  Created on: 2024Äê1ÔÂ27ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_PS0_TASKS_H_
#define SRC_PS0_TASKS_H_

#include "Project_Define.h"

#include "xtime_l.h"

#include "FreeRTOS.h"
#include "task.h"

#ifdef ZYNQ_USE_7020
#include "lcd.h"
#include "TouchMenu.h"
#endif

#include <Comm_Control.h>
#include "AfeSim.h"

#include "BatterySim.h"
#ifdef ZYNQ_USE_7020
#include "InfoDisplay.h"
#endif

typedef enum
{
	OS_Task_LCD,
	OS_Task_Comm,
	OS_Task_BatSim,
	OS_Task_AfeSim,
	OS_Task_Num
}OS_TaskType;

typedef enum
{
	OS_Screen_CPU_Loading,
}OS_DebugScreenType;

typedef struct
{
	/*Task*/
	uint32 RunCnt;
	uint8 RunIndex;
	uint32 RunTimeSequence[256];
	uint32 RunTimeCnt_us;
	uint32 RunTimeCnt_ms;
	uint32 RunTimeCnt_s;

	/*OS*/
	uint32 LastRunTime_ms;
	uint32 AverageRunTime_ms;
	float CPU_Loading_Percent;
}Task_ManagerType;

typedef struct
{
	uint8 switchScreen;
	uint8 timingEnable;
}OS_DebugType;

typedef struct
{
	uint32 OS_RunCnt;
	OS_DebugType debug;
	Task_ManagerType taskMan[10];
}OS_ManagerType;

#ifdef ZYNQ_USE_7020
extern void LCD_Task(void * pvParameters);
#endif
extern void Comm_Task(void * pvParameters);
extern void AfeSim_Task(void * pvParameters);
extern void BatterySim_Task(void * pvParameters);
extern void OS_Statistic_Task(void * pvParameters);

#endif /* SRC_PS0_TASKS_H_ */
