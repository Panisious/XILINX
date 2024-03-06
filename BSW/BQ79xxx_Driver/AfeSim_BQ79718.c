/*
 * Afe_133_BQ79718.c
 *
 *  Created on: 2024年1月6日
 *      Author: Administrator
 */

#include "AfeSim_BQ79718.h"

AfeSim_DeviceGpioUseType AfeSim_BQ79718_GpioInitConfig[3][AFESIM_MAX_GPIO_NUM] =
{
		{	/*BQ79600*/
				AFESIM_DEVICE_GPIO_NOUSE,			AFESIM_DEVICE_GPIO_NOUSE,
				AFESIM_DEVICE_GPIO_NOUSE,			AFESIM_DEVICE_GPIO_NOUSE,
				AFESIM_DEVICE_GPIO_NOUSE,			AFESIM_DEVICE_GPIO_NOUSE,
				AFESIM_DEVICE_GPIO_NOUSE,			AFESIM_DEVICE_GPIO_NOUSE,
				AFESIM_DEVICE_GPIO_NOUSE,			AFESIM_DEVICE_GPIO_NOUSE,
				AFESIM_DEVICE_GPIO_NOUSE,
		},
		{	/*BQ79758*/
				AFESIM_DEVICE_GPIO_MODULE_TEMP,		AFESIM_DEVICE_GPIO_MODULE_TEMP,
				AFESIM_DEVICE_GPIO_MODULE_TEMP,		AFESIM_DEVICE_GPIO_SHUNT_TEMP,
				AFESIM_DEVICE_GPIO_SHUNT_TEMP,		AFESIM_DEVICE_GPIO_CURRENT,
				AFESIM_DEVICE_GPIO_BALANCE_TEMP,	AFESIM_DEVICE_GPIO_BALANCE_TEMP,
				AFESIM_DEVICE_GPIO_TSREF,			AFESIM_DEVICE_GPIO_NOUSE,
				AFESIM_DEVICE_GPIO_NOUSE
		},
		{	/*BQ79718*/
				AFESIM_DEVICE_GPIO_MODULE_TEMP,		AFESIM_DEVICE_GPIO_MODULE_TEMP,
				AFESIM_DEVICE_GPIO_MODULE_TEMP,		AFESIM_DEVICE_GPIO_MODULE_TEMP,
				AFESIM_DEVICE_GPIO_MODULE_TEMP,		AFESIM_DEVICE_GPIO_MODULE_TEMP,
				AFESIM_DEVICE_GPIO_BALANCE_TEMP,	AFESIM_DEVICE_GPIO_BALANCE_TEMP,
				AFESIM_DEVICE_GPIO_TSREF,			AFESIM_DEVICE_GPIO_NOUSE,
				AFESIM_DEVICE_GPIO_NOUSE
		},
};

static void AfeSim_BQ79718_FaultSimulation_VccbOpenWire(void);
static void AfeSim_BQ79718_FaultSimulation(void);
static void AfeSim_BQ79718_PhysicalSimualtion_CellVoltage(void);
static void AfeSim_BQ79718_PhysicalSimualtion_GpioVoltage(void);
static void AfeSim_BQ79718_PhysicalSimualtion_DieTemperature(void);
static void AfeSim_BQ79718_PhysicalSimulation(void);
void AfeSim_BQ79718_Fsm(void);

/**/

void AfeSim_BQ79718_WriteCommandResponse_COMM_CTRL(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir)
{
	AFESIM_BQ79718_REG_COMM_CTRL_RegType* COMM_CTRL = &AfeSim_Reg[phyDevId][AFESIM_BQ79718_REG_COMM_CTRL];

	if(COMM_CTRL->reg.field.TOP_STACK)
	{
		AFESIM_BQ79600_REG_CONTROL1_RegType* CONTROL1 = &AfeSim_Reg[phyDevId][AFESIM_BQ79600_REG_CONTROL1];

		if(CONTROL1->reg.field.DIR_SEL == AFESIM_DEVICE_USE_COML)
		{
			BQ79600Man->stack.StackNum[1] = AfeMan->afeChipRealNum - phyDevId + 1;
		}
		else
		{
			BQ79600Man->stack.StackNum[0] = phyDevId;
		}
	}
}

void AfeSim_BQ79718_WriteCommandResponse_DEV_CONF2(uint8 phyDevId)
{
	int i;
	uint8 activeCellNum = (AfeSim_Reg[phyDevId][AFESIM_BQ79718_REG_DEV_CONF2] & 0x0F) + 6;
	/*判断数值范围*/
	if(activeCellNum > 18)
	{
		AfeSim_Reg[phyDevId][AFESIM_BQ79718_REG_DEV_CONF2] = 0x0C;
	}
	else if(activeCellNum < 9)
	{
		AfeSim_Reg[phyDevId][AFESIM_BQ79718_REG_DEV_CONF2] = 0x03;
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

/* AfeSimX1_BQ79718_WriteCommandResponse_ADC_CTRL2
 * ADC_CTRL2 指令响应
 * X1 已经升级为位域操作 */
void AfeSimX1_BQ79718_WriteCommandResponse_ADC_CTRL2(uint8 physicalId)
{
	AFESIM_BQ79718_REG_ADC_CTRL2_RegType* ADC_CTRL2 = &AfeSim_Reg[physicalId][AFESIM_BQ79718_REG_ADC_CTRL2];

	/* BQ79758 */
	if(physicalId == AFESIM_DEVID_1)
	{/*warning need check ADC_CTRL2 behavior */
		if(ADC_CTRL2->reg.field.ADC_GO)
		{
			printf("Dev %d ADC Enable\r\n", physicalId);
			AfeSim_Reg[physicalId][AFESIM_BQ79718_REG_ADC_DATA_RDY] = 0x13;	/*设置ADC和CSADC就绪*/
			ADC_CTRL2->reg.field.ADC_GO = 0;
		}
	}
	else
	{
		if(ADC_CTRL2->reg.field.ADC_GO)
		{
			printf("Dev %d ADC Enable\r\n", physicalId);
			AfeSim_Reg[physicalId][AFESIM_BQ79718_REG_ADC_DATA_RDY] = 0x03;	/*设置ADC就绪*/
			ADC_CTRL2->reg.field.ADC_GO = 0;
		}
	}
}

static void AfeSim_BQ79718_FaultSimulation_VccbOpenWire(void)
{

}

static void AfeSim_BQ79718_FaultSimulation(void)
{
	AfeSim_BQ79718_FaultSimulation_VccbOpenWire();
}

static void AfeSim_BQ79718_PhysicalSimualtion_CellVoltage(void)
{

}

static void AfeSim_BQ79718_PhysicalSimualtion_GpioVoltage(void)
{

}

static void AfeSim_BQ79718_PhysicalSimualtion_DieTemperature(void)
{

}

static void AfeSim_BQ79718_PhysicalSimulation(void)
{
	AfeSim_BQ79718_PhysicalSimualtion_CellVoltage();
	AfeSim_BQ79718_PhysicalSimualtion_GpioVoltage();
	AfeSim_BQ79718_PhysicalSimualtion_DieTemperature();
}


void AfeSim_BQ79718_Fsm(void)
{
	//AfeSim_BQ79718_FaultSimulation();
	//AfeSim_BQ79718_PhysicalSimulation();
}
