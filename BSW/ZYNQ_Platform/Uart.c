/*
 * Uart.c
 *
 *  Created on: 2024年1月20日
 *      Author: Administrator
 */

#include "Uart.h"

static uint8 UART_DEVICE_ID_REGISTER[UART_DEVICE_MAX_NUM] =
{
#ifdef XPAR_PS7_UART_0_DEVICE_ID
		UART_DEVICE_VALID,
#else
		UART_DEVICE_INVALID,
#endif

#ifdef XPAR_PS7_UART_1_DEVICE_ID
		UART_DEVICE_VALID,
#else
		UART_DEVICE_INVALID,
#endif
};

#define UART_ASSERT_DEVICE_ID_VALID(uartDeviceId)		\
	if( ((uint32)uartDeviceId >= (uint32)UARTXPS_MAX_DEVICE_NUM) || (UART_DEVICE_ID_REGISTER[uartDeviceId] == UART_DEVICE_INVALID) )\
	{\
		while(1)\
		{\
			xil_printf("Error: Invalid UART Device ID %d\r\nFile %s\r\nLine %d\r\n", uartDeviceId, __FILE__, __LINE__ );\
			sleep(1);\
		}\
	}

#ifndef UARTXPS_USE_OUTER_BUFF
UART_BufferType UART0_RecvBuffer;
UART_BufferType UART0_SendBuffer;
UART_BufferType UART1_RecvBuffer;
UART_BufferType UART1_SendBuffer;
#endif

XScuGic UART_XScuGic_Instance;

UART_RxApiType UART_GlobalRxApiReturn;
UART_TxApiType UART_GlobalTxApiReturn;

static UART_ManagerType UART_Manager[UARTXPS_MAX_DEVICE_NUM];

static XUartPs UART_Instance[UARTXPS_MAX_DEVICE_NUM];

static UART_InitializerType UART_Initializer[UARTXPS_MAX_DEVICE_NUM];


/* Function Prototypes */
int UART_Init(void);
static int UART_DeviceInit(UART_InitializerType* init);
static int UART_Manager_Init(UART_InitializerType* init);
static int UART_Intr_Init(UART_InitializerType* init);

static int UART_ReadDataFromDevice(UART_ManagerType* manager, uint8 data);

static void UART_IRQHandler(void *callbackRef);
static void UART_IRQCallback_ReadData(UART_ManagerType* manager);
static void UART_IRQCallback_RecvTimeout(UART_ManagerType* manager);
static void UART_IRQCallback_SendEmpty(UART_ManagerType* manager);

/* Function Defines */

/* API - Uart_Init
 * Initialize All UART Devices */
int UART_Init(void)
{
	int status;
#ifdef XPAR_PS7_UART_0_DEVICE_ID
	UART_Initializer[0u].intcInstance = &UART_INTR_Instance;
	UART_Initializer[0u].uartIntrId = INTR_UART0PS_INTR_ID;
	UART_Initializer[0u].uartInstance = &UART_Instance[0u];
	UART_Initializer[0u].uartDeviceId = UART0PS_DEVICE_ID;
	UART_Initializer[0u].uartBaudRate = UART0PS_BAUD_RATE;
	UART_Initializer[0u].uartFifoThreshold = UART0PS_FIFO_THRESHOLD;
	UART_Initializer[0u].uartRecvTimeout = UART0PS_RECV_TIMEOUT;
	UART_Initializer[0u].uartIntrPriority =	UART0PS_INTR_PRIORITY;
	UART_Initializer[0u].uartIntrMask = UART0PS_INTR_MASK;

	UART_Initializer[0u].manager = &UART_Manager[0u];
	UART_Initializer[0u].uartTxBuff = &UART0_SendBuffer;
	UART_Initializer[0u].uartRxBuff = &UART0_RecvBuffer;

	status = UART_DeviceInit(&UART_Initializer[0u]);
	if(status != XST_SUCCESS)
	{
		printf("\r\n Uart0Ps Init Fail\r\n");
		return status;
	}
#endif

#ifdef XPAR_PS7_UART_1_DEVICE_ID
	UART_Initializer[1u].intcInstance = &UART_INTR_Instance;
	UART_Initializer[1u].uartIntrId = INTR_UART1PS_INTR_ID;
	UART_Initializer[1u].uartInstance = &UART_Instance[1u];
	UART_Initializer[1u].uartDeviceId = UART1PS_DEVICE_ID;
	UART_Initializer[1u].uartBaudRate = UART1PS_BAUD_RATE;
	UART_Initializer[1u].uartFifoThreshold = UART1PS_FIFO_THRESHOLD;
	UART_Initializer[1u].uartRecvTimeout = UART1PS_RECV_TIMEOUT;
	UART_Initializer[1u].uartIntrPriority =	UART1PS_INTR_PRIORITY;
	UART_Initializer[1u].uartIntrMask = UART1PS_INTR_MASK;

	UART_Initializer[1u].manager = &UART_Manager[1u];
	UART_Initializer[1u].uartTxBuff = (uint8*)UART1_SendBuffer;
	UART_Initializer[1u].uartRxBuff = (uint8*)UART1_RecvBuffer;

	status = UART_DeviceInit(&UART_Initializer[1u]);
	if(status != XST_SUCCESS)
	{
		printf("\r\n Uart1Ps Init Fail\r\n");
		return status;
	}
#endif

	return XST_SUCCESS;
}


