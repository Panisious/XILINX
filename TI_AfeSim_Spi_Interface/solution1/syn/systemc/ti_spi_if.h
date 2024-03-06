// ==============================================================
// RTL generated by Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC
// Version: 2018.3
// Copyright (C) 1986-2018 Xilinx, Inc. All Rights Reserved.
// 
// ===========================================================

#ifndef _ti_spi_if_HH_
#define _ti_spi_if_HH_

#include "systemc.h"
#include "AESL_pkg.h"

#include "ti_spi_if_AXILiteS_s_axi.h"

namespace ap_rtl {

template<unsigned int C_S_AXI_AXILITES_ADDR_WIDTH = 14,
         unsigned int C_S_AXI_AXILITES_DATA_WIDTH = 32>
struct ti_spi_if : public sc_module {
    // Port declarations 30
    sc_in_clk ap_clk;
    sc_in< sc_logic > ap_rst_n;
    sc_in< sc_lv<8> > MOSI_in;
    sc_out< sc_lv<8> > MISO_out;
    sc_in< sc_lv<1> > Update_Byte_In;
    sc_in< sc_lv<1> > Update_String_In;
    sc_out< sc_lv<1> > Update_Byte_Out;
    sc_out< sc_lv<1> > Update_String_Out;
    sc_out< sc_lv<1> > RDY_SET;
    sc_out< sc_lv<1> > RDY_RESET;
    sc_out< sc_lv<1> > INH_SET;
    sc_out< sc_lv<1> > INH_RESET;
    sc_in< sc_logic > s_axi_AXILiteS_AWVALID;
    sc_out< sc_logic > s_axi_AXILiteS_AWREADY;
    sc_in< sc_uint<C_S_AXI_AXILITES_ADDR_WIDTH> > s_axi_AXILiteS_AWADDR;
    sc_in< sc_logic > s_axi_AXILiteS_WVALID;
    sc_out< sc_logic > s_axi_AXILiteS_WREADY;
    sc_in< sc_uint<C_S_AXI_AXILITES_DATA_WIDTH> > s_axi_AXILiteS_WDATA;
    sc_in< sc_uint<C_S_AXI_AXILITES_DATA_WIDTH/8> > s_axi_AXILiteS_WSTRB;
    sc_in< sc_logic > s_axi_AXILiteS_ARVALID;
    sc_out< sc_logic > s_axi_AXILiteS_ARREADY;
    sc_in< sc_uint<C_S_AXI_AXILITES_ADDR_WIDTH> > s_axi_AXILiteS_ARADDR;
    sc_out< sc_logic > s_axi_AXILiteS_RVALID;
    sc_in< sc_logic > s_axi_AXILiteS_RREADY;
    sc_out< sc_uint<C_S_AXI_AXILITES_DATA_WIDTH> > s_axi_AXILiteS_RDATA;
    sc_out< sc_lv<2> > s_axi_AXILiteS_RRESP;
    sc_out< sc_logic > s_axi_AXILiteS_BVALID;
    sc_in< sc_logic > s_axi_AXILiteS_BREADY;
    sc_out< sc_lv<2> > s_axi_AXILiteS_BRESP;
    sc_out< sc_logic > interrupt;
    sc_signal< sc_logic > ap_var_for_const0;
    sc_signal< sc_lv<32> > ap_var_for_const1;


    // Module declarations
    ti_spi_if(sc_module_name name);
    SC_HAS_PROCESS(ti_spi_if);

    ~ti_spi_if();

    sc_trace_file* mVcdFile;

