`timescale 1ns / 1ps
//////////////////////////////////////////////////////////////////////////////////
// Company: 
// Engineer: 
// 
// Create Date: 2024/03/03 11:14:19
// Design Name: 
// Module Name: VIF_Comm
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


module VIF_Comm(
    /* system signal */
    input sys_clk,  /* 100MHz 10ns */ /* output need to switch every 250 ns */
    input sys_rst_n,
    /* IO signal */
    input RX_IO,
    output DE_IO,
    output RE_n_IO,
    output TX_IO,
    /*Internal TX stream signal*/
    output reg TX_Ready,
    input TX_Valid,
    input [15:0] TX_StreamData,
    /*Internal RX stream signal*/
    input RX_Ready,
    output reg RX_Valid,
    output reg [15:0] RX_StreamData
    );
 /********************************************************************************************/
    /* tx state machine */
 /********************************************************************************************/
    reg TX_InSendProgress;
    wire TX_Clear;
    reg [15:0] TX_Counter;
    reg [7:0] TX_BitCounter;
    wire TX_BitEnable;
    reg [5:0] TX_ByteCnt;
    reg[15:0] TX_ByteSendCache;
    
    /* TX Stream Input Control */
    always @(posedge sys_clk) begin
        if(~sys_rst_n || TX_Clear) begin
            TX_InSendProgress <= 0;
            TX_Ready <= 1;
            TX_ByteSendCache <= 0;
            end    
        else if(TX_Valid && TX_Ready) begin
            TX_InSendProgress <= 1;
            TX_Ready <= 0;
            TX_ByteSendCache <= TX_StreamData;
            end
    end
    /* tx top timing control */
    assign TX_Clear = TX_Counter >= 1000 ? 1 : 0;
    always @(posedge sys_clk) begin
        if(!sys_rst_n || TX_Clear)
            TX_Counter <= 0;
        else if(TX_InSendProgress)
            TX_Counter <= TX_Counter + 1;
    end
    
    /* tx bit timing control */
    assign TX_BitEnable = (TX_Counter >= 125) ? ((TX_BitCounter <= 26) ? 1 : 0) : 0;
    always @(posedge sys_clk) begin
        if(!sys_rst_n || TX_Clear)
            TX_BitCounter <= 0;
        else
            if((TX_Counter % 25 == 0) && TX_BitEnable)
                TX_BitCounter <= TX_BitCounter + 1;
            else
                TX_BitCounter <= TX_BitCounter;
    end
    
    /* tx byte timing control */
    always @(posedge sys_clk)begin
        if(!sys_rst_n)
            TX_ByteCnt<=0;
        else if(TX_Clear)
            TX_ByteCnt<=TX_ByteCnt+1;
    end
    
/************************************************/
    /* tx signal generate */
/************************************************/
    wire TX_StartOfByte;
    assign TX_StartOfByte = 1;
    wire [3:0] TX_SyncOfByte;
    assign TX_SyncOfByte = 4'b0101;
    wire [1:0] TX_StartOfFrame;
    assign TX_StartOfFrame = TX_ByteSendCache[8] == 1 ? 2'b10 : 2'b01;
    wire [15:0] TX_DataOfByte;
    assign TX_DataOfByte = {TX_ByteSendCache[0], ~TX_ByteSendCache[0],
                            TX_ByteSendCache[1], ~TX_ByteSendCache[1],
                            TX_ByteSendCache[2], ~TX_ByteSendCache[2],
                            TX_ByteSendCache[3], ~TX_ByteSendCache[3],
                            TX_ByteSendCache[4], ~TX_ByteSendCache[4],
                            TX_ByteSendCache[5], ~TX_ByteSendCache[5],
                            TX_ByteSendCache[6], ~TX_ByteSendCache[6],
                            TX_ByteSendCache[7], ~TX_ByteSendCache[7]};
    wire [1:0] TX_ErrorBit;
    assign TX_ErrorBit = TX_ByteSendCache[9] == 0 ? 2'b01 : 2'b10;
    wire TX_EndOfByte;
    assign TX_EndOfByte = 0;
    
    wire [27:0]TX_TransmitByteSequence;
    assign TX_TransmitByteSequence = {  1'b0,               /*27*/
                                        TX_StartOfByte,     /*26*/
                                        TX_SyncOfByte,      /*25-22*/
                                        TX_StartOfFrame,    /*21-20*/
                                        TX_DataOfByte,      /*19-4*/
                                        TX_ErrorBit,        /*3-2*/
                                        TX_EndOfByte,       /*1*/
                                        1'b0};              /*0*/
                            
    assign TX_IO = TX_TransmitByteSequence[27 - TX_BitCounter];
    
/********************************************************************************************/
    /* rx state machine */
/********************************************************************************************/
    /* first, detect 4 edges : fall - 250ns - rise - 250ns - fall - 250ns - rise (A.K.A. SYNC)*/
    /* when 4 edges receive in expected order and time, start byte receive state machine*/
    wire RX_Clear;
    reg [15:0] RX_Counter;
    reg [15:0] RX_ToneCounter;
    reg [3:0] RX_State;
    wire RX_Rise, RX_Fall;
    wire RX_SyncSuccess, RX_SyncFail;
    wire RX_ReceiveSuccess, RX_ReceiveFail;

    /* state definition 
    0 not received  if(recv fall, enter 1)
    1 in sync       if(recv rise fall rise, enter 2) else(enter 0)
    2 in receive
    3 end check */
    always @(posedge sys_clk)begin
    if(!sys_rst_n)
        RX_State <= 0;
    else
        case(RX_State)
            0: RX_State <= RX_Fall ? 1 : 0;
            1: RX_State <= RX_SyncSuccess    ? 2 : (RX_SyncFail    ? 0 : RX_State);
            2: RX_State <= RX_ReceiveSuccess ? 3 : (RX_ReceiveFail ? 0 : RX_State);
            3: RX_State <= 0;
        endcase
    end
    
    /* RX System Counter */
    assign RX_Clear = 0;
    always @(posedge sys_clk)begin
        if(~sys_rst_n)
            RX_Counter <= 0;
        else if(RX_State > 0)
            RX_Counter <= RX_Clear ? 0 : RX_Counter+1;
    end
    
/************************************************/
    /* rx preprocess */
/************************************************/
    /* RX Input Signal Preprocess */
    reg [2:0]RX_Filter;
    always @(posedge sys_clk)begin
        if(~sys_rst_n)
            RX_Filter <= 3'b111;
        else
            RX_Filter <= {RX_Filter[1:0], RX_IO};
    end
    reg RX_Debounce;
    always @(posedge sys_clk)begin
        if(~sys_rst_n)
            RX_Debounce <= 1;
        else if(RX_Filter == 3'b111)
            RX_Debounce <= 1;
        else if(RX_Filter == 3'b000)
            RX_Debounce <= 0;
        else
            RX_Debounce <= RX_Debounce;
    end
    reg [1:0]RX_EgdeCapture;
    always @(posedge sys_clk)begin
        if(~sys_rst_n)
            RX_EgdeCapture <= 2'b11;
        else
            RX_EgdeCapture <= {RX_EgdeCapture[0], RX_Filter};
    end
    assign RX_Rise = RX_EgdeCapture == 2'b01 ? 1 : 0;
    assign RX_Fall = RX_EgdeCapture == 2'b10 ? 1 : 0;
    
/************************************************/
    /* rx sync */
/************************************************/
    /* Sync State Description 
    0 = not in sync
    1 = in capture of 2nd sync
    2 = in capture of 3rd sync
    3 = in capture of 4th sync
    4 = sync finish
    */
    reg [3:0] RX_SyncState;
    reg [15:0] RX_SyncCounter;
    wire RX_Sync2ndRiseDetectEnable;
    wire RX_Sync3rdFallDetectEnable;
    wire RX_Sync4thRiseDetectEnable;
    wire RX_Sync2ndRiseFail;
    wire RX_Sync3rdFallFail;
    wire RX_Sync4thRiseFail;
    wire RX_SyncStateForceStop;
    
    assign RX_Sync2ndRiseDetectEnable = ((RX_SyncCounter > 20) && (RX_SyncCounter < 30)) ? 1 : 0;
    assign RX_Sync3rdFallDetectEnable = ((RX_SyncCounter > 45) && (RX_SyncCounter < 55)) ? 1 : 0;
    assign RX_Sync4thRiseDetectEnable = ((RX_SyncCounter > 70) && (RX_SyncCounter < 80)) ? 1 : 0;
    assign RX_Sync2ndRiseFail = ((RX_SyncCounter > 30) && (RX_SyncState == 1)) ? 1 : 0;
    assign RX_Sync3rdFallFail = ((RX_SyncCounter > 55) && (RX_SyncState == 2)) ? 1 : 0;
    assign RX_Sync4thRiseFail = ((RX_SyncCounter > 80) && (RX_SyncState == 3)) ? 1 : 0;
    assign RX_SyncStateForceStop = ((RX_SyncState==4) || RX_SyncFail) ? 1 : 0;
    assign RX_SyncSuccess = (RX_SyncState == 4) ? 1 : 0;
    assign RX_SyncFail = RX_Sync2ndRiseFail || RX_Sync3rdFallFail || RX_Sync4thRiseFail;
    
    always @(posedge sys_clk) begin
        if(~sys_rst_n || RX_SyncStateForceStop)
            RX_SyncCounter <= 0;
        else
            if(RX_State == 1)
                RX_SyncCounter <= RX_SyncCounter+1;
    end
    
    always @(posedge sys_clk) begin
        if(~sys_rst_n || RX_SyncStateForceStop)
            RX_SyncState<=0;
        else if(RX_State == 1)
            case(RX_SyncState)
                0: RX_SyncState <= 1;
                1: RX_SyncState <= (RX_Sync2ndRiseDetectEnable && RX_Rise) ? 2 : RX_SyncState;
                2: RX_SyncState <= (RX_Sync3rdFallDetectEnable && RX_Fall) ? 3 : RX_SyncState;
                3: RX_SyncState <= (RX_Sync4thRiseDetectEnable && RX_Rise) ? 4 : RX_SyncState;
                4: RX_SyncState <= 0;
            endcase
    end
    
/************************************************/
    /* rx receive */
/************************************************/
    /* receive state description
    0 = not in receive
    1 = in receive of byte
    2 = receive end
    */
    reg[3:0] RX_BitCounter;
    reg[3:0] RX_RecvState;
    reg[15:0] RX_DataCaptureSyncCounter;
    reg[15:0] RX_RecvCounter;
    wire[15:0] RX_RecvCounterDif;
    wire RX_RecvDataEnd;
    wire RX_RecvByteEnd;
    wire RX_DataInCapture;
    reg[15:0] RX_Data;
    wire RX_Data_StartOfFrame;
    wire[7:0] RX_Data_Byte;
    wire RX_Data_ErrorBit;
    wire[15:0] RX_Data_Output;
    
    parameter RX_Window = 12;
    parameter RX_RecvTime = 50;
    assign RX_RecvCounterDif = RX_RecvCounter - RX_DataCaptureSyncCounter;
    assign RX_DataInCapture = (RX_RecvCounterDif > (RX_RecvTime - RX_Window)) && (RX_RecvCounterDif < (RX_RecvTime + RX_Window)) ? 1 : 0;

    assign RX_RecvDataEnd = RX_BitCounter == 10 ? 1 : 0;
    assign RX_RecvByteEnd = RX_RecvCounter > 600 ? 1 : 0;
    assign RX_ReceiveSuccess = RX_RecvByteEnd;
    assign RX_ReceiveFail = RX_RecvCounterDif > RX_RecvTime + RX_Window ? 1 : 0;
    assign RX_Data_StartOfFrame = RX_Data[0];
    assign RX_Data_Byte = RX_Data[8:1];
    assign RX_Data_ErrorBit = RX_Data[9];
    assign RX_Data_Output = {RX_Data_ErrorBit, 6'b000000, RX_Data_StartOfFrame, RX_Data_Byte};

    always @(posedge sys_clk)begin
        if(~sys_rst_n || RX_RecvByteEnd || RX_ReceiveFail) begin
            RX_DataCaptureSyncCounter <= 0;
            RX_BitCounter <= 0;
            RX_Data <= 0;
            end
        else if(RX_State == 2) begin
            if(RX_DataInCapture) begin
                if(RX_Rise) begin
                    RX_DataCaptureSyncCounter <= RX_RecvCounter;
                    RX_Data[RX_BitCounter] = 0;
                    RX_BitCounter <= RX_BitCounter + 1;
                    end
                else if(RX_Fall) begin
                    RX_DataCaptureSyncCounter <= RX_RecvCounter;
                    RX_Data[RX_BitCounter] = 1;
                    RX_BitCounter <= RX_BitCounter + 1;
                    end
                end
            end
    end
    always @(posedge sys_clk)begin
        if(~sys_rst_n || RX_RecvByteEnd || RX_ReceiveFail)
            RX_RecvCounter <= 0;
        else if(RX_State == 2)
            RX_RecvCounter <= RX_RecvCounter+1;
    end
    always @(posedge sys_clk) begin
        if(~sys_rst_n || RX_ReceiveFail)
            RX_RecvState <= 0;
        else if(RX_State == 2)
            case(RX_RecvState)
                0: RX_RecvState <= 1;
                1: RX_RecvState <= RX_RecvDataEnd ? 2 : RX_RecvState;
                2: RX_RecvState <= RX_RecvByteEnd ? 0 : RX_RecvState;
            endcase
    end
 
     /* RX Stream Output Control */
    always @(posedge sys_clk)begin
        if(~sys_rst_n) begin
            RX_Valid <= 0;
            RX_StreamData <= 0;
            end
        else begin
            if(RX_ReceiveSuccess) begin
                RX_Valid <= 1;
                RX_StreamData <= RX_Data_Output;
                end
            else if(RX_Ready && RX_Valid) begin
                RX_Valid <= 0;
                RX_StreamData <= RX_StreamData;
                end
            end
    end
 /********************************************************************************************/
    /* rx tx direction select */
 /********************************************************************************************/
    assign DE_IO = TX_BitEnable;
    assign RE_n_IO = DE_IO;
    
endmodule
