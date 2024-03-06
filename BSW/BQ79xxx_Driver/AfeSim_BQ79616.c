/*
 * Afe_133_BQ79616.c
 *
 *  Created on: 2024�??????1�??????6�??????
 *      Author: Administrator
 */


#include "AfeSim_BQ79616.h"

static void AfeSim_BQ79616_PhysicalSimualtion_CellVoltage(void);
static void AfeSim_BQ79616_PhysicalSimualtion_GpioVoltage(void);
static void AfeSim_BQ79616_PhysicalSimualtion_DieTemperature(void);
static void AfeSim_BQ79616_PhysicalSimulation(void);
void AfeSim_BQ79616_Init(void);
void AfeSim_BQ79616_Fsm(void);


AfeSim_DeviceGpioUseType AfeSim_BQ79616_GpioInitConfig[3][AFESIM_MAX_GPIO_NUM] =
{
		{	/*BQ79600*/
				AFESIM_DEVICE_GPIO_NOUSE,			AFESIM_DEVICE_GPIO_NOUSE,
				AFESIM_DEVICE_GPIO_NOUSE,			AFESIM_DEVICE_GPIO_NOUSE,
				AFESIM_DEVICE_GPIO_NOUSE,			AFESIM_DEVICE_GPIO_NOUSE,
				AFESIM_DEVICE_GPIO_NOUSE,			AFESIM_DEVICE_GPIO_NOUSE,
		},
		{	/*BQ79616*/
				AFESIM_DEVICE_GPIO_MODULE_TEMP,		AFESIM_DEVICE_GPIO_MODULE_TEMP,
				AFESIM_DEVICE_GPIO_MODULE_TEMP,		AFESIM_DEVICE_GPIO_MODULE_TEMP,
				AFESIM_DEVICE_GPIO_MODULE_TEMP,		AFESIM_DEVICE_GPIO_MODULE_TEMP,
				AFESIM_DEVICE_GPIO_BALANCE_TEMP,	AFESIM_DEVICE_GPIO_BALANCE_TEMP,
		},
		{	/*BQ79656*/
				AFESIM_DEVICE_GPIO_MODULE_TEMP,		AFESIM_DEVICE_GPIO_MODULE_TEMP,
				AFESIM_DEVICE_GPIO_MODULE_TEMP,		AFESIM_DEVICE_GPIO_MODULE_TEMP,
				AFESIM_DEVICE_GPIO_MODULE_TEMP,		AFESIM_DEVICE_GPIO_MODULE_TEMP,
				AFESIM_DEVICE_GPIO_BALANCE_TEMP,	AFESIM_DEVICE_GPIO_BALANCE_TEMP,
		},
};

static void AfeSim_BQ79616_PhysicalSimualtion_CellVoltage(void)
{

}

static void AfeSim_BQ79616_PhysicalSimualtion_GpioVoltage(void)
{

}

static void AfeSim_BQ79616_PhysicalSimualtion_DieTemperature(void)
{

}

static void AfeSim_BQ79616_PhysicalSimulation(void)
{
	AfeSim_BQ79616_PhysicalSimualtion_CellVoltage();
	AfeSim_BQ79616_PhysicalSimualtion_GpioVoltage();
	AfeSim_BQ79616_PhysicalSimualtion_DieTemperature();
}

void AfeSim_BQ79616_ResetAuxVolt(uint8 phyDevId)
{
	uint8 start = phyDevId;
	uint8 end = phyDevId;
	if(phyDevId == 0xFF)
	{
		start = 1;
		end = AfeMan->afeChipRealNum;
	}
	for(phyDevId = start; phyDevId <= end; phyDevId++)
	{
		AfeSim_Device[phyDevId].power.Aux_CELL_100uV = 34000;
		AfeSim_Device[phyDevId].power.Aux_GPIO_100uV = 25000;
		AfeSim_Device[phyDevId].power.Aux_BAT_mV = AfeSim_Device[phyDevId].power.VBAT_mV;
		AfeSim_Device[phyDevId].power.Aux_REFL_100uV = 11000;
		AfeSim_Device[phyDevId].power.Aux_VBG2_100uV = 11000;
		AfeSim_Device[phyDevId].power.Aux_VCM_100uV = 25000;
		AfeSim_Device[phyDevId].power.Aux_AVAO_REF_100uV = 24700;
		AfeSim_Device[phyDevId].power.Aux_AVDD_REF_100uV = 24700;
		AfeSim_Device[phyDevId].power.Aux_OVDAC_100uV = 44750;		/* OV_THRESH default = 0x2E */
		AfeSim_Device[phyDevId].power.Aux_UVDAC_100uV = 12000;		/* UV_THRESH default = 0x00 */
		AfeSim_Device[phyDevId].power.Aux_OT_OTCB_DAC_100uV = 5000; /* OTUT_THRESH default = 0xE0 */
		AfeSim_Device[phyDevId].power.Aux_UTDAC_100uV = 40000;
		AfeSim_Device[phyDevId].power.Aux_VCBDONE_DAC_100uV = 24500; /* VCB_DONE_THRESH default = 0x00 */
	}
}


