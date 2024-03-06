// ==============================================================
// File generated on Fri Feb 23 00:36:57 +0800 2024
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:38:27 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
// AXILiteS
// 0x0000 : Control signals
//          bit 0  - ap_start (Read/Write/COH)
//          bit 1  - ap_done (Read/COR)
//          bit 2  - ap_idle (Read)
//          bit 3  - ap_ready (Read)
//          bit 7  - auto_restart (Read/Write)
//          others - reserved
// 0x0004 : Global Interrupt Enable Register
//          bit 0  - Global Interrupt Enable (Read/Write)
//          others - reserved
// 0x0008 : IP Interrupt Enable Register (Read/Write)
//          bit 0  - Channel 0 (ap_done)
//          bit 1  - Channel 1 (ap_ready)
//          others - reserved
// 0x000c : IP Interrupt Status Register (Read/TOW)
//          bit 0  - Channel 0 (ap_done)
//          bit 1  - Channel 1 (ap_ready)
//          others - reserved
// 0x0010 : Data signal of Ctrl_PS
//          bit 31~0 - Ctrl_PS[31:0] (Read/Write)
// 0x0014 : reserved
// 0x0018 : Data signal of FifoByteNum_PS
//          bit 31~0 - FifoByteNum_PS[31:0] (Read/Write)
// 0x001c : reserved
// 0x0020 : Data signal of StringUpdate
//          bit 31~0 - StringUpdate[31:0] (Read)
// 0x0024 : Control signal of StringUpdate
//          bit 0  - StringUpdate_ap_vld (Read/COR)
//          others - reserved
// 0x0028 : Data signal of StringLen
//          bit 31~0 - StringLen[31:0] (Read)
// 0x002c : Control signal of StringLen
//          bit 0  - StringLen_ap_vld (Read/COR)
//          others - reserved
// 0x0030 : Data signal of MISO_Sel_i
//          bit 31~0 - MISO_Sel_i[31:0] (Read/Write)
// 0x0034 : reserved
// 0x0038 : Data signal of MISO_Sel_o
//          bit 31~0 - MISO_Sel_o[31:0] (Read)
// 0x003c : Control signal of MISO_Sel_o
//          bit 0  - MISO_Sel_o_ap_vld (Read/COR)
//          others - reserved
// 0x1000 ~
// 0x1fff : Memory 'MOSI_data' (4096 * 8b)
//          Word n : bit [ 7: 0] - MOSI_data[4n]
//                   bit [15: 8] - MOSI_data[4n+1]
//                   bit [23:16] - MOSI_data[4n+2]
//                   bit [31:24] - MOSI_data[4n+3]
// 0x2000 ~
// 0x207f : Memory 'MISO_data' (128 * 8b)
//          Word n : bit [ 7: 0] - MISO_data[4n]
//                   bit [15: 8] - MISO_data[4n+1]
//                   bit [23:16] - MISO_data[4n+2]
//                   bit [31:24] - MISO_data[4n+3]
// (SC = Self Clear, COR = Clear on Read, TOW = Toggle on Write, COH = Clear on Handshake)

#define XTI_SPI_IF_AXILITES_ADDR_AP_CTRL             0x0000
#define XTI_SPI_IF_AXILITES_ADDR_GIE                 0x0004
#define XTI_SPI_IF_AXILITES_ADDR_IER                 0x0008
#define XTI_SPI_IF_AXILITES_ADDR_ISR                 0x000c
#define XTI_SPI_IF_AXILITES_ADDR_CTRL_PS_DATA        0x0010
#define XTI_SPI_IF_AXILITES_BITS_CTRL_PS_DATA        32
#define XTI_SPI_IF_AXILITES_ADDR_FIFOBYTENUM_PS_DATA 0x0018
#define XTI_SPI_IF_AXILITES_BITS_FIFOBYTENUM_PS_DATA 32
#define XTI_SPI_IF_AXILITES_ADDR_STRINGUPDATE_DATA   0x0020
#define XTI_SPI_IF_AXILITES_BITS_STRINGUPDATE_DATA   32
#define XTI_SPI_IF_AXILITES_ADDR_STRINGUPDATE_CTRL   0x0024
#define XTI_SPI_IF_AXILITES_ADDR_STRINGLEN_DATA      0x0028
#define XTI_SPI_IF_AXILITES_BITS_STRINGLEN_DATA      32
#define XTI_SPI_IF_AXILITES_ADDR_STRINGLEN_CTRL      0x002c
#define XTI_SPI_IF_AXILITES_ADDR_MISO_SEL_I_DATA     0x0030
#define XTI_SPI_IF_AXILITES_BITS_MISO_SEL_I_DATA     32
#define XTI_SPI_IF_AXILITES_ADDR_MISO_SEL_O_DATA     0x0038
#define XTI_SPI_IF_AXILITES_BITS_MISO_SEL_O_DATA     32
#define XTI_SPI_IF_AXILITES_ADDR_MISO_SEL_O_CTRL     0x003c
#define XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_BASE      0x1000
#define XTI_SPI_IF_AXILITES_ADDR_MOSI_DATA_HIGH      0x1fff
#define XTI_SPI_IF_AXILITES_WIDTH_MOSI_DATA          8
#define XTI_SPI_IF_AXILITES_DEPTH_MOSI_DATA          4096
#define XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_BASE      0x2000
#define XTI_SPI_IF_AXILITES_ADDR_MISO_DATA_HIGH      0x207f
#define XTI_SPI_IF_AXILITES_WIDTH_MISO_DATA          8
#define XTI_SPI_IF_AXILITES_DEPTH_MISO_DATA          128

