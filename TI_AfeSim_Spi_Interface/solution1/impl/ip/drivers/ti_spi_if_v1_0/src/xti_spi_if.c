// ==============================================================
// File generated on Fri Feb 23 00:36:57 +0800 2024
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:38:27 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
/***************************** Include Files *********************************/
#include "xti_spi_if.h"

/************************** Function Implementation *************************/
#ifndef __linux__
int XTi_spi_if_CfgInitialize(XTi_spi_if *InstancePtr, XTi_spi_if_Config *ConfigPtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(ConfigPtr != NULL);

    InstancePtr->Axilites_BaseAddress = ConfigPtr->Axilites_BaseAddress;
    InstancePtr->IsReady = XIL_COMPONENT_IS_READY;

    return XST_SUCCESS;
}
#endif

void XTi_spi_if_Start(XTi_spi_if *InstancePtr) {
    u32 Data;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_AP_CTRL) & 0x80;
    XTi_spi_if_WriteReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_AP_CTRL, Data | 0x01);
}

u32 XTi_spi_if_IsDone(XTi_spi_if *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_AP_CTRL);
    return (Data >> 1) & 0x1;
}

u32 XTi_spi_if_IsIdle(XTi_spi_if *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_AP_CTRL);
    return (Data >> 2) & 0x1;
}

u32 XTi_spi_if_IsReady(XTi_spi_if *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_AP_CTRL);
    // check ap_start to see if the pcore is ready for next input
    return !(Data & 0x1);
}

void XTi_spi_if_EnableAutoRestart(XTi_spi_if *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTi_spi_if_WriteReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_AP_CTRL, 0x80);
}

void XTi_spi_if_DisableAutoRestart(XTi_spi_if *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTi_spi_if_WriteReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_AP_CTRL, 0);
}

void XTi_spi_if_Set_Ctrl_PS(XTi_spi_if *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTi_spi_if_WriteReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_CTRL_PS_DATA, Data);
}

u32 XTi_spi_if_Get_Ctrl_PS(XTi_spi_if *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_CTRL_PS_DATA);
    return Data;
}

void XTi_spi_if_Set_FifoByteNum_PS(XTi_spi_if *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTi_spi_if_WriteReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_FIFOBYTENUM_PS_DATA, Data);
}

u32 XTi_spi_if_Get_FifoByteNum_PS(XTi_spi_if *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_FIFOBYTENUM_PS_DATA);
    return Data;
}

u32 XTi_spi_if_Get_StringUpdate(XTi_spi_if *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_STRINGUPDATE_DATA);
    return Data;
}

u32 XTi_spi_if_Get_StringUpdate_vld(XTi_spi_if *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_STRINGUPDATE_CTRL);
    return Data & 0x1;
}

u32 XTi_spi_if_Get_StringLen(XTi_spi_if *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_STRINGLEN_DATA);
    return Data;
}

u32 XTi_spi_if_Get_StringLen_vld(XTi_spi_if *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_STRINGLEN_CTRL);
    return Data & 0x1;
}

void XTi_spi_if_Set_MISO_Sel_i(XTi_spi_if *InstancePtr, u32 Data) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTi_spi_if_WriteReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_MISO_SEL_I_DATA, Data);
}

u32 XTi_spi_if_Get_MISO_Sel_i(XTi_spi_if *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_MISO_SEL_I_DATA);
    return Data;
}

u32 XTi_spi_if_Get_MISO_Sel_o(XTi_spi_if *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_MISO_SEL_O_DATA);
    return Data;
}

u32 XTi_spi_if_Get_MISO_Sel_o_vld(XTi_spi_if *InstancePtr) {
    u32 Data;

    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Data = XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_MISO_SEL_O_CTRL);
    return Data & 0x1;
}

u32 XTi_spi_if_Get_MOSI_data_BaseAddress(XTi_spi_if *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_BASE);
}

u32 XTi_spi_if_Get_MOSI_data_HighAddress(XTi_spi_if *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_HIGH);
}

u32 XTi_spi_if_Get_MOSI_data_TotalBytes(XTi_spi_if *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_HIGH - XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_BASE + 1);
}

u32 XTi_spi_if_Get_MOSI_data_BitWidth(XTi_spi_if *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTI_SPI_IF_AXILITES_WIDTH_MOSI_DATA;
}