void AfeSim_BQ79616_DiagnoseBistProcess(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_DIAG_STAT_RegType* DIAG_STAT = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DIAG_STAT];
	AFESIM_BQ79616_REG_ADC_STAT2_RegType* ADC_STAT2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ADC_STAT2];
		
	switch (AfeSim_Device[phyDevId].diag.bist.type)
	{
		case AFESIM_BQ79616_DIAG_PWR_BIST:
		{
			AFESIM_BQ79616_REG_FAULT_PWR1_RegType* FAULT_PWR1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_PWR1];
			AFESIM_BQ79616_REG_FAULT_PWR2_RegType* FAULT_PWR2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_PWR2];

			FAULT_PWR1->reg.bist.BIST = 0xFF;
			FAULT_PWR2->reg.bist.BIST = 0x1F;

			/* CAN Monitor Calibration, make at least one channel check fail */
			if(AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_PWR_BIST])
			{
				uint8 mask = AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_PWR_BIST] & 0x000000FF;
				FAULT_PWR1->reg.bist.BIST ^= mask;
				mask = (AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_PWR_BIST] & 0x00001F00) >> 8;
				FAULT_PWR2->reg.bist.BIST ^= mask;
				FAULT_PWR2->reg.bist.FAIL = 1;
			}
			
			/* POWER BIST FINISH */
			DIAG_STAT->reg.field.DRDY_BIST_PWR = 1;
		}break;
		
		case AFESIM_BQ79616_DIAG_VCCB_FAULT_INJ_BIST:
		{
			AFESIM_BQ79616_REG_FAULT_COMP_VCCB1_RegType* FAULT_COMP_VCCB1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_VCCB1];
			AFESIM_BQ79616_REG_FAULT_COMP_VCCB2_RegType* FAULT_COMP_VCCB2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_VCCB2];

			sint8 maxVC = AfeSim_Device[phyDevId].activeCell;
			for(maxVC; maxVC > 0; maxVC--)
			{
				if(maxVC > 8)
				{
					FAULT_COMP_VCCB1->reg.bist.BIST |= (1 << (maxVC - 9));
				}
				else
				{
					FAULT_COMP_VCCB2->reg.bist.BIST |= (1 << (maxVC - 1));
				}
			}
			if(AfeSim_Device[phyDevId].busbar.bbvc_enable)
			{
				FAULT_COMP_VCCB1->reg.val |= ((AfeSim_Device[phyDevId].busbar.bbvc_posn & 0xFF00) >> 8);
				FAULT_COMP_VCCB2->reg.val |= (AfeSim_Device[phyDevId].busbar.bbvc_posn & 0x00FF);
			}
			if(AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_VCCB_FAULT_INJ_BIST])
			{
				uint8 mask = AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_VCCB_FAULT_INJ_BIST] & 0x000000FF;
				FAULT_COMP_VCCB1->reg.bist.BIST ^= mask;
				mask = (AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_VCCB_FAULT_INJ_BIST] & 0x0000FF00) >> 8;
				FAULT_COMP_VCCB2->reg.bist.BIST ^= mask;
			}

			/* VCCB BIST FINISH */
			ADC_STAT2->reg.field.DRDY_VCCB = 1;
		}break;
		
		case AFESIM_BQ79616_DIAG_VCOW_FAULT_INJ_BIST:
		{
			AFESIM_BQ79616_REG_FAULT_COMP_VCOW1_RegType* FAULT_COMP_VCOW1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_VCOW1];
			AFESIM_BQ79616_REG_FAULT_COMP_VCOW2_RegType* FAULT_COMP_VCOW2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_VCOW2];

			sint8 maxVC = AfeSim_Device[phyDevId].activeCell;
			for(maxVC; maxVC > 0; maxVC--)
			{
				if(maxVC > 8)
				{
					FAULT_COMP_VCOW1->reg.bist.BIST |= (1 << (maxVC - 9));
				}
				else
				{
					FAULT_COMP_VCOW2->reg.bist.BIST |= (1 << (maxVC - 1));
				}
			}
			if(AfeSim_Device[phyDevId].busbar.bbvc_enable)
			{
				FAULT_COMP_VCOW1->reg.val |= ((AfeSim_Device[phyDevId].busbar.bbvc_posn & 0xFF00) >> 8);
				FAULT_COMP_VCOW2->reg.val |= (AfeSim_Device[phyDevId].busbar.bbvc_posn & 0x00FF);
			}
			if(AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_VCOW_FAULT_INJ_BIST])
			{
				uint8 mask = AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_VCOW_FAULT_INJ_BIST] & 0x000000FF;
				FAULT_COMP_VCOW1->reg.bist.BIST ^= mask;
				mask = (AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_VCOW_FAULT_INJ_BIST] & 0x0000FF00) >> 8;
				FAULT_COMP_VCOW2->reg.bist.BIST ^= mask;
			}
			
			/* VCOW BIST FINISH */
			ADC_STAT2->reg.field.DRDY_VCOW = 1;
		}break;
		
		case AFESIM_BQ79616_DIAG_CBOW_FAULT_INJ_BIST:
		{
			AFESIM_BQ79616_REG_FAULT_COMP_CBOW1_RegType* FAULT_COMP_CBOW1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_CBOW1];
			AFESIM_BQ79616_REG_FAULT_COMP_CBOW2_RegType* FAULT_COMP_CBOW2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_CBOW2];

			sint8 maxVC = AfeSim_Device[phyDevId].activeCell;
			for(maxVC; maxVC > 0; maxVC--)
			{
				if(maxVC > 8)
				{
					FAULT_COMP_CBOW1->reg.bist.BIST |= (1 << (maxVC - 9));
				}
				else
				{
					FAULT_COMP_CBOW2->reg.bist.BIST |= (1 << (maxVC - 1));
				}
			}
			
			if(AfeSim_Device[phyDevId].busbar.bbvc_enable)
			{
				FAULT_COMP_CBOW1->reg.val |= ((AfeSim_Device[phyDevId].busbar.bbvc_posn & 0xFF00) >> 8);
				FAULT_COMP_CBOW2->reg.val |= (AfeSim_Device[phyDevId].busbar.bbvc_posn & 0x00FF);
			}
			if(AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_CBOW_FAULT_INJ_BIST])
			{
				uint8 mask = AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_CBOW_FAULT_INJ_BIST] & 0x000000FF;
				FAULT_COMP_CBOW1->reg.bist.BIST ^= mask;
				mask = (AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_CBOW_FAULT_INJ_BIST] & 0x0000FF00) >> 8;
				FAULT_COMP_CBOW2->reg.bist.BIST ^= mask;
			}
			
			/* CBOW BIST FINISH */
			ADC_STAT2->reg.field.DRDY_CBOW = 1;

		}break;
		
		case AFESIM_BQ79616_DIAG_CBFET_FAULT_INJ_BIST:
		{
			AFESIM_BQ79616_REG_FAULT_COMP_CBFET1_RegType* FAULT_COMP_CBFET1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_CBFET1];
			AFESIM_BQ79616_REG_FAULT_COMP_CBFET2_RegType* FAULT_COMP_CBFET2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_CBFET2];

			sint8 maxVC = AfeSim_Device[phyDevId].activeCell;
			for(maxVC; maxVC > 0; maxVC--)
			{
				if(maxVC > 8)
				{
					FAULT_COMP_CBFET1->reg.bist.BIST |= (1 << (maxVC - 9));
				}
				else
				{
					FAULT_COMP_CBFET2->reg.bist.BIST |= (1 << (maxVC - 1));
				}
			}

			if(AfeSim_Device[phyDevId].busbar.bbvc_enable)
			{
				FAULT_COMP_CBFET1->reg.val |= ((AfeSim_Device[phyDevId].busbar.bbvc_posn & 0xFF00) >> 8);
				FAULT_COMP_CBFET2->reg.val |= (AfeSim_Device[phyDevId].busbar.bbvc_posn & 0x00FF);
			}
			if(AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_CBFET_FAULT_INJ_BIST])
			{
				uint8 mask = AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_CBFET_FAULT_INJ_BIST] & 0x000000FF;
				FAULT_COMP_CBFET1->reg.bist.BIST ^= mask;
				mask = (AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_CBFET_FAULT_INJ_BIST] & 0x0000FF00) >> 8;
				FAULT_COMP_CBFET2->reg.bist.BIST ^= mask;
			}
			
			/* CBFET BIST FINISH */
			ADC_STAT2->reg.field.DRDY_CBFET = 1;

		}break;
		
		case AFESIM_BQ79616_DIAG_GPIO_FAULT_INJ_BIST:
		{
			AFESIM_BQ79616_REG_FAULT_COMP_GPIO_RegType* FAULT_COMP_GPIO = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_GPIO];

			FAULT_COMP_GPIO->reg.bist.BIST = 0xFF;

			if(AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_GPIO_FAULT_INJ_BIST])
			{
				uint8 mask = AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_GPIO_FAULT_INJ_BIST] & 0x000000FF;
				FAULT_COMP_GPIO->reg.bist.BIST ^= mask;
			}
			/* VCOW BIST FINISH */
			ADC_STAT2->reg.field.DRDY_GPIO = 1;
		}break;
	}
	AfeSim_Device[phyDevId].diag.bist.step++;
}

