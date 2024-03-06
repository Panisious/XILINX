/*
 * Comm_Control.h
 *
 *  Created on: 2024Äê1ÔÂ20ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_COMM_CONTROL_COMM_CONTROL_H_
#define SRC_COMM_CONTROL_COMM_CONTROL_H_

#include "ZYNQ_Platform_HAL.h"

#define COMM_CAN0_ID	0
#define COMM_CAN1_ID	1
#define COMM_UART0_ID	0
#define COMM_UART1_ID	1

typedef struct
{
	uint8 initDone;
	CAN_MessageType heartBeat;
}CANCommManagerType;

typedef struct
{

}UARTCommManagerType;

typedef struct
{
	CANCommManagerType can;
}CommManagerType;

extern CommManagerType Coman;

extern void Comm_Control_Receive_Callback(uint8 data);

extern void Comm_Control_Timeout_Callback(void);

extern void Comm_Init(void);

extern void Comm_NormalRun(void);

#endif /* SRC_COMM_CONTROL_COMM_CONTROL_H_ */