u32 XTi_spi_if_Get_MOSI_data_Depth(XTi_spi_if *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTI_SPI_IF_AXILITES_DEPTH_MOSI_DATA;
}

u32 XTi_spi_if_Write_MOSI_data_Words(XTi_spi_if *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_HIGH - XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Axilites_BaseAddress + XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XTi_spi_if_Read_MOSI_data_Words(XTi_spi_if *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_HIGH - XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Axilites_BaseAddress + XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_BASE + (offset + i)*4);
    }
    return length;
}

u32 XTi_spi_if_Write_MOSI_data_Bytes(XTi_spi_if *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_HIGH - XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Axilites_BaseAddress + XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XTi_spi_if_Read_MOSI_data_Bytes(XTi_spi_if *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_HIGH - XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Axilites_BaseAddress + XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_BASE + offset + i);
    }
    return length;
}

u32 XTi_spi_if_Get_MISO_data_BaseAddress(XTi_spi_if *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_BASE);
}

u32 XTi_spi_if_Get_MISO_data_HighAddress(XTi_spi_if *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (InstancePtr->Axilites_BaseAddress + XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_HIGH);
}

u32 XTi_spi_if_Get_MISO_data_TotalBytes(XTi_spi_if *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return (XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_HIGH - XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_BASE + 1);
}

u32 XTi_spi_if_Get_MISO_data_BitWidth(XTi_spi_if *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTI_SPI_IF_AXILITES_WIDTH_MISO_DATA;
}

u32 XTi_spi_if_Get_MISO_data_Depth(XTi_spi_if *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTI_SPI_IF_AXILITES_DEPTH_MISO_DATA;
}

u32 XTi_spi_if_Write_MISO_data_Words(XTi_spi_if *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_HIGH - XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(int *)(InstancePtr->Axilites_BaseAddress + XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_BASE + (offset + i)*4) = *(data + i);
    }
    return length;
}

u32 XTi_spi_if_Read_MISO_data_Words(XTi_spi_if *InstancePtr, int offset, int *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length)*4 > (XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_HIGH - XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(int *)(InstancePtr->Axilites_BaseAddress + XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_BASE + (offset + i)*4);
    }
    return length;
}

u32 XTi_spi_if_Write_MISO_data_Bytes(XTi_spi_if *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_HIGH - XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(char *)(InstancePtr->Axilites_BaseAddress + XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_BASE + offset + i) = *(data + i);
    }
    return length;
}

u32 XTi_spi_if_Read_MISO_data_Bytes(XTi_spi_if *InstancePtr, int offset, char *data, int length) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr -> IsReady == XIL_COMPONENT_IS_READY);

    int i;

    if ((offset + length) > (XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_HIGH - XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_BASE + 1))
        return 0;

    for (i = 0; i < length; i++) {
        *(data + i) = *(char *)(InstancePtr->Axilites_BaseAddress + XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_BASE + offset + i);
    }
    return length;
}

void XTi_spi_if_InterruptGlobalEnable(XTi_spi_if *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTi_spi_if_WriteReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_GIE, 1);
}

void XTi_spi_if_InterruptGlobalDisable(XTi_spi_if *InstancePtr) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTi_spi_if_WriteReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_GIE, 0);
}

void XTi_spi_if_InterruptEnable(XTi_spi_if *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_IER);
    XTi_spi_if_WriteReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_IER, Register | Mask);
}

void XTi_spi_if_InterruptDisable(XTi_spi_if *InstancePtr, u32 Mask) {
    u32 Register;

    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    Register =  XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_IER);
    XTi_spi_if_WriteReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_IER, Register & (~Mask));
}

void XTi_spi_if_InterruptClear(XTi_spi_if *InstancePtr, u32 Mask) {
    Xil_AssertVoid(InstancePtr != NULL);
    Xil_AssertVoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    XTi_spi_if_WriteReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_ISR, Mask);
}

u32 XTi_spi_if_InterruptGetEnabled(XTi_spi_if *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_IER);
}

u32 XTi_spi_if_InterruptGetStatus(XTi_spi_if *InstancePtr) {
    Xil_AssertNonvoid(InstancePtr != NULL);
    Xil_AssertNonvoid(InstancePtr->IsReady == XIL_COMPONENT_IS_READY);

    return XTi_spi_if_ReadReg(InstancePtr->Axilites_BaseAddress, XTI_SPI_IF_AXILITES_ADDR_ISR);
}

