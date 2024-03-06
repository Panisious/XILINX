/*
 * AfeSim_DaisyChain.h
 *
 *  Created on: 2024年1月28日
 *      Author: Administrator
 */

#ifndef SRC_BSW_BQ79XXX_DRIVER_AFESIM_DAISYCHAIN_H_
#define SRC_BSW_BQ79XXX_DRIVER_AFESIM_DAISYCHAIN_H_

#include "RTE.h"
#include "AfeSim_Device_Cfg.h"
#include "AfeSim_Device_Type.h"
#include "AfeSim_BQ79600.h"
#include "AfeSim_BQ79616.h"
#include "AfeSim_BQ79718.h"

typedef enum
{
	AFESIM_SHUTDOWN_TO_ACTIVE,
	AFESIM_SLEEP_TO_ACTIVE,
	AFESIM_ACTIVE_TO_SLEEP,
	AFESIM_ACTIVE_TO_SHUTDOWN,
}AfeSim_DevicePowerStatusChangeType;

typedef struct
{
	AfeSim_DeviceManangerType* dev;
	uint8 (*reg)[AFESIM_MAX_REG_NUM];
	uint8 afeMaxCellNum;						/* 每个AFE最大电芯数，初始化时取决于芯片类型 */
	uint8 afeMaxGpioNum;
	uint16 cellNum[AFESIM_MAX_AFE_NUM + 1];		/* 每个AFE的电芯数量 */
	uint16 cellNumSum[AFESIM_MAX_AFE_NUM + 1];	/* 从低到高电芯数量的累加和 */
	/* 标定配置量, 默认15个AFE
	 * 该配置量决定反链从倒数第几个芯片开始计算 */
	uint8 afeChipRealNum;
	uint32 noiseFailCnt;
}AfeSim_DeviceManagerType;

extern AfeSim_AfeGeneralType AfeSim_UseType;
extern AfeSim_DeviceManagerType* const AfeMan;
extern uint8 AfeSim_Reg[AFESIM_MAX_AFE_NUM + 1][AFESIM_MAX_REG_NUM];

extern AfeSim_DeviceManangerType AfeSim_Device[AFESIM_MAX_AFE_NUM + 1];

extern uint8 AfeSim_DaisyChainCommData[1024];

extern void AfeSim_DeviceInit(void);

/* AfeDevice Assert Functions */
extern uint8 AfeSim_AssertDeviceCommDir(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir);
extern uint8 AfeSim_AssertBaseDeviceCanRW(void);
extern uint8 AfeSim_AssertBaseDeviceCanTransmit(AfeSim_DeviceCommDirectionType commDir);
extern uint8 AfeSim_AssertDeviceIsStackTop(uint8 phyDevId);
extern uint8 AfeSim_AssertDeviceCanTransmit(uint8 phyDevId);
extern uint8 AfeSim_AssertDeviceCanWrite(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir);
extern uint8 AfeSim_AssertDeviceCanRead(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir);
extern uint8 AfeSim_AssertDeviceCanReceive(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir);
extern uint8 AfeSim_AssertDeviceCanSend(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir);
extern uint8 AfeSim_AssertDeviceIsBroken(uint8 phyDevId);
extern uint8 AfeSim_AssertDeviceIsAwake(uint8 phyDevId);
extern uint8 AfeSim_AssertDeviceIsSleep(uint8 phyDevId);
extern uint8 AfeSim_AssertDeviceIsShutdown(uint8 phyDevId);

extern uint8 AfeSim_OneDeviceWakeup(uint8 devId);
extern uint8 AfeSim_OneDeviceSleep(uint8 devId);
extern uint8 AfeSim_OneDeviceShutdown(uint8 devId);

extern uint8 AfeSim_DeviceWakeUp(AfeSim_DeviceCommDirectionType commDir, AfeSim_DevicePowerStatusChangeType wakeType);
extern uint8 AfeSim_DeviceSleepShutdown(AfeSim_DeviceCommDirectionType commDir, AfeSim_DevicePowerStatusChangeType shutdownType);

extern uint8 AfeSimX1_DeviceSingleWrite(uint8 targetDevId, uint16 regAddrStart, uint8* data, uint8 len, AfeSim_DeviceCommDirectionType commDir);
extern uint8 AfeSimX1_DeviceStackWrite(uint16 regAddrStart, uint8* data, uint8 len, AfeSim_DeviceCommDirectionType commDir, uint8 isBroadcast);

extern uint8 AfeSimX1_DeviceSingleRead(uint8 targetDevId, uint16 regAddrStart, uint8* buff, uint8 len, AfeSim_DeviceCommDirectionType commDir);
extern uint8 AfeSimX1_DeviceStackRead(uint16 regAddrStart, uint8* buff, uint8 rwBytes, AfeSim_DeviceCommDirectionType commDir, uint8 isBroadcast);

#endif /* SRC_BSW_BQ79XXX_DRIVER_AFESIM_DAISYCHAIN_H_ */
