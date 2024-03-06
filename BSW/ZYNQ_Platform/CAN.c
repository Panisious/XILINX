/*
 * CAN.c
 *
 *  Created on: 2024年2月9日
 *      Author: Administrator
 */

#include "CAN.h"

/* Private Defines */
static uint8 CAN_DEVICE_ID_REGISTER[CAN_DEVICE_MAX_NUM]=
{
#ifdef XPAR_XCANPS_0_DEVICE_ID
		CAN_DEVICE_VALID,
#else
		CAN_DEVICE_INVALID,
#endif

#ifdef XPAR_XCANPS_1_DEVICE_ID
		CAN_DEVICE_VALID,
#else
		CAN_DEVICE_INVALID,
#endif
};

#define CAN_ASSERT_DEVICE_ID_VALID(canDeviceID)					\
	if(((uint32)canDeviceID >= (uint32)CAN_DEVICE_MAX_NUM) || (CAN_DEVICE_ID_REGISTER[canDeviceID] != CAN_DEVICE_VALID))	\
	{\
		while(1)\
		{\
			xil_printf("Error: Invalid CAN Device ID %d, %s  Line %d\r\n", canDeviceID, __FILE__, __LINE__ );\
			sleep(1);\
		}\
	}\
	if(CAN_Manager[canDeviceID].initDone != TRUE)\
	{\
		return NULL;\
	}
#define CAN_SOURCE
#define CAN_DESTINY


/* Variables */

/* CAN设备使用的ZYNQ系统中断控制器实例 */
XScuGic CAN_XScuGic_Instance;

/* CAN设备管理结构体 */
CAN_DeviceManagerType CAN_Manager[CAN_DEVICE_NUM];

/* CAN设备实例 */
static XCanPs CAN_Instance[CAN_DEVICE_NUM];    /* Instance of the Can driver */

/* CAN设备初始化配置量 */
static XCanPs_Initilizer CAN_Initializer[CAN_DEVICE_NUM];



/* Function Prototypes */

/* CAN Demo Function */
void CAN_Demo(void);

/* CAN Init Functions */
int CAN_Init(void);
static int CAN_DeviceInit(XCanPs_Initilizer* canxInit);
static void CAN_BaudConfig(XCanPs * InstancePtr);
static int CAN_SetupInterruptSystem(XCanPs_Initilizer* canxInit);

static u32 CAN_FromHandlerToID(XCanPs *CanPtr);

/* CAN Communication Functions */
int CAN_SendFrame(const u32 canDeviceId, const CAN_MessageType* msg);
int CAN_InsertTxFrame(const u32 canDeviceId, const CAN_MessageType* msg);
CAN_MessageType* CAN_GetTxSendFrame(const u32 canDeviceId);
int CAN_StartTransmitTxFrame(const u32 canDeviceId);
int CAN_ReadRxFrame(const u32 canDeviceId, CAN_MessageType* msg);

/* CAN Interrupt Handler Functions */
static void CAN_SendHandler(void *CallBackRef);
static void CAN_RecvHandler(void *CallBackRef);
static void CAN_ErrorHandler(void *CallBackRef, u32 ErrorMask);
static void CAN_EventHandler(void *CallBackRef, u32 Mask);


/* Function Definitions */

/* API - CAN_Init
 * 初始化所有CAN设备 */
