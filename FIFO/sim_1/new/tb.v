`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/03/05 23:07:43
// Design Name: 
// Module Name: tb
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


module tb(

    );
    
    reg sys_clk, sys_rst_n;
    reg[31:0] RX_Data;
    reg RX_Valid;
    reg TX_Ready;
    wire RX_Ready;
    wire[31:0] TX_Data;
    wire TX_Valid;
    
    myFifo 
    #(
        .FIFO_SIZE(32)
    )
    u0
    (
         sys_clk,
         sys_rst_n,
         RX_Data,
         RX_Valid,
         RX_Ready,
         TX_Data,
         TX_Valid,
         TX_Ready
    );
    
    always #5 sys_clk = ~sys_clk;
    
    integer i;
    
    initial begin
    sys_clk = 0; sys_rst_n = 0; RX_Data = 0; RX_Valid = 0; TX_Ready = 0;
    #2000
    sys_rst_n = 1;

    for(i=0;i<32;i=i+1)
    begin
        RX_Data = 1 + i;
        RX_Valid = 1;
        #10;
    end
    RX_Valid = 0;
    
    #100 TX_Ready = 1;
    #2000; TX_Ready = 0;
    
    for(i=32;i<64;i=i+1)
    begin
        RX_Data = 1 + i;
        RX_Valid = 1;
        #10;
    end
    RX_Valid = 0;
    
    #100 TX_Ready = 1;
    #2000;
    for(i=100;i<120;i=i+1)
    begin
        RX_Data = 1 + i;
        RX_Valid = 1;
        #10;
    end
    
    #2000 $stop;
    end
endmodule
