// ==============================================================
// File generated on Fri Feb 23 00:36:57 +0800 2024
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2018.3 (64-bit)
// SW Build 2405991 on Thu Dec  6 23:38:27 MST 2018
// IP Build 2404404 on Fri Dec  7 01:43:56 MST 2018
// Copyright 1986-2018 Xilinx, Inc. All Rights Reserved.
// ==============================================================
`timescale 1ns/1ps
module ti_spi_if_AXILiteS_s_axi
#(parameter
    C_S_AXI_ADDR_WIDTH = 14,
    C_S_AXI_DATA_WIDTH = 32
)(
    // axi4 lite slave signals
    input  wire                          ACLK,
    input  wire                          ARESET,
    input  wire                          ACLK_EN,
    input  wire [C_S_AXI_ADDR_WIDTH-1:0] AWADDR,
    input  wire                          AWVALID,
    output wire                          AWREADY,
    input  wire [C_S_AXI_DATA_WIDTH-1:0] WDATA,
    input  wire [C_S_AXI_DATA_WIDTH/8-1:0] WSTRB,
    input  wire                          WVALID,
    output wire                          WREADY,
    output wire [1:0]                    BRESP,
    output wire                          BVALID,
    input  wire                          BREADY,
    input  wire [C_S_AXI_ADDR_WIDTH-1:0] ARADDR,
    input  wire                          ARVALID,
    output wire                          ARREADY,
    output wire [C_S_AXI_DATA_WIDTH-1:0] RDATA,
    output wire [1:0]                    RRESP,
    output wire                          RVALID,
    input  wire                          RREADY,
    output wire                          interrupt,
    // user signals
    output wire                          ap_start,
    input  wire                          ap_done,
    input  wire                          ap_ready,
    input  wire                          ap_idle,
    output wire [31:0]                   Ctrl_PS,
    output wire [31:0]                   FifoByteNum_PS,
    input  wire [31:0]                   StringUpdate,
    input  wire                          StringUpdate_ap_vld,
    input  wire [31:0]                   StringLen,
    input  wire                          StringLen_ap_vld,
    output wire [31:0]                   MISO_Sel_i,
    input  wire [31:0]                   MISO_Sel_o,
    input  wire                          MISO_Sel_o_ap_vld,
    input  wire [11:0]                   MOSI_data_address0,
    input  wire                          MOSI_data_ce0,
    input  wire                          MOSI_data_we0,
    input  wire [7:0]                    MOSI_data_d0,
    input  wire [6:0]                    MISO_data_address0,
    input  wire                          MISO_data_ce0,
    output wire [7:0]                    MISO_data_q0
);
//------------------------Address Info-------------------
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

//------------------------Parameter----------------------
localparam
    ADDR_AP_CTRL               = 14'h0000,
    ADDR_GIE                   = 14'h0004,
    ADDR_IER                   = 14'h0008,
    ADDR_ISR                   = 14'h000c,
    ADDR_CTRL_PS_DATA_0        = 14'h0010,
    ADDR_CTRL_PS_CTRL          = 14'h0014,
    ADDR_FIFOBYTENUM_PS_DATA_0 = 14'h0018,
    ADDR_FIFOBYTENUM_PS_CTRL   = 14'h001c,
    ADDR_STRINGUPDATE_DATA_0   = 14'h0020,
    ADDR_STRINGUPDATE_CTRL     = 14'h0024,
    ADDR_STRINGLEN_DATA_0      = 14'h0028,
    ADDR_STRINGLEN_CTRL        = 14'h002c,
    ADDR_MISO_SEL_I_DATA_0     = 14'h0030,
    ADDR_MISO_SEL_I_CTRL       = 14'h0034,
    ADDR_MISO_SEL_O_DATA_0     = 14'h0038,
    ADDR_MISO_SEL_O_CTRL       = 14'h003c,
    ADDR_MOSI_DATA_BASE        = 14'h1000,
    ADDR_MOSI_DATA_HIGH        = 14'h1fff,
    ADDR_MISO_DATA_BASE        = 14'h2000,
    ADDR_MISO_DATA_HIGH        = 14'h207f,
    WRIDLE                     = 2'd0,
    WRDATA                     = 2'd1,
    WRRESP                     = 2'd2,
    WRRESET                    = 2'd3,
    RDIDLE                     = 2'd0,
    RDDATA                     = 2'd1,
    RDRESET                    = 2'd2,
    ADDR_BITS         = 14;

//------------------------Local signal-------------------
    reg  [1:0]                    wstate = WRRESET;
    reg  [1:0]                    wnext;
    reg  [ADDR_BITS-1:0]          waddr;
    wire [31:0]                   wmask;
    wire                          aw_hs;
    wire                          w_hs;
    reg  [1:0]                    rstate = RDRESET;
    reg  [1:0]                    rnext;
    reg  [31:0]                   rdata;
    wire                          ar_hs;
    wire [ADDR_BITS-1:0]          raddr;
    // internal registers
    reg                           int_ap_idle;
    reg                           int_ap_ready;
    reg                           int_ap_done = 1'b0;
    reg                           int_ap_start = 1'b0;
    reg                           int_auto_restart = 1'b0;
    reg                           int_gie = 1'b0;
    reg  [1:0]                    int_ier = 2'b0;
    reg  [1:0]                    int_isr = 2'b0;
    reg  [31:0]                   int_Ctrl_PS = 'b0;
    reg  [31:0]                   int_FifoByteNum_PS = 'b0;
    reg  [31:0]                   int_StringUpdate = 'b0;
    reg                           int_StringUpdate_ap_vld;
    reg  [31:0]                   int_StringLen = 'b0;
    reg                           int_StringLen_ap_vld;
    reg  [31:0]                   int_MISO_Sel_i = 'b0;
    reg  [31:0]                   int_MISO_Sel_o = 'b0;
    reg                           int_MISO_Sel_o_ap_vld;
    // memory signals
    wire [9:0]                    int_MOSI_data_address0;
    wire                          int_MOSI_data_ce0;
    wire                          int_MOSI_data_we0;
    wire [3:0]                    int_MOSI_data_be0;
    wire [31:0]                   int_MOSI_data_d0;
    wire [31:0]                   int_MOSI_data_q0;
    wire [9:0]                    int_MOSI_data_address1;
    wire                          int_MOSI_data_ce1;
    wire                          int_MOSI_data_we1;
    wire [3:0]                    int_MOSI_data_be1;
    wire [31:0]                   int_MOSI_data_d1;
    wire [31:0]                   int_MOSI_data_q1;
    reg                           int_MOSI_data_read;
    reg                           int_MOSI_data_write;
    reg  [1:0]                    int_MOSI_data_shift;
    wire [4:0]                    int_MISO_data_address0;
    wire                          int_MISO_data_ce0;
    wire                          int_MISO_data_we0;
    wire [3:0]                    int_MISO_data_be0;
    wire [31:0]                   int_MISO_data_d0;
    wire [31:0]                   int_MISO_data_q0;
    wire [4:0]                    int_MISO_data_address1;
    wire                          int_MISO_data_ce1;
    wire                          int_MISO_data_we1;
    wire [3:0]                    int_MISO_data_be1;
    wire [31:0]                   int_MISO_data_d1;
    wire [31:0]                   int_MISO_data_q1;
    reg                           int_MISO_data_read;
    reg                           int_MISO_data_write;
    reg  [1:0]                    int_MISO_data_shift;

//------------------------Instantiation------------------
// int_MOSI_data
ti_spi_if_AXILiteS_s_axi_ram #(
    .BYTES    ( 4 ),
    .DEPTH    ( 1024 )
) int_MOSI_data (
    .clk0     ( ACLK ),
    .address0 ( int_MOSI_data_address0 ),
    .ce0      ( int_MOSI_data_ce0 ),
    .we0      ( int_MOSI_data_we0 ),
    .be0      ( int_MOSI_data_be0 ),
    .d0       ( int_MOSI_data_d0 ),
    .q0       ( int_MOSI_data_q0 ),
    .clk1     ( ACLK ),
    .address1 ( int_MOSI_data_address1 ),
    .ce1      ( int_MOSI_data_ce1 ),
    .we1      ( int_MOSI_data_we1 ),
    .be1      ( int_MOSI_data_be1 ),
    .d1       ( int_MOSI_data_d1 ),
    .q1       ( int_MOSI_data_q1 )
);
// int_MISO_data
ti_spi_if_AXILiteS_s_axi_ram #(
    .BYTES    ( 4 ),
    .DEPTH    ( 32 )
) int_MISO_data (
    .clk0     ( ACLK ),
    .address0 ( int_MISO_data_address0 ),
    .ce0      ( int_MISO_data_ce0 ),
    .we0      ( int_MISO_data_we0 ),
    .be0      ( int_MISO_data_be0 ),
    .d0       ( int_MISO_data_d0 ),
    .q0       ( int_MISO_data_q0 ),
    .clk1     ( ACLK ),
    .address1 ( int_MISO_data_address1 ),
    .ce1      ( int_MISO_data_ce1 ),
    .we1      ( int_MISO_data_we1 ),
    .be1      ( int_MISO_data_be1 ),
    .d1       ( int_MISO_data_d1 ),
    .q1       ( int_MISO_data_q1 )
);

//------------------------AXI write fsm------------------
assign AWREADY = (wstate == WRIDLE);
assign WREADY  = (wstate == WRDATA);
assign BRESP   = 2'b00;  // OKAY
assign BVALID  = (wstate == WRRESP);
assign wmask   = { {8{WSTRB[3]}}, {8{WSTRB[2]}}, {8{WSTRB[1]}}, {8{WSTRB[0]}} };
assign aw_hs   = AWVALID & AWREADY;
assign w_hs    = WVALID & WREADY;

// wstate
always @(posedge ACLK) begin
    if (ARESET)
        wstate <= WRRESET;
    else if (ACLK_EN)
        wstate <= wnext;
end

// wnext
always @(*) begin
    case (wstate)
        WRIDLE:
            if (AWVALID)
                wnext = WRDATA;
            else
                wnext = WRIDLE;
        WRDATA:
            if (WVALID)
                wnext = WRRESP;
            else
                wnext = WRDATA;
        WRRESP:
            if (BREADY)
                wnext = WRIDLE;
            else
                wnext = WRRESP;
        default:
            wnext = WRIDLE;
    endcase
end

// waddr
always @(posedge ACLK) begin
    if (ACLK_EN) begin
        if (aw_hs)
            waddr <= AWADDR[ADDR_BITS-1:0];
    end
end

//------------------------AXI read fsm-------------------
assign ARREADY = (rstate == RDIDLE);
assign RDATA   = rdata;
assign RRESP   = 2'b00;  // OKAY
assign RVALID  = (rstate == RDDATA) & !int_MOSI_data_read & !int_MISO_data_read;
assign ar_hs   = ARVALID & ARREADY;
assign raddr   = ARADDR[ADDR_BITS-1:0];

// rstate
always @(posedge ACLK) begin
    if (ARESET)
        rstate <= RDRESET;
    else if (ACLK_EN)
        rstate <= rnext;
end

// rnext
always @(*) begin
    case (rstate)
        RDIDLE:
            if (ARVALID)
                rnext = RDDATA;
            else
                rnext = RDIDLE;
        RDDATA:
            if (RREADY & RVALID)
                rnext = RDIDLE;
            else
                rnext = RDDATA;
        default:
            rnext = RDIDLE;
    endcase
end

// rdata
always @(posedge ACLK) begin
    if (ACLK_EN) begin
        if (ar_hs) begin
            rdata <= 1'b0;
            case (raddr)
                ADDR_AP_CTRL: begin
                    rdata[0] <= int_ap_start;
                    rdata[1] <= int_ap_done;
                    rdata[2] <= int_ap_idle;
                    rdata[3] <= int_ap_ready;
                    rdata[7] <= int_auto_restart;
                end
                ADDR_GIE: begin
                    rdata <= int_gie;
                end
                ADDR_IER: begin
                    rdata <= int_ier;
                end
                ADDR_ISR: begin
                    rdata <= int_isr;
                end
                ADDR_CTRL_PS_DATA_0: begin
                    rdata <= int_Ctrl_PS[31:0];
                end
                ADDR_FIFOBYTENUM_PS_DATA_0: begin
                    rdata <= int_FifoByteNum_PS[31:0];
                end
                ADDR_STRINGUPDATE_DATA_0: begin
                    rdata <= int_StringUpdate[31:0];
                end
                ADDR_STRINGUPDATE_CTRL: begin
                    rdata[0] <= int_StringUpdate_ap_vld;
                end
                ADDR_STRINGLEN_DATA_0: begin
                    rdata <= int_StringLen[31:0];
                end
                ADDR_STRINGLEN_CTRL: begin
                    rdata[0] <= int_StringLen_ap_vld;
                end
                ADDR_MISO_SEL_I_DATA_0: begin
                    rdata <= int_MISO_Sel_i[31:0];
                end
                ADDR_MISO_SEL_O_DATA_0: begin
                    rdata <= int_MISO_Sel_o[31:0];
                end
                ADDR_MISO_SEL_O_CTRL: begin
                    rdata[0] <= int_MISO_Sel_o_ap_vld;
                end
            endcase
        end
        else if (int_MOSI_data_read) begin
            rdata <= int_MOSI_data_q1;
        end
        else if (int_MISO_data_read) begin
            rdata <= int_MISO_data_q1;
        end
    end
end


//------------------------Register logic-----------------
assign interrupt      = int_gie & (|int_isr);
assign ap_start       = int_ap_start;
assign Ctrl_PS        = int_Ctrl_PS;
assign FifoByteNum_PS = int_FifoByteNum_PS;
assign MISO_Sel_i     = int_MISO_Sel_i;
// int_ap_start
always @(posedge ACLK) begin
    if (ARESET)
        int_ap_start <= 1'b0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_AP_CTRL && WSTRB[0] && WDATA[0])
            int_ap_start <= 1'b1;
        else if (ap_ready)
            int_ap_start <= int_auto_restart; // clear on handshake/auto restart
    end
end

// int_ap_done
always @(posedge ACLK) begin
    if (ARESET)
        int_ap_done <= 1'b0;
    else if (ACLK_EN) begin
        if (ap_done)
            int_ap_done <= 1'b1;
        else if (ar_hs && raddr == ADDR_AP_CTRL)
            int_ap_done <= 1'b0; // clear on read
    end
end

// int_ap_idle
always @(posedge ACLK) begin
    if (ARESET)
        int_ap_idle <= 1'b0;
    else if (ACLK_EN) begin
            int_ap_idle <= ap_idle;
    end
end

// int_ap_ready
always @(posedge ACLK) begin
    if (ARESET)
        int_ap_ready <= 1'b0;
    else if (ACLK_EN) begin
            int_ap_ready <= ap_ready;
    end
end

// int_auto_restart
always @(posedge ACLK) begin
    if (ARESET)
        int_auto_restart <= 1'b0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_AP_CTRL && WSTRB[0])
            int_auto_restart <=  WDATA[7];
    end
end

// int_gie
always @(posedge ACLK) begin
    if (ARESET)
        int_gie <= 1'b0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_GIE && WSTRB[0])
            int_gie <= WDATA[0];
    end
end

// int_ier
always @(posedge ACLK) begin
    if (ARESET)
        int_ier <= 1'b0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_IER && WSTRB[0])
            int_ier <= WDATA[1:0];
    end
end

// int_isr[0]
always @(posedge ACLK) begin
    if (ARESET)
        int_isr[0] <= 1'b0;
    else if (ACLK_EN) begin
        if (int_ier[0] & ap_done)
            int_isr[0] <= 1'b1;
        else if (w_hs && waddr == ADDR_ISR && WSTRB[0])
            int_isr[0] <= int_isr[0] ^ WDATA[0]; // toggle on write
    end
end

// int_isr[1]
always @(posedge ACLK) begin
    if (ARESET)
        int_isr[1] <= 1'b0;
    else if (ACLK_EN) begin
        if (int_ier[1] & ap_ready)
            int_isr[1] <= 1'b1;
        else if (w_hs && waddr == ADDR_ISR && WSTRB[0])
            int_isr[1] <= int_isr[1] ^ WDATA[1]; // toggle on write
    end
end

// int_Ctrl_PS[31:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_Ctrl_PS[31:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_CTRL_PS_DATA_0)
            int_Ctrl_PS[31:0] <= (WDATA[31:0] & wmask) | (int_Ctrl_PS[31:0] & ~wmask);
    end
end

// int_FifoByteNum_PS[31:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_FifoByteNum_PS[31:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_FIFOBYTENUM_PS_DATA_0)
            int_FifoByteNum_PS[31:0] <= (WDATA[31:0] & wmask) | (int_FifoByteNum_PS[31:0] & ~wmask);
    end
end

// int_StringUpdate
always @(posedge ACLK) begin
    if (ARESET)
        int_StringUpdate <= 0;
    else if (ACLK_EN) begin
        if (StringUpdate_ap_vld)
            int_StringUpdate <= StringUpdate;
    end
end

// int_StringUpdate_ap_vld
always @(posedge ACLK) begin
    if (ARESET)
        int_StringUpdate_ap_vld <= 1'b0;
    else if (ACLK_EN) begin
        if (StringUpdate_ap_vld)
            int_StringUpdate_ap_vld <= 1'b1;
        else if (ar_hs && raddr == ADDR_STRINGUPDATE_CTRL)
            int_StringUpdate_ap_vld <= 1'b0; // clear on read
    end
end

// int_StringLen
always @(posedge ACLK) begin
    if (ARESET)
        int_StringLen <= 0;
    else if (ACLK_EN) begin
        if (StringLen_ap_vld)
            int_StringLen <= StringLen;
    end
end

// int_StringLen_ap_vld
always @(posedge ACLK) begin
    if (ARESET)
        int_StringLen_ap_vld <= 1'b0;
    else if (ACLK_EN) begin
        if (StringLen_ap_vld)
            int_StringLen_ap_vld <= 1'b1;
        else if (ar_hs && raddr == ADDR_STRINGLEN_CTRL)
            int_StringLen_ap_vld <= 1'b0; // clear on read
    end
end

// int_MISO_Sel_i[31:0]
always @(posedge ACLK) begin
    if (ARESET)
        int_MISO_Sel_i[31:0] <= 0;
    else if (ACLK_EN) begin
        if (w_hs && waddr == ADDR_MISO_SEL_I_DATA_0)
            int_MISO_Sel_i[31:0] <= (WDATA[31:0] & wmask) | (int_MISO_Sel_i[31:0] & ~wmask);
    end
end

// int_MISO_Sel_o
always @(posedge ACLK) begin
    if (ARESET)
        int_MISO_Sel_o <= 0;
    else if (ACLK_EN) begin
        if (MISO_Sel_o_ap_vld)
            int_MISO_Sel_o <= MISO_Sel_o;
    end
end

// int_MISO_Sel_o_ap_vld
always @(posedge ACLK) begin
    if (ARESET)
        int_MISO_Sel_o_ap_vld <= 1'b0;
    else if (ACLK_EN) begin
        if (MISO_Sel_o_ap_vld)
            int_MISO_Sel_o_ap_vld <= 1'b1;
        else if (ar_hs && raddr == ADDR_MISO_SEL_O_CTRL)
            int_MISO_Sel_o_ap_vld <= 1'b0; // clear on read
    end
end


//------------------------Memory logic-------------------
// MOSI_data
assign int_MOSI_data_address0 = MOSI_data_address0 >> 2;
assign int_MOSI_data_ce0      = MOSI_data_ce0;
assign int_MOSI_data_we0      = MOSI_data_we0;
assign int_MOSI_data_be0      = 1 << MOSI_data_address0[1:0];
assign int_MOSI_data_d0       = {4{MOSI_data_d0}};
assign int_MOSI_data_address1 = ar_hs? raddr[11:2] : waddr[11:2];
assign int_MOSI_data_ce1      = ar_hs | (int_MOSI_data_write & WVALID);
assign int_MOSI_data_we1      = int_MOSI_data_write & WVALID;
assign int_MOSI_data_be1      = WSTRB;
assign int_MOSI_data_d1       = WDATA;
// MISO_data
assign int_MISO_data_address0 = MISO_data_address0 >> 2;
assign int_MISO_data_ce0      = MISO_data_ce0;
assign int_MISO_data_we0      = 1'b0;
assign int_MISO_data_be0      = 1'b0;
assign int_MISO_data_d0       = 1'b0;
assign MISO_data_q0           = int_MISO_data_q0 >> (int_MISO_data_shift * 8);
assign int_MISO_data_address1 = ar_hs? raddr[6:2] : waddr[6:2];
assign int_MISO_data_ce1      = ar_hs | (int_MISO_data_write & WVALID);
assign int_MISO_data_we1      = int_MISO_data_write & WVALID;
assign int_MISO_data_be1      = WSTRB;
assign int_MISO_data_d1       = WDATA;
// int_MOSI_data_read
always @(posedge ACLK) begin
    if (ARESET)
        int_MOSI_data_read <= 1'b0;
    else if (ACLK_EN) begin
        if (ar_hs && raddr >= ADDR_MOSI_DATA_BASE && raddr <= ADDR_MOSI_DATA_HIGH)
            int_MOSI_data_read <= 1'b1;
        else
            int_MOSI_data_read <= 1'b0;
    end
end

// int_MOSI_data_write
always @(posedge ACLK) begin
    if (ARESET)
        int_MOSI_data_write <= 1'b0;
    else if (ACLK_EN) begin
        if (aw_hs && AWADDR[ADDR_BITS-1:0] >= ADDR_MOSI_DATA_BASE && AWADDR[ADDR_BITS-1:0] <= ADDR_MOSI_DATA_HIGH)
            int_MOSI_data_write <= 1'b1;
        else if (WVALID)
            int_MOSI_data_write <= 1'b0;
    end
end

// int_MOSI_data_shift
always @(posedge ACLK) begin
    if (ACLK_EN) begin
        if (MOSI_data_ce0)
            int_MOSI_data_shift <= MOSI_data_address0[1:0];
    end
end

// int_MISO_data_read
always @(posedge ACLK) begin
    if (ARESET)
        int_MISO_data_read <= 1'b0;
    else if (ACLK_EN) begin
        if (ar_hs && raddr >= ADDR_MISO_DATA_BASE && raddr <= ADDR_MISO_DATA_HIGH)
            int_MISO_data_read <= 1'b1;
        else
            int_MISO_data_read <= 1'b0;
    end
end

// int_MISO_data_write
always @(posedge ACLK) begin
    if (ARESET)
        int_MISO_data_write <= 1'b0;
    else if (ACLK_EN) begin
        if (aw_hs && AWADDR[ADDR_BITS-1:0] >= ADDR_MISO_DATA_BASE && AWADDR[ADDR_BITS-1:0] <= ADDR_MISO_DATA_HIGH)
            int_MISO_data_write <= 1'b1;
        else if (WVALID)
            int_MISO_data_write <= 1'b0;
    end
end

// int_MISO_data_shift
always @(posedge ACLK) begin
    if (ACLK_EN) begin
        if (MISO_data_ce0)
            int_MISO_data_shift <= MISO_data_address0[1:0];
    end
end


endmodule


`timescale 1ns/1ps

