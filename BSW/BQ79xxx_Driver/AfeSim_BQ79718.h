/*
 * Afe_133_BQ79718.h
 *
 *  Created on: 2024Äê1ÔÂ6ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_AFESIM_BQ79718_H_
#define SRC_AFESIM_BQ79718_H_

#include "ZYNQ_Platform.h"

#include "AfeSim_Device_Type.h"

#include "AfeSim_Device.h"

#include "AfeSim_BQ79600.h"

extern AfeSim_DeviceGpioUseType AfeSim_BQ79718_GpioInitConfig[3][AFESIM_MAX_GPIO_NUM];

extern void AfeSim_BQ79718_Fsm(void);

extern void AfeSim_BQ79718_WriteCommandResponse_COMM_CTRL(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir);
extern void AfeSim_BQ79718_WriteCommandResponse_DEV_CONF2(uint8 phyDevId);
extern void AfeSimX1_BQ79718_WriteCommandResponse_ADC_CTRL2(uint8 physicalId);

extern void AfeSimX1_BQ79718_WriteCommandResponse(AfeSim_DeviceCommDirectionType commDir);

#endif /* SRC_AFESIM_BQ79718_H_ */
