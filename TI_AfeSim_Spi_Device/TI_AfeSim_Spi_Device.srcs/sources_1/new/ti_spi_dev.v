`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2023/12/29 21:52:59
// Design Name: 
// Module Name: BQ79600_SpiSlaveDevice
// Project Name: 
// Target Devices: 
// Tool Versions: 
// Description: 
// 
// Dependencies: 
// 
// Revision:
// Revision 0.01 - File Created
// Additional Comments:
// 
//////////////////////////////////////////////////////////////////////////////////


module ti_spi_dev(          // This is a slave device
    input sys_clk,                      // 50MHz system clock
    input sys_rst_n,                     // System reset
    
    input SPI_MOSI,                     // SPI Line - Mosi as input
    output SPI_MISO,                    // SPI Line - Miso as output
    input SPI_CLK,                      // SPI Line - SPI_CLK as input
    input SPI_CS,                       // SPI Line - CS as input
    output reg SPI_RDY,                 // SPI Line - SPI Ready
    output reg INH,                     // Reverse Wakeup
    
    input [7:0] MISO_TxData,            // SPI Data - from CMC to BMU     - connect to SPI FIFO
    output [7:0] MOSI_RxData,           // SPI Data - from BMU to CMC     - connect to SPI FIFO
    input Ctrl_SetRDY,                  // SPI Control
    input Ctrl_ResetRDY,                // SPI Control
    output Stat_ByteUpdate,             // SPI Flag - Recv a new byte       set 1 when Debug_BitCnt change from 7 to 0
    output Stat_StringUpdate,           // SPI Flag - Recv a full string    set 1 when CS rise
    
    output [7:0] Debug_ByteCnt,         // SPI Stat - Num of recv bytes     - clear when CS fall
    output [3:0] Debug_BitCnt           // SPI Stat - Num of recv bits      - increase when SPI_CLK rise
    );
    
    // input signal debounce
    reg [2:0]CS_Debounce;
    always @(posedge sys_clk)begin
    if(~sys_rst_n)
        CS_Debounce <= 3'b111;
    else
        CS_Debounce <= {CS_Debounce[1:0], SPI_CS};
    end
    reg CS;
    always @(posedge sys_clk)begin
    if(CS_Debounce == 3'b111)
        CS <= 1;
    else if(CS_Debounce == 3'b000)
        CS <= 0;
    end
    
    // system signal circuit
    wire Enable;
    wire Disable;
    assign Enable = sys_rst_n && !CS;
    assign Disable = ~Enable;
    reg [1:0]CS_Edge;
    reg [1:0]SCLK_Edge;
    wire CS_Rise, CS_Fall;
    wire SCLK_Rise, SCLK_Fall;
    always @(posedge sys_clk)begin
    if(!sys_rst_n)begin
        CS_Edge <= 0;
        SCLK_Edge <= 0;end
    else begin
        CS_Edge[0] <= CS;
        CS_Edge[1] <= CS_Edge[0];
        SCLK_Edge[0] <= SPI_CLK;
        SCLK_Edge[1] <= SCLK_Edge[0];end
    end
    assign CS_Rise = CS_Edge[0] & !CS_Edge[1] ? 1 : 0;
    assign CS_Fall = !CS_Edge[0] & CS_Edge[1] ? 1 : 0;
    assign SCLK_Rise = !CS && SCLK_Edge[0] & !SCLK_Edge[1] ? 1 : 0;
    assign SCLK_Fall = !CS && !SCLK_Edge[0] & SCLK_Edge[1] ? 1 : 0;
    
    // bit processing
    reg [3:0]RX_BitCount;           // +1 when SPI_CLK fall
    reg [3:0]RX_BitCount_Prev;      // Previous status of RX_BitCount
    reg [7:0]RX_ShiftReg;           // Shift when SPI_CLK rist
    reg TX_Buffer;                  // MISO output register
    assign Debug_BitCnt = RX_BitCount;
    assign SPI_MISO = TX_Buffer;

    always @(posedge sys_clk) begin
        if(~sys_rst_n || CS_Fall || CS_Rise) begin
            RX_ShiftReg<=0;
            RX_BitCount<=0;
            RX_BitCount_Prev<=0; end
        else if(SCLK_Rise)
            RX_ShiftReg <= {RX_ShiftReg[6:0],SPI_MOSI};
        else if(SCLK_Fall)begin
            RX_BitCount <= RX_BitCount < 7 ? RX_BitCount + 1'b1 : 3'b000;
            RX_BitCount_Prev <= RX_BitCount; end
    end
    always @(posedge sys_clk) begin
        if(~sys_rst_n)
            TX_Buffer <= 1;
        else
            TX_Buffer <= MISO_TxData[7-RX_BitCount];     
    end
    
    // byte processing
    wire ByteUpdate;                // when receive 8 bit, set it 1
    assign ByteUpdate = (RX_BitCount == 0 && RX_BitCount_Prev == 7) ? 1 : 0;
    
    reg [1:0]ByteUpdate_Edge;
    always @ (posedge sys_clk)
    if(~sys_rst_n || CS_Rise)
        ByteUpdate_Edge <= 0;
    else
        ByteUpdate_Edge <= {ByteUpdate_Edge[0], ByteUpdate};
        
    wire ByteUpdate_Rise;
    assign ByteUpdate_Rise = ByteUpdate_Edge[0] && !ByteUpdate_Edge[1];
    
    reg [4:0]ByteUpdate_Hold;
    always @(posedge sys_clk) begin
        if(~sys_rst_n || CS_Fall || CS_Rise)
            ByteUpdate_Hold<=0;
        else
            ByteUpdate_Hold<={ByteUpdate_Hold[3:0], ByteUpdate_Rise};
    end
    assign Stat_ByteUpdate = ByteUpdate_Hold>0 ? 1: 0;
    
    reg [7:0]RxOutputBuff;
    assign MOSI_RxData = RxOutputBuff;
    always @(posedge sys_clk) begin
        if(!sys_rst_n)
            RxOutputBuff <= 0;
        else if(ByteUpdate_Rise)
            RxOutputBuff <= RX_ShiftReg;
    end

    // string processing
    reg [7:0] ByteCount;
    assign Debug_ByteCnt = ByteCount;
    always @(posedge sys_clk) begin
        if(!sys_rst_n || CS_Fall)
            ByteCount <= 0;
        else if(ByteUpdate_Rise)
            ByteCount <= ByteCount+1;
    end
    wire StringUpdate;
    assign StringUpdate = CS_Rise && ByteCount ? 1:0;
    reg [4:0]StringUpdate_Hold;
    always @(posedge sys_clk) begin
        if(~sys_rst_n || CS_Fall)
            StringUpdate_Hold<=0;
        else
            StringUpdate_Hold<={StringUpdate_Hold[3:0], StringUpdate};
    end
    assign Stat_StringUpdate = StringUpdate_Hold > 0 ? 1:0;
    
    always @(posedge sys_clk)begin
        if(~sys_rst_n || Ctrl_SetRDY)
            SPI_RDY <= 1;
        else if(ByteCount == 1 && (RxOutputBuff==8'h80 || RxOutputBuff==8'hA0 || RxOutputBuff==8'hC0 ))
            SPI_RDY <= 0;
        else if(CS_Rise && ByteCount == 128)
            SPI_RDY <= 0;
        else if(Ctrl_ResetRDY)
            SPI_RDY <= 0;
    end
    
    /**/
    always @(posedge sys_clk)
        INH <= 0;
endmodule