/* API - UartxPs_GetValidRxData */
int UART_GetValidRxData(uint32 uartDeviceId, UART_RxApiType* apiReturn)
{
	UART_ASSERT_DEVICE_ID_VALID(uartDeviceId);

	UART_ManagerType* manager = &UART_Manager[uartDeviceId];

	/* There is valid data, allow read */
	if(manager->rx.queueLen > 0)
	{
		manager->rx.queueLen--;
		uint8 index = manager->rx.readIndex.mask++;
		uint16 len = manager->rx.byteNum[index];
		manager->rx.byteNum[index] = 0u;
		apiReturn->valid = UARTXPS_DATA_VALID;
		apiReturn->len = len;
		apiReturn->ptr = (*manager->rx.buff)[index];
		return XST_SUCCESS;
	}
	else
	{
		return XST_FAILURE;
	}
}


/* API - UartxPs_GetSendDataBuff
 * return one send buff for user to fill
 * After use UartxPs_GetSendDataBuff, fill buffer, and use UartxPs_InsertSendData */
int UART_GetSendDataBuff(uint32 uartDeviceId, UART_TxApiType* apiReturn)
{
	UART_ASSERT_DEVICE_ID_VALID(uartDeviceId);

	UART_ManagerType* manager = &UART_Manager[uartDeviceId];
	uint16 writeIndex = manager->tx.writeIndex.mask;

	/* 若队列未满，数据无效，则允许插入UART数据 */
	if(manager->tx.queueLen >= UARTXPS_MAX_QUEUE_NUM)
	{
		return XST_FAILURE;
	}
	if(manager->tx.valid[writeIndex] == UARTXPS_DATA_VALID)
	{
		return XST_FAILURE;
	}
	/* Clear send length before fill buffer */
	manager->tx.byteNum[writeIndex] = 0u;
	manager->tx.txCurrentPos[writeIndex] = 0u;

	/* Fill chart for return */
	apiReturn->ptr = &((*manager->tx.buff)[writeIndex]);
	apiReturn->len = &(manager->tx.byteNum[writeIndex]);

	manager->tx.valid[writeIndex] = UARTXPS_DATA_VALID;
	manager->tx.writeIndex.mask++;
	manager->tx.queueLen++;

	return XST_SUCCESS;
	//return apiReturn;
}


/* API - UartxPs_InsertSendData
 * Check if user correctly fill buffer */
int UART_ConfirmSendDataBuff(uint32 uartDeviceId)
{
	UART_ASSERT_DEVICE_ID_VALID(uartDeviceId);

	UART_ManagerType* manager = &UART_Manager[uartDeviceId];
	uint16 writeIndex = manager->tx.writeIndex.mask;

	/* Check if user correctly fill buffer*/
	if(manager->tx.byteNum[writeIndex] > 0 &&\
			manager->tx.txCurrentPos[writeIndex] == 0u)
	{

		return XST_SUCCESS;
	}
	else
	{
		return XST_FAILURE;
	}
}


