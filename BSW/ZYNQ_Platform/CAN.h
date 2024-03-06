/*
 * CAN.h
 *
 *  Created on: 2024年2月9日
 *      Author: Administrator
 */

#ifndef SRC_CAN_DRIVER_CAN_H_
#define SRC_CAN_DRIVER_CAN_H_

#include "Project_Define.h"
#include "ZYNQ_BasicType.h"

#ifdef ZYNQ_USE_FREERTOS
//#include "FreeRTOS.h"
#endif

#include "xparameters.h"
#include "xcanps.h"
#include "xil_exception.h"
#include "xil_printf.h"
#include "sleep.h"
#include "xscugic.h"

#ifdef CANXPS_USE_GLOBAL_INTR_INIT
#include "Interrupts.h"
#endif

#define INTC							XScuGic
#define INTC_DEVICE_ID					XPAR_SCUGIC_SINGLE_DEVICE_ID

#define CAN_DEVICE_VALID				1u
#define CAN_DEVICE_INVALID				0u

#define CAN_DATA_VALID 					1u
#define CAN_DATA_INVALID				0u

#define CAN_DEVICE_MAX_NUM				(2u)

#define CAN_DEVICE_NUM					(0u)

#define CAN_MESSAGE_MAX_NUM				(2048u)
#define CAN_FIFO_MAX_NUM				(1u)

#ifdef XPAR_XCANPS_0_DEVICE_ID

#undef CAN_DEVICE_NUM
#define CAN_DEVICE_NUM					1u

#define CAN0_DEVICE_ID					XPAR_XCANPS_0_DEVICE_ID
#define CAN0_INTR_VEC_ID				XPAR_XCANPS_0_INTR
#define CAN0_INTR_PRIORITY				(16 << 3)
#define CAN0_BAUDRATE					500000	/* 500k baud rate */
#define CAN0_BRPR_BAUD_PRESCALAR		((5000000 / CAN0_BAUDRATE) - 1)
#define CAN0_BTR_SYNCJUMPWIDTH			3
#define CAN0_BTR_SECOND_TIMESEGMENT		2
#define CAN0_BTR_FIRST_TIMESEGMENT		15
#define CAN0_FRAME_DATA_LENGTH			8 /* Frame Data field length */
#endif

#ifdef XPAR_XCANPS_1_DEVICE_ID

#undef CAN_DEVICE_NUM
#define CAN_DEVICE_NUM					2u

#define CAN1_DEVICE_ID					XPAR_XCANPS_1_DEVICE_ID
#define CAN1_INTR_VEC_ID				XPAR_XCANPS_1_INTR
#define CAN1_INTR_PRIORITY				(17 << 3)
#define CAN1_BAUDRATE					500000	/* 500k baud rate */
#define CAN1_BRPR_BAUD_PRESCALAR		((5000000 / CAN1_BAUDRATE) - 1)
#define CAN1_BTR_SYNCJUMPWIDTH			3
#define CAN1_BTR_SECOND_TIMESEGMENT		2
#define CAN1_BTR_FIRST_TIMESEGMENT		15
#define CAN1_FRAME_DATA_LENGTH			8 /* Frame Data field length */
#endif


/* Maximum CAN frame length in word */
#define XCANPS_MAX_FRAME_SIZE_IN_WORDS (XCANPS_MAX_FRAME_SIZE / sizeof(u32))


/* Typedef */

/* Store Fault Information */
typedef struct
{
	u32 sendErrCnt;
	u32 recvErrCnt;

	u32 ackErrCnt;
	u32 bitErrCnt;
	u32 stuffErrCnt;
	u32 formErrCnt;
	u32 crcErrCnt;

	u32 busOffErrCnt;
	u32 rxovfErrCnt;
}CAN_FaultManagerType;