int CAN_Init(void)
{
	int status;

#ifdef XPAR_XCANPS_0_DEVICE_ID
	CAN_Initializer[0u].canInstance = &CAN_Instance[0u];
	CAN_Initializer[0u].intcInstance = &CAN_INTR_Instance;
	CAN_Initializer[0u].canDeviceID = CAN0_DEVICE_ID;
	CAN_Initializer[0u].canIntrID = CAN0_INTR_VEC_ID;
	CAN_Initializer[0u].canIntrPriority = CAN0_INTR_PRIORITY;
	CAN_Initializer[0u].canBaudConfig.prescalar = CAN0_BRPR_BAUD_PRESCALAR;
	CAN_Initializer[0u].canBaudConfig.syncJumpWidth = CAN0_BTR_SYNCJUMPWIDTH;
	CAN_Initializer[0u].canBaudConfig.secondTimeSegment = CAN0_BTR_SECOND_TIMESEGMENT;
	CAN_Initializer[0u].canBaudConfig.firstTimeSegment = CAN0_BTR_FIRST_TIMESEGMENT;
	status = CAN_DeviceInit(&CAN_Initializer[0u]);
	if(status != XST_SUCCESS)
	{

	}
	CAN_Manager[0u].initDone = TRUE;
#endif

#ifdef XPAR_XCANPS_1_DEVICE_ID
	CAN_Initializer[1u].canInstance = &CAN_Instance[1u];
	CAN_Initializer[1u].intcInstance = &CAN_INTR_Instance;
	CAN_Initializer[1u].canDeviceID = CAN1_DEVICE_ID;
	CAN_Initializer[1u].canIntrID = CAN1_INTR_VEC_ID;
	CAN_Initializer[1u].canIntrPriority = CAN1_INTR_PRIORITY;
	CAN_Initializer[1u].canBaudConfig.prescalar = CAN1_BRPR_BAUD_PRESCALAR;
	CAN_Initializer[1u].canBaudConfig.syncJumpWidth = CAN1_BTR_SYNCJUMPWIDTH;
	CAN_Initializer[1u].canBaudConfig.secondTimeSegment = CAN1_BTR_SECOND_TIMESEGMENT;
	CAN_Initializer[1u].canBaudConfig.firstTimeSegment = CAN1_BTR_FIRST_TIMESEGMENT;
	status = CAN_DeviceInit(&CAN_Initializer[1u]);
	if(status != XST_SUCCESS)
	{

	}
	CAN_Manager[1u].initDone = TRUE;
#endif

	return status;
}

/* Private - CAN_DeviceInit
 * 初始化单个CAN设备 */
static int CAN_DeviceInit(XCanPs_Initilizer* canxInit)
{
	int Status;
	XCanPs *CanInstPtr = canxInit->canInstance;
	u32 CanDeviceId = canxInit->canDeviceID;
	XCanPs_Config *ConfigPtr;

	/*
	 * Initialize the Can device.
	 */
	ConfigPtr = XCanPs_LookupConfig(CanDeviceId);
	if (ConfigPtr == NULL)
	{
		return XST_FAILURE;
	}
	XCanPs_CfgInitialize(CanInstPtr, ConfigPtr, ConfigPtr->BaseAddr);

	/*
	 * Run self-test on the device, which verifies basic sanity of the
	 * device and the driver.
	 */
	Status = XCanPs_SelfTest(CanInstPtr);
	if (Status != XST_SUCCESS)
	{
		return XST_FAILURE;
	}

	/*
	 * Configure CAN device.
	 */
	CAN_BaudConfig(CanInstPtr);

	/*
	 * Set interrupt handlers.
	 */
	XCanPs_SetHandler(CanInstPtr, XCANPS_HANDLER_SEND,
			(void *)CAN_SendHandler, (void *)CanInstPtr);
	XCanPs_SetHandler(CanInstPtr, XCANPS_HANDLER_RECV,
			(void *)CAN_RecvHandler, (void *)CanInstPtr);
	XCanPs_SetHandler(CanInstPtr, XCANPS_HANDLER_ERROR,
			(void *)CAN_ErrorHandler, (void *)CanInstPtr);
	XCanPs_SetHandler(CanInstPtr, XCANPS_HANDLER_EVENT,
			(void *)CAN_EventHandler, (void *)CanInstPtr);

	/*
	 * Connect to the interrupt controller.
	 */
	Status = CAN_SetupInterruptSystem(canxInit);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	/*
	 * Enable all interrupts in CAN device.
	 */
	XCanPs_IntrEnable(CanInstPtr, XCANPS_IXR_ALL);

	/*
	 * Enter Normal Mode.
	 */
	XCanPs_EnterMode(CanInstPtr, XCANPS_MODE_NORMAL);
	while(XCanPs_GetMode(CanInstPtr) != XCANPS_MODE_NORMAL);

	return XST_SUCCESS;
}


/* Private - CAN_BaudConfig
 * 初始化一个CAN设备的波特率配置 */
