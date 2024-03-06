`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/02/26 00:11:34
// Design Name: 
// Module Name: AliveDetector
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


module AliveDetector(
    input sys_clk,
    input sys_rst_n,
    input CS_IN,
    input Intr_IN,
    output reg LED_CS_Alive,
    output reg LED_Intr_Alive
    );
    
    wire cnt1_clear;
    wire cnt2_clear;
    reg[15:0] cnt1;
    reg[1:0] CS_IN_Delay;
    always @(posedge sys_clk) begin
        CS_IN_Delay <= {CS_IN_Delay[0], CS_IN};
    end
    always @(posedge sys_clk) begin
        if(!sys_rst_n || cnt1_clear)
            cnt1 <= 0;
        else if(CS_IN_Delay[0] != CS_IN_Delay[1])
            cnt1 <= cnt1+1;
    end
    reg[15:0] cnt2;
    reg[1:0] Intr_IN_Delay;
    always @(posedge sys_clk) begin
        Intr_IN_Delay <= {Intr_IN_Delay[0], Intr_IN};
    end
    always @(posedge sys_clk) begin
        if(!sys_rst_n || cnt2_clear)
            cnt2 <= 0;
        else if(Intr_IN_Delay[0] != Intr_IN_Delay[1])
            cnt2 <= cnt2+1;
    end
    
    reg[31:0] cnt_100ms;
    always @(posedge sys_clk) begin
        if(!sys_rst_n)
            cnt_100ms <= 0;
        else
            cnt_100ms <= cnt_100ms < (100_000_000 / 10) ? cnt_100ms+1 : 0;
    end
    
    assign cnt1_clear = cnt_100ms == 0 ? 1 : 0;
    assign cnt2_clear = cnt_100ms == 0 ? 1 : 0;
    always @(posedge sys_clk) begin
        if(!sys_rst_n) begin
            LED_CS_Alive <= 0;
            LED_Intr_Alive <= 0;
        end
        else begin
            LED_CS_Alive <= ((cnt_100ms == 1_000_000) && (cnt1 > 0)) ? ~LED_CS_Alive : LED_CS_Alive;
            LED_Intr_Alive <= ((cnt_100ms == 1_000_000) && (cnt2 > 0)) ? ~LED_Intr_Alive : LED_Intr_Alive;
        end
    end
endmodule
