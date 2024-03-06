/*
 * Interrupts.h
 *
 *  Created on: 2024Äê1ÔÂ20ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_ZYNQ_PLATFORM_INTERRUPTS_INTERRUPTS_H_
#define SRC_ZYNQ_PLATFORM_INTERRUPTS_INTERRUPTS_H_

#include "Project_Define.h"
#include "ZYNQ_BasicType.h"

#include "xparameters.h"
#include "xparameters_ps.h"
#include "xil_exception.h"
#include "xscugic.h"
#include "xil_printf.h"
#include "xgpio.h"

#ifdef ZYNQ_USE_FREERTOS
//#include "FreeRTOS.h"
#else

#endif

/* Interrupt Priority Table
 * 0
 * 1	XTi_Spi_If(use GPIO)
 * 2	XTi_Daisy_If(COMH or COML)
 * 3
 * 4
 * 5
 * 6
 * 7
 * 8
 * 9
 * 10
 * 11
 * 12
 * 13
 * 14
 * 15
 * 16	CAN0
 * 17	CAN1
 * 18	UART0
 * 19	UART1
 * 20
 * 21
 * 22
 * 23
 * 24
 * 25
 * 26
 * 27
 * 28
 * 29
 * 30
 * 31
 * */
#define INTR_GPIO_TRIGGER_TYPE_ACTIVE_HIGH				0x01
#define INTR_GPIO_TRIGGER_TYPE_RISING_EDGE				0x03

#define INTR_DEVICE_ID									XPAR_PS7_SCUGIC_0_DEVICE_ID

#define INTR_GPIO_DEVICE_ID								XPAR_GPIO_0_DEVICE_ID
#define INTR_GPIO_TI_SPI_IF_STRING_UPDATE_CHANNEL		1
#define INTR_GPIO_TI_SPI_IF_STRING_UPDATE_INTR_ID		XPAR_FABRIC_AXI_GPIO_0_IP2INTC_IRPT_INTR
#define INTR_TI_SPI_IF_STRING_UPDATE_PRIORITY			(1u << 3u)
#define INTR_TI_SPI_IF_STRING_UPDATE_TRIGGER_TYPE		INTR_GPIO_TRIGGER_TYPE_RISING_EDGE

#define INTR_UART0PS_INTR_ID							XPAR_XUARTPS_0_INTR
#define INTR_UART1PS_INTR_ID							XPAR_XUARTPS_1_INTR

#ifdef ZYNQ_USE_FREERTOS
	#ifndef Global_XScuGic_Instance
		extern XScuGic xInterruptController;
		#define Global_XScuGic_Instance		xInterruptController
	#endif
#else
	#ifndef Global_XScuGic_Instance
		extern XScuGic INTR_XScuGic_Instance;
		#define Global_XScuGic_Instance		INTR_XScuGic_Instance
	#endif
#endif

extern int INTR_Init(void);

#endif /* SRC_ZYNQ_PLATFORM_INTERRUPTS_INTERRUPTS_H_ */
