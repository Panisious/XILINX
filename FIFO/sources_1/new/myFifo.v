`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/03/05 22:53:34
// Design Name: 
// Module Name: myFifo
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


module myFifo
    #(
        parameter FIFO_SIZE = 16
    )
    (
    input sys_clk,
    input sys_rst_n,
    input [31:0] RX_Data,
    input RX_Valid,
    output RX_Ready,
    output [31:0] TX_Data,
    output TX_Valid,
    input TX_Ready
    );
    
    
    wire FIFO_IS_FULL;
    wire FIFO_IS_EMPTY;
    wire FIFO_IS_HALF_FULL;
    wire FIFO_IS_NEAR_FULL;
    wire FIFO_IS_NEAR_EMPTY;
    
    reg [$clog2(FIFO_SIZE)-1:0] FIFO_RX_Ptr;
    reg [$clog2(FIFO_SIZE)-1:0] FIFO_TX_Ptr;
    reg [$clog2(FIFO_SIZE)-1:0] FIFO_Num;
    reg [31:0] FIFO_Reg[FIFO_SIZE - 1:0];
    integer i;
    initial begin
        FIFO_RX_Ptr <= 0;
        FIFO_TX_Ptr <= 0;
        FIFO_Num <= 0;
        for(i=0;i<FIFO_SIZE;i=i+1)
            FIFO_Reg[i] <= 0;
    end
    
    assign FIFO_IS_FULL = FIFO_Num == FIFO_SIZE-1 ? 1 : 0;
    assign FIFO_IS_EMPTY = FIFO_Num == 0 ? 1 : 0;
    assign FIFO_IS_HALF_FULL = FIFO_Num >= FIFO_SIZE/2 ? 1:0;
    assign FIFO_IS_NEAR_FULL = FIFO_Num >= FIFO_SIZE-2 ? 1 : 0;
    assign FIFO_IS_NEAR_EMPTY = FIFO_Num <= 1 ? 1 : 0;
    
    assign RX_Ready = FIFO_IS_FULL ? 0 : 1;
    assign TX_Valid = FIFO_IS_EMPTY ? 0 : 1;
    
    wire RX_Enable;
    wire TX_Enable;
    wire RX_TX_Enable;
    assign RX_Enable = RX_Valid && RX_Ready;
    assign TX_Enable = TX_Valid && TX_Ready;
    assign RX_TX_Enable = RX_Enable && TX_Enable;
    
    always @(posedge sys_clk)begin
        if(~sys_rst_n)
            FIFO_RX_Ptr <= 0;
        else
            if(RX_Enable && ~FIFO_IS_FULL)
                FIFO_RX_Ptr <= FIFO_RX_Ptr + 1;
        end
        
    always @(posedge sys_clk)begin
        if(~sys_rst_n)
            FIFO_TX_Ptr <= 0;
        else
            if(TX_Enable && ~FIFO_IS_EMPTY)
                FIFO_TX_Ptr <= FIFO_TX_Ptr + 1;
        end
        
    always @(posedge sys_clk)begin
        if(~sys_rst_n)
            FIFO_Num <= 0;
        else if(RX_TX_Enable)
            FIFO_Num <= FIFO_Num;
        else if(RX_Enable && ~FIFO_IS_FULL)
            FIFO_Num <= FIFO_Num+1;
        else if(TX_Enable && ~FIFO_IS_EMPTY)
            FIFO_Num <= FIFO_Num-1;
        end
        
    assign TX_Data = FIFO_Reg[FIFO_TX_Ptr];
    always @(posedge sys_clk)begin
        if(~sys_rst_n)
            FIFO_Reg[FIFO_RX_Ptr] <= 0;
        else if(RX_Enable && ~FIFO_IS_FULL)
            FIFO_Reg[FIFO_RX_Ptr] <= RX_Data;
        end
        
endmodule
