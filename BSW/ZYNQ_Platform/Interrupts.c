/*
 * Interrupts.c
 *
 *  Created on: 2024Äê1ÔÂ20ÈÕ
 *      Author: Administrator
 */

#include "Interrupts.h"

XScuGic INTR_XScuGic_Instance; /* The Instance of the Interrupt Controller Driver */

static XGpio GpioForInternalInterrupt; /* The Instance of the GPIO Driver */

#ifdef ZYNQ_AFESIM_USE_TI_AFE
static unsigned GlobalIntrMask_Ti_Spi_If;
#endif

/*
 * Declaration of GPIO Interrupt (TI_Spi_If StringUpdate as trigger source)
 * */
int INTR_GPIO_SPI_Receive_Init(XScuGic * intrInstance);
int GpioSetupIntrSystem(XScuGic *IntrInstancePtr, XGpio *GpioInstancePtr, u16 IntrDeviceId, u16 IntrId, unsigned IntrMask,
		u8 priority, u8 trigger);
#ifdef ZYNQ_AFESIM_USE_TI_AFE
void Intr_Ti_Spi_If_StringUpdate_IRQHandler(void *CallbackRef);
extern void AfeSim_BQ79600_IrqCallBack(void);
#endif


#ifndef ZYNQ_USE_FREERTOS
int Intr_ScuGicInit(void)
{
	int status = XST_SUCCESS;
	XScuGic_Config *IntcConfig;
	u16 IntrDeviceId = INTR_DEVICE_ID;
	XScuGic * IntrInstancePtr = &Global_XScuGic_Instance;
	/*
	 * Initialize the interrupt controller driver so that it is ready to
	 * use.
	 */
	IntcConfig = XScuGic_LookupConfig(IntrDeviceId);
	if (NULL == IntcConfig)
	{
		printf("Fail XScuGic_LookupConfig\r\n");
		return XST_FAILURE;
	}

	status = XScuGic_CfgInitialize(IntrInstancePtr,
			IntcConfig,
			IntcConfig->CpuBaseAddress);
	if (status != XST_SUCCESS)
	{
		printf("Fail XScuGic_CfgInitialize\r\n");
		return XST_FAILURE;
	}
	return status;
}

int Intr_ExceptionInit(void)
{
	XScuGic * IntrInstancePtr = &Global_XScuGic_Instance;
	/*
	 * Initialize the exception table and register the interrupt
	 * controller handler with the exception table
	 */
	Xil_ExceptionInit();
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XScuGic_InterruptHandler,
			IntrInstancePtr);
	/* Enable non-critical exceptions */
	Xil_ExceptionEnable();
	return XST_SUCCESS;
}
#endif


/* Intr_Init
 * Initialize All GPIO Interrupts */
int INTR_Init(void)
{
	int status = XST_SUCCESS;
	XScuGic * IntrInstancePtr = &Global_XScuGic_Instance;

#ifndef ZYNQ_USE_FREERTOS
	Intr_ScuGicInit();
#endif
	/*
	 * Initialize all peripheral devices
	 * */
	INTR_GPIO_SPI_Receive_Init(IntrInstancePtr);

#ifndef ZYNQ_USE_FREERTOS
	Intr_ExceptionInit();
#endif
	return status;
}


/* Gpio_Intr_SPI_Receive_Init
 * Initialize TI AFE SPI receive Interrupt */
int INTR_GPIO_SPI_Receive_Init(XScuGic * intrInstance)
{
	int Status;
	/* Initialize GPIO device for interrupt */
	Status = XGpio_Initialize(&GpioForInternalInterrupt,
			INTR_GPIO_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		printf("Fail XGpio_Initialize\r\n");
		return XST_FAILURE;
	}

	Status = GpioSetupIntrSystem(intrInstance,
			&GpioForInternalInterrupt,
			INTR_DEVICE_ID,
			INTR_GPIO_TI_SPI_IF_STRING_UPDATE_INTR_ID,
			INTR_GPIO_TI_SPI_IF_STRING_UPDATE_CHANNEL,
			INTR_TI_SPI_IF_STRING_UPDATE_PRIORITY,
			INTR_TI_SPI_IF_STRING_UPDATE_TRIGGER_TYPE);

	if (Status != XST_SUCCESS) {
		printf("\r\nSet Interrupt Fail 2\r\n");
		return XST_FAILURE;
	}
	printf("\r\nSet Interrupt Success\r\n");
	return XST_SUCCESS;
}


/**/
int GpioSetupIntrSystem(XScuGic *IntrInstancePtr, XGpio *GpioInstancePtr, u16 IntrDeviceId, u16 IntrId, unsigned IntrMask,
		u8 priority, u8 trigger)
{
	int Result;

#ifdef ZYNQ_AFESIM_USE_TI_AFE
	GlobalIntrMask_Ti_Spi_If = IntrMask;
#endif

	XScuGic_SetPriorityTriggerType(IntrInstancePtr,
			IntrId,
			priority,
			trigger);
	/*
	 * Connect the interrupt handler that will be called when an
	 * interrupt occurs for the device.
	 */
#ifdef ZYNQ_AFESIM_USE_TI_AFE
	Result = XScuGic_Connect(IntrInstancePtr,
			IntrId,
			(Xil_ExceptionHandler)Intr_Ti_Spi_If_StringUpdate_IRQHandler,
			GpioInstancePtr);
#endif

	if (Result != XST_SUCCESS) {
		return Result;
	}

	/* Enable the interrupt for the GPIO device.*/
	XScuGic_Enable(IntrInstancePtr, IntrId);

	/*
	 * Enable the GPIO channel interrupts so that push button can be
	 * detected and enable interrupts for the GPIO device
	 */
	XGpio_SetDataDirection(GpioInstancePtr, IntrMask, 1);
	XGpio_InterruptEnable(GpioInstancePtr, IntrMask);
	XGpio_InterruptGlobalEnable(GpioInstancePtr);

	return XST_SUCCESS;
}

#ifdef ZYNQ_AFESIM_USE_TI_AFE
void Intr_Ti_Spi_If_StringUpdate_IRQHandler(void *CallbackRef)
{
	XGpio *GpioPtr = (XGpio *)CallbackRef;
	XGpio_InterruptClear(GpioPtr, GlobalIntrMask_Ti_Spi_If);
	AfeSim_BQ79600_IrqCallBack();
}
#endif