/* Store Common Static Data */
typedef struct
{
	/* 接收报文总数 */
	u32 recvCnt;
	/* 接收报文未及时处理, 队列溢出次数 */
	u32 recvOverrunCnt;

	/* 发送报文总数 */
	u32 sendCnt;
	/* 发送FIFO被填满次数 */
	u32 sendFifoFullCnt;

}CAN_StatManagerType;

typedef union
{
	u32 data32;
	struct
	{
		u32 remoteTxRequest : 		1u  ;
		u32 extMsgId :        		18u ;
		u32 extMsgEnable :	   		1u  ;
		u32 subRemodeTxRequest : 	1u  ;
		u32 msgId :			 		11u ;
	}field;
}CAN_MsgID;

typedef union
{
	u32 data32;
	struct
	{
		u32 null : 28u ;
		u32 len  : 4u  ;
	}field;
}CAN_MsgDlc;

typedef union
{
	u32 data32[2];
	u16 data16[4];
	u8  data8[8];
}CAN_Data;

typedef struct
{
	CAN_MsgID 		msgId;
	CAN_MsgDlc		msgDlc;
	CAN_Data		data;
	u8   valid;
}CAN_MessageType;

typedef struct
{
	CAN_MessageType msg[CAN_MESSAGE_MAX_NUM];
	u32  	writeIndex; 	/* if TX, write from RTE, if RX, write from canDev */
	u32  	readIndex;		/* if TX, read to canDev, if RX, read to RTE */
	u32		queueLen;		/* number of message in queue */
}CAN_MsgQueueType;

typedef struct
{
	CAN_MsgQueueType rx;
	CAN_MsgQueueType tx;
	CAN_StatManagerType stat;
	CAN_FaultManagerType fault;
	u8 initDone;
}CAN_DeviceManagerType;

/* CAN Initializer */
typedef struct
{
	XCanPs * canInstance;
	XScuGic * intcInstance;
	u32 canDeviceID;
	u32 canIntrID;
	u32 canIntrPriority;
	struct
	{
		u32 prescalar;
		u32 syncJumpWidth;
		u32 secondTimeSegment;
		u32 firstTimeSegment;
	}canBaudConfig;
}XCanPs_Initilizer;

#define TEST_MESSAGE_ID		0x123


#ifdef ZYNQ_USE_FREERTOS
	#ifndef Global_XScuGic_Instance
		extern XScuGic xInterruptController;
		#define Global_XScuGic_Instance 	xInterruptController
	#endif
	#ifndef CAN_INTR_Insance
		#define CAN_INTR_Instance	Global_XScuGic_Instance
	#endif
#else
	#ifdef CANXPS_USE_GLOBAL_INTR_INIT
		#ifndef Global_XScuGic_Instance
			extern XScuGic INTR_XScuGic_Instance;
			#define Global_XScuGic_Instance 	INTR_XScuGic_Instance
		#endif
	#ifndef CAN_INTR_Instance
		#define CAN_INTR_Instance	Global_XScuGic_Instance
	#endif
	#else
		#ifndef CAN_INTR_Instance
			extern XScuGic CAN_XScuGic_Instance;
			#define CAN_INTR_Instance	CAN_XScuGic_Instance
		#endif
	#endif
#endif

extern CAN_DeviceManagerType CAN_Manager[CAN_DEVICE_NUM];


/* API Functions */
extern int CAN_Init(void);

extern void CAN_Demo(void);

extern int CAN_SendFrame(const u32 canDeviceId, const CAN_MessageType* msg);

extern int CAN_InsertTxFrame(const u32 canDeviceId, const CAN_MessageType* msg);

extern CAN_MessageType* CAN_GetTxSendFrame(const u32 canDeviceId);

extern int CAN_StartTransmitTxFrame(const u32 canDeviceId);

extern int CAN_ReadRxFrame(const u32 canDeviceId, CAN_MessageType* msg);

extern CAN_MessageType* CAN_GetRxFrame(const u32 canDeviceId);

#endif /* SRC_CAN_DRIVER_CAN_H_ */