void AfeSim_BQ79616_DiagnoseNormalProcess(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_DIAG_STAT_RegType* DIAG_STAT = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DIAG_STAT];
	AFESIM_BQ79616_REG_ADC_STAT2_RegType* ADC_STAT2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ADC_STAT2];
		
	switch (AfeSim_Device[phyDevId].diag.cycle.type)
	{
		case AFESIM_BQ79616_DIAG_VCCB_FAULT:
		{
			AFESIM_BQ79616_REG_FAULT_COMP_VCCB1_RegType* FAULT_COMP_VCCB1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_VCCB1];
			AFESIM_BQ79616_REG_FAULT_COMP_VCCB2_RegType* FAULT_COMP_VCCB2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_VCCB2];
			//FAULT_COMP_VCCB1->reg.val = 0x00;
			//FAULT_COMP_VCCB2->reg.val = 0x00;
			if(AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_VCCB_FAULT])
			{
				uint8 mask = AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_VCCB_FAULT] & 0x000000FF;
				FAULT_COMP_VCCB1->reg.val |= mask;
				mask = (AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_VCCB_FAULT] & 0x0000FF00) >> 8;
				FAULT_COMP_VCCB2->reg.val |= mask;
			}

			/* VCCB COMP DIAG FINISH */
			ADC_STAT2->reg.field.DRDY_VCCB = 1;
			AfeSim_Device[phyDevId].diag.cycle.diagEnable = FALSE;
		}
		break;
		case AFESIM_BQ79616_DIAG_VCOW_FAULT:
		{
			AFESIM_BQ79616_REG_FAULT_COMP_VCOW1_RegType* FAULT_COMP_VCOW1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_VCOW1];
			AFESIM_BQ79616_REG_FAULT_COMP_VCOW2_RegType* FAULT_COMP_VCOW2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_VCOW2];
			//FAULT_COMP_VCOW1->reg.val = 0x00;
			//FAULT_COMP_VCOW2->reg.val = 0x00;
			if(AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_VCOW_FAULT])
			{
				uint8 mask = AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_VCOW_FAULT] & 0x000000FF;
				FAULT_COMP_VCOW1->reg.val |= mask;
				mask = (AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_VCOW_FAULT] & 0x0000FF00) >> 8;
				FAULT_COMP_VCOW2->reg.val |= mask;
			}

			/* VCOW COMP DIAG FINISH */
			ADC_STAT2->reg.field.DRDY_VCOW = 1;
			AfeSim_Device[phyDevId].diag.cycle.diagEnable = FALSE;
		}
		break;
		case AFESIM_BQ79616_DIAG_CBOW_FAULT:
		{
			AFESIM_BQ79616_REG_FAULT_COMP_CBOW1_RegType* FAULT_COMP_CBOW1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_CBOW1];
			AFESIM_BQ79616_REG_FAULT_COMP_CBOW2_RegType* FAULT_COMP_CBOW2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_CBOW2];
			//FAULT_COMP_CBOW1->reg.val = 0x00;
			//FAULT_COMP_CBOW2->reg.val = 0x00;
			if(AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_CBOW_FAULT])
			{
				uint8 mask = AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_CBOW_FAULT] & 0x000000FF;
				FAULT_COMP_CBOW1->reg.val |= mask;
				mask = (AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_CBOW_FAULT] & 0x0000FF00) >> 8;
				FAULT_COMP_CBOW2->reg.val |= mask;
			}
			/* CBOW COMP DIAG FINISH */
			ADC_STAT2->reg.field.DRDY_CBOW = 1;
			AfeSim_Device[phyDevId].diag.cycle.diagEnable = FALSE;
		}
		break;
		case AFESIM_BQ79616_DIAG_CBFET_FAULT:
		{
			AFESIM_BQ79616_REG_FAULT_COMP_CBFET1_RegType* FAULT_COMP_CBFET1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_CBFET1];
			AFESIM_BQ79616_REG_FAULT_COMP_CBFET2_RegType* FAULT_COMP_CBFET2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_CBFET2];
			//FAULT_COMP_CBFET1->reg.val = 0x00;
			//FAULT_COMP_CBFET2->reg.val = 0x00;
			if(AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_CBFET_FAULT])
			{
				uint8 mask = AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_CBFET_FAULT] & 0x000000FF;
				FAULT_COMP_CBFET1->reg.val |= mask;
				mask = (AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_CBFET_FAULT] & 0x0000FF00) >> 8;
				FAULT_COMP_CBFET2->reg.val |= mask;
			}

			/* CBFET COMP DIAG FINISH */
			ADC_STAT2->reg.field.DRDY_CBFET = 1;
			AfeSim_Device[phyDevId].diag.cycle.diagEnable = FALSE;
		}
		break;
		case AFESIM_BQ79616_DIAG_GPIO_FAULT:
		{
			AFESIM_BQ79616_REG_FAULT_COMP_GPIO_RegType* FAULT_COMP_GPIO = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_GPIO];
			//FAULT_COMP_GPIO->reg.val = 0x00;
			if(AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_GPIO_FAULT])
			{
				uint8 mask = AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[AFESIM_BQ79616_DIAG_GPIO_FAULT] & 0x000000FF;
				FAULT_COMP_GPIO->reg.val |= mask;
			}

			/* GPIO COMP DIAG FINISH */
			ADC_STAT2->reg.field.DRDY_GPIO = 1;
			AfeSim_Device[phyDevId].diag.cycle.diagEnable = FALSE;
		}
		break;
	}
}

