/*
 * BQ79600_Src.h
 *
 *  Created on: 2024Äê1ÔÂ6ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_AFESIM_BQ79600_H_
#define SRC_AFESIM_BQ79600_H_



#include "AfeSim_BQ79600_Type.h"
#include "AfeSim_BQ79616_Type.h"
#include "AfeSim_BQ79718_Type.h"
#include "AfeSim_BQ79600_Cfg.h"
#include "AfeSim_BQ79616_Cfg.h"
#include "AfeSim_BQ79718_Cfg.h"
#include "AfeSim_BQ79616.h"
#include "AfeSim_BQ79718.h"
#include "AfeSim_Device_Type.h"

extern AfeSim_BQ79600_ManagerType* const BQ79600Man;


extern void AfeSimX1_BQ79600_CommandResp_DIR0_ADDR(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir);
extern void AfeSim_BQ79600_CommandResp_DIR1_ADDR(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir);
extern void AfeSim_BQ79600_CommandResp_COMM_CTRL(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir);
extern void AfeSimX1_BQ79600_CommandResp_CONTROL1(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir);
extern void AfeSim_BQ79600_CommErrorHandler(uint8 commDir, uint8 faultType);

extern void AfeSim_BQ79600_Init(void);
extern void AfeSim_BQ79600_IrqCallBack(void);
extern void AfeSim_BQ79600_Fsm(void);

#endif /* SRC_AFESIM_BQ79600_H_ */
