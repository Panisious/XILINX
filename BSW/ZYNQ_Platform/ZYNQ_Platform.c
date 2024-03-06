/*
 * AfeSim_ZYNQ_Platform.c
 *
 *  Created on: 2024Äê1ÔÂ7ÈÕ
 *      Author: Administrator
 */

#include <ZYNQ_Platform.h>

typedef enum
{
	BQ79600_MISO_FF   = 0x00,
	BQ79600_MISO_DATA = 0x01
}AfeSim_BQ79600_MisoType;

static XTi_spi_if Spi_inst;
static XTi_spi_if* AfeSim_ZYNQ_SpiSlaveDevice = &Spi_inst;

void ZYNQ_Platform_Init(void)
{
	//Uart_Init();
	INTR_Init();
}

/* BQ79600 Init */
void ZYNQ_BQ79600_Init(void)
{
	XTi_spi_if_Initialize(AfeSim_ZYNQ_SpiSlaveDevice, XPAR_XTI_SPI_IF_0_DEVICE_ID);

	XTi_spi_if_EnableAutoRestart(AfeSim_ZYNQ_SpiSlaveDevice);

	XTi_spi_if_Start(AfeSim_ZYNQ_SpiSlaveDevice);
}

/* MOSI Control */
uint8 ZYNQ_BQ79600_MosiGetDataStatus(void)
{
	return XTi_spi_if_Get_StringLen_vld(AfeSim_ZYNQ_SpiSlaveDevice);
}

uint8 ZYNQ_BQ79600_MosiDataRead(uint8 *data)
{
	uint8 length = XTi_spi_if_Get_StringLen(AfeSim_ZYNQ_SpiSlaveDevice);
	XTi_spi_if_Read_MOSI_data_Bytes(AfeSim_ZYNQ_SpiSlaveDevice, 0, (sint8*)data, length);
	return length;
}

/* MISO Control */
void ZYNQ_BQ79600_MisoDataWrite(uint8 *data, int length)
{
	XTi_spi_if_Write_MISO_data_Bytes(AfeSim_ZYNQ_SpiSlaveDevice, 0, (sint8*)data, length);
	XTi_spi_if_Set_FifoByteNum_PS(AfeSim_ZYNQ_SpiSlaveDevice, length);
}

void ZYNQ_BQ79600_MisoFF(void)
{
	XTi_spi_if_Set_MISO_Sel_i(AfeSim_ZYNQ_SpiSlaveDevice, BQ79600_MISO_FF);	// Let SpiSlaveInterface transmit FF
}

void ZYNQ_BQ79600_MisoData(void)
{
	XTi_spi_if_Set_MISO_Sel_i(AfeSim_ZYNQ_SpiSlaveDevice, BQ79600_MISO_DATA);	// Let SpiSlaveInterface transmit MISO data
}

/* RDY Control */
void ZYNQ_BQ79600_SetRdyPinHigh(void)
{
	XTi_spi_if_Set_Ctrl_PS(AfeSim_ZYNQ_SpiSlaveDevice, 1);   // Set RDY HIGH
	usleep(1);
	XTi_spi_if_Set_Ctrl_PS(AfeSim_ZYNQ_SpiSlaveDevice, 0);   // Clear Control
}

void ZYNQ_BQ79600_SetRdyPinLow(void)
{
	XTi_spi_if_Set_Ctrl_PS(AfeSim_ZYNQ_SpiSlaveDevice, 2);   // Set RDY LOW
	usleep(1);
	XTi_spi_if_Set_Ctrl_PS(AfeSim_ZYNQ_SpiSlaveDevice, 0);   // Clear Control
}

/* INH Control */
void ZYNQ_BQ79600_SetInhPinHigh(void)
{

}

void ZYNQ_BQ79600_SetInhPinLow(void)
{

}

