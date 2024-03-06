/*
 * InfoDisplay.c
 *
 *  Created on: 2024年1月27日
 *      Author: Administrator
 */

#include "InfoDisplay.h"
#include "math.h"

InfoDisplay_ManagerType InfoMan;

extern sint32 AfeDeviceSim_ModuleVolt_mV[AFESIM_MAX_AFE_NUM];

static sint16 xy[2];
static sint16 dxdy[2];


void InfoDisplay_Run50ms(void)
{
	int i;
	static uint8 touchStatePrev = 0;
	static sint16 lockx = 0;
	static uint8 touchState;

	touchStatePrev = touchState;
	RTE_Read_LCDTouchState(&touchState);

	/*第一次点按*/
	if(!touchStatePrev && touchState)
	{
		RTE_Read_LCDTouchCoordinate(xy);
	}
	/*持续点按*/
	if(touchState)
	{
		RTE_Read_LCDTouchCoordinate(xy);
		RTE_Read_LCDTouchDxDy(dxdy);
		if(dxdy[0] > 200)
		{
			if(!InfoMan.switchScreenLock)
			{
				InfoMan.switchScreenLock = 1;
				InfoMan.switchScreenCmd = -1;
			}
		}
		else if(dxdy[0] < -200)
		{
			if(!InfoMan.switchScreenLock)
			{
				InfoMan.switchScreenLock = 1;
				InfoMan.switchScreenCmd = 1;
			}
		}
	}
	/*松开点按*/
	else
	{
		InfoMan.switchScreenLock = 0;
	}
}

void InfoDisplay_Screen1(void * par)
{
	sint32 current;
	u16 x = 10, y = 10;
	u16 i, j;
	u8 msg[64];
	u8 phyDevId;
	for(i=0;i<18;i++)
	{
		sprintf(msg, "Cell%02d %.1fmV, Soc %.3fAH  ",
				i+1, (float)Bat[i].volt_bias_100uV / 10.0, (float)Bat[i].soc_mC / 3600000.0);
		LCD_DrawString(x, y, msg);
		y+=18;
	}
	x = 300, y=10;
	for(i=0;i<6;i++)
	{
		sprintf(msg, "ModuleTemp%d %.2fC  ",
				i+1, (float)ModuleTemp_mC[1][i] / 1000.0);
		LCD_DrawString(x, y, msg);
		y+=18;
	}
	for(i=6;i<8;i++)
	{
		sprintf(msg, "BalanceTemp%d %.2fC  ",
				i+1, (float)BalanceTemp_mC[1][i] / 1000.0);
		LCD_DrawString(x, y, msg);
		y+=18;
	}
	for(i=0;i<2;i++)
	{
		sprintf(msg, "DieTemp%d %.2fC  ",
						i+1, (float)DieTemp_mC[1][i] / 1000.0);
		LCD_DrawString(x, y, msg);
		y+=18;
	}
	for(i=0;i<2;i++)
	{
		sprintf(msg, "ShuntTemp%d %.2fC  ",
						i+1, (float)ShuntTemp_mC[i] / 1000.0);
		LCD_DrawString(x, y, msg);
		y+=18;
	}
	sprintf(msg, "PackTemp %.2fC  ",
			(float)PackTemp_mC / 1000.0);
	LCD_DrawString(x, y, msg);
	y+=18;

	sprintf(msg, "AirTemp %.2fC  ",
			(float)AirTemp_mC / 1000.0);
	LCD_DrawString(x, y, msg);
	y+=36;
	RTE_Read_BatterySysCurrent(&current);
	sprintf(msg, "Current %.2fA   ",
				(float)current / 1000.0);
	LCD_DrawString(x, y, msg);
	x = 500, y=10;
	for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
	{
		if(phyDevId <= AfeMan->afeChipRealNum)
		{
			sprintf(msg, "Module%02d %02dCells Vbat %.3fV",
					phyDevId, AfeSim_Device[phyDevId].activeCell, (float)AfeSim_Device[phyDevId].power.VBAT_mV/1000.0);
		}
		else
		{
			sprintf(msg, " ");
		}
		LCD_DrawString2(x, y, msg, 300);
		y+=18;
	}
}

/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/

typedef struct
{
	u8 startPhyDevId;
	u8 endPhyDevId;
}InfoDisplay_Screen2_ServiceType;

