/*
 * PS0_Tasks.c
 *
 *  Created on: 2024Äê1ÔÂ27ÈÕ
 *      Author: Administrator
 */

#include "PS0_Tasks.h"

OS_ManagerType OSman;

void PS0_Init(void * pvParameters)
{

}

void PS0_Run10ms(void * pvParameters)
{

}

void PS0_Run20ms(void * pvParameters)
{

}

void PS0_Run50ms(void * pvParameters)
{

}

void PS0_Run100ms(void * pvParameters)
{

}

void PS0_Run200ms(void * pvParameters)
{

}

void PS0_Run500ms(void * pvParameters)
{

}

void PS0_Run1000ms(void * pvParameters)
{

}

#ifdef ZYNQ_USE_7020
void LCD_Task(void * pvParameters)
{
#define LCD_TASK_RUNTIME 		50
#define LCD_TASK_RUNPERIOD		1000
	u32 runCnt = 0;
	XTime tEnd, tCur;
	u32 tUsed;
	static portTickType PreviousWakeTime;
	const portTickType TimeIncrement = pdMS_TO_TICKS(LCD_TASK_RUNTIME);
	PreviousWakeTime = xTaskGetTickCount();

	LCD_Init();
	InfoDisplay_Init();
	TouchMenu_Init();

	while(1)
	{
		/* log 20240227 cpu loading 45%
		 * */
		vTaskDelayUntil( &PreviousWakeTime, TimeIncrement );
		XTime_GetTime(&tCur);

		if(runCnt % 50 == 0)
		{
			LCD_Run50ms();
			InfoDisplay_Run50ms();
		}
		if(runCnt % 100 == 0)
		{
			InfoDisplay_Run100ms();
		}
		if(runCnt % 200 == 0)
		{
			TouchMenu_Run200ms();
		}
		if(runCnt % 500 == 0)
		{

		}
		if(runCnt % 1000 == 0)
		{

		}
		Xil_DCacheFlush();

		runCnt += LCD_TASK_RUNTIME;
		if(runCnt >= LCD_TASK_RUNPERIOD)
		{
			runCnt = 0;
		}

		XTime_GetTime(&tEnd);
		tUsed = ((tEnd-tCur)*1000000)/(COUNTS_PER_SECOND);
		OSman.taskMan[OS_Task_LCD].RunCnt++;
		OSman.taskMan[OS_Task_LCD].RunTimeSequence[OSman.taskMan[OS_Task_LCD].RunIndex++]=tUsed;
		OSman.taskMan[OS_Task_LCD].RunTimeCnt_us += tUsed;
		if(OSman.taskMan[OS_Task_LCD].RunTimeCnt_us > 1000)
		{
			OSman.taskMan[OS_Task_LCD].RunTimeCnt_ms += OSman.taskMan[OS_Task_LCD].RunTimeCnt_us / 1000;
			OSman.taskMan[OS_Task_LCD].RunTimeCnt_us %= 1000;
		}
	}
}
#endif

void Comm_Task(void * pvParameters)
{
#define COMM_RUNTIME 		10
#define COMM_RUNPERIOD		1000
	uint32 runCnt = 0;
	XTime tEnd, tCur;
	u32 tUsed;
	static portTickType PreviousWakeTime;
	const portTickType TimeIncrement = pdMS_TO_TICKS(10);
	PreviousWakeTime = xTaskGetTickCount();

	Comm_Init();

	while(1)
	{
		/* log 20240227 cpu loading 0%
		 * */
		vTaskDelayUntil( &PreviousWakeTime, TimeIncrement );
		XTime_GetTime(&tCur);

		Comm_NormalRun();
		if(runCnt % 100 == 0)
		{

		}
		if(runCnt % 200 == 0)
		{
			AfeCalibration_Run200ms();
		}
		if(runCnt % 500 == 0)
		{
			
		}
		if(runCnt % 1000 == 0)
		{
		}
		runCnt += COMM_RUNTIME;
		if(runCnt >= COMM_RUNPERIOD)
		{
			runCnt = 0;
		}

		XTime_GetTime(&tEnd);
		tUsed = ((tEnd-tCur)*1000000)/(COUNTS_PER_SECOND);
		OSman.taskMan[OS_Task_Comm].RunCnt++;
		OSman.taskMan[OS_Task_Comm].RunTimeSequence[OSman.taskMan[OS_Task_Comm].RunIndex++]=tUsed;
		OSman.taskMan[OS_Task_Comm].RunTimeCnt_us += tUsed;
		if(OSman.taskMan[OS_Task_Comm].RunTimeCnt_us > 1000)
		{
			OSman.taskMan[OS_Task_Comm].RunTimeCnt_ms += OSman.taskMan[OS_Task_Comm].RunTimeCnt_us / 1000;
			OSman.taskMan[OS_Task_Comm].RunTimeCnt_us %= 1000;
		}
	}
}