static int UART_SendData(UART_ManagerType* manager)
{
	uint16 i;
	XUartPs* uartInstance = manager->uartInstance;
	uint16 readIndex = manager->tx.readIndex.mask;

	if(manager->tx.queueLen <= 0u)
	{
		return XST_FAILURE;
	}
	if(manager->tx.valid[readIndex] != UARTXPS_DATA_VALID)
	{
		return XST_FAILURE;
	}

	uint16 sendLen = manager->tx.byteNum[readIndex];
	uint16* currentPos =  &manager->tx.txCurrentPos[readIndex];
	if((sendLen > 0) && (*currentPos == 0))
	{
		manager->tx.ptr = &(*manager->tx.buff)[readIndex];
		uint16 singleSendLen = sendLen;
		if(singleSendLen > UARTXPS_FIFO_MAX_LEN)
		{
			singleSendLen = UARTXPS_FIFO_MAX_LEN;
			*currentPos += UARTXPS_FIFO_MAX_LEN;
		}
		else
		{
			*currentPos += sendLen;
		}
		for(i = 0; i < singleSendLen; i++)
		{
			XUartPs_SendByte(uartInstance->Config.BaseAddress, (*manager->tx.ptr)[i]);
		}
	}
	return XST_SUCCESS;
}


/* API - UartxPs_StartTransmitData */
int UART_StartTransmitData(uint32 uartDeviceId)
{
	UART_ASSERT_DEVICE_ID_VALID(uartDeviceId);
	UART_ManagerType* manager = &UART_Manager[uartDeviceId];
	return UART_SendData(manager);
}


/* Private - UartxPs_Init
 * Initialize one UART device */
static int UART_DeviceInit(UART_InitializerType* init)
{
	int status;
	XUartPs* uartInstance = init->uartInstance;
	uint32 deviceId = init->uartDeviceId;
	uint32 baudRate = init->uartBaudRate;
	uint32 fifoThreshold = init->uartFifoThreshold;
	uint32 uartRecvTimeout = init->uartRecvTimeout;
	XUartPs_Config * uartConfig;

	uartConfig = XUartPs_LookupConfig(deviceId);
	if(uartConfig == NULL)
	{
		return XST_FAILURE;
	}
	status = XUartPs_CfgInitialize(uartInstance, uartConfig, uartConfig->BaseAddress);
	if(status != XST_SUCCESS)
	{
		return status;
	}
	status = XUartPs_SelfTest(uartInstance);
	if(status != XST_SUCCESS)
	{
		return status;
	}

	XUartPs_SetOperMode(uartInstance, XUARTPS_OPER_MODE_NORMAL);
	XUartPs_SetBaudRate(uartInstance, baudRate);
	XUartPs_SetFifoThreshold(uartInstance, fifoThreshold);
	XUartPs_SetRecvTimeout(uartInstance, uartRecvTimeout);

	UART_Manager_Init(init);

	UART_Intr_Init(init);

	return XST_SUCCESS;
}


/* Private - UartxPs_Manager_Init
 * Initialize one UART manager */
static int UART_Manager_Init(UART_InitializerType* init)
{
	UART_ManagerType * manager = init->manager;

	manager->tx.buff = init->uartTxBuff;
	manager->tx.ptr = init->uartTxBuff[0u];
	manager->rx.buff = init->uartRxBuff;
	manager->rx.ptr = init->uartRxBuff[0u];

	manager->uartInstance = init->uartInstance;

	return XST_SUCCESS;
}


/* Private - Uart_Intr_Init
 * Initialize Interrupt of one UART device */