void AfeSim_BQ79616_DiagnoseNormalHandler(uint8 phyDevId)
{
	if(AfeSim_Device[phyDevId].diag.diagIsBist == TRUE)
	{
		AfeSim_BQ79616_DiagnoseBistProcess(phyDevId);
		AfeSim_Device[phyDevId].diag.diagIsBist = FALSE;
	}
	else if(AfeSim_Device[phyDevId].diag.cycle.diagEnable == TRUE)
	{
		AfeSim_BQ79616_DiagnoseNormalProcess(phyDevId);
		AfeSim_Device[phyDevId].diag.cycle.step++;
		if(AfeSim_Device[phyDevId].diag.cycle.step >= 20)
		{
			AfeSim_Device[phyDevId].diag.cycle.diagEnable = FALSE;
			AFESIM_BQ79616_REG_FAULT_COMP_MISC_RegType* FAULT_COMP_MISC = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_MISC];
			FAULT_COMP_MISC->reg.field.COMP_ADC_ABORT = 1;
		}
	}
}


void AfeSim_BQ79616_DiagnoseCallBackHandler(uint8 phyDevId)
{
	
}

/* 0x0002 */
void AfeSim_BQ79616_WriteCommandResponse_DEV_CONF(uint8 phyDevId)
{
	
}

/* 0x0003 */
void AfeSim_BQ79616_WriteCommandResponse_ACTIVE_CELL(uint8 phyDevId)
{
	int i;
	uint8 activeCellNum = AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ACTIVE_CELL] + 6;
	/*判断数�?�范�??????*/
	if(activeCellNum > 16)
	{
		activeCellNum = 16;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ACTIVE_CELL] = 0x0A;
	}
	else if(activeCellNum < 6)
	{
		activeCellNum = 6;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ACTIVE_CELL] = 0x00;
	}
	AfeSim_Device[phyDevId].activeCell = activeCellNum;

	AfeMan->cellNum[phyDevId] = activeCellNum;
	AfeMan->cellNum[AFESIM_DEVID_0] = 0;

	/* 刷新AFE电芯数量配置 */
	for(i = 1; i <= AfeMan->afeChipRealNum; i++)
	{
		AfeMan->cellNumSum[i] = AfeMan->cellNumSum[i-1] + AfeMan->cellNum[i];
	}
}


/* 0x0005 */
void AfeSim_BQ79616_WriteCommandResponse_BBVC_POSN1(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_BBVC_POSN1_RegType* BBVC_POSN1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_BBVC_POSN1];
	uint16 bbvc_mask = BBVC_POSN1->reg.val;
	AfeSim_Device[phyDevId].busbar.bbvc_posn &= 0xFF00;
	AfeSim_Device[phyDevId].busbar.bbvc_posn |= bbvc_mask;
	if(AfeSim_Device[phyDevId].busbar.bbvc_posn)
	{
		AfeSim_Device[phyDevId].busbar.busbar_enable = 1;
	}
	else
	{
		AfeSim_Device[phyDevId].busbar.busbar_enable = 0;
	}
}

/* 0x0006 */
void AfeSim_BQ79616_WriteCommandResponse_BBVC_POSN2(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_BBVC_POSN2_RegType* BBVC_POSN2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_BBVC_POSN2];
	uint16 bbvc_mask = BBVC_POSN2->reg.val << 8;
	AfeSim_Device[phyDevId].busbar.bbvc_posn &= 0x00FF;
	AfeSim_Device[phyDevId].busbar.bbvc_posn |= bbvc_mask;
	if(AfeSim_Device[phyDevId].busbar.bbvc_posn)
	{
		AfeSim_Device[phyDevId].busbar.busbar_enable = 1;
	}
	else
	{
		AfeSim_Device[phyDevId].busbar.busbar_enable = 0;
	}
}

/* 0x0007 */
void AfeSim_BQ79616_WriteCommandResponse_ADC_CONF1(uint8 phyDevId)
{
	
}

/* 0x000E */
void AfeSim_BQ79616_WriteCommandResponse_GPIO_CONF1(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_GPIO_CONF1_RegType* GPIO_CONF1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_GPIO_CONF1];
	AFESIM_BQ79616_REG_GPIO_STAT_RegType* GPIO_STAT = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_GPIO_STAT];

	AfeSim_Device[phyDevId].gpio[0].mode = GPIO_CONF1->reg.field.GPIO1;
	AfeSim_Device[phyDevId].gpio[1].mode = GPIO_CONF1->reg.field.GPIO2;
	switch(GPIO_CONF1->reg.field.GPIO1)
	{
		case 0x04:
			GPIO_STAT->reg.field.GPIO1 = 1;
			break;
		case 0x05:
			GPIO_STAT->reg.field.GPIO1 = 0;
			break;
	}
	switch(GPIO_CONF1->reg.field.GPIO2)
	{
		case 0x04:
			GPIO_STAT->reg.field.GPIO2 = 1;
			break;
		case 0x05:
			GPIO_STAT->reg.field.GPIO2 = 0;
			break;
	}
}

/* 0x000F */
void AfeSim_BQ79616_WriteCommandResponse_GPIO_CONF2(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_GPIO_CONF2_RegType* GPIO_CONF2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_GPIO_CONF2];
	AFESIM_BQ79616_REG_GPIO_STAT_RegType* GPIO_STAT = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_GPIO_STAT];
	
	AfeSim_Device[phyDevId].gpio[2].mode = GPIO_CONF2->reg.field.GPIO3;
	AfeSim_Device[phyDevId].gpio[3].mode = GPIO_CONF2->reg.field.GPIO4;
	switch(GPIO_CONF2->reg.field.GPIO3)
	{
		case 0x04:
			GPIO_STAT->reg.field.GPIO3 = 1;
			break;
		case 0x05:
			GPIO_STAT->reg.field.GPIO3 = 0;
			break;
	}
	switch(GPIO_CONF2->reg.field.GPIO4)
	{
		case 0x04:
			GPIO_STAT->reg.field.GPIO4 = 1;
			break;
		case 0x05:
			GPIO_STAT->reg.field.GPIO4 = 0;
			break;
	}
}