    ofstream mHdltvinHandle;
    ofstream mHdltvoutHandle;
    ti_spi_if_AXILiteS_s_axi<C_S_AXI_AXILITES_ADDR_WIDTH,C_S_AXI_AXILITES_DATA_WIDTH>* ti_spi_if_AXILiteS_s_axi_U;
    sc_signal< sc_logic > ap_rst_n_inv;
    sc_signal< sc_logic > ap_start;
    sc_signal< sc_logic > ap_done;
    sc_signal< sc_logic > ap_idle;
    sc_signal< sc_lv<1> > ap_CS_fsm;
    sc_signal< sc_logic > ap_CS_fsm_pp0_stage0;
    sc_signal< sc_logic > ap_enable_reg_pp0_iter0;
    sc_signal< sc_logic > ap_enable_reg_pp0_iter1;
    sc_signal< sc_logic > ap_enable_reg_pp0_iter2;
    sc_signal< sc_logic > ap_idle_pp0;
    sc_signal< sc_logic > ap_ready;
    sc_signal< bool > ap_block_state1_pp0_stage0_iter0;
    sc_signal< bool > ap_block_state2_pp0_stage0_iter1;
    sc_signal< sc_logic > StringUpdate_1_ack_in;
    sc_signal< sc_logic > StringLen_1_ack_in;
    sc_signal< sc_logic > MISO_Sel_1_ack_in;
    sc_signal< bool > ap_block_state3_pp0_stage0_iter2;
    sc_signal< bool > ap_block_pp0_stage0_11001;
    sc_signal< sc_lv<32> > Ctrl_PS;
    sc_signal< sc_lv<32> > Ctrl_PS_0_data_reg;
    sc_signal< sc_logic > Ctrl_PS_0_vld_reg;
    sc_signal< sc_logic > Ctrl_PS_0_ack_out;
    sc_signal< sc_lv<32> > FifoByteNum_PS;
    sc_signal< sc_lv<32> > StringUpdate_1_data_reg;
    sc_signal< sc_lv<32> > StringUpdate_1_data_in;
    sc_signal< sc_logic > StringUpdate_1_vld_reg;
    sc_signal< sc_logic > StringUpdate_1_vld_in;
    sc_signal< sc_lv<32> > StringLen_1_data_reg;
    sc_signal< sc_logic > StringLen_1_vld_reg;
    sc_signal< sc_logic > StringLen_1_vld_in;
    sc_signal< sc_lv<32> > MISO_Sel_i;
    sc_signal< sc_lv<32> > MISO_Sel_0_data_reg;
    sc_signal< sc_logic > MISO_Sel_0_vld_reg;
    sc_signal< sc_logic > MISO_Sel_0_ack_out;
    sc_signal< sc_logic > MISO_Sel_1_vld_reg;
    sc_signal< sc_logic > MISO_Sel_1_vld_in;
    sc_signal< sc_lv<12> > MOSI_data_address0;
    sc_signal< sc_logic > MOSI_data_ce0;
    sc_signal< sc_logic > MOSI_data_we0;
    sc_signal< sc_lv<7> > MISO_data_address0;
    sc_signal< sc_logic > MISO_data_ce0;
    sc_signal< sc_lv<8> > MISO_data_q0;
    sc_signal< sc_lv<1> > Update_Byte_Delay_0;
    sc_signal< sc_lv<1> > Update_Byte_Delay_1;
    sc_signal< sc_lv<1> > Update_String_Delay_s;
    sc_signal< sc_lv<1> > Update_String_Delay_1;
    sc_signal< sc_lv<16> > ByteCnt;
    sc_signal< sc_lv<8> > MOSI_in_read_reg_372;
    sc_signal< sc_lv<8> > MOSI_in_read_reg_372_pp0_iter1_reg;
    sc_signal< sc_lv<1> > Update_Byte_Out_load_fu_245_p2;
    sc_signal< sc_lv<1> > Update_Byte_Out_load_reg_377;
    sc_signal< sc_lv<1> > Update_Byte_Out_load_reg_377_pp0_iter1_reg;
    sc_signal< sc_lv<1> > Update_String_Out_lo_fu_265_p2;
    sc_signal< sc_lv<1> > Update_String_Out_lo_reg_382;
    sc_signal< sc_lv<1> > Update_String_Out_lo_reg_382_pp0_iter1_reg;
    sc_signal< sc_lv<16> > ByteCnt_load_reg_387;
    sc_signal< sc_lv<64> > tmp_1_fu_299_p1;
    sc_signal< sc_lv<64> > tmp_1_reg_392;
    sc_signal< sc_lv<64> > tmp_1_reg_392_pp0_iter1_reg;
    sc_signal< sc_lv<8> > MISO_data_load_reg_402;
    sc_signal< sc_lv<32> > tmp_7_fu_322_p1;
    sc_signal< bool > ap_block_pp0_stage0_subdone;
    sc_signal< bool > ap_block_pp0_stage0;
    sc_signal< sc_lv<16> > tmp_4_fu_310_p2;
    sc_signal< bool > ap_block_pp0_stage0_01001;
    sc_signal< sc_lv<1> > tmp_3_fu_239_p2;
    sc_signal< sc_lv<1> > tmp_5_fu_259_p2;
    sc_signal< sc_lv<1> > tmp_s_fu_358_p2;
    sc_signal< sc_lv<1> > ap_NS_fsm;
    sc_signal< sc_logic > ap_idle_pp0_0to1;
    sc_signal< sc_logic > ap_reset_idle_pp0;
    sc_signal< sc_logic > ap_enable_pp0;
    sc_signal< bool > ap_condition_58;
    sc_signal< bool > ap_condition_338;
    static const sc_logic ap_const_logic_1;
    static const sc_logic ap_const_logic_0;
    static const sc_lv<1> ap_ST_fsm_pp0_stage0;
    static const sc_lv<32> ap_const_lv32_0;
    static const bool ap_const_boolean_1;
    static const bool ap_const_boolean_0;
    static const sc_lv<1> ap_const_lv1_0;
    static const sc_lv<16> ap_const_lv16_0;
    static const int C_S_AXI_DATA_WIDTH;
    static const sc_lv<1> ap_const_lv1_1;
    static const sc_lv<32> ap_const_lv32_1;
    static const sc_lv<16> ap_const_lv16_1;
    static const sc_lv<32> ap_const_lv32_2;
    static const sc_lv<32> ap_const_lv32_3;
    static const sc_lv<8> ap_const_lv8_FF;
    // Thread declarations
    void thread_ap_var_for_const0();
    void thread_ap_var_for_const1();
    void thread_ap_clk_no_reset_();
    void thread_Ctrl_PS_0_ack_out();
    void thread_INH_RESET();
    void thread_INH_SET();
    void thread_MISO_Sel_0_ack_out();
    void thread_MISO_Sel_1_ack_in();
    void thread_MISO_Sel_1_vld_in();
    void thread_MISO_data_address0();
    void thread_MISO_data_ce0();
    void thread_MISO_out();
    void thread_MOSI_data_address0();
    void thread_MOSI_data_ce0();
    void thread_MOSI_data_we0();
    void thread_RDY_RESET();
    void thread_RDY_SET();
    void thread_StringLen_1_ack_in();
    void thread_StringLen_1_vld_in();
    void thread_StringUpdate_1_ack_in();
    void thread_StringUpdate_1_data_in();
    void thread_StringUpdate_1_vld_in();
    void thread_Update_Byte_Out();
    void thread_Update_Byte_Out_load_fu_245_p2();
    void thread_Update_String_Out();
    void thread_Update_String_Out_lo_fu_265_p2();
    void thread_ap_CS_fsm_pp0_stage0();
    void thread_ap_block_pp0_stage0();
    void thread_ap_block_pp0_stage0_01001();
    void thread_ap_block_pp0_stage0_11001();
    void thread_ap_block_pp0_stage0_subdone();
    void thread_ap_block_state1_pp0_stage0_iter0();
    void thread_ap_block_state2_pp0_stage0_iter1();
    void thread_ap_block_state3_pp0_stage0_iter2();
    void thread_ap_condition_338();
    void thread_ap_condition_58();
    void thread_ap_done();
    void thread_ap_enable_pp0();
    void thread_ap_enable_reg_pp0_iter0();
    void thread_ap_idle();
    void thread_ap_idle_pp0();
    void thread_ap_idle_pp0_0to1();
    void thread_ap_ready();
    void thread_ap_reset_idle_pp0();
    void thread_ap_rst_n_inv();
    void thread_tmp_1_fu_299_p1();
    void thread_tmp_3_fu_239_p2();
    void thread_tmp_4_fu_310_p2();
    void thread_tmp_5_fu_259_p2();
    void thread_tmp_7_fu_322_p1();
    void thread_tmp_s_fu_358_p2();
    void thread_ap_NS_fsm();
    void thread_hdltv_gen();
};

}

using namespace ap_rtl;

#endif