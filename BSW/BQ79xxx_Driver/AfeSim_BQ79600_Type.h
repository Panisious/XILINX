/*
 * AfeSim_BQ79600_Type.h
 *
 *  Created on: 2024年1月7日
 *      Author: Administrator
 */

#ifndef SRC_AFESIM_BQ79600_TYPE_H_
#define SRC_AFESIM_BQ79600_TYPE_H_

#include "AfeSim_Device_Type.h"

typedef enum
{
	AFESIM_AUTOADDR_STATE_IDLE,
	AFESIM_AUTOADDR_INTERNAL_DLL1,
	AFESIM_AUTOADDR_INTERNAL_DLL2,
	AFESIM_AUTOADDR_INTERNAL_DLL3,
	AFESIM_AUTOADDR_INTERNAL_DLL4,
	AFESIM_AUTOADDR_INTERNAL_DLL5,
	AFESIM_AUTOADDR_INTERNAL_DLL6,
	AFESIM_AUTOADDR_INTERNAL_DLL7,
	AFESIM_AUTOADDR_INTERNAL_DLL8,
	AFESIM_AUTOADDR_STATE_ACTIVE,
	AFESIM_AUTOADDR_INTERNAL_DLL9,
	AFESIM_AUTOADDR_INTERNAL_DLL10,
	AFESIM_AUTOADDR_INTERNAL_DLL11,
	AFESIM_AUTOADDR_INTERNAL_DLL12,
	AFESIM_AUTOADDR_INTERNAL_DLL13,
	AFESIM_AUTOADDR_INTERNAL_DLL14,
	AFESIM_AUTOADDR_INTERNAL_DLL15,
	AFESIM_AUTOADDR_INTERNAL_DLL_DONE,
	AFESIM_AUTOADDR_INTERNAL_DLL_INVALID = 0xFFFF,
}AfeSim_AutoAddrStateType;



typedef enum
{
	AFESIM_BASE_DEVICE = 0,
	AFESIM_TOP_STACK = 1,
	AFESIM_STACK_DEVICE = 2,
}AfeSim_StackRoleType;


typedef struct
{
	uint32 RecvTimes;
	uint32 TransTimes;
	uint32 SpiErrorTimes;
}AfeSim_BQ79600_StatisticType;


typedef struct
{
	AfeSim_DeviceCommDirectionType  commDir;			/* 通信方向  0正链  1反链*/
	uint8  StackNum[2];							/* 正反链芯片数 */
	AfeSim_AutoAddrStateType  AutoAddrState;	/* 自动编址状态 */
}AfeSim_BQ79600_StackManagerType;

typedef struct
{
	uint8  CmdType;					/* Command Type */
	uint8  IsMultipleOp;			/* 1 = is stack/broadcast cmd, 0 = is single cmd*/
	uint8  deviceId;				/* Device Id in last command */
	uint16 rwBytes;				/* Valid data length in last command */
	uint16 rwRegisterAddr;		/* Register to R/W in last command */
	uint16 Crc16Value;
}AfeSim_BQ79600_CommandManagerType;

typedef struct
{
	uint8  UpdateFlag;				/* New data string received, need process */
	uint8  PingUpdateFlag;			/* Receive Wake Ping, Shutdown Ping */
	uint8  StringLength;			/* Received string length */
	uint16 replyBufferLength;		/* */
	uint8  MOSI_RxData[128];
	uint8  *MISO_TxData;
	uint8  MISO_TxFullData[1024];
	uint16 ReadbackLastBytes;
}AfeSim_BQ79600_SpiMangerType;

typedef struct
{
	/* SPI Communication */
	AfeSim_BQ79600_SpiMangerType spi;
	/* SPI Command */
	AfeSim_BQ79600_CommandManagerType cmd;

	AfeSim_BQ79600_StatisticType stat;

	AfeSim_BQ79600_StackManagerType stack;
}AfeSim_BQ79600_ManagerType; /* a.k.a Man */

typedef enum
{
	BQ79600_CLEAR = 0x00,
	BQ79600_SINGLE_READ = 0x80,
	BQ79600_SINGLE_WRITE = 0x90,
	BQ79600_STACK_READ = 0xA0,
	BQ79600_STACK_WRITE = 0xB0,
	BQ79600_BROADCAST_READ = 0xC0,
	BQ79600_BROADCAST_WRITE = 0xD0,
	BQ79600_BROADCAST_REVERSE_WRITE = 0xE0,
	BQ79600_READ_BACK_FF = 0xF0,
}AfeSim_BQ79600_CommandType;

typedef enum
{
	BQ79600_WRITE = 0x00,
	BQ79600_READ,
	BQ79600_READ_BACK,
	BQ79600_INVALID_CMD
}AfeSim_BQ79600_CommandClass;


#endif /* SRC_AFESIM_BQ79600_TYPE_H_ */