/* 0x0010 */
void AfeSim_BQ79616_WriteCommandResponse_GPIO_CONF3(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_GPIO_CONF3_RegType* GPIO_CONF3 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_GPIO_CONF3];
	AFESIM_BQ79616_REG_GPIO_STAT_RegType* GPIO_STAT = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_GPIO_STAT];
	
	AfeSim_Device[phyDevId].gpio[4].mode = GPIO_CONF3->reg.field.GPIO5;
	AfeSim_Device[phyDevId].gpio[5].mode = GPIO_CONF3->reg.field.GPIO6;
	switch(GPIO_CONF3->reg.field.GPIO5)
	{
		case 0x04:
			GPIO_STAT->reg.field.GPIO5 = 1;
			break;
		case 0x05:
			GPIO_STAT->reg.field.GPIO5 = 0;
			break;
	}
	switch(GPIO_CONF3->reg.field.GPIO6)
	{
		case 0x04:
			GPIO_STAT->reg.field.GPIO6 = 1;
			break;
		case 0x05:
			GPIO_STAT->reg.field.GPIO6 = 0;
			break;
	}
}

/* 0x0011 */
void AfeSim_BQ79616_WriteCommandResponse_GPIO_CONF4(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_GPIO_CONF4_RegType* GPIO_CONF4 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_GPIO_CONF4];
	AFESIM_BQ79616_REG_GPIO_STAT_RegType* GPIO_STAT = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_GPIO_STAT];
	
	AfeSim_Device[phyDevId].gpio[6].mode = GPIO_CONF4->reg.field.GPIO7;
	AfeSim_Device[phyDevId].gpio[7].mode = GPIO_CONF4->reg.field.GPIO8;
	switch(GPIO_CONF4->reg.field.GPIO7)
	{
		case 0x04:
			GPIO_STAT->reg.field.GPIO7 = 1;
			break;
		case 0x05:
			GPIO_STAT->reg.field.GPIO7 = 0;
			break;
	}
	switch(GPIO_CONF4->reg.field.GPIO8)
	{
		case 0x04:
			GPIO_STAT->reg.field.GPIO8 = 1;
			break;
		case 0x05:
			GPIO_STAT->reg.field.GPIO8 = 0;
			break;
	}
}

/* 0x002A */
void AfeSim_BQ79616_WriteCommandResponse_BBP_LOC(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_BBP_LOC_RegType* BBP_LOC = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_BBP_LOC];
	AfeSim_Device[phyDevId].busbar.bbp_loc = BBP_LOC->reg.field.LOC;
	if(AfeSim_Device[phyDevId].busbar.bbp_loc)
	{
		AfeSim_Device[phyDevId].busbar.busbar_enable = 1;
	}
	else
	{
		AfeSim_Device[phyDevId].busbar.busbar_enable = 0;
	}
}

/* 0x030A */
void AfeSim_BQ79616_WriteCommandResponse_CONTROL2(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_CONTROL2_RegType* CONTROL2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_CONTROL2];
	if(CONTROL2->reg.field.TSREF_EN)
	{
		AfeSim_Device[phyDevId].power.tsrefIsOn = 1;
		AfeSim_Device[phyDevId].power.TSREF_100uV = 50000;
		//AfeSim_Device[phyDevId].power.TSREFbias_100uV = AfeSim_Device[phyDevId].power.TSREF_100uV;
	}
	else
	{
		AfeSim_Device[phyDevId].power.tsrefIsOn = 0;
		AfeSim_Device[phyDevId].power.TSREF_100uV = 0;
		//AfeSim_Device[phyDevId].power.TSREFbias_100uV = AfeSim_Device[phyDevId].power.TSREF_100uV;
	}
}

/* 0x030D */
void AfeSim_BQ79616_WriteCommandResponse_ADC_CTRL1(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_ADC_CTRL1_RegType* ADC_CTRL1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ADC_CTRL1];
	AFESIM_BQ79616_REG_ADC_STAT1_RegType* ADC_STAT1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ADC_STAT1];
	AFESIM_BQ79616_REG_ADC_STAT2_RegType* ADC_STAT2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ADC_STAT2];
	AFESIM_BQ79616_REG_DEV_STAT_RegType* DEV_STAT = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DEV_STAT];
	
	if(ADC_CTRL1->reg.field.MAIN_GO)
	{
		ADC_CTRL1->reg.field.MAIN_GO = 0;
		ADC_STAT1->reg.clear.MAIN_GO_Clear = 0;
		ADC_STAT2->reg.clear.MAIN_GO_Clear = 0;
		DEV_STAT->reg.field.MAIN_RUN = 1;
		/*
			閲囨牱閫昏緫鏈疄鐜�??????
		*/
		ADC_STAT2->reg.field.DRDY_LPF = 1;
		ADC_STAT1->reg.field.DRDY_MAIN_ADC = 1;
	}
}

/* 0x030E */
void AfeSim_BQ79616_WriteCommandResponse_ADC_CTRL2(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_ADC_CTRL2_RegType* ADC_CTRL2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ADC_CTRL2];
	/* 
		Internal Logic Not Implemented
	*/
}

/* 0x030F */
void AfeSim_BQ79616_WriteCommandResponse_ADC_CTRL3(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_ADC_CTRL3_RegType* ADC_CTRL3 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ADC_CTRL3];
	AFESIM_BQ79616_REG_ADC_STAT1_RegType* ADC_STAT1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ADC_STAT1];
	AFESIM_BQ79616_REG_DEV_STAT_RegType* DEV_STAT = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DEV_STAT];

	if(ADC_CTRL3->reg.field.AUX_GO)
	{
		ADC_CTRL3->reg.field.AUX_GO = 0;
		ADC_STAT1->reg.clear.AUX_GO_Clear = 0;
		DEV_STAT->reg.field.AUX_RUN = 1;
		/*
			閲囨牱閫昏緫鏈疄鐜�??????
		*/
		ADC_STAT1->reg.field.DRDY_AUX_GPIO = 1;
		//ADC_STAT1.reg.field.DRDY_AUX_CELL = 1;
		ADC_STAT1->reg.field.DRDY_AUX_MISC = 1;
	}
}