static int UART_Intr_Init(UART_InitializerType* init)
{
	int status = XST_SUCCESS;
	UART_ManagerType * manager = init->manager;
	XScuGic* intcInstance = init->intcInstance;
	XUartPs* uartInstance = init->uartInstance;
	uint32 uartIntrId = init->uartIntrId;
	uint32 uartIntrMask = init->uartIntrMask;
	uint8 priority;
	uint8 trigger;

#ifndef UARTXPS_USE_GLOBAL_INTR_INIT
	XScuGic_Config *IntcConfig; /* Instance of the interrupt controller */

	Xil_ExceptionInit();
	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	IntcConfig = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	if (NULL == IntcConfig) {
		return XST_FAILURE;
	}
	status = XScuGic_CfgInitialize(intcInstance, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
				(Xil_ExceptionHandler)XScuGic_InterruptHandler,
				intcInstance);
#endif

	XScuGic_Connect(intcInstance,
					uartIntrId,
					(Xil_ExceptionHandler)UART_IRQHandler,
					(void *) manager);

	XUartPs_SetInterruptMask(uartInstance, uartIntrMask);

	XScuGic_GetPriorityTriggerType(intcInstance, uartIntrId, &priority, &trigger);
	priority = init->uartIntrPriority;
	XScuGic_SetPriorityTriggerType(intcInstance, uartIntrId, priority, trigger);

	XScuGic_Enable(intcInstance, uartIntrId);

#ifndef UARTXPS_USE_GLOBAL_INTR_INIT
	/*
	 * Enable interrupts in the Processor.
	 */
	Xil_ExceptionEnable();
#endif

	return status;
}


/* Private - UartxPs_IRQHandler
 * Called when UART interrupt happens */
static void UART_IRQHandler(void *callbackRef)
{
	UART_ManagerType* manager = (UART_ManagerType*)callbackRef;
	XUartPs* uartInstancePtr = manager->uartInstance;
	uint32 isr_status; /*中断状态标志*/

	/*读取中断 ID 寄存器，判断触发的是哪种中断*/
	isr_status = XUartPs_ReadReg(uartInstancePtr->Config.BaseAddress,
			XUARTPS_IMR_OFFSET);
	isr_status &= XUartPs_ReadReg(uartInstancePtr->Config.BaseAddress,
			XUARTPS_ISR_OFFSET);

	/* Clean Interrupt Status */
	XUartPs_WriteReg(uartInstancePtr->Config.BaseAddress, XUARTPS_ISR_OFFSET, isr_status);

	/* UART Receive Process */
	if(isr_status & (XUARTPS_IXR_RXOVR | XUARTPS_IXR_RXFULL) )	/* Receive Data Trigger/Full */
	{
		UART_IRQCallback_ReadData(manager);
	}
	if(isr_status & XUARTPS_IXR_TOUT)
	{
		UART_IRQCallback_RecvTimeout(manager);
	}

	/* UART Transmit Process */
	if(isr_status & XUARTPS_IXR_TXEMPTY)
	{
		UART_IRQCallback_SendEmpty(manager);
	}
}


static void UART_IRQCallback_ReadData(UART_ManagerType* manager)
{
	int status;
	XUartPs * uartInstance = manager->uartInstance;
	uint32 baseAddress = uartInstance->Config.BaseAddress;
	uint8 data;

	while(XUartPs_IsReceiveData(baseAddress))
	{
		data = XUartPs_RecvByte(baseAddress);
		status = UART_ReadDataFromDevice(manager, data);
	}
	(void)status;
}


/* Private - UartxPs_ReadDataFromDevice
 * Insert one byte read from UART device into UART manager buffer */
static int UART_ReadDataFromDevice(UART_ManagerType* manager, uint8 data)
{
	int status = XST_SUCCESS;
	uint16 writeIndex = manager->rx.writeIndex.mask;
	uint16 *byteNum = &manager->rx.byteNum[writeIndex];
	UartSingleBufferType *ptr = manager->rx.ptr;

	if(*byteNum >= UARTXPS_MAX_BYTE_NUM)
	{
		writeIndex = ++manager->rx.writeIndex.mask;
		byteNum = &manager->rx.byteNum[writeIndex];
		ptr = manager->rx.ptr = &(*manager->rx.buff)[writeIndex];
		byteNum[writeIndex] = 0u;
		manager->rx.queueLen++;
	}
	(*ptr)[*byteNum] = data;
	(*byteNum)++;

	return status;
}

