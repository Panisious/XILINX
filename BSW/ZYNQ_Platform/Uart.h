/*
 * Uart.h
 *
 *  Created on: 2024年1月20日
 *      Author: Administrator
 */

#ifndef SRC_ZYNQ_PLATFORM_UART_UART_H_
#define SRC_ZYNQ_PLATFORM_UART_UART_H_

#include "Project_Define.h"

#include "xscugic.h"
#include "xuartps.h"
#include "sleep.h"

#include "ZYNQ_BasicType.h"

#ifdef UARTXPS_USE_GLOBAL_INTR_INIT
#include "Interrupts.h"
#else
#ifndef INTR_UART0PS_INTR_ID
#define INTR_UART0PS_INTR_ID  		XPAR_XUARTPS_0_INTR
#endif
#ifndef INTR_UART1PS_INTR_ID
#define INTR_UART1PS_INTR_ID		XPAR_XUARTPS_1_INTR
#endif
#endif

#define UART_DEVICE_MAX_NUM			2u
#define UART_DEVICE_VALID			1u
#define UART_DEVICE_INVALID			0u

#define UARTXPS_DATA_INVALID		0u
#define UARTXPS_DATA_VALID			1u

#define UARTXPS_FIFO_MAX_LEN		64u

#define UARTXPS_MAX_QUEUE_EXP		6u
#define UARTXPS_MAX_QUEUE_NUM		(1u << UARTXPS_MAX_QUEUE_EXP)	/* 32 */
#define UARTXPS_MAX_BYTE_EXP		8u
#define UARTXPS_MAX_BYTE_NUM		(1u << UARTXPS_MAX_BYTE_EXP)	/* 256 */

#define UARTXPS_MAX_BUFF_SIZE		(UARTXPS_MAX_BYTE_NUM * UARTXPS_MAX_QUEUE_NUM)	/* 8192   */
#define UARTXPS_MAX_BUFF_MASK		(UARTXPS_MAX_BUFF_SIZE - 1u)					/* 0x1FFF */

/* UART0 初始化配置 */
#ifdef XPAR_PS7_UART_0_DEVICE_ID

#define UARTXPS_MAX_DEVICE_NUM		(1u)

#define UART0PS_DEVICE_ID			(XPAR_PS7_UART_0_DEVICE_ID)

#define UART0PS_BAUD_RATE			(115200u)

#define UART0PS_FIFO_THRESHOLD		(32u)

#define UART0PS_RECV_TIMEOUT		(4u)

#ifndef UART0PS_INTR_PRIORITY
#define UART0PS_INTR_PRIORITY		(18u << 3u)
#endif

#ifndef UART0PS_INTR_MASK
#define UART0PS_INTR_MASK			(	XUARTPS_IXR_RXOVR 		|\
										XUARTPS_IXR_RXFULL 		|\
										XUARTPS_IXR_TOUT		|\
										XUARTPS_IXR_TXEMPTY		)
#endif
#endif


/* UART1 初始化配置 */
#ifdef XPAR_PS7_UART_1_DEVICE_ID

#define UARTXPS_MAX_DEVICE_NUM		(1u)

#define UART1PS_DEVICE_ID			(XPAR_PS7_UART_1_DEVICE_ID)

#define UART1PS_BAUD_RATE			(115200u)

#define UART1PS_FIFO_THRESHOLD		(32u)

#define UART1PS_RECV_TIMEOUT		(4u)

#ifndef UART1PS_INTR_PRIORITY
#define UART1PS_INTR_PRIORITY		(18u << 3u)
#endif

#ifndef UART1PS_INTR_MASK
#define UART1PS_INTR_MASK			(	XUARTPS_IXR_RXOVR 		|\
										XUARTPS_IXR_RXFULL 		|\
										XUARTPS_IXR_TOUT		|\
										XUARTPS_IXR_TXEMPTY		)
#endif
#endif

