// ==============================================================
// File generated on Fri Feb 23 00:36:57 +0800 2024
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:38:27 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef __linux__

#include "xstatus.h"
#include "xparameters.h"
#include "xti_spi_if.h"

extern XTi_spi_if_Config XTi_spi_if_ConfigTable[];

XTi_spi_if_Config *XTi_spi_if_LookupConfig(u16 DeviceId) {
	XTi_spi_if_Config *ConfigPtr = NULL;

	int Index;

	for (Index = 0; Index < XPAR_XTI_SPI_IF_NUM_INSTANCES; Index++) {
		if (XTi_spi_if_ConfigTable[Index].DeviceId == DeviceId) {
			ConfigPtr = &XTi_spi_if_ConfigTable[Index];
			break;
		}
	}

	return ConfigPtr;
}

int XTi_spi_if_Initialize(XTi_spi_if *InstancePtr, u16 DeviceId) {
	XTi_spi_if_Config *ConfigPtr;

	Xil_AssertNonvoid(InstancePtr != NULL);

	ConfigPtr = XTi_spi_if_LookupConfig(DeviceId);
	if (ConfigPtr == NULL) {
		InstancePtr->IsReady = 0;
		return (XST_DEVICE_NOT_FOUND);
	}

	return XTi_spi_if_CfgInitialize(InstancePtr, ConfigPtr);
}

#endif

