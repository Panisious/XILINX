/*
 * Afe_133_BQ79616.h
 *
 *  Created on: 2024Äê1ÔÂ6ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_AFESIM_BQ79616_H_
#define SRC_AFESIM_BQ79616_H_

#include "AfeSim_BQ79616_Cfg.h"
#include "AfeSim_BQ79616_Type.h"
#include "AfeSim_Device.h"

extern uint8 AfeSim_Reg[16][0x3000];
extern AfeSim_DeviceGpioUseType AfeSim_BQ79616_GpioInitConfig[3][AFESIM_MAX_GPIO_NUM];

extern void AfeSim_BQ79616_ResetAuxVolt(uint8 phyDevId);

extern void AfeSim_BQ79616_WriteCommandResponse_ACTIVE_CELL(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_BBVC_POSN1(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_BBVC_POSN2(uint8 phyDevId);

extern void AfeSim_BQ79616_WriteCommandResponse_GPIO_CONF1(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_GPIO_CONF2(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_GPIO_CONF3(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_GPIO_CONF4(uint8 phyDevId);

extern void AfeSim_BQ79616_WriteCommandResponse_CONTROL2(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_ADC_CTRL1(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_ADC_CTRL2(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_ADC_CTRL3(uint8 phyDevId);

extern void AfeSim_BQ79616_WriteCommandResponse_OVUV_CTRL(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_OTUT_CTRL(uint8 phyDevId);

extern void AfeSim_BQ79616_WriteCommandResponse_BAL_CTRL1(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_BAL_CTRL2(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_FAULT_RST1(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_FAULT_RST2(uint8 phyDevId);

extern void AfeSim_BQ79616_WriteCommandResponse_DIAG_PWR_CTRL(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_DIAG_COMP_CTRL3(uint8 phyDevId);
extern void AfeSim_BQ79616_WriteCommandResponse_DIAG_COMP_CTRL4(uint8 phyDevId);

extern void AfeSim_BQ79616_CommErrorHandler(uint8 phyDevId, uint8 errorType, uint8 commDir);

extern void AfeSim_BQ79616_Fsm(void);


#endif /* SRC_AFESIM_BQ79616_H_ */