static void CAN_BaudConfig(XCanPs * InstancePtr)
{
	XCanPs_Initilizer* canxInit;
	if(InstancePtr == &CAN_Instance[0u])
	{
		canxInit = &CAN_Initializer[0u];
	}
	else if(InstancePtr == &CAN_Instance[1u])
	{
		canxInit = &CAN_Initializer[1u];
	}
	u32 prescalar = canxInit->canBaudConfig.prescalar;
	u32 syncJumpWidth = canxInit->canBaudConfig.syncJumpWidth;
	u32 secondTimeSegment = canxInit->canBaudConfig.secondTimeSegment;
	u32 firstTimeSegment = canxInit->canBaudConfig.firstTimeSegment;

	XCanPs_EnterMode(InstancePtr, XCANPS_MODE_CONFIG);
	while(XCanPs_GetMode(InstancePtr) != XCANPS_MODE_CONFIG);

	XCanPs_SetBaudRatePrescaler(InstancePtr, prescalar);
	XCanPs_SetBitTiming(InstancePtr,
			syncJumpWidth,
			secondTimeSegment,
			firstTimeSegment);
}


/* Private - CAN_SetupInterruptSystem
 * 建立一个CAN设备的中断处理系统 */
static int CAN_SetupInterruptSystem(XCanPs_Initilizer* canxInit)
{
	int Status;
	XScuGic *IntcInstancePtr = canxInit->intcInstance;
	XCanPs *CanInstancePtr = canxInit->canInstance;
	u16 CanIntrId = canxInit->canIntrID;

#ifndef CANXPS_USE_OUTER_INTR_INIT
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
	Status = XScuGic_CfgInitialize(IntcInstancePtr, IntcConfig,
					IntcConfig->CpuBaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	/*
	 * Connect the interrupt controller interrupt handler to the hardware
	 * interrupt handling logic in the processor.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
				(Xil_ExceptionHandler)XScuGic_InterruptHandler,
				IntcInstancePtr);
#endif

	/*
	 * Connect the device driver handler that will be called when an
	 * interrupt for the device occurs, the handler defined above performs
	 * the specific interrupt processing for the device.
	 */
	Status = XScuGic_Connect(IntcInstancePtr, CanIntrId,
				(Xil_InterruptHandler)XCanPs_IntrHandler,
				(void *)CanInstancePtr);
	if (Status != XST_SUCCESS) {
		return Status;
	}

	/*
	 * Enable the interrupt for the CAN device.
	 */
	XScuGic_Enable(IntcInstancePtr, CanIntrId);

	/* Set CANx Intr Priority */
	u8 priority;
	u8 trigger;
	XScuGic_GetPriorityTriggerType(IntcInstancePtr, CanIntrId, &priority, &trigger);
	priority = canxInit->canIntrPriority;
	XScuGic_SetPriorityTriggerType(IntcInstancePtr, CanIntrId, priority, trigger);

#ifndef CANXPS_USE_OUTER_INTR_INIT
	/*
	 * Enable interrupts in the Processor.
	 */
	Xil_ExceptionEnable();
#endif

	return XST_SUCCESS;
}


/* Private - CAN_FromHandlerToID
 * 根据CAN实例的指针, 返回设备ID, 用于检查CAN实例指针是否有效 */
static u32 CAN_FromHandlerToID(XCanPs *CanPtr)
{
	if(CanPtr == &CAN_Instance[0u])
	{
		return 0u;
	}
	else if(CanPtr == &CAN_Instance[1u])
	{
		return 1u;
	}
	else
	{
		while(1)
		{
			xil_printf("\r\nError: Invalid CAN Handler\r\n");
			sleep(1);
		}
	}
}


/* API - CAN_SendFrame
 * 给定CAN设备ID和报文指针，直接发送报文 */
int CAN_SendFrame(const u32 canDeviceId, const CAN_MessageType* msg)
{
	int Status;
	CAN_ASSERT_DEVICE_ID_VALID(canDeviceId);

	XCanPs * CanPtr = &CAN_Instance[canDeviceId];

	/*
	 * Wait until the TX FIFO is not full and send the frame.
	 */
	while (XCanPs_IsTxFifoFull(CanPtr) == TRUE);

	Status = XCanPs_Send(CanPtr, (u32*)msg);
	if (Status != XST_SUCCESS)
	{
		CAN_Manager[canDeviceId].fault.sendErrCnt++;
	}
	else
	{
		CAN_Manager[canDeviceId].stat.sendCnt++;
	}
	return XST_SUCCESS;
}


/* API - CANx_InsertFrame
 * 在外部程序生成CAN报文，向CAN驱动传递【报文指针】
 * CAN驱动通过【报文指针】将报文内容复制到发送报文队列 */
int CAN_InsertTxFrame(const u32 canDeviceId, const CAN_MessageType* msg)
{
	CAN_ASSERT_DEVICE_ID_VALID(canDeviceId);

	XCanPs* CanPtr = &CAN_Instance[canDeviceId];

	u32 * queueLen = &CAN_Manager[canDeviceId].tx.queueLen;

	if(*queueLen < CAN_MESSAGE_MAX_NUM - 1u)
	{
		u32 * index = &CAN_Manager[canDeviceId].tx.writeIndex;
		memcpy( CAN_DESTINY 	&CAN_Manager[canDeviceId].tx.msg[*index],
				CAN_SOURCE 		msg,
								sizeof(CAN_MessageType));

		CAN_Manager[canDeviceId].tx.msg[*index].valid = TRUE;

		//printf("Insert index %d, ID %X\r\n", *index, msg->msgId.field.msgId);
		(*index) = (*index)+1 >= CAN_MESSAGE_MAX_NUM ? 0 : (*index)+1;
		(*queueLen)++;
		return XST_SUCCESS;
	}
	else
	{
		return XST_FAILURE;
	}
}


/* API - CAN_FillTxSendFrame
 * 从指定CAN设备的发送报文队列寻找可用项，并返回【报文指针】
 * 外部程序通过【报文指针】直接填写指定的报文
 * 但漏填某些项目会导致意外情况
 * */
CAN_MessageType* CAN_GetTxSendFrame(const u32 canDeviceId)
{
	CAN_ASSERT_DEVICE_ID_VALID(canDeviceId);
	CAN_MessageType* apiReturn = NULL;
	XCanPs* CanPtr = &CAN_Instance[canDeviceId];
	(void)CanPtr;
	u32* queueLen = &CAN_Manager[canDeviceId].tx.queueLen;
	if(*queueLen < CAN_MESSAGE_MAX_NUM - 1u)
	{
		u32 * index = &CAN_Manager[canDeviceId].tx.writeIndex;
		apiReturn = &CAN_Manager[canDeviceId].tx.msg[*index];
		(*index) = (*index)+1 >= CAN_MESSAGE_MAX_NUM ? 0 : (*index)+1;
		(*queueLen)++;
	}
	return apiReturn;
}


/* API - CAN_StartTransmitTxFrame
 * 完成CAN发送报文队列填表后，调用此函数发送第一帧报文
 * 当报文发送完成后，触发发送中断，自动发送下一帧报文
 */
int CAN_StartTransmitTxFrame(const u32 canDeviceId)
{
	CAN_ASSERT_DEVICE_ID_VALID(canDeviceId);
	int Status;
	XCanPs* CanPtr = &CAN_Instance[canDeviceId];
	u32 * queueLen = &CAN_Manager[canDeviceId].tx.queueLen;
	u32 * index;
	CAN_MessageType* msg;

	while(*queueLen > 0)
	{
		index = &CAN_Manager[canDeviceId].tx.readIndex;
		msg = &CAN_Manager[canDeviceId].tx.msg[*index];
		//printf("Send index %d, ID %X\r\n", *index, msg->msgId.field.msgId);

		/* After insert one message, set this message as valid
		 * only allow send valid messages */
		if(msg->valid != CAN_DATA_VALID)
		{
			return XST_FAILURE;
		}
		Status = XCanPs_Send(CanPtr, (u32*)msg);
		if (Status != XST_SUCCESS)
		{
			CAN_Manager[canDeviceId].stat.sendFifoFullCnt++;
			return XST_FAILURE;
		}
		else
		{
			CAN_Manager[canDeviceId].stat.sendCnt++;
			msg->valid = CAN_DATA_INVALID;
			(*queueLen)--;
			(*index) = (*index)+1 >= CAN_MESSAGE_MAX_NUM ? 0 : (*index)+1;
		}
	}
	return XST_SUCCESS;
}


/* API - CANx_ReadRxFrame
 * 给定CAN设备ID和【报文指针】，CAN驱动从接收队列中寻找有效的报文
 * 找到后，CAN驱动将报文内容填写入【报文指针】中 */
int CAN_ReadRxFrame(const u32 canDeviceId, CAN_MessageType* msg)
{
	CAN_ASSERT_DEVICE_ID_VALID(canDeviceId);
	CAN_MessageType* msgPtr;
	XCanPs* CanPtr =  &CAN_Instance[canDeviceId];
	(void)CanPtr;

	u32* queueLen = &CAN_Manager[canDeviceId].rx.queueLen;

	if(*queueLen > 0)
	{
		u32 * index = &CAN_Manager[canDeviceId].rx.readIndex;
		msgPtr = &CAN_Manager[canDeviceId].rx.msg[*index];
		/* When CANx Receive one message, this flag will set as valid
		 * only read valid data
		 * */
		if(msgPtr->valid != CAN_DATA_VALID)
		{
			return XST_FAILURE;
		}
		memcpy(	CAN_DESTINY		msg,
				CAN_SOURCE		msgPtr,
								sizeof(CAN_MessageType));
		/* After read one message, you should set this message as invalid */
		msgPtr->valid = CAN_DATA_INVALID;
		(*index) = (*index)+1 >= CAN_MESSAGE_MAX_NUM ? 0 : (*index)+1;
		(*queueLen)--;
		return XST_SUCCESS;
	}
	else
	{
		return XST_FAILURE;
	}
}


/* API - CAN_GetRxFrame
 * 给定CAN设备ID，CAN驱动从接收队列中寻找有效的报文
 * 找到后，CAN驱动返回找到的【报文指针】 */
CAN_MessageType* CAN_GetRxFrame(const u32 canDeviceId)
{
	CAN_ASSERT_DEVICE_ID_VALID(canDeviceId);
	CAN_MessageType* msgPtr;
	u32* queueLen = &CAN_Manager[canDeviceId].rx.queueLen;

	if(*queueLen > 0)
	{
		u32 * index = &CAN_Manager[canDeviceId].rx.readIndex;
		msgPtr = &CAN_Manager[canDeviceId].rx.msg[*index];
		/* When CANx Receive one message, this flag will set as valid
		 * only read valid data
		 * */
		if(msgPtr->valid != CAN_DATA_VALID)
		{
			return NULL;
		}

		/* After read one message, you should set this message as invalid */
		msgPtr->valid = CAN_DATA_INVALID;
		(*index) = (*index)+1 >= CAN_MESSAGE_MAX_NUM ? 0 : (*index)+1;
		(*queueLen)--;
		return msgPtr;
	}
	else
	{
		return NULL;
	}
}

/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/
/* Private - CANx_SendHandler
 * 一帧CAN报文发送完成后触发
 * 向CAN FIFO中填入下一帧待发送的报文
 * 若没有剩余报文，则退出发送
 * */
static void CAN_SendHandler(void *CallBackRef)
{
#if 1
	XCanPs *CanPtr = (XCanPs *)CallBackRef;
	u32 canDeviceId = CAN_FromHandlerToID(CanPtr);
	CAN_MessageType* msg;

	/* 获取队列长度 */
	u32 * queueLen = &CAN_Manager[canDeviceId].tx.queueLen;

	/* decide if need to send next message */
	if(*queueLen > 0)
	{
		u32 * index =  &CAN_Manager[canDeviceId].tx.readIndex;
		msg = &CAN_Manager[canDeviceId].tx.msg[*index];
		//printf("Send index %d, ID %X\r\n", *index, msg->msgId.field.msgId);
		int Status = XCanPs_Send(CanPtr, (u32*)msg);
		if (Status != XST_SUCCESS)
		{
			/*正常情况下发完一帧，FIFO必然非满*/
			CAN_Manager[canDeviceId].fault.sendErrCnt++;
			CAN_Manager[canDeviceId].stat.sendFifoFullCnt++;
		}
		else
		{
			CAN_Manager[canDeviceId].stat.sendCnt++;
			(*index) = (*index)+1 >= CAN_MESSAGE_MAX_NUM ? 0 : (*index)+1;
			(*queueLen)--;
		}
	}
#endif
}


/* Private - CAN_RecvHandler
 * CAN接收一帧报文后触发的中断处理函数
 *  */
static void CAN_RecvHandler(void *CallBackRef)
{
	int Status;
	u32 *index;
	u32 *queueLen;
	u32 *msg;
	XCanPs *CanPtr = (XCanPs *)CallBackRef;
	u32 canDeviceID = CAN_FromHandlerToID(CanPtr);

	index = &CAN_Manager[canDeviceID].rx.writeIndex;
	queueLen = &CAN_Manager[canDeviceID].rx.queueLen;
	msg = (u32*)&CAN_Manager[canDeviceID].rx.msg[*index];

	Status = XCanPs_Recv(CanPtr, msg);
	if (Status != XST_SUCCESS)
	{
		CAN_Manager[canDeviceID].fault.recvErrCnt++;
		return;
	}
	else
	{
		CAN_Manager[canDeviceID].stat.recvCnt++;
		CAN_Manager[canDeviceID].rx.msg[*index].valid = CAN_DATA_VALID;
		(*index) = (*index)+1 >= CAN_MESSAGE_MAX_NUM ? 0 : (*index)+1;
		(*queueLen)++;
	}
}


/* Private - CAN_ErrorHandler
 * 故障中断事件处理函数 */
static void CAN_ErrorHandler(void *CallBackRef, u32 ErrorMask)
{
	if(ErrorMask & XCANPS_ESR_ACKER_MASK) {
		/*
		 * ACK Error handling code should be put here.
		 */
		CAN_Manager[0u].fault.ackErrCnt++;
	}

	if(ErrorMask & XCANPS_ESR_BERR_MASK) {
		/*
		 * Bit Error handling code should be put here.
		 */
		CAN_Manager[0u].fault.bitErrCnt++;
	}

	if(ErrorMask & XCANPS_ESR_STER_MASK) {
		/*
		 * Stuff Error handling code should be put here.
		 */
		CAN_Manager[0u].fault.stuffErrCnt++;
	}

	if(ErrorMask & XCANPS_ESR_FMER_MASK) {
		/*
		 * Form Error handling code should be put here.
		 */
		CAN_Manager[0u].fault.formErrCnt++;
	}

	if(ErrorMask & XCANPS_ESR_CRCER_MASK) {
		/*
		 * CRC Error handling code should be put here.
		 */
		CAN_Manager[0u].fault.crcErrCnt++;
	}
}


/* Private - CAN_EventHandler
 * 特殊中断事件处理函数 */
static void CAN_EventHandler(void *CallBackRef, u32 IntrMask)
{
	XCanPs *CanPtr = (XCanPs *)CallBackRef;

	if (IntrMask & XCANPS_IXR_BSOFF_MASK)
	{
		/*
		 * Entering Bus off status interrupt requires
		 * the CAN device be reset and reconfigured.
		 */
		CAN_Manager[0u].fault.busOffErrCnt++;
		XCanPs_Reset(CanPtr);
		CAN_BaudConfig(CanPtr);
		return;
	}

	if(IntrMask & XCANPS_IXR_RXOFLW_MASK)
	{
		/*
		 * Code to handle RX FIFO Overflow Interrupt should be put here.
		 */
		CAN_Manager[0u].fault.rxovfErrCnt++;
	}

	if(IntrMask & XCANPS_IXR_RXUFLW_MASK)
	{
		/*
		 * Code to handle RX FIFO Underflow Interrupt
		 * should be put here.
		 */
	}

	if(IntrMask & XCANPS_IXR_TXBFLL_MASK)
	{
		/*
		 * Code to handle TX High Priority Buffer Full
		 * Interrupt should be put here.
		 */
	}

	if(IntrMask & XCANPS_IXR_TXFLL_MASK)
	{
		/*
		 * Code to handle TX FIFO Full Interrupt should be put here.
		 */
	}

	if (IntrMask & XCANPS_IXR_WKUP_MASK)
	{
		/*
		 * Code to handle Wake up from sleep mode Interrupt
		 * should be put here.
		 */
	}

	if (IntrMask & XCANPS_IXR_SLP_MASK)
	{
		/*
		 * Code to handle Enter sleep mode Interrupt should be put here.
		 */
	}

	if (IntrMask & XCANPS_IXR_ARBLST_MASK)
	{
		/*
		 * Code to handle Lost bus arbitration Interrupt
		 * should be put here.
		 */
	}
}


/*****************************************************************************************/
/*****************************************************************************************/
/*****************************************************************************************/

/* API - CAN_Demo
 * CAN 驱动演示程序*/
void CAN_Demo(void)
{
	int i;
	int Status;
	CAN_MessageType msg;
	CAN_MessageType* txMsg;

	while(1)
	{
		Status = CAN_ReadRxFrame(0, &msg);
		if(Status != XST_SUCCESS)
		{
			break;
		}
		else
		{
			//printf("Recv ID %X ", msg.msgId.field.msgId);
			for(i=0; i<msg.msgDlc.field.len; i++)
			{
				//printf("%02X ", msg.data.data[i]);
			}
			//printf("\r\n");

			msg.msgDlc.field.len = 8;
			msg.data.data32[0] = 0x12345678;
			msg.data.data32[1]++;
			msg.valid = CAN_DATA_VALID;
			CAN_InsertTxFrame(0, &msg);
#if 0
			msg.msgId.field.msgId = 1;
			msg.msgId.field.extMsgEnable = 0;
			msg.msgId.field.extMsgId = 0;
			msg.msgId.field.remoteTxRequest = 0;
			msg.msgId.field.subRemodeTxRequest = 0;
			msg.data.buff[1]++;
			CAN_InsertTxFrame(0, &msg);
			msg.msgId.field.msgId = 2;
			CAN_InsertTxFrame(0, &msg);
			msg.msgId.field.msgId = 3;
			CAN_InsertTxFrame(0, &msg);
			msg.msgId.field.msgId = 4;
			CAN_InsertTxFrame(0, &msg);
			msg.msgId.field.msgId = 5;
			CAN_InsertTxFrame(0, &msg);
			msg.msgId.field.msgId = 6;
			CAN_InsertTxFrame(0, &msg);
			msg.msgId.field.msgId = 7;
			CAN_InsertTxFrame(0, &msg);
			msg.msgId.field.msgId = 8;
			CAN_InsertTxFrame(0, &msg);
			msg.msgId.field.msgId = 9;
			CAN_InsertTxFrame(0, &msg);
			msg.msgId.field.msgId = 10;
			CAN_InsertTxFrame(0, &msg);
			msg.msgId.field.msgId = 11;
			CAN_InsertTxFrame(0, &msg);
			msg.msgId.field.msgId = 12;
			CAN_InsertTxFrame(0, &msg);
			msg.msgId.field.msgId = 13;
			CAN_InsertTxFrame(0, &msg);
#endif
			txMsg = CAN_GetTxSendFrame(0);
			txMsg->valid = CAN_DATA_VALID;
			txMsg->msgId.field.msgId = 0x666;
			txMsg->msgId.field.extMsgEnable = 1;
			txMsg->msgId.field.extMsgId = 0x777;
			txMsg->msgId.field.remoteTxRequest = 0;
			txMsg->msgId.field.subRemodeTxRequest = 0;
			txMsg->msgDlc.field.len = 8;
			txMsg->data.data32[0] = 0xAABBCCDD;
			txMsg->data.data32[1] = 0x99887766;

			txMsg = CAN_GetTxSendFrame(0);
			txMsg->valid = CAN_DATA_VALID;
			txMsg->msgId.field.msgId = 0x555;
			txMsg->msgId.field.extMsgEnable = 1;
			txMsg->msgId.field.extMsgId = 0x444;
			txMsg->msgId.field.remoteTxRequest = 0;
			txMsg->msgId.field.subRemodeTxRequest = 0;
			txMsg->msgDlc.field.len = 8;
			txMsg->data.data32[0] = 0x11223344;
			txMsg->data.data32[1] = 0x55667788;

			CAN_StartTransmitTxFrame(0);
		}
	}

#if 0
	/* 测试防越界断言函数 */
	CAN_GetRxFrame(2);
#endif
}

#ifndef ZYNQ_DISABLE_DEMO
int main(void)
{
	int i;
	CAN_Init();
	while(1)
	{
		CAN_Demo();
		for(i=1000000;i>0;i--);
	}
}
#endif