/* 0x032C */
void AfeSim_BQ79616_WriteCommandResponse_OVUV_CTRL(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_OVUV_CTRL_RegType* 		OVUV_CTRL = 		&AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_OVUV_CTRL];
	AFESIM_BQ79616_REG_DIAG_STAT_RegType* 		DIAG_STAT = 		&AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DIAG_STAT];
	AFESIM_BQ79616_REG_DEV_STAT_RegType* 		DEV_STAT = 			&AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DEV_STAT];
	AFESIM_BQ79616_REG_OV_THRESH_RegType* 		OV_THRESH = 		&AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_OV_THRESH];
	AFESIM_BQ79616_REG_UV_THRESH_RegType* 		UV_THRESH = 		&AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_UV_THRESH];
	AFESIM_BQ79616_REG_VCB_DONE_THRESH_RegType* VCB_DONE_THRESH =  	&AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_VCB_DONE_THRESH];
	
	if(OVUV_CTRL->reg.field.OVUV_GO)
	{
		OVUV_CTRL->reg.field.OVUV_GO = 0;
		uint8 ov_thr = OV_THRESH->reg.field.OV_THR;
		if(ov_thr < 0x02)
		{
			AfeSim_Device[phyDevId].power.Aux_OVDAC_100uV = 27000;
		}
		else if(ov_thr <= 0x0E)
		{
			AfeSim_Device[phyDevId].power.Aux_OVDAC_100uV = 27000 + (ov_thr-0x02)*250;
		}
		else if(ov_thr < 0x12)
		{
			AfeSim_Device[phyDevId].power.Aux_OVDAC_100uV = 27000;
		}
		else if(ov_thr <= 0x1E)
		{
			AfeSim_Device[phyDevId].power.Aux_OVDAC_100uV = 35000 + (ov_thr-0x12)*250;
		}
		else if(ov_thr < 0x22)
		{
			AfeSim_Device[phyDevId].power.Aux_OVDAC_100uV = 27000;
		}
		else if(ov_thr <= 0x2E)
		{
			AfeSim_Device[phyDevId].power.Aux_OVDAC_100uV = 41750 + (ov_thr-0x22)*250;
		}
		else
		{
			AfeSim_Device[phyDevId].power.Aux_OVDAC_100uV = 27000;
		}
		
		uint8 uv_thr = UV_THRESH->reg.field.UV_THR;
		if(uv_thr <= 0x26)
		{
			AfeSim_Device[phyDevId].power.Aux_UVDAC_100uV = 12000 + uv_thr*500;
		}
		else
		{
			AfeSim_Device[phyDevId].power.Aux_UVDAC_100uV = 31000;
		}

		uint8 vcbdone_thr = VCB_DONE_THRESH->reg.field.CB_THR;
		if(vcbdone_thr == 0)
		{
			AfeSim_Device[phyDevId].power.Aux_VCBDONE_DAC_100uV = 40000;
		}
		else
		{
			AfeSim_Device[phyDevId].power.Aux_VCBDONE_DAC_100uV = 24500 + vcbdone_thr*250;
		}
		
		switch(OVUV_CTRL->reg.field.OVUV_MODE)
		{
		case 0x00:
			DEV_STAT->reg.field.OVUV_RUN = 0;
			break;
		case 0x01:
			DIAG_STAT->reg.field.DRDY_OVUV = 0;
			DEV_STAT->reg.field.OVUV_RUN = 1;
			/* 
				Internal Diag Logic not implemented
			*/
			DIAG_STAT->reg.field.DRDY_OVUV = 1;
			break;
		case 0x02:
			DIAG_STAT->reg.field.DRDY_BIST_OVUV = 0;
			DEV_STAT->reg.field.OVUV_RUN = 1;
			/* 
				BIST Internal Diag Logic not implemented
			*/
			DEV_STAT->reg.field.OVUV_RUN = 0;
			DIAG_STAT->reg.field.DRDY_BIST_OVUV = 1;
			break;
		case 0x03:
			DEV_STAT->reg.field.OVUV_RUN = 1;
			/* 
				Internal Diag Logic not implemented
			*/
			break;
		}
	}
}

/* 0x032D */
void AfeSim_BQ79616_WriteCommandResponse_OTUT_CTRL(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_OTUT_CTRL_RegType* OTUT_CTRL = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_OTUT_CTRL];
	AFESIM_BQ79616_REG_DIAG_STAT_RegType* DIAG_STAT = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DIAG_STAT];
	AFESIM_BQ79616_REG_DEV_STAT_RegType* DEV_STAT = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DEV_STAT];
	AFESIM_BQ79616_REG_OTUT_THRESH_RegType* OTUT_THRESH = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_OTUT_THRESH];
	
	if(OTUT_CTRL->reg.field.OTUT_GO)
	{
		OTUT_CTRL->reg.field.OTUT_GO = 0;
		float tsref = AfeSim_Device[phyDevId].power.TSREF_100uV;
		float ut_thr = 0.66 + 0.02 * (float)OTUT_THRESH->reg.field.UT_THR;
		float ot_thr = 0.10 + 0.01 * (float)OTUT_THRESH->reg.field.OT_THR;
		AfeSim_Device[phyDevId].power.Aux_UTDAC_100uV = (sint32)(tsref * ut_thr);
		AfeSim_Device[phyDevId].power.Aux_OT_OTCB_DAC_100uV = (sint32)(tsref * ot_thr);
		switch(OTUT_CTRL->reg.field.OTUT_MODE)
		{
		case 0x00:
			DEV_STAT->reg.field.OTUT_RUN = 0;
			break;
		case 0x01:
			DIAG_STAT->reg.field.DRDY_OTUT = 0;
			DEV_STAT->reg.field.OTUT_RUN = 1;
			/* 
				Internal Diag Logic not Implemented
			*/
			DIAG_STAT->reg.field.DRDY_OTUT = 1;
			break;
		case 0x02:
			DIAG_STAT->reg.field.DRDY_BIST_OTUT = 0;
			DEV_STAT->reg.field.OTUT_RUN = 1;
			/* 
				Internal Diag Logic not Implemented
			*/
			DEV_STAT->reg.field.OTUT_RUN = 0;
			DIAG_STAT->reg.field.DRDY_BIST_OTUT = 1;
			break;
		case 0x03:
			DEV_STAT->reg.field.OTUT_RUN = 1;
			break;
		}
	}

}

/* 0x032E */
void AfeSim_BQ79616_WriteCommandResponse_BAL_CTRL1(uint8 phyDevId)
{
	/*
		Warning: Cell Balance Logic Not Implemented
	*/
}