InfoDisplay_Screen2_ServiceType InfoDisplay_Screen2_ServiceHandler =
{
	.startPhyDevId = 1,
	.endPhyDevId = 4,
};

void InfoDisplay_Screen2_OnlyCellVolt(void * par)
{
	u16 x = 10, y = 10;
	u16 phyDevId;
	u16 cellId;
	u8 msg[64];
	u16 batId;
	char sign;
	InfoDisplay_Screen2_ServiceType* serviceHandler = (InfoDisplay_Screen2_ServiceType*)par;
	u8 startPhyDevId = serviceHandler->startPhyDevId;
	u8 endPhyDevId = serviceHandler->endPhyDevId;

	for(phyDevId = startPhyDevId; phyDevId <= endPhyDevId && phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
	{
		x = (phyDevId - startPhyDevId)*200 + 5;
		y = 5;
		sprintf(msg, "AFE %d   ", phyDevId);
		LCD_DrawString(x, y, msg);
		y += 25;
		sign = AfeCalib[AFECALIB_MODULEVOLT_BIAS_OFFSET + phyDevId] >= 0 ? '+' : '-';
		sprintf(msg, "VBat %.1f %c %.1f V", (float)AfeSim_Device[phyDevId].power.VBAT_mV/1000.0, sign,
				fabs(AfeCalib[AFECALIB_MODULEVOLT_BIAS_OFFSET + phyDevId])/1000.0);
		LCD_DrawString2(x, y, msg, 196);
		y += 18;
		for(cellId=0; cellId < AFESIM_MAX_CELL_NUM; cellId++)
		{
			if(cellId < AfeMan->cellNum[phyDevId] && phyDevId <= AfeMan->afeChipRealNum)
			{
				batId = AfeMan->cellNumSum[phyDevId-1] + cellId;
				sign = AfeCalib[AFECALIB_CELLVOLT_BIAS_OFFSET + batId] >= 0 ? '+' : '-';
				sprintf(msg, "C%02d %.1f %c %.1f mV", cellId+1, (float)Bat[batId].volt_100uV / 10.0, 
					sign, fabs(AfeCalib[AFECALIB_CELLVOLT_BIAS_OFFSET + batId])/10.0);
			}
			else
			{
				sprintf(msg, " ");
			}
			LCD_DrawString2(x, y, msg, 196);
			y+=18;
		}
	}
}


void InfoDisplay_Screen2_Service(void * par)
{
	InfoDisplay_Screen2_ServiceType* serviceHandler = (InfoDisplay_Screen2_ServiceType*)par;
	static int init = 1;
	static TouchKeyType key0;
	static TouchKeyType key1;
	if(init)
	{
		init = 0;
		TouchKey_Init(&key0, 500, 380, 120, 80, "  AFE\r\nNext Page", NULL);
		TouchKey_Init(&key1, 200, 380, 120, 80, "  AFE\r\nPrev Page", NULL);
	}
	key0.displaySelf(&key0);
	key0.touchDetect(&key0);
	key1.displaySelf(&key1);
	key1.touchDetect(&key1);
	if(key0.touchState.pressDown == TRUE)
	{
		key0.touchState.pressDown = FALSE;
		switch(serviceHandler->startPhyDevId)
		{
		case 1:
			serviceHandler->startPhyDevId = 5;
			serviceHandler->endPhyDevId = 8;
			break;
		case 5:
			serviceHandler->startPhyDevId = 9;
			serviceHandler->endPhyDevId = 12;
			break;
		case 9:
			serviceHandler->startPhyDevId = 13;
			serviceHandler->endPhyDevId = 15;
			break;
		case 13:
			serviceHandler->startPhyDevId = 1;
			serviceHandler->endPhyDevId = 4;
			break;
		}
		LCD_Clear();
	}
	if(key1.touchState.pressDown == TRUE)
	{
		key1.touchState.pressDown = FALSE;
		switch(serviceHandler->startPhyDevId)
		{
		case 1:
			serviceHandler->startPhyDevId = 13;
			serviceHandler->endPhyDevId = 15;
			break;
		case 5:
			serviceHandler->startPhyDevId = 1;
			serviceHandler->endPhyDevId = 4;
			break;
		case 9:
			serviceHandler->startPhyDevId = 5;
			serviceHandler->endPhyDevId = 8;
			break;
		case 13:
			serviceHandler->startPhyDevId = 9;
			serviceHandler->endPhyDevId = 12;
			break;
		}
		LCD_Clear();
	}
}

/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/

void InfoDisplay_Screen5_ModuleBalcanceTemp(void * par)
{
	u16 x,y;
	u8 msg[64];
	u8 mTempIndex;
	u8 bTempIndex;
	u8 dTempIndex;
	u8 phyDevId;
	u8 startPhyDevId = 1;
	u8 endPhyDevId = 8;
	for(phyDevId = startPhyDevId; phyDevId <= endPhyDevId; phyDevId++)
	{
		x = (phyDevId - startPhyDevId)*100 + 5;
		y = 5;
		sprintf(msg, "AFE %d   ", phyDevId);
		LCD_DrawString(x, y, msg);
		y += 20;
		if(phyDevId <= AfeMan->afeChipRealNum)
		{
			for(mTempIndex = 1; mTempIndex <= 6; mTempIndex++)
			{
				sprintf(msg, "MT%d %.1fC  ", mTempIndex, (float)ModuleTemp_mC[phyDevId][mTempIndex-1] / 1000.0);
				LCD_DrawString(x, y, msg);
				y+=18;
			}
			for(bTempIndex = 7; bTempIndex <= 8; bTempIndex++)
			{
				sprintf(msg, "BT%d %.1fC  ", bTempIndex, (float)BalanceTemp_mC[phyDevId][bTempIndex-1] / 1000.0);
				LCD_DrawString(x, y, msg);
				y+=18;
			}
			for(dTempIndex = 1; dTempIndex <= 2; dTempIndex++)
			{
				sprintf(msg, "DT%d %.1fC  ", dTempIndex, (float)DieTemp_mC[phyDevId][dTempIndex-1] / 1000.0);
				LCD_DrawString(x, y, msg);
				y+=18;
			}
		}
		else
		{
			for(mTempIndex = 1; mTempIndex <= 10; mTempIndex++)
			{
				sprintf(msg, "            ");
				LCD_DrawString(x, y, msg);
				y+=18;
			}
		}
	}
	startPhyDevId = 9;
	endPhyDevId = 15;
	for(phyDevId = startPhyDevId; phyDevId <= endPhyDevId; phyDevId++)
	{
		x = (phyDevId - startPhyDevId)*100 + 5;
		y = 220;
		sprintf(msg, "AFE %d   ", phyDevId);
		LCD_DrawString(x, y, msg);
		y += 20;
		if(phyDevId <= AfeMan->afeChipRealNum)
		{
			for(mTempIndex = 1; mTempIndex <= 6; mTempIndex++)
			{
				sprintf(msg, "MT%d %.1fC  ", mTempIndex, (float)ModuleTemp_mC[phyDevId][mTempIndex-1] / 1000.0);
				LCD_DrawString(x, y, msg);
				y+=18;
			}
			for(bTempIndex = 7; bTempIndex <= 8; bTempIndex++)
			{
				sprintf(msg, "BT%d %.1fC  ", bTempIndex, (float)BalanceTemp_mC[phyDevId][bTempIndex-1] / 1000.0);
				LCD_DrawString(x, y, msg);
				y+=18;
			}
			for(dTempIndex = 1; dTempIndex <= 2; dTempIndex++)
			{
				sprintf(msg, "DT%d %.1fC  ", dTempIndex, (float)DieTemp_mC[phyDevId][dTempIndex-1] / 1000.0);
				LCD_DrawString(x, y, msg);
				y+=18;
			}
		}
		else
		{
			for(mTempIndex = 1; mTempIndex <= 10; mTempIndex++)
			{
				sprintf(msg, "            ");
				LCD_DrawString(x, y, msg);
				y+=18;
			}
		}
	}
}

/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/
/********************************************************************************************************************************************/


typedef struct
{
	u8 startPhyDevId;
	u8 endPhyDevId;
}InfoDisplay_Screen6_ServiceType;

InfoDisplay_Screen6_ServiceType InfoDisplay_Screen6_ServiceHandler =
{
	.startPhyDevId = 1,
	.endPhyDevId = 4,
};
	

void InfoDisplay_Screen6_GpioVolt(void * par)
{
	u16 x,y;
	u8 msg[64];
	u8 gpioId;
	u8 phyDevId;
	
	InfoDisplay_Screen2_ServiceType* serviceHandler = (InfoDisplay_Screen2_ServiceType*)par;
	u8 startPhyDevId = serviceHandler->startPhyDevId;
	u8 endPhyDevId = serviceHandler->endPhyDevId;
	
	for(phyDevId = startPhyDevId; phyDevId <= endPhyDevId; phyDevId++)
	{
		x = (phyDevId - startPhyDevId)*200 + 5;
		y = 5;
		sprintf(msg, "AFE %d   ", phyDevId);
		LCD_DrawString(x, y, msg);
		y += 18;
		if(phyDevId <= AfeMan->afeChipRealNum)
		{
			for(gpioId = 0; gpioId <= AFESIM_MAX_GPIO_NUM; gpioId++)
			{
				if(gpioId == 0)
				{
					char sign = AfeCalib[AFECALIB_TSREF_BIAS_OFFSET + phyDevId] >= 0 ? '+' : '-';
					sprintf(msg, "TSREF %.4f %c %.4f V", (float)AfeSim_Device[phyDevId].power.TSREF_100uV / 10000.0, sign,
							fabs(AfeCalib[AFECALIB_TSREF_BIAS_OFFSET + phyDevId])/10000.0);
					y+=10;		
				}
				else
				if(AfeSim_Device[phyDevId].gpio[gpioId-1].useType != AFESIM_DEVICE_GPIO_NOUSE)
				{
					char sign = (AfeCalib[AFECALIB_GPIOVOLT_BIAS_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId-1]) >= 0 ? '+' : '-';
					sprintf(msg, "G%02d %.4f %c %.4f V", gpioId, (float)AfeSim_Device[phyDevId].gpio[gpioId-1].inputVoltage_100uV / 10000.0, sign,
							fabs((AfeCalib[AFECALIB_GPIOVOLT_BIAS_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId-1]))/10000.0);
				}
				else
				{
					sprintf(msg, "            ");
				}
				LCD_DrawString2(x, y, msg, 196);
				y+=20;
			}
		}
		else
		{
			for(gpioId = 0; gpioId <= AFESIM_MAX_GPIO_NUM; gpioId++)
			{
				sprintf(msg, "            ");
				LCD_DrawString2(x, y, msg, 196);
				y+=20;
			}
		}
	}
}

void InfoDisplay_Screen6_Service(void * par)
{
	InfoDisplay_Screen6_ServiceType* serviceHandler = (InfoDisplay_Screen6_ServiceType*)par;
	static int init = 1;
	static TouchKeyType key0;
	static TouchKeyType key1;
	if(init)
	{
		init = 0;
		TouchKey_Init(&key0, 500, 380, 120, 80, "  AFE\r\nNext Page", NULL);
		TouchKey_Init(&key1, 200, 380, 120, 80, "  AFE\r\nPrev Page", NULL);
	}
	key0.displaySelf(&key0);
	key0.touchDetect(&key0);
	key1.displaySelf(&key1);
	key1.touchDetect(&key1);
	if(key0.touchState.pressDown == TRUE)
	{
		key0.touchState.pressDown = FALSE;
		switch(serviceHandler->startPhyDevId)
		{
		case 1:
			serviceHandler->startPhyDevId = 5;
			serviceHandler->endPhyDevId = 8;
			break;
		case 5:
			serviceHandler->startPhyDevId = 9;
			serviceHandler->endPhyDevId = 12;
			break;
		case 9:
			serviceHandler->startPhyDevId = 13;
			serviceHandler->endPhyDevId = 15;
			break;
		case 13:
			serviceHandler->startPhyDevId = 1;
			serviceHandler->endPhyDevId = 4;
			break;
		}
		LCD_Clear();
	}
	if(key1.touchState.pressDown == TRUE)
	{
		key1.touchState.pressDown = FALSE;
		switch(serviceHandler->startPhyDevId)
		{
		case 1:
			serviceHandler->startPhyDevId = 13;
			serviceHandler->endPhyDevId = 15;
			break;
		case 5:
			serviceHandler->startPhyDevId = 1;
			serviceHandler->endPhyDevId = 4;
			break;
		case 9:
			serviceHandler->startPhyDevId = 5;
			serviceHandler->endPhyDevId = 8;
			break;
		case 13:
			serviceHandler->startPhyDevId = 9;
			serviceHandler->endPhyDevId = 12;
			break;
		}
		LCD_Clear();
	}
}


typedef struct
{
	uint8 screenType;
}InfoDisplay_SysDebug_HandlerType;

InfoDisplay_SysDebug_HandlerType InfoDisplay_SysDebug_Handler;

void InfoDisplay_SysDebug_Service(void* par)
{

}

void InfoDisplay_SysDebug(void * par)
{
	u16 phyDevId;
	u16 x,y;
	u8 msg[128];
	extern AfeCalib_ManagerType AfeCalib_Manager;
	u16 i,j,k;
	u16 reg = AfeCalib_Manager.randRegHandler.readStartRegPos;
	u8 dev = AfeCalib_Manager.randRegHandler.readPhyDevId;
	if(AfeCalib_Manager.randRegHandler.readEnable)
	{
		if(AfeCalib_Manager.randRegHandler.isGlobalR)
		{
			x = 20; y = 20;
			sprintf(msg, "Register %04X", reg);
			LCD_DrawString2(x, y, msg, 150);
			y += 20;
			for(phyDevId = 0; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
			{
				for(i = 0, j = 0; i < 16 && j < AfeCalib_Manager.randRegHandler.readLen; i++, j++)
				{
					if(i % 16 == 0)
					{
						sprintf(msg, "%04X: ", reg+i);
						LCD_DrawString(x, y, msg);
						x += 50;
					}
					sprintf(msg, "%02X ", AfeSim_Reg[phyDevId][reg+i]);
					LCD_DrawString(x, y, msg);
					x += 8*3;
					if((i+1) % 16 == 0)
					{
						x = 20;
						y += 20;
					}
				}
			}
		}
		else
		{
			x = 20; y = 20;
			sprintf(msg, "Register %04X", reg);
			LCD_DrawString2(x, y, msg, 150);
			y += 20;
			for(i = 0, j = 0; i < 256 && j < AfeCalib_Manager.randRegHandler.readLen; i++, j++)
			{
				if(i % 16 == 0)
				{
					sprintf(msg, "%04X: ", reg+i);
					LCD_DrawString(x, y, msg);
					x += 50;
				}
				sprintf(msg, "%02X ", AfeSim_Reg[dev][reg+i]);
				LCD_DrawString(x, y, msg);
				x += 8*3;
				if((i+1) % 16 == 0)
				{
					x = 20;
					y += 20;
				}
			}
		}
	}
}

void InfoDisplay_Init(void)
{
	InfoMan.screenMaxNum = 0;
//	InfoMan.screen[InfoMan.screenMaxNum] = &InfoDisplay_Screen1;
//	InfoMan.serviceFunction[InfoMan.screenMaxNum] = NULL;
//	InfoMan.serviceHandler[InfoMan.screenMaxNum] = NULL;
//	InfoMan.screenMaxNum++;

	InfoMan.screen[InfoMan.screenMaxNum] = &InfoDisplay_Screen2_OnlyCellVolt;
	InfoMan.serviceFunction[InfoMan.screenMaxNum] = &InfoDisplay_Screen2_Service;
	InfoMan.serviceHandler[InfoMan.screenMaxNum] = &InfoDisplay_Screen2_ServiceHandler;
	InfoMan.screenMaxNum++;

	InfoMan.screen[InfoMan.screenMaxNum] = &InfoDisplay_Screen5_ModuleBalcanceTemp;
	InfoMan.serviceFunction[InfoMan.screenMaxNum] = NULL;
	InfoMan.serviceHandler[InfoMan.screenMaxNum] = NULL;
	InfoMan.screenMaxNum++;

	InfoMan.screen[InfoMan.screenMaxNum] = &InfoDisplay_Screen6_GpioVolt;
	InfoMan.serviceFunction[InfoMan.screenMaxNum] = &InfoDisplay_Screen6_Service;
	InfoMan.serviceHandler[InfoMan.screenMaxNum] = &InfoDisplay_Screen6_ServiceHandler;
	InfoMan.screenMaxNum++;

	InfoMan.screen[InfoMan.screenMaxNum] = &InfoDisplay_Screen1;
	InfoMan.serviceFunction[InfoMan.screenMaxNum] = NULL;
	InfoMan.serviceHandler[InfoMan.screenMaxNum] = NULL;
	InfoMan.screenMaxNum++;

	InfoMan.screen[InfoMan.screenMaxNum] = &InfoDisplay_SysDebug;
	InfoMan.serviceFunction[InfoMan.screenMaxNum] = &InfoDisplay_SysDebug_Service;
	InfoMan.serviceHandler[InfoMan.screenMaxNum] = &InfoDisplay_SysDebug_Handler;
	InfoMan.screenMaxNum++;
}

void InfoDisplay_AfeSimAliveDetect(void)
{
	static uint32 rxCnt_1000 = 0;
	static uint32 txCnt_1000 = 0;
	static uint32 rxCnt = 0;
	static uint32 txCnt = 0;
	static uint32 rxPrev = 0;
	static uint32 txPrev = 0;
	uint32 rx = BQ79600Man->stat.RecvTimes;
	uint32 tx = BQ79600Man->stat.TransTimes;
	static uint32 rxDeg = 0;
	static uint32 txDeg = 0;
	uint16 x, y;
	uint8 r,g,b;
	uint8 msg[32];
	if(rx != rxPrev)
	{
		rxCnt += rx - rxPrev;
		rxDeg += 5;
		x = 20 + 15*sin((float)rxDeg*3.14159/180.0);
		y = 460 + 15*cos((float)rxDeg*3.14159/180.0);
		if(rxDeg < 360)
		{
			r = 0xFF; g = 0; b = 0;
		}
		else if(rxDeg < 360*2)
		{
			r = 0; g = 0xFF; b = 0;
		}
		else if(rxDeg < 360*3)
		{
			r = 0; g = 0; b = 0xFF;
		}
		else
		{
			rxDeg = 0;
			r = 0xFF; g = 0; b = 0;
		}
		LCD_DrawPoint(x,y,r,g,b);
		sprintf(msg, "RX: %8d ", rxCnt);
		LCD_DrawString(60,440,msg);
		rxPrev = rx;
	}
	if(tx != txPrev)
	{
		txCnt += tx - txPrev;
		txDeg += 5;
		x = 40 + 15*sin((float)txDeg*3.14159/180.0);
		y = 460 + 15*cos((float)txDeg*3.14159/180.0);
		if(txDeg < 360)
		{
			r = 0xFF; g = 0; b = 0;
		}
		else if(txDeg < 360*2)
		{
			r = 0; g = 0xFF; b = 0;
		}
		else if(txDeg < 360*3)
		{
			r = 0; g = 0; b = 0xFF;
		}
		else
		{
			txDeg = 0;
			r = 0xFF; g = 0; b = 0;
		}
		LCD_DrawPoint(x,y,r,g,b);
		sprintf(msg, "TX: %8d ", txCnt);
		LCD_DrawString(60,460,msg);
		txPrev = tx;
	}
	sprintf(msg, "Err:   %8d", BQ79600Man->stat.SpiErrorTimes);
	LCD_DrawString(650,440,msg);
	sprintf(msg, "Noise: %8d", AfeMan->noiseFailCnt);
	LCD_DrawString(650,460,msg);
}

void InfoDisplay_Run100ms(void)
{
	if(InfoMan.switchScreenCmd > 0)
	{
		InfoMan.switchScreenCmd--;
		InfoMan.currentScreen = InfoMan.currentScreen + 1 >= InfoMan.screenMaxNum ? 0 : InfoMan.currentScreen + 1;
		LCD_Clear();
	}
	else if(InfoMan.switchScreenCmd < 0)
	{
		InfoMan.switchScreenCmd++;
		InfoMan.currentScreen = InfoMan.currentScreen - 1 < 0 ? InfoMan.screenMaxNum - 1 : InfoMan.currentScreen - 1;
		LCD_Clear();
	}

	if(InfoMan.serviceFunction[InfoMan.currentScreen] != NULL)
	{
		InfoMan.serviceFunction[InfoMan.currentScreen](InfoMan.serviceHandler[InfoMan.currentScreen]);
	}
	if(InfoMan.screen[InfoMan.currentScreen] != NULL)
	{
		InfoMan.screen[InfoMan.currentScreen](InfoMan.serviceHandler[InfoMan.currentScreen]);
	}

	InfoDisplay_AfeSimAliveDetect();
}