typedef uint8 UartSingleBufferType[UARTXPS_MAX_BYTE_NUM];
typedef uint8 UART_BufferType[UARTXPS_MAX_QUEUE_NUM][UARTXPS_MAX_BYTE_NUM];

typedef struct
{
	UART_BufferType * buff;
	UartSingleBufferType *ptr;
	uint16 byteNum[UARTXPS_MAX_QUEUE_NUM];			/* RX & TX */
	uint16 txCurrentPos[UARTXPS_MAX_QUEUE_NUM];		/* TX */
	uint8 valid[UARTXPS_MAX_QUEUE_NUM];				/* TX */
	struct
	{
		uint16 mask : UARTXPS_MAX_QUEUE_EXP;
	}writeIndex;
	struct
	{
		uint16 mask : UARTXPS_MAX_QUEUE_EXP;
	}readIndex;
	uint16 queueLen;
}UART_MsgQueueType;

typedef struct
{
	uint32 sendCnt;
	uint32 recvCnt;
}UART_StatManagerType;

typedef struct
{
	uint32 errCnt;
}UART_FaultManagerType;


typedef struct
{
	UART_MsgQueueType		tx;
	UART_MsgQueueType 		rx;
	UART_StatManagerType 	stat;
	UART_FaultManagerType 	fault;
	XUartPs*				uartInstance;
}UART_ManagerType;

typedef struct
{
	XUartPs * uartInstance;
	XScuGic * intcInstance;
	uint32 uartDeviceId;
	uint32 uartIntrId;
	uint32 uartBaudRate;
	uint32 uartFifoThreshold;
	uint32 uartRecvTimeout;
	uint32 uartIntrPriority;
	uint32 uartIntrMask;

	UART_ManagerType * manager;
	UART_BufferType * uartTxBuff;
	UART_BufferType * uartRxBuff;
}UART_InitializerType;

typedef struct
{
	uint8 valid;
	uint16 len;
	uint8* ptr;
}UART_RxApiType;

typedef struct
{
	UartSingleBufferType* ptr;
	uint16* len;
}UART_TxApiType;

//#undef ZYNQ_USE_FREERTOS
//#undef UARTXPS_USE_GLOBAL_INTR_INIT

#ifdef ZYNQ_USE_FREERTOS
	#ifndef Global_XScuGic_Instance
		extern XScuGic xInterruptController;
		#define Global_XScuGic_Instance 	xInterruptController
	#endif
	#ifndef UART_INTR_Instance
		#define UART_INTR_Instance	Global_XScuGic_Instance
	#endif
#else
	#ifdef UARTXPS_USE_GLOBAL_INTR_INIT
		#ifndef Global_XScuGic_Instance
			extern XScuGic INTR_XScuGic_Instance;
			#define Global_XScuGic_Instance 	INTR_XScuGic_Instance
		#endif
	#ifndef UART_INTR_Instance
		#define UART_INTR_Instance	Global_XScuGic_Instance
	#endif
	#else
		#ifndef UART_INTR_Instance
			extern XScuGic UART_XScuGic_Instance;
			#define UART_INTR_Instance	UART_XScuGic_Instance
		#endif
	#endif
#endif


extern UART_BufferType UART0_RecvBuffer;
extern UART_BufferType UART0_SendBuffer;
extern UART_BufferType UART1_RecvBuffer;
extern UART_BufferType UART1_SendBuffer;

/* API Function */
extern int UART_Init(void);
extern void UART_Demo(void);

extern int UART_GetValidRxData(uint32 uartDeviceId, UART_RxApiType* apiReturn);
extern int UART_GetSendDataBuff(uint32 uartDeviceId, UART_TxApiType* apiReturn);
extern int UART_ConfirmSendDataBuff(u32 uartDeviceId);
extern int UART_StartTransmitData(u32 uartDeviceId);


#endif /* SRC_ZYNQ_PLATFORM_UART_UART_H_ */
