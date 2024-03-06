/*
 * RTE.c
 *
 *  Created on: 2024年1月27日
 *      Author: Administrator
 */

#include "RTE.h"

#ifdef ZYNQ_USE_7020
/* ONLY ZYNQ 7020 USE RGBLCD Driver */
uint8 RTE_LCDTouchState = 0;
sint16 RTE_LCDTouchDxDy[2] = {0,0};
sint16 RTE_LCDTouchCoordinate[2] = {400,400};
#endif

uint8 RTE_AfeSimUseType = 1;
sint32 RTE_BatterySysCurrent = 5000;
uint8 RTE_BatterySOXsimEnable = 1;
uint8 RTE_AfeGpioSimEnable = 1;
uint8 RTE_BatteryTempSimEnable = 1;
uint8 RTE_BatteryBalSimEnable = 1;
uint8 RTE_DeviceRegSimEnable = 1;
uint32 RTE_DaisyChainErrRate_1_10000 = 0;

#ifdef ZYNQ_USE_7020
/* ONLY ZYNQ 7020 USE RGBLCD Driver */
void RTE_Write_LCDTouchState(uint8 state)
{
	RTE_LCDTouchState = state;
}

void RTE_Read_LCDTouchState(uint8 *state)
{
	*state = RTE_LCDTouchState;
}

void RTE_Write_LCDTouchDxDy(sint16 dx, sint16 dy)
{
	RTE_LCDTouchDxDy[0] = dx;
	RTE_LCDTouchDxDy[1] = dy;
}

void RTE_Read_LCDTouchDxDy(sint16 dxdy[2])
{
	dxdy[0] = RTE_LCDTouchDxDy[0];
	dxdy[1] = RTE_LCDTouchDxDy[1];
}

void RTE_Write_LCDTouchCoordinate(sint16 x, sint16 y)
{
	RTE_LCDTouchCoordinate[0] = x;
	RTE_LCDTouchCoordinate[1] = y;
}

void RTE_Read_LCDTouchCoordinate(sint16 xy[2])
{
	xy[0] = RTE_LCDTouchCoordinate[0];
	xy[1] = RTE_LCDTouchCoordinate[1];
}
#endif

void RTE_Write_BatterySysCurrent(sint32 current)
{
	RTE_BatterySysCurrent = current;
}

void RTE_Read_BatterySysCurrent(sint32* current)
{
	*current = RTE_BatterySysCurrent + AfeCalib[AFECALIB_1stCURRENT_OFFSET];
}

/* 1 继续SOX仿真
 * 0 暂停SOX仿真*/
void RTE_Write_BatterySOXsimEnable(uint8 enable)
{
	RTE_BatterySOXsimEnable = enable;
}
/* 1 继续SOX仿真
 * 0 暂停SOX仿真*/
void RTE_Read_BatterySOXsimEnable(uint8* enable)
{
	*enable = RTE_BatterySOXsimEnable;
}
/* 1 继续从物理量转换成GPIO电压
 * 0 暂停从物理量转换成GPIO电压*/
void RTE_Write_AfeGpioSimEnable(uint8 enable)
{
	RTE_AfeGpioSimEnable = enable;
}
/* 1 继续从物理量转换成GPIO电压
 * 0 暂停从物理量转换成GPIO电压*/
void RTE_Read_AfeGpioSimEnable(uint8* enable)
{
	*enable = RTE_AfeGpioSimEnable;
}
/* 1 继续温度仿真
 * 0 暂停温度仿真*/
void RTE_Write_BatteryTempSimEnable(uint8 enable)
{
	RTE_BatteryTempSimEnable = enable;
}
/* 1 继续温度仿真
 * 0 暂停温度仿真*/
void RTE_Read_BatteryTempSimEnable(uint8* enable)
{
	*enable = RTE_BatteryTempSimEnable;
}
/* 1 继续均衡仿真
 * 0 暂停均衡仿真*/
void RTE_Write_BatteryBalSimEnable(uint8 enable)
{
	RTE_BatteryBalSimEnable = enable;
}
/* 1 继续均衡仿真
 * 0 暂停均衡仿真*/
void RTE_Read_BatteryBalSimEnable(uint8* enable)
{
	*enable = RTE_BatteryBalSimEnable;
}
/* 1 继续寄存器仿真
   0 暂停寄存器仿真 */
void RTE_Write_DeviceRegSimEnable(uint8 enable)
{
	RTE_DeviceRegSimEnable = enable;
}
/* 1 继续寄存器仿真
   0 暂停寄存器仿真 */
void RTE_Read_DeviceRegSimEnable(uint8* enable)
{
	*enable = RTE_DeviceRegSimEnable;
}

/* 0 BQ79718
 * 1 BQ79616 */
void RTE_Write_AfeSimUseType(uint8 type)
{
	RTE_AfeSimUseType = type;
}
/* 0 BQ79718
 * 1 BQ79616 */
void RTE_Read_AfeSimUseType(uint8* type)
{
	*type = RTE_AfeSimUseType;
}

void RTE_Write_DaisyChainErrRate(uint32 noise)
{
	RTE_DaisyChainErrRate_1_10000 = noise;
}

void RTE_Read_DaisyChainErrRate(uint32* noise)
{
	*noise = RTE_DaisyChainErrRate_1_10000;
}