/* 0x032F */
void AfeSim_BQ79616_WriteCommandResponse_BAL_CTRL2(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_BAL_CTRL2_RegType* BAL_CTRL2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_BAL_CTRL2];
	AFESIM_BQ79616_REG_BAL_STAT_RegType* BAL_STAT = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_BAL_STAT];

	if(BAL_CTRL2->reg.field.BAL_GO)
	{
		BAL_CTRL2->reg.field.BAL_GO = 0;
		BAL_STAT->reg.clear.BAL_GO_Clear = 0;
		/*
			Warning: Cell Balance Logic Not Implemented
		*/
		BAL_STAT->reg.field.CB_RUN = 1;
	}
}


/* 0x0331 */
void AfeSim_BQ79616_WriteCommandResponse_FAULT_RST1(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_FAULT_RST1_RegType* FAULT_RST1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_RST1];
	uint8 simEnable;
	RTE_Read_DeviceRegSimEnable(&simEnable);
	if(!simEnable)
		return;
	if(FAULT_RST1->reg.field.RST_PWR)
	{
		FAULT_RST1->reg.field.RST_PWR = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_PWR1] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_PWR2] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_PWR3] = 0;
	}
	if(FAULT_RST1->reg.field.RST_SYS)
	{
		FAULT_RST1->reg.field.RST_SYS = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_SYS] = 0;
	}
	if(FAULT_RST1->reg.field.RST_COMP)
	{
		FAULT_RST1->reg.field.RST_COMP = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_GPIO] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_VCCB1] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_VCCB2] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_VCOW1] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_VCOW2] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_CBOW1] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_CBOW2] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_CBFET1] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_CBFET2] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_MISC] = 0;
	}
	if(FAULT_RST1->reg.field.RST_OV)
	{
		FAULT_RST1->reg.field.RST_OV = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_OV1] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_OV2] = 0;
	}
	if(FAULT_RST1->reg.field.RST_UV)
	{
		FAULT_RST1->reg.field.RST_UV = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_UV1] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_UV2] = 0;
	}
	if(FAULT_RST1->reg.field.RST_OT)
	{
		FAULT_RST1->reg.field.RST_OT = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_OT] = 0;
	}
	if(FAULT_RST1->reg.field.RST_UT)
	{
		FAULT_RST1->reg.field.RST_UT = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_UT] = 0;
	}
	if(FAULT_RST1->reg.field.RST_PROT)
	{
		FAULT_RST1->reg.field.RST_PROT = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_PROT1] = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_PROT2] = 0;
	}
}

/* 0x0332 */
void AfeSim_BQ79616_WriteCommandResponse_FAULT_RST2(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_FAULT_RST2_RegType* FAULT_RST2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_RST2];
	uint8 simEnable;
	RTE_Read_DeviceRegSimEnable(&simEnable);
	if(!simEnable)
		return;
	if(FAULT_RST2->reg.field.RST_COMM1)
	{
		FAULT_RST2->reg.field.RST_COMM1 = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMM1] = 0;
	}
	if(FAULT_RST2->reg.field.RST_COMM2)
	{
		FAULT_RST2->reg.field.RST_COMM2 = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMM2] = 0;
	}
	if(FAULT_RST2->reg.field.RST_COMM3_HB)
	{
		FAULT_RST2->reg.field.RST_COMM3_HB = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMM3] &= ~0x03;
	}
	if(FAULT_RST2->reg.field.RST_COMM3_FTONE)
	{
		FAULT_RST2->reg.field.RST_COMM3_FTONE = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMM3] &= ~0x04;
	}
	if(FAULT_RST2->reg.field.RST_COMM3_FCOMM)
	{
		FAULT_RST2->reg.field.RST_COMM3_FCOMM = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMM3] &= ~0x08;
	}
	if(FAULT_RST2->reg.field.RST_OTP_DATA)
	{
		FAULT_RST2->reg.field.RST_OTP_DATA = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_OTP] &= ~0x60;
	}
	if(FAULT_RST2->reg.field.RST_OTP_CRC)
	{
		FAULT_RST2->reg.field.RST_OTP_CRC = 0;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_OTP] &= ~0x18;
	}

}



/* 0x0337 */
void AfeSim_BQ79616_WriteCommandResponse_DIAG_PWR_CTRL(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_DIAG_PWR_CTRL_RegType* DIAG_PWR_CTRL = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DIAG_PWR_CTRL];
	AFESIM_BQ79616_REG_DIAG_STAT_RegType* DIAG_STAT = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DIAG_STAT];

	if(DIAG_PWR_CTRL->reg.field.PWR_BIST_GO)
	{
		DIAG_PWR_CTRL->reg.field.PWR_BIST_GO = 0;
		DIAG_STAT->reg.field.DRDY_BIST_PWR = 0;
		AfeSim_Device[phyDevId].diag.diagIsBist = 1;
		AfeSim_Device[phyDevId].diag.bist.type = AFESIM_BQ79616_DIAG_PWR_BIST;
		AfeSim_Device[phyDevId].diag.bist.step = 0;
		AfeSim_BQ79616_DiagnoseCallBackHandler(phyDevId);
	}
}