static void UART_IRQCallback_RecvTimeout(UART_ManagerType* manager)
{
	int status;
	uint8 data;
	XUartPs * uartInstance = manager->uartInstance;
	uint32 baseAddress = uartInstance->Config.BaseAddress;
	uint16 writeIndex = manager->rx.writeIndex.mask;

	/* 接收剩余字符 */
	while(XUartPs_IsReceiveData(baseAddress))
	{
		data = XUartPs_RecvByte(baseAddress);
		status = UART_ReadDataFromDevice(manager, data);
	}
	/* 字符串尾补0 */
	(*manager->rx.ptr)[manager->rx.byteNum[writeIndex]] = 0u;

	/* 若成功收到至少一个字符，则切换到队列下一个Buffer */
	if(manager->rx.byteNum[writeIndex] > 0)
	{
		writeIndex = ++manager->rx.writeIndex.mask;
		manager->rx.ptr = &(*manager->rx.buff)[writeIndex];
		manager->rx.byteNum[writeIndex] = 0u;
		manager->rx.queueLen++;
		(void)status;
	}
}


/* Private - UartxPs_IRQCallback_SendEmpty
 * 当 UART TX FIFO为空时，检查当前完整buffer是否发送完成
 * 若是，切换下一个buffer，检查数据是否有效，有效则发送
 * 若否，向fifo填入后续byte，发送
 * */
static void UART_IRQCallback_SendEmpty(UART_ManagerType* manager)
{
	uint16 i;
	uint16 readIndex = manager->tx.readIndex.mask;
	if(manager->tx.valid[readIndex] != UARTXPS_DATA_VALID)
	{
		return;
	}
	XUartPs * uartInstance = manager->uartInstance;
	uint32 baseAddress = uartInstance->Config.BaseAddress;
	uint16 totalSendLen = manager->tx.byteNum[readIndex];
	uint16* currentPos = &manager->tx.txCurrentPos[readIndex];

	uint16 sendLen = totalSendLen - *currentPos;
	if(sendLen > 0)
	{
		if(sendLen > UARTXPS_FIFO_MAX_LEN)
		{
			sendLen = UARTXPS_FIFO_MAX_LEN;
		}
		manager->tx.ptr = &(*manager->tx.buff)[readIndex];
		uint16 start = *currentPos;
		uint16 end = start + sendLen;
		for(i = start; i < end; i++)
		{
			XUartPs_SendByte(baseAddress, (*manager->tx.ptr)[i]);
		}
		*currentPos = end;
	}
	else
	{
		manager->tx.valid[readIndex] = UARTXPS_DATA_INVALID;
		manager->tx.readIndex.mask++;
		if(manager->tx.queueLen > 0)
		{
			manager->tx.queueLen--;
		}
		UART_SendData(manager);
	}
}

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

/* API - UART_Demo */
void UART_Demo(void)
{
	int status;
	uint16 recvCnt = 0;
	UART_RxApiType rx;
	UART_TxApiType tx;
	while(1)
	{
		status = UART_GetValidRxData(0, &rx);
		if(status != XST_SUCCESS)
		{
			break;
		}
		else
		{
#if 1
			//xil_printf("Fuck\r\n");
			recvCnt++;
			status = UART_GetSendDataBuff(0, &tx);
			if(status == XST_SUCCESS)
			{
				memcpy(*tx.ptr, rx.ptr, rx.len);
				*tx.len = rx.len;
			}
#else
			xil_printf(rx->ptr);
#endif
		}
	}
	if(recvCnt > 0)
	{
		UART_StartTransmitData(0);
	}
	else
	{
		//xil_printf("No UART Data Received\r\n");
	}
}



#ifndef ZYNQ_DISABLE_DEMO
int main(void)
{
	int i;
	UART_Init();
	while(1)
	{
		UART_Demo();
		for(i=1000000;i>0;i--);
	}
}
#endif

