`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/03/03 11:19:40
// Design Name: 
// Module Name: tb_vif_rx_gen
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


module tb_vif_rx_gen(
    input sys_clk,
    input sys_rst_n,
    //input [7:0] byte_in,
    //input [7:0] byte_num,
    input trans_en,
    output TX
    );
    
    reg [15:0] bitCnt;
    wire bitEnable;
    
    reg [15:0] cnt;
    wire cnt_clear;
    always @(posedge sys_clk) begin
        if(!sys_rst_n || cnt_clear)
            cnt <= 0;
        else
            cnt <= cnt + 1;
    end
    assign cnt_clear = cnt >= 1000 ? 1:0;
    
    assign bitEnable = (cnt >= 125) ? ((bitCnt <= 26) ? 1 : 0) : 0;
    always @(posedge sys_clk) begin
        if(!sys_rst_n || cnt_clear)
            bitCnt <= 0;
        else
            if((cnt % 25 == 0) && bitEnable)
                bitCnt <= bitCnt + 1;
            else
                bitCnt <= bitCnt;
    end
    
    reg [4:0] byteCnt;
    always @(posedge sys_clk)begin
        if(!sys_rst_n || !trans_en)
            byteCnt<=0;
        else if(trans_en)
            byteCnt <= cnt_clear ? byteCnt+1 : byteCnt;
    end
    
    
    always @(posedge sys_clk) begin
        if(!sys_rst_n || cnt_clear)
            bitCnt <= 0;
        else
            if((cnt % 25 == 0) && bitEnable)
                bitCnt <= bitCnt + 1;
            else
                bitCnt <= bitCnt;
    end
    
    reg[7:0] byteList[15:0];
    reg[15:0] i;
    initial begin
        byteList[0] <= 8'h12;
        byteList[1] <= 8'h34;
        byteList[2] <= 8'h56;
        byteList[3] <= 8'h78;
        byteList[4] <= 8'h9A;
        byteList[5] <= 8'hBC;
        byteList[6] <= 8'hDE;
        byteList[7] <= 8'hF0;
        byteList[8] <= 8'h00;
        byteList[9] <= 8'h00;
        byteList[10] <= 8'h00;
        byteList[11] <= 8'h00;
        byteList[12] <= 8'h00;
        byteList[13] <= 8'h00;
        byteList[14] <= 8'h00;
        byteList[15] <= 8'h00;
    end

    wire startOfByte;
    assign startOfByte = 1;
    wire [3:0] SyncByte;
    assign SyncByte = 4'b0101;
    wire [1:0] frameInit;
    assign frameInit = byteCnt == 0 ? 2'b10 : 2'b01;
    wire[15:0] daisyByte;
    assign daisyByte = {byteList[byteCnt[3:0]][0],~byteList[byteCnt[3:0]][0],
                        byteList[byteCnt[3:0]][1],~byteList[byteCnt[3:0]][1],
                        byteList[byteCnt[3:0]][2],~byteList[byteCnt[3:0]][2],
                        byteList[byteCnt[3:0]][3],~byteList[byteCnt[3:0]][3],
                        byteList[byteCnt[3:0]][4],~byteList[byteCnt[3:0]][4],
                        byteList[byteCnt[3:0]][5],~byteList[byteCnt[3:0]][5],
                        byteList[byteCnt[3:0]][6],~byteList[byteCnt[3:0]][6],
                        byteList[byteCnt[3:0]][7],~byteList[byteCnt[3:0]][7]};
    wire [1:0] errorBit;
    assign errorBit = 2'b01;
    wire endofByte;
    assign endofByte = 0;
    
    wire [27:0]transmitByte;
    assign transmitByte = { 1'b1,           /*27*/
                            startOfByte,    /*26*/
                            SyncByte,       /*25-22*/
                            frameInit,      /*21-20*/
                            daisyByte,      /*19-4*/
                            errorBit,       /*3-2*/
                            endofByte,      /*1*/
                            1'b1};          /*0*/
                            
    assign TX = bitEnable ? transmitByte[27-bitCnt] : 1;
endmodule