module ti_spi_if_AXILiteS_s_axi_ram
#(parameter
    BYTES  = 4,
    DEPTH  = 256,
    AWIDTH = log2(DEPTH)
) (
    input  wire               clk0,
    input  wire [AWIDTH-1:0]  address0,
    input  wire               ce0,
    input  wire               we0,
    input  wire [BYTES-1:0]   be0,
    input  wire [BYTES*8-1:0] d0,
    output reg  [BYTES*8-1:0] q0,
    input  wire               clk1,
    input  wire [AWIDTH-1:0]  address1,
    input  wire               ce1,
    input  wire               we1,
    input  wire [BYTES-1:0]   be1,
    input  wire [BYTES*8-1:0] d1,
    output reg  [BYTES*8-1:0] q1
);
//------------------------Local signal-------------------
reg  [BYTES*8-1:0] mem[0:DEPTH-1];
//------------------------Task and function--------------
function integer log2;
    input integer x;
    integer n, m;
begin
    n = 1;
    m = 2;
    while (m < x) begin
        n = n + 1;
        m = m * 2;
    end
    log2 = n;
end
endfunction
//------------------------Body---------------------------
// read port 0
always @(posedge clk0) begin
    if (ce0) q0 <= mem[address0];
end

// read port 1
always @(posedge clk1) begin
    if (ce1) q1 <= mem[address1];
end

genvar i;
generate
    for (i = 0; i < BYTES; i = i + 1) begin : gen_write
        // write port 0
        always @(posedge clk0) begin
            if (ce0 & we0 & be0[i]) begin
                mem[address0][8*i+7:8*i] <= d0[8*i+7:8*i];
            end
        end
        // write port 1
        always @(posedge clk1) begin
            if (ce1 & we1 & be1[i]) begin
                mem[address1][8*i+7:8*i] <= d1[8*i+7:8*i];
            end
        end
    end
endgenerate

endmodule

