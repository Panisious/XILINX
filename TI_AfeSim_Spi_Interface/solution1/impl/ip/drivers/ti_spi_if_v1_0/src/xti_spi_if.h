// ==============================================================
// File generated on Fri Feb 23 00:36:57 +0800 2024
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:38:27 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XTI_SPI_IF_H
#define XTI_SPI_IF_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xti_spi_if_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Axilites_BaseAddress;
} XTi_spi_if_Config;
#endif

typedef struct {
    u32 Axilites_BaseAddress;
    u32 IsReady;
} XTi_spi_if;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XTi_spi_if_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XTi_spi_if_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XTi_spi_if_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XTi_spi_if_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XTi_spi_if_Initialize(XTi_spi_if *InstancePtr, u16 DeviceId);
XTi_spi_if_Config* XTi_spi_if_LookupConfig(u16 DeviceId);
int XTi_spi_if_CfgInitialize(XTi_spi_if *InstancePtr, XTi_spi_if_Config *ConfigPtr);
#else
int XTi_spi_if_Initialize(XTi_spi_if *InstancePtr, const char* InstanceName);
int XTi_spi_if_Release(XTi_spi_if *InstancePtr);
#endif

void XTi_spi_if_Start(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_IsDone(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_IsIdle(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_IsReady(XTi_spi_if *InstancePtr);
void XTi_spi_if_EnableAutoRestart(XTi_spi_if *InstancePtr);
void XTi_spi_if_DisableAutoRestart(XTi_spi_if *InstancePtr);

void XTi_spi_if_Set_Ctrl_PS(XTi_spi_if *InstancePtr, u32 Data);
u32 XTi_spi_if_Get_Ctrl_PS(XTi_spi_if *InstancePtr);
void XTi_spi_if_Set_FifoByteNum_PS(XTi_spi_if *InstancePtr, u32 Data);
u32 XTi_spi_if_Get_FifoByteNum_PS(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_StringUpdate(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_StringUpdate_vld(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_StringLen(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_StringLen_vld(XTi_spi_if *InstancePtr);
void XTi_spi_if_Set_MISO_Sel_i(XTi_spi_if *InstancePtr, u32 Data);
u32 XTi_spi_if_Get_MISO_Sel_i(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_MISO_Sel_o(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_MISO_Sel_o_vld(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_MOSI_data_BaseAddress(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_MOSI_data_HighAddress(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_MOSI_data_TotalBytes(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_MOSI_data_BitWidth(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_MOSI_data_Depth(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Write_MOSI_data_Words(XTi_spi_if *InstancePtr, int offset, int *data, int length);
u32 XTi_spi_if_Read_MOSI_data_Words(XTi_spi_if *InstancePtr, int offset, int *data, int length);
u32 XTi_spi_if_Write_MOSI_data_Bytes(XTi_spi_if *InstancePtr, int offset, char *data, int length);
u32 XTi_spi_if_Read_MOSI_data_Bytes(XTi_spi_if *InstancePtr, int offset, char *data, int length);
u32 XTi_spi_if_Get_MISO_data_BaseAddress(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_MISO_data_HighAddress(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_MISO_data_TotalBytes(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_MISO_data_BitWidth(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Get_MISO_data_Depth(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_Write_MISO_data_Words(XTi_spi_if *InstancePtr, int offset, int *data, int length);
u32 XTi_spi_if_Read_MISO_data_Words(XTi_spi_if *InstancePtr, int offset, int *data, int length);
u32 XTi_spi_if_Write_MISO_data_Bytes(XTi_spi_if *InstancePtr, int offset, char *data, int length);
u32 XTi_spi_if_Read_MISO_data_Bytes(XTi_spi_if *InstancePtr, int offset, char *data, int length);

void XTi_spi_if_InterruptGlobalEnable(XTi_spi_if *InstancePtr);
void XTi_spi_if_InterruptGlobalDisable(XTi_spi_if *InstancePtr);
void XTi_spi_if_InterruptEnable(XTi_spi_if *InstancePtr, u32 Mask);
void XTi_spi_if_InterruptDisable(XTi_spi_if *InstancePtr, u32 Mask);
void XTi_spi_if_InterruptClear(XTi_spi_if *InstancePtr, u32 Mask);
u32 XTi_spi_if_InterruptGetEnabled(XTi_spi_if *InstancePtr);
u32 XTi_spi_if_InterruptGetStatus(XTi_spi_if *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
