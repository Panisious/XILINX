/*
 * AfeSim_ZYNQ_Platform_HAL_CPU0.h
 *
 *  Created on: 2024Äê1ÔÂ21ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_ZYNQ_PLATFORM_CPU0_ZYNQ_PLATFORM_HAL_CPU0_H_
#define SRC_ZYNQ_PLATFORM_CPU0_ZYNQ_PLATFORM_HAL_CPU0_H_

#include "Project_Define.h"

#include "ZYNQ_BasicType.h"

#ifdef ZYNQ_USE_FREERTOS
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"
#endif

#include "Interrupts.h"
#include "Uart.h"
#include "CAN.h"
#include "CRC16.h"

#define HAL_COMM_RX_BUFF	UART_RecvBuffer
#define HAL_COMM_TX_BUFF	UART_TranBuffer

#endif /* SRC_ZYNQ_PLATFORM_CPU0_ZYNQ_PLATFORM_HAL_CPU0_H_ */
