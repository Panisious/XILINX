/*
 * AfeSim_Device_Type.h
 *
 *  Created on: 2024年1月28日
 *      Author: Administrator
 */

#ifndef SRC_BSW_BQ79XXX_DRIVER_AFESIM_DEVICE_TYPE_H_
#define SRC_BSW_BQ79XXX_DRIVER_AFESIM_DEVICE_TYPE_H_

#include "AfeSim_TI_HAL.h"
#include "AfeSim_Device_Cfg.h"


typedef struct
{
	sint32 r;
	sint32 i;
}cplx32;

typedef struct
{
	short r;
	short i;
}cplx16;

/* Logical-Mathematical-Physical-Realistic Simulation Level */
typedef enum
{
	AFESIM_LEVEL_0,
	AFESIM_LEVEL_1,
	AFESIM_LEVEL_2,
	AFESIM_LEVEL_3
}AfeSim_SimLevelType;

typedef enum
{
	AFESIM_AFE_USE_NULL = 0,
	AFESIM_AFE_USE_BQ79600,
	AFESIM_AFE_USE_BQ79616,
	AFESIM_AFE_USE_BQ79656,
	AFESIM_AFE_USE_BQ79718,
	AFESIM_AFE_USE_BQ79758
}AfeSim_AfeUseType;

typedef enum
{
	AFESIM_TYPE_79718 = 0,
	AFESIM_TYPE_79616 = 1,
}AfeSim_AfeGeneralType;

typedef enum
{
	AFESIM_DEVICE_ENABLE,
	AFESIM_DEVICE_DISABLE,
}AfeSim_DeviceEnableType;

typedef enum
{
	AFESIM_DEVID_0 = 0,
	AFESIM_DEVID_1,
	AFESIM_DEVID_2,
	AFESIM_DEVID_3,
	AFESIM_DEVID_4,
	AFESIM_DEVID_5,
	AFESIM_DEVID_6,
	AFESIM_DEVID_7,
	AFESIM_DEVID_8,
	AFESIM_DEVID_9,
	AFESIM_DEVID_10,
	AFESIM_DEVID_11,
	AFESIM_DEVID_12,
	AFESIM_DEVID_13,
	AFESIM_DEVID_14,
	AFESIM_DEVID_15,
	AFESIM_DEVID_STACK = 0xFE,
	AFESIM_DEVID_BROADCAST = 0xFF
}AfeSim_DeviceIdType;

typedef enum
{
	AFESIM_COMM_NO_ERR				=   0,
	AFESIM_COMM_ERR_CRC_FLIP 		=   1,
	AFESIM_COMM_ERR_NOISE_BIT_FLIP	=	2,
}AFESIM_CommErrType;

typedef enum
{
	AFESIM_COMM_BIT_FLIP       		=   0, 
	AFESIM_COMM_RC_FAULT			=   1,
	AFESIM_COMM_RR_FAULT			=   2,
	AFESIM_COMM_TR_FAULT			=   3,
}AFESIM_DeviceCommErrType;

typedef enum
{
	AFESIM_PULL_NONE,
	AFESIM_PULL_UP,
	AFESIM_PULL_DOWN
}AfeSim_PullType;

typedef enum
{
	AFESIM_DEVICE_USE_COMH = 0,	/* 从低到高正链通信 */
	AFESIM_DEVICE_USE_COML = 1	/* 从高到低反链通信 */
}AfeSim_DeviceCommDirectionType;

typedef enum
{
	AFESIM_DEVICE_POWER_SHUTDOWN = 0x00,	/* 默认 */
	AFESIM_DEVICE_POWER_SLEEP = 0x01,
	AFESIM_DEVICE_POWER_WAKE = 0x02,		/* 唤醒才能响应各种指令 */
	AFESIM_DEVICE_POWER_ON_RESET = 0x03,	/* VBAT低于8V则自动关机，高于9V则进入Shutdown */
	AFESIM_DEVICE_VALIDATE_79600 = 0x04,

	AFESIM_DEVICE_DESTROYED = 0x0F			/* 标定损坏 */
}AfeSim_DevicePowerType;

typedef enum
{
	AFESIM_DEVICE_IS_BASE,
	AFESIM_DEVICE_IS_STACK,
	AFESIM_DEVICE_IS_TOP
}AfeSim_DeviceStackType;

typedef enum
{
	AFESIM_DEVICE_GPIO_NOUSE,
	AFESIM_DEVICE_GPIO_MODULE_TEMP,
	AFESIM_DEVICE_GPIO_BALANCE_TEMP,
	AFESIM_DEVICE_GPIO_SHUNT_TEMP,
	AFESIM_DEVICE_GPIO_TSREF,
	AFESIM_DEVICE_GPIO_CURRENT,
	AFESIM_DEVICE_GPIO_DIGITAL_OUTPUT,
	AFESIM_DEVICE_GPIO_DIGITAL_INPUT,
}AfeSim_DeviceGpioUseType;

typedef struct
{
	uint8 devId;
	AfeSim_DeviceStackType stackType;
}AfeSim_DeviceInfomationType;

typedef struct
{
	/* Transmission Parameters */
	sint32 TxAttnenuation_DB;	/* Factor 0.001 */
	cplx32 TxImpedence_Z;

	/* Receive Parameters */
	sint32 rxAttnenuation_DB;
	sint32 rxThreshold_DB;
	cplx32 rxImpedence_Z;

	/* Connectivity Parameters */
	uint8 isBroken;
}AfeSim_DaisyChain_Model;

typedef struct
{
	float signalPower_dBm;	/* ±5V = 27dBm */
	float noisePower_dBm;	/* cause random bit reverse */
	float snr;
	uint8 noiseEnable;

	uint8 *recvBuff;
	uint16 recvSingleBuffLength;
	uint16 recvTotalBuffLength;
}AfeSim_CommDataType;


#endif /* SRC_BSW_BQ79XXX_DRIVER_AFESIM_DEVICE_TYPE_H_ */