void BatterySim_Task(void * pvParameters)
{
	XTime tEnd, tCur;
	u32 tUsed;
	static portTickType PreviousWakeTime;
	const portTickType TimeIncrement = pdMS_TO_TICKS(100);
	PreviousWakeTime = xTaskGetTickCount();

	BatterySim_Init();

	while(1)
	{
		/* log 20240227 cpu loading 200us
		 * */
		vTaskDelayUntil( &PreviousWakeTime, TimeIncrement );

		XTime_GetTime(&tCur);

		BatterySim_NormalRun();

		XTime_GetTime(&tEnd);
		tUsed = ((tEnd-tCur)*1000000)/(COUNTS_PER_SECOND);
		OSman.taskMan[OS_Task_BatSim].RunCnt++;
		OSman.taskMan[OS_Task_BatSim].RunTimeSequence[OSman.taskMan[OS_Task_BatSim].RunIndex++]=tUsed;
		OSman.taskMan[OS_Task_BatSim].RunTimeCnt_us += tUsed;
		if(OSman.taskMan[OS_Task_BatSim].RunTimeCnt_us > 1000)
		{
			OSman.taskMan[OS_Task_BatSim].RunTimeCnt_ms += OSman.taskMan[OS_Task_BatSim].RunTimeCnt_us / 1000;
			OSman.taskMan[OS_Task_BatSim].RunTimeCnt_us %= 1000;
		}
	}
}

void AfeSim_Task(void * pvParameters)
{
#define AFESIM_RUNTIME 		10
#define AFESIM_RUNPERIOD	1000
	uint32 runCnt = 0;
	XTime tEnd, tCur;
	u32 tUsed;
	static portTickType PreviousWakeTime;
	const portTickType TimeIncrement = pdMS_TO_TICKS(AFESIM_RUNTIME);
	PreviousWakeTime = xTaskGetTickCount();

	AfeSim_Init();

	while(1)
	{
		/* log 20240227 cpu loading 313us per 100ms
		 * */
		vTaskDelayUntil( &PreviousWakeTime, TimeIncrement );
		XTime_GetTime(&tCur);
		if(runCnt % 10 == 0)
		{
			AfeDeviceSim_Run10ms();
		}
		if(runCnt % 20 == 0)
		{
			AfeDeviceSim_Run20ms();
		}
		if(runCnt % 50 == 0)
		{
			AfeDeviceSim_Run50ms();
		}
		if(runCnt % 100 == 0)
		{
			AfeCalibration_Run100ms();
			AfeDeviceSim_Run100ms();
		}
		if(runCnt % 500 == 0)
		{
			
		}
		if(runCnt % 1000 == 0)
		{

		}
		runCnt += AFESIM_RUNTIME;
		if(runCnt >= AFESIM_RUNPERIOD)
		{
			runCnt = 0;
		}

		XTime_GetTime(&tEnd);
		tUsed = ((tEnd-tCur)*1000000)/(COUNTS_PER_SECOND);
		OSman.taskMan[OS_Task_AfeSim].RunCnt++;
		OSman.taskMan[OS_Task_AfeSim].RunTimeSequence[OSman.taskMan[OS_Task_AfeSim].RunIndex++]=tUsed;
		OSman.taskMan[OS_Task_AfeSim].RunTimeCnt_us += tUsed;
		if(OSman.taskMan[OS_Task_AfeSim].RunTimeCnt_us > 1000)
		{
			OSman.taskMan[OS_Task_AfeSim].RunTimeCnt_ms += OSman.taskMan[OS_Task_AfeSim].RunTimeCnt_us / 1000;
			OSman.taskMan[OS_Task_AfeSim].RunTimeCnt_us %= 1000;
		}
	}
}



void OS_Statistic_Task(void * pvParameters)
{
#define OS_RUNTIME	500
	int i,j,k;
	int time_ms;
	static portTickType PreviousWakeTime;
	const portTickType TimeIncrement = pdMS_TO_TICKS(OS_RUNTIME);
	PreviousWakeTime = xTaskGetTickCount();

	memset(&OSman, 0, sizeof(OSman));

	while(1)
	{
		vTaskDelayUntil( &PreviousWakeTime, TimeIncrement );
		OSman.OS_RunCnt++;
		/*Get Run Time And */
		for(i=0;i<OS_Task_Num;i++)
		{
			time_ms = OSman.taskMan[i].RunTimeCnt_ms - OSman.taskMan[i].LastRunTime_ms;
			OSman.taskMan[i].LastRunTime_ms = OSman.taskMan[i].RunTimeCnt_ms;
			OSman.taskMan[i].CPU_Loading_Percent = (float)time_ms * (100.0 / (float)(OS_RUNTIME));
		}
	}
}
