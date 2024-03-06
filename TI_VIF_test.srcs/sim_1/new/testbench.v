`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/03/03 11:15:45
// Design Name: 
// Module Name: testbench
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


module testbench(

    );
    
    reg sys_clk, sys_rst_n;
    reg sys_clk_bias;
    wire DE, n_RE, RX, TX;
    reg trans_en;
    
    wire TX_Ready;
    reg TX_Valid;
    reg [15:0]TX_StreamData;
    
    reg RX_Ready;
    wire RX_Valid;
    wire [15:0]RX_StreamData;
    reg [7:0]RX_Byte[15:0];
    reg [3:0]RX_Cnt;
    
    VIF_Comm u0(
    .sys_clk (sys_clk),  /* 100MHz 10ns */ /* output need to switch every 250 ns */
    .sys_rst_n (sys_rst_n),
    
    .RX_IO (TX),
    .DE_IO (DE),
    .RE_n_IO (n_RE),
    .TX_IO (RX),
    
    .TX_Ready (TX_Ready),
    .TX_Valid (TX_Valid),
    .TX_StreamData (TX_StreamData),
    
    .RX_Ready (RX_Ready),
    .RX_Valid (RX_Valid),
    .RX_StreamData (RX_StreamData)
    );
    
    tb_vif_rx_gen u1(
    .sys_clk (sys_clk_bias),  /* 100MHz 10ns */ /* output need to switch every 250 ns */
    .sys_rst_n (sys_rst_n),
    .trans_en (trans_en),
    .TX (TX)
    );
    always #5 sys_clk = ~sys_clk;
    always #5 sys_clk_bias = ~sys_clk_bias;
    
    reg[15:0] tx_testSendArray[15:0];
    reg[15:0] rx_testRecvArray[15:0];
    initial begin
        tx_testSendArray[0] =  {6'b0, 1'b0, 1'b1, 8'h13};
        tx_testSendArray[1] =  {6'b0, 1'b0, 1'b0, 8'h24};
        tx_testSendArray[2] =  {6'b0, 1'b0, 1'b0, 8'h35};
        tx_testSendArray[3] =  {6'b0, 1'b0, 1'b0, 8'h46};
        tx_testSendArray[4] =  {6'b0, 1'b0, 1'b0, 8'h57};
        tx_testSendArray[5] =  {6'b0, 1'b0, 1'b0, 8'h68};
        tx_testSendArray[6] =  {6'b0, 1'b0, 1'b0, 8'h79};
        tx_testSendArray[7] =  {6'b0, 1'b0, 1'b0, 8'h8A};
        tx_testSendArray[8] =  {6'b0, 1'b0, 1'b0, 8'h9B};
        tx_testSendArray[9] =  {6'b0, 1'b0, 1'b0, 8'hAC};
        tx_testSendArray[10] = {6'b0, 1'b0, 1'b0, 8'hBD};
        tx_testSendArray[11] = {6'b0, 1'b0, 1'b0, 8'hCE};
        tx_testSendArray[12] = {6'b0, 1'b0, 1'b0, 8'hDF};
        tx_testSendArray[13] = {6'b0, 1'b0, 1'b0, 8'hE0};
        tx_testSendArray[14] = {6'b0, 1'b0, 1'b0, 8'hF1};
        tx_testSendArray[15] = {6'b0, 1'b0, 1'b0, 8'hAA};
    end
    integer i,j,k;
    initial begin
    sys_clk = 0; sys_clk_bias=0; sys_rst_n = 0; trans_en =0;
    TX_Valid = 0; TX_StreamData = 0;
    RX_Ready = 0;
    i=0;j=0;k=0;
    #2000 sys_rst_n = 1;
    #2000 trans_en = 1;
    
    for(i=0; i<10000; i=i+1) begin
        if(TX_Ready) begin
            TX_Valid = 1;
            TX_StreamData = tx_testSendArray[j[3:0]];
            j = j+1;
            end
        //else
            //TX_Valid = 0;
        if(RX_Valid) begin
            RX_Ready = 1;
            rx_testRecvArray[k[3:0]] = RX_StreamData;
            RX_Byte[k[3:0]] = RX_StreamData[7:0];
            k=k+1;
            end
        else
            RX_Ready = 0;
            
        #10;
    end
    
    $stop;
    end
endmodule
