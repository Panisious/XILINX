/*
 * RTE.h
 *
 *  Created on: 2024Äê1ÔÂ27ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_RTE_H_
#define SRC_RTE_H_

#include "Project_Define.h"

#include "ZYNQ_BasicType.h"
#include "ZYNQ_Platform_HAL.h"
#include "ZYNQ_Platform.h"

#ifdef ZYNQ_USE_7020
#include "lcd.h"
#endif

#include <Comm_Control.h>
#include "AfeSim_BQ79600.h"
#include "AfeSim_BQ79616.h"
#include "AfeSim_BQ79718.h"
#include "AfeSim_TI_HAL.h"

#include "AfeCalibration.h"

#ifdef ZYNQ_USE_7020
extern void RTE_Write_LCDTouchCoordinate(sint16 x, sint16 y);
extern void RTE_Read_LCDTouchCoordinate(sint16 xy[2]);

extern void RTE_Write_LCDTouchState(uint8 state);
extern void RTE_Read_LCDTouchState(uint8 *state);

extern void RTE_Write_LCDTouchDxDy(sint16 dx, sint16 dy);
extern void RTE_Read_LCDTouchDxDy(sint16 dxdy[2]);
#endif

extern void RTE_Write_BatterySysCurrent(sint32 current);
extern void RTE_Read_BatterySysCurrent(sint32* current);

extern void RTE_Write_BatterySOXsimEnable(uint8 enable);
extern void RTE_Read_BatterySOXsimEnable(uint8 *enable);
extern void RTE_Write_AfeGpioSimEnable(uint8 enable);
extern void RTE_Read_AfeGpioSimEnable(uint8* enable);
extern void RTE_Write_BatteryTempSimEnable(uint8 enable);
extern void RTE_Read_BatteryTempSimEnable(uint8* enable);
extern void RTE_Write_BatteryBalSimEnable(uint8 enable);
extern void RTE_Read_BatteryBalSimEnable(uint8* enable);
extern void RTE_Write_DeviceRegSimEnable(uint8 enable);
extern void RTE_Read_DeviceRegSimEnable(uint8* enable);

extern void RTE_Write_AfeSimUseType(uint8 type);
extern void RTE_Read_AfeSimUseType(uint8* type);

extern void RTE_Write_DaisyChainErrRate(uint32 noise);
extern void RTE_Read_DaisyChainErrRate(uint32* noise);


#endif /* SRC_RTE_H_ */
