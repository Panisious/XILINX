#include "ap_cint.h"

void ti_spi_if(
		uint32 Ctrl_PS,
		uint32 FifoByteNum_PS,
		uint32 *StringUpdate,
		uint32 *StringLen,
		uint32 *MISO_Sel,

		uint8 MOSI_data[4096],
		uint8 MISO_data[128],

		uint8 MOSI_in,
		uint8 *MISO_out,

		uint1 Update_Byte_In,
		uint1 Update_String_In,

		uint1 *Update_Byte_Out,
		uint1 *Update_String_Out,

		uint1 *RDY_SET,
		uint1 *RDY_RESET,
		uint1 *INH_SET,
		uint1 *INH_RESET
		)
{
#pragma HLS PIPELINE II=1
#pragma HLS UNROLL

#pragma HLS INTERFACE s_axilite register port=FifoByteNum_PS
#pragma HLS INTERFACE s_axilite register port=Ctrl_PS
#pragma HLS INTERFACE s_axilite register port=StringUpdate
#pragma HLS INTERFACE s_axilite register port=StringLen
#pragma HLS INTERFACE s_axilite register port=MISO_Select

#pragma HLS INTERFACE s_axilite port=MISO_data
#pragma HLS INTERFACE s_axilite port=MOSI_data

#pragma HLS INTERFACE ap_none port=MISO_out
#pragma HLS INTERFACE ap_none port=MOSI_in

#pragma HLS INTERFACE ap_none port=Update_Byte_In
#pragma HLS INTERFACE ap_none port=Update_String_In
#pragma HLS INTERFACE ap_none port=Update_Byte_Out
#pragma HLS INTERFACE ap_none port=Update_String_Out

#pragma HLS INTERFACE ap_none port=RDY_RESET
#pragma HLS INTERFACE ap_none port=RDY_SET
#pragma HLS INTERFACE ap_none port=INH_SET
#pragma HLS INTERFACE ap_none port=INH_RESET

#pragma HLS INTERFACE s_axilite register port=return

	uint16 i;
	static uint16 ByteCnt = 0;
	static const uint8 MisoFF = 0xFF;
	static uint8 RDY_ForceLowCondition = 0;

	static uint1 Update_Byte_Delay[2] = {0};
#pragma HLS ARRAY_PARTITION variable=Update_Byte_Delay complete dim=1
	static uint1 Update_String_Delay[2] = {0};
#pragma HLS ARRAY_PARTITION variable=Update_String_Delay complete dim=1

#pragma HLS PIPELINE II=1
#pragma HLS UNROLL

	/* Data Update Process */
	*Update_Byte_Out 		= Update_Byte_Delay[0] 		&& !Update_Byte_Delay[1];
	*Update_String_Out 		= Update_String_Delay[0] 	&& !Update_String_Delay[1];
	Update_Byte_Delay[1] 	= Update_Byte_Delay[0];
	Update_String_Delay[1]  = Update_String_Delay[0];
	Update_Byte_Delay[0] 	= Update_Byte_In;
	Update_String_Delay[0] 	= Update_String_In;

	/* Module Control */
//	if(*MISO_Sel)
//	{
//		if(MOSI_in != 0xFF)
//		{
//			RDY_ForceLowCondition = 1;
//		}
//		else if(ByteCnt >= FifoByteNum_PS)
//		{
//			RDY_ForceLowCondition = 1;
//		}
//	}
//	else
//	{
//		RDY_ForceLowCondition = 0;
//	}
//
//	if(RDY_ForceLowCondition)
//	{
//		*RDY_RESET  =   1;
//		*RDY_SET   	= 	0;
//		*MISO_Sel   =   0;
//	}
//	else
	{
		*RDY_RESET 	= 	(Ctrl_PS & 0x00000002) ? 1 : 0;
		*RDY_SET   	= 	(Ctrl_PS & 0x00000001) ? 1 : 0;
	}

	*INH_SET 	= 	(Ctrl_PS & 0x00000004) ? 1 : 0;
	*INH_RESET 	= 	(Ctrl_PS & 0x00000008) ? 1 : 0;

	/* Output Control */
	if(*MISO_Sel)
		*MISO_out = MISO_data[ByteCnt];
	else
		*MISO_out = MisoFF;

	/* Data Update */
	if(*Update_Byte_Out)
	{
		MOSI_data[ByteCnt] = MOSI_in;
		*StringUpdate = 0;
		ByteCnt++;
	}
	else if(*Update_String_Out)
	{
		*MISO_Sel = 0;
		*StringUpdate = 1;
		*StringLen = ByteCnt;
		ByteCnt = 0;
	}
}
