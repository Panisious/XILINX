/*
 * Comm_Control.c
 *
 *  Created on: 2024Äê1ÔÂ20ÈÕ
 *      Author: Administrator
 */

#include <Comm_Control.h>
#include "Comm_Decode.h"
#include "string.h"

#ifdef UARTXPS_USE_OUTER_BUFF
UART_BufferType UART0_RecvBuffer;
UART_BufferType UART0_SendBuffer;
UART_BufferType UART1_RecvBuffer;
UART_BufferType UART1_SendBuffer;
#endif

CommManagerType Coman;

u32 total_recv_bytes = 0;


static void Comm_ManagerInit(void);
static int Comm_CanReceiveHandle(CAN_MessageType* rxMsg);

void Comm_Init(void)
{
#ifdef ZYNQ_USE_7020
	UART_Init();
#endif
	CAN_Init();
	Comm_ManagerInit();
}

static void Comm_ManagerInit(void)
{
	CAN_MessageType* heartBeat = &Coman.can.heartBeat;
	heartBeat->msgId.field.msgId = 0x7FF;
	heartBeat->msgId.field.extMsgEnable = 0;
	heartBeat->msgDlc.field.len = 4;
	heartBeat->data.data32[0] = 0;
	heartBeat->valid = 1;
}

static int Comm_CanReceiveHandle(CAN_MessageType* rxMsg)
{
	int status = XST_SUCCESS;
	//uint32 msgId = rxMsg->msgId.field.msgId;
	//uint32
	AfeCalib_CanMsgDecode(rxMsg);
	return status;
}


static void Comm_CanTransmitHeartBeat(uint8 canDeviceId)
{
	CAN_MessageType* txMsg = &Coman.can.heartBeat;
	txMsg->data.data32[0]++;
	CAN_InsertTxFrame(canDeviceId, txMsg);
}

void Comm_CanTransmitHandle(CAN_MessageType* txMsg)
{
	static uint8 heartBeat = 0;

	if(++heartBeat == 100)
	{
		heartBeat = 0;
		Comm_CanTransmitHeartBeat(COMM_CAN0_ID);
	}
	CAN_StartTransmitTxFrame(COMM_CAN0_ID);
}


void Comm_CanProcess(void)
{
	int status;
	CAN_MessageType rxMsg;
	CAN_MessageType txMsg;
	while(1)
	{
		status = CAN_ReadRxFrame(0, &rxMsg);
		if(status != XST_SUCCESS)
		{
			break;
		}
		status = Comm_CanReceiveHandle(&rxMsg);
	}
	Comm_CanTransmitHandle(&txMsg);
}


void Comm_NormalRun(void)
{
	//CAN_Demo();
	//UART_Demo();
	Comm_CanProcess();
}