/* 0x033C */
void AfeSim_BQ79616_WriteCommandResponse_DIAG_COMP_CTRL3(uint8 phyDevId)
{
	AFESIM_BQ79616_REG_DIAG_COMP_CTRL3_RegType* DIAG_COMP_CTRL3 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DIAG_COMP_CTRL3];
	AFESIM_BQ79616_REG_DIAG_COMP_CTRL4_RegType* DIAG_COMP_CTRL4 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DIAG_COMP_CTRL4];
	AFESIM_BQ79616_REG_FAULT_COMP_MISC_RegType* FAULT_COMP_MISC = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMP_MISC];
	AFESIM_BQ79616_REG_ADC_STAT2_RegType* ADC_STAT2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ADC_STAT2];

	if(DIAG_COMP_CTRL3->reg.field.COMP_ADC_GO)
	{
		DIAG_COMP_CTRL3->reg.field.COMP_ADC_GO = 0;
		ADC_STAT2->reg.clear.COMP_ADC_GO_Clear = 0;
		switch(DIAG_COMP_CTRL3->reg.field.COMP_ADC_SEL)
		{
			case 0x01:
				if(DIAG_COMP_CTRL4->reg.field.COMP_FAULT_INJ)
				{
					AfeSim_Device[phyDevId].diag.diagIsBist = 1;
					AfeSim_Device[phyDevId].diag.bist.type = AFESIM_BQ79616_DIAG_VCCB_FAULT_INJ_BIST;
					AfeSim_Device[phyDevId].diag.bist.step = 0;
				}
				else
				{
					AfeSim_Device[phyDevId].diag.cycle.diagEnable = 1;
					AfeSim_Device[phyDevId].diag.cycle.type = AFESIM_BQ79616_DIAG_VCCB_FAULT;
					AfeSim_Device[phyDevId].diag.cycle.step = 0;
				}
				AfeSim_BQ79616_DiagnoseCallBackHandler(phyDevId);
				break;
			case 0x02:
				if(DIAG_COMP_CTRL4->reg.field.COMP_FAULT_INJ)
				{
					AfeSim_Device[phyDevId].diag.diagIsBist = 1;
					AfeSim_Device[phyDevId].diag.bist.type = AFESIM_BQ79616_DIAG_VCOW_FAULT_INJ_BIST;
					AfeSim_Device[phyDevId].diag.bist.step = 0;
				}
				else
				{
					AfeSim_Device[phyDevId].diag.cycle.diagEnable = 1;
					AfeSim_Device[phyDevId].diag.cycle.type = AFESIM_BQ79616_DIAG_VCOW_FAULT;
					AfeSim_Device[phyDevId].diag.cycle.step = 0;
				}
				AfeSim_BQ79616_DiagnoseCallBackHandler(phyDevId);
				break;
			case 0x03:
				if(DIAG_COMP_CTRL4->reg.field.COMP_FAULT_INJ)
				{
					AfeSim_Device[phyDevId].diag.diagIsBist = 1;
					AfeSim_Device[phyDevId].diag.bist.type = AFESIM_BQ79616_DIAG_CBOW_FAULT_INJ_BIST;
					AfeSim_Device[phyDevId].diag.bist.step = 0;
				}
				else
				{
					AfeSim_Device[phyDevId].diag.cycle.diagEnable = 1;
					AfeSim_Device[phyDevId].diag.cycle.type = AFESIM_BQ79616_DIAG_CBOW_FAULT;
					AfeSim_Device[phyDevId].diag.cycle.step = 0;
				}
				AfeSim_BQ79616_DiagnoseCallBackHandler(phyDevId);
				break;
			case 0x04:
				if(DIAG_COMP_CTRL4->reg.field.COMP_FAULT_INJ)
				{
					AfeSim_Device[phyDevId].diag.diagIsBist = 1;
					AfeSim_Device[phyDevId].diag.bist.type = AFESIM_BQ79616_DIAG_CBFET_FAULT_INJ_BIST;
					AfeSim_Device[phyDevId].diag.bist.step = 0;
				}
				else
				{
					AfeSim_Device[phyDevId].diag.cycle.diagEnable = 1;
					AfeSim_Device[phyDevId].diag.cycle.type = AFESIM_BQ79616_DIAG_CBFET_FAULT;
					AfeSim_Device[phyDevId].diag.cycle.step = 0;
				}
				AfeSim_BQ79616_DiagnoseCallBackHandler(phyDevId);
				break;
			case 0x05:
				if(DIAG_COMP_CTRL4->reg.field.COMP_FAULT_INJ)
				{
					AfeSim_Device[phyDevId].diag.diagIsBist = 1;
					AfeSim_Device[phyDevId].diag.bist.type = AFESIM_BQ79616_DIAG_GPIO_FAULT_INJ_BIST;
					AfeSim_Device[phyDevId].diag.bist.step = 0;
				}
				else
				{
					AfeSim_Device[phyDevId].diag.cycle.diagEnable = 1;
					AfeSim_Device[phyDevId].diag.cycle.type = AFESIM_BQ79616_DIAG_GPIO_FAULT;
					AfeSim_Device[phyDevId].diag.cycle.step = 0;
				}
				AfeSim_BQ79616_DiagnoseCallBackHandler(phyDevId);
				break;
		}
		if(DIAG_COMP_CTRL4->reg.field.LPF_FAULT_INJ)
		{
			/* Judge If LPF Fault Inj Bist Will Fail */
			if(AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_LPF_FAULT_INJ_BIST])
			{
				FAULT_COMP_MISC->reg.field.LPF_FAIL = 0;
			}
			else
			{
				FAULT_COMP_MISC->reg.field.LPF_FAIL = 1;
			}
		}
	}
}


/* 0x033C */
void AfeSim_BQ79616_WriteCommandResponse_DIAG_COMP_CTRL4(uint8 phyDevId)
{
	(void)phyDevId;
	/* NO OP */
}

void AfeSim_BQ79616_CommErrorHandler(uint8 phyDevId, uint8 errorType, uint8 commDir)
{
	/*
	3 = ������Ӧ֡ʧ�� FAULT_COMM2 COMH_TR
	2 = ������Ӧ֡ʧ�� FAULT_COMM2 COMH_TR
	1 = ����ָ��֡ʧ�� FAULT_COMM2 COMH_TR
	0 = Bit����          FAULT_COMM2 COMH_BIT
	*/
	commDir = commDir >= 1 ? 1 : 0;
	AFESIM_BQ79616_REG_FAULT_COMM2_RegType* FAULT_COMM2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_FAULT_COMM2];
	if(errorType <= 3)
	{
		if(commDir == AFESIM_DEVICE_USE_COMH)
		{
			switch (errorType)
			{
			case 0: FAULT_COMM2->reg.field.COMH_BIT = 1; break;
			case 1: FAULT_COMM2->reg.field.COMH_RC = 1; break;
			case 2: 
			{
				if(FALSE == AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_CRC_FLIP_FAULT])
				{
					FAULT_COMM2->reg.field.COMH_RR = 1;
				}
			}
			break;
			case 3: FAULT_COMM2->reg.field.COMH_TR = 1; break;
			}
		}
		else
		{
			switch(errorType)
			{
			
				case 0: FAULT_COMM2->reg.field.COML_BIT = 1; break;
				case 1: FAULT_COMM2->reg.field.COML_RC = 1; break;
				case 2: 
				{
					if(FALSE == AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_CRC_FLIP_FAULT])
					{
						FAULT_COMM2->reg.field.COML_RR = 1;
					}
				}
				break;
				case 3: FAULT_COMM2->reg.field.COML_TR = 1; break;
			}
		}
	}
}



void AfeSim_BQ79616_Init(void)
{

}

void AfeSim_BQ79616_Fsm(void)
{
	//AfeSim_BQ79616_PhysicalSimulation();
	uint8 phyDevId = 1;
	for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
	{
		AfeSim_BQ79616_DiagnoseNormalHandler(phyDevId);
	}
}


