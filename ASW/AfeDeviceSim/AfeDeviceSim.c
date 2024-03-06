/*
 * AfeDeviceSim.c
 *
 *  Created on: 2024年1月27日
 *      Author: Administrator
 */

#include "AfeDeviceSim.h"
#include "BatterySim.h"

extern sint32 BatteryBusCurrent_mA;

sint32 AfeDeviceSim_ModuleVolt_mV[AFESIM_MAX_AFE_NUM];
sint32 AfeDeviceSim_VCellActSum_mV[AFESIM_MAX_AFE_NUM];


/* AFE仿真默认使用的模组温度RT表 */
sint32 ModuleNTCtable_temp_mC_Default1[AFEDEVSIM_TEMP_RT_TABLE_SIZE] =
{
		-50000,
		-41000,-40000, -39000,-30000,-20000,     -10000,00000, 10000, 20000, 30000,
		 40000, 50000,  60000, 70000, 80000,      90000,100000,110000,120000,124000,
		125000, 126000, 150000,999000,999000,     999000,999000,999000,999000,999000,
		999000,
};
/* AFE仿真默认使用的模组温度RT表 */
sint32 ModuleNTCtable_volt_Default1[AFEDEVSIM_TEMP_RT_TABLE_SIZE] =
{
		9999,
		9613, 9587, 9560, 9237, 8686,    		7888, 6862, 5706, 4546, 3492,
		2614, 1931, 1421, 1048, 778,       		582,  440,  337,  261,  237,
		231,  225,  0,    0,    0,				0,    0,    0,    0,    0,
	    0,
};


sint32 ModuleNTCtable_temp_mC_Cust[AFEDEVSIM_TEMP_RT_TABLE_SIZE] =	/* 支持CAN报文标定外部RT表（目前未实现）*/
{
		-50000,
		-41000,-40000, -39000,-30000,-20000,     -10000,00000, 10000, 20000, 30000,
		 40000, 50000,  60000, 70000, 80000,      90000,100000,110000,120000,124000,
		125000, 126000, 150000,999000,999000,     999000,999000,999000,999000,999000,
		999000,
};
sint32 ModuleNTCtable_volt_Cust[AFEDEVSIM_TEMP_RT_TABLE_SIZE] = 		/* 支持CAN报文标定外部RT表（目前未实现）*/
{
		9999,
		9613, 9587, 9560, 9237, 8686,			7888, 6862, 5706, 4546, 3492,
		2614, 1931, 1421, 1048, 778,			582,  440,	337,  261,	237,
		231,  225,	0,	  0,	0,				0,	  0,	0,	  0,	0,
		0,
};
sint32 (*ModuleNTCtable_temp_mC)[AFEDEVSIM_TEMP_RT_TABLE_SIZE] = &ModuleNTCtable_temp_mC_Default1;
sint32 (*ModuleNTCtable_volt)[AFEDEVSIM_TEMP_RT_TABLE_SIZE] = &ModuleNTCtable_volt_Default1;


/* AFE仿真默认使用的均衡温度RT表 */
sint32 BalanceNTCtable_temp_mC_Default1[AFEDEVSIM_TEMP_RT_TABLE_SIZE] =
{
		-50000,
		-40000,-30000,-20000,-10000,00000,     	10000, 20000, 30000, 40000, 50000,
		 60000, 70000, 80000, 90000,100000,    	110000,120000,125000,150000,999000,
		999000,999000,999000,999000,999000,    	999000,999000,999000,999000,999000,
		999000,
};
/* AFE仿真默认使用的均衡温度RT表 */
sint32 BalanceNTCtable_volt_Default1[AFEDEVSIM_TEMP_RT_TABLE_SIZE] =
{
		9999,
		9791, 9634, 9389, 9021, 8514,    		7858, 7062, 6167, 5244, 4361,
		3563, 2873, 2297, 1829, 1457,       	1165, 935,  840,  0,    0,
	    0,    0,    0,    0,    0,				0,    0,    0,    0,    0,
		0,
};

sint32 BalanceNTCtable_temp_mC_Cust[AFEDEVSIM_TEMP_RT_TABLE_SIZE] =	/* 支持CAN报文标定外部RT表（目前未实现）*/
{
		-50000,
		-40000,-30000,-20000,-10000,00000,		10000, 20000, 30000, 40000, 50000,
		 60000, 70000, 80000, 90000,100000, 	110000,120000,125000,150000,999000,
		999000,999000,999000,999000,999000, 	999000,999000,999000,999000,999000,
		999000,
};

sint32 BalanceNTCtable_volt_Cust[AFEDEVSIM_TEMP_RT_TABLE_SIZE] =		/* 支持CAN报文标定外部RT表（目前未实现）*/
{
		9999,
		9791, 9634, 9389, 9021, 8514,			7858, 7062, 6167, 5244, 4361,
		3563, 2873, 2297, 1829, 1457,			1165, 935,	840,  0,	0,
		0,	  0,	0,	  0,	0,				0,	  0,	0,	  0,	0,
		0,
};

sint32 (*BalanceNTCtable_temp_mC)[AFEDEVSIM_TEMP_RT_TABLE_SIZE] = &BalanceNTCtable_temp_mC_Default1;
sint32 (*BalanceNTCtable_volt)[AFEDEVSIM_TEMP_RT_TABLE_SIZE] = &BalanceNTCtable_volt_Default1;



/* AFE仿真默认使用的Shunt温度RT表 */
sint32 ShuntNTCtable_temp_mC_Default1[AFEDEVSIM_TEMP_RT_TABLE_SIZE] =
{
		-100000,
		-50000, -40000, -30000, -20000, -10000, 00000, 10000, 20000, 30000, 40000,
		50000, 60000, 70000, 80000, 90000,		100000,110000,120000,130000,140000,
		150000,170000,999000,999000,999000,		999000,999000,999000,999000,999000,
		999000,
};
/* AFE仿真默认使用的Shunt温度RT表 */
sint32 ShuntNTCtable_volt_Default1[AFEDEVSIM_TEMP_RT_TABLE_SIZE] =
{
		9999,
		9960, 9917, 9837, 9698, 9469,			9116, 8606, 7923, 7080, 6127,
		5138, 4192, 3346, 2631, 2049,			1590, 1233, 960,  750,  590,
		467,  0,    0,    0,    0,              0,    0,    0,    0,    0,
		0,
};

sint32 ShuntNTCtable_temp_mC_Cust[AFEDEVSIM_TEMP_RT_TABLE_SIZE] =	/* 支持CAN报文标定外部RT表（目前未实现）*/
{
		-100000,
		-50000, -40000, -30000, -20000, -10000, 00000, 10000, 20000, 30000, 40000,
		50000, 60000, 70000, 80000, 90000,		100000,110000,120000,130000,140000,
		150000,170000,999000,999000,999000,		999000,999000,999000,999000,999000,
		999000,
};

sint32 ShuntNTCtable_volt_Cust[AFEDEVSIM_TEMP_RT_TABLE_SIZE] =		/* 支持CAN报文标定外部RT表（目前未实现）*/
{
		9999,
		9960, 9917, 9837, 9698, 9469,			9116, 8606, 7923, 7080, 6127,
		5138, 4192, 3346, 2631, 2049,			1590, 1233, 960,  750,	590,
		467,  0,	0,	  0,	0,				0,	  0,	0,	  0,	0,
		0,
};

sint32 (*ShuntNTCtable_temp_mC)[AFEDEVSIM_TEMP_RT_TABLE_SIZE] = &ShuntNTCtable_temp_mC_Default1;
sint32 (*ShuntNTCtable_volt)[AFEDEVSIM_TEMP_RT_TABLE_SIZE] = &ShuntNTCtable_volt_Default1;


/* 二分查表将模组温度mC换算成GPIO电压 */
static void AfeDeviceSim_ModuleTempToGpioVolt(sint32 temp_mC, uint32 tsrefVol_100uV, sint32* gpioVol)
{
	sint32 volt_div;
	static uint32 size = AFEDEVSIM_TEMP_RT_TABLE_SIZE;
	uint32 index = size / 2;
	uint32 div = size / 4;

	while(div)
	{
		if((*ModuleNTCtable_temp_mC)[index] > temp_mC)
		{
			index -= div;
		}
		else
		{
			index += div;
		}
		div = div >> 1;
	}
	if((*ModuleNTCtable_temp_mC)[index] > temp_mC)
	{
		index -= 1;
	}
	/* 线性插值计算电压 */
	float linear = (float)(temp_mC - (*ModuleNTCtable_temp_mC)[index]) /
			(float)((*ModuleNTCtable_temp_mC)[index + 1] - (*ModuleNTCtable_temp_mC)[index]);

	volt_div = (*ModuleNTCtable_volt)[index] +
			(sint32)(linear * ((*ModuleNTCtable_volt)[index + 1] - (*ModuleNTCtable_volt)[index]));

	volt_div = (uint16)((float)volt_div * (float)tsrefVol_100uV / 10000.0);

	*gpioVol = volt_div;
}

/* 二分查表将均衡温度mC换算成GPIO电压 */
static void AfeDeviceSim_BalanceTempToGpioVolt(sint32 temp_mC, uint32 tsrefVol_100uV, sint32* gpioVol)
{
	sint32 volt_div;
	static uint32 size = AFEDEVSIM_TEMP_RT_TABLE_SIZE;
	uint32 index = size / 2;
	uint32 div = size / 4;
	while(div)
	{
		if((*BalanceNTCtable_temp_mC)[index] > temp_mC)
		{
			index -= div;
		}
		else
		{
			index += div;
		}
		div = div >> 1;
	}
	if((*BalanceNTCtable_temp_mC)[index] > temp_mC)
	{
		index -= 1;
	}
	/* 线性插值计算电压 */
	float linear = (float)(temp_mC - (*BalanceNTCtable_temp_mC)[index]) /
			(float)((*BalanceNTCtable_temp_mC)[index + 1] - (*BalanceNTCtable_temp_mC)[index]);

	volt_div = (*BalanceNTCtable_volt)[index] +
			(sint32)(linear * ((*BalanceNTCtable_volt)[index + 1] - (*BalanceNTCtable_volt)[index]));

	volt_div = (uint16)((float)volt_div * tsrefVol_100uV / 10000.0);

	*gpioVol = volt_div;
}

/* 二分查表将Shunt温度mC换算成GPIO电压 */
static void AfeDeviceSim_ShuntTempToGpioVolt(sint32 temp_mC, uint32 tsrefVol_100uV, sint32* gpioVol)
{
	sint32 volt_div;
	static uint32 size = AFEDEVSIM_TEMP_RT_TABLE_SIZE;
	uint32 index = size / 2;
	uint32 div = size / 4;
	while(div)
	{
		if((*ShuntNTCtable_temp_mC)[index] > temp_mC)
		{
			index -= div;
		}
		else
		{
			index += div;
		}
		div = div >> 1;
	}
	if((*ShuntNTCtable_temp_mC)[index] > temp_mC)
	{
		index -= 1;
	}
	/* 线性插值计算电压 */
	float linear = (float)(temp_mC - (*ShuntNTCtable_temp_mC)[index]) /
			(float)((*ShuntNTCtable_temp_mC)[index + 1] - (*ShuntNTCtable_temp_mC)[index]);

	volt_div = (*ShuntNTCtable_volt)[index] +
			(sint32)(linear * ((*ShuntNTCtable_volt)[index + 1] - (*ShuntNTCtable_volt)[index]));

	volt_div = (uint16)((float)volt_div * (float)tsrefVol_100uV / 10000.0);

	*gpioVol = volt_div;
}

/* BQ79758 的电流采样, 计算公式来自HSIS, 取决于实际电路 */
static void AfeDeviceSim_GetBQ79758CurrentData(void)
{
	uint8 simEnable;
	RTE_Read_DeviceRegSimEnable(&simEnable);
	if(FALSE == simEnable)
	{
		return;
	}
	sint32 CSADC_Current = BatteryBusCurrent_mA / 4;
	sint32 GPIO6_Current = (sint32)(((float)(BatteryBusCurrent_mA + AfeCalib[AFECALIB_2ndCURRENT_OFFSET]) + 3333333.3) / 133.333333);

	AfeSim_Reg[AFESIM_DEVID_1][AFESIM_BQ79718_REG_GPIO6_HI] = (GPIO6_Current >> 8) & 0xFF;
	AfeSim_Reg[AFESIM_DEVID_1][AFESIM_BQ79718_REG_GPIO6_LO] = (GPIO6_Current     ) & 0xFF;

	AfeSim_Reg[AFESIM_DEVID_1][AFESIM_BQ79758_REG_CURRENT_HI]  = (CSADC_Current >> 16) & 0xFF;
	AfeSim_Reg[AFESIM_DEVID_1][AFESIM_BQ79758_REG_CURRENT_MID] = (CSADC_Current >> 8 ) & 0xFF;
	AfeSim_Reg[AFESIM_DEVID_1][AFESIM_BQ79758_REG_CURRENT_LO]  = (CSADC_Current      ) & 0xFF;
}

static void AfeDeviceSim_GetBQ79656CurrentData(void)
{

}


static void AfeDeviceSimX1_GetAfeDevPowerStatus(void)
{
	uint8 phyDevId;
	uint16 batId;
	RTE_Read_AfeSimUseType(&AfeSim_UseType);
	
	for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
	{
		if(AFESIM_DEVICE_POWER_ON_RESET == AfeSim_Device[phyDevId].power.powerState)
		{
			if(AfeSim_Device[phyDevId].power.VBAT_mV > 9000)
			{
				AfeSim_Device[phyDevId].power.powerState = AFESIM_DEVICE_POWER_SHUTDOWN;
			}
		}
		else
		{
			if(AfeSim_Device[phyDevId].power.VBAT_mV < 8000)
			{
				AfeSim_Device[phyDevId].power.powerState = AFESIM_DEVICE_POWER_ON_RESET;
			}
			else
			{
				for(batId = AfeMan->cellNumSum[phyDevId-1]; batId < AfeMan->cellNumSum[phyDevId]; batId++)
				{
					Bat[batId].currentExtra_uA = 120000;
				}
			}
		}
	}
}

/* 根据模组温度, 均衡温度, TSREF值, 换算出GPIO电压, 并赋值到寄存器
 * X1 : Function Upgrade Completed: Support GPIO Calib and BQ79718 616 */
static void AfeDeviceSimX1_GetGpioVoltage(void)
{
	//int gpioIndex;
	int phyDevId = 1;
	uint32 tsrefVolt;
	uint32 tsrefBiasVolt;
	uint8 tsrefVoltHI;
	uint8 tsrefVoltLO;
	//uint16 regFullVal;
	//uint8 regVal[2];
	uint8 gpioId;
	//uint8 gpioNum;
	sint32 gpioVol;
	uint8 gpioVolHI;
	uint8 gpioVolLO;
	uint16 startReg;
	uint16 regAddr;
	sint32 GPIO_Current = 0;
	uint8 gpioSimEnable = 1;
	uint8 shuntTempCnt = 0;
	
	RTE_Read_AfeGpioSimEnable(&gpioSimEnable);
	RTE_Read_AfeSimUseType(&AfeSim_UseType);
	
	if(AfeSim_UseType == AFESIM_TYPE_79718)
	{
		startReg = AFESIM_BQ79718_REG_GPIO1_HI;
		GPIO_Current = (sint32)(((float)(BatteryBusCurrent_mA + AfeCalib[AFECALIB_2ndCURRENT_OFFSET]) + 3333333.3) / 133.333333);
	}
	else
	{
		startReg = AFESIM_BQ79616_REG_GPIO1_HI;
		GPIO_Current = 0;
	}

	/*从模组温度, 均衡温度, Shunt温度, 第二电流传感器, TSREF获取 GPIO 电压 */
	if(gpioSimEnable)
	{
		for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
		{
			tsrefVolt = AfeSim_Device[phyDevId].power.TSREF_100uV;
			tsrefBiasVolt = AfeSim_Device[phyDevId].power.TSREFbias_100uV;
			
			for(gpioId = 0; gpioId < AfeMan->afeMaxGpioNum; gpioId++)
			{
				if(AfeCalib[AFECALIB_GPIOVOLT_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId] & AFECALIB_ENABLE_MASK)
				{
					AfeSim_Device[phyDevId].gpio[gpioId].inputVoltage_100uV = AfeCalib[AFECALIB_GPIOVOLT_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId] & AFECALIB_DATA_MASK;
				}
				else
				{
					switch(AfeSim_Device[phyDevId].gpio[gpioId].useType)
					{
					case AFESIM_DEVICE_GPIO_NOUSE:
						AfeSim_Device[phyDevId].gpio[gpioId].inputVoltage_100uV = 0xFFFF;
						break;
					case AFESIM_DEVICE_GPIO_MODULE_TEMP:
						AfeDeviceSim_ModuleTempToGpioVolt(ModuleTemp_mC[phyDevId][gpioId], tsrefVolt, &gpioVol);
						AfeSim_Device[phyDevId].gpio[gpioId].inputVoltage_100uV = gpioVol;
						break;
					case AFESIM_DEVICE_GPIO_BALANCE_TEMP:
						AfeDeviceSim_BalanceTempToGpioVolt(BalanceTemp_mC[phyDevId][gpioId], tsrefVolt, &gpioVol);
						AfeSim_Device[phyDevId].gpio[gpioId].inputVoltage_100uV = gpioVol;
						break;
					case AFESIM_DEVICE_GPIO_SHUNT_TEMP:
						AfeDeviceSim_ShuntTempToGpioVolt(ShuntTemp_mC[shuntTempCnt++], tsrefVolt, &gpioVol);
						AfeSim_Device[phyDevId].gpio[gpioId].inputVoltage_100uV = gpioVol;
						break;
					case AFESIM_DEVICE_GPIO_TSREF:
						AfeSim_Device[phyDevId].gpio[gpioId].inputVoltage_100uV = tsrefBiasVolt;
						break;
					case AFESIM_DEVICE_GPIO_CURRENT:
						AfeSim_Device[phyDevId].gpio[gpioId].inputVoltage_100uV = (uint16)GPIO_Current;
						break;
					}
				}
			}
		}
	}

	uint8 simEnable;
	RTE_Read_DeviceRegSimEnable(&simEnable);
	if(FALSE == simEnable)
	{
		return;
	}
	/*将GPIO电压写入寄存器*/
	switch(AfeSim_UseType)
	{
	case AFESIM_TYPE_79718:
		for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
		{
			for(gpioId = 0; gpioId < AfeMan->afeMaxGpioNum; gpioId++)
			{
				/*标定GPIO偏移量*/
				gpioVol = AfeSim_Device[phyDevId].gpio[gpioId].inputVoltageBias_100uV;
				gpioVol += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_GPIO_VOLT]);

				/*取值范围限制*/
				gpioVol = gpioVol > 0xFFFF ? 0xFFFF : gpioVol;
				gpioVol = gpioVol < 0 ? 0 : gpioVol;

				/*填写芯片寄存器*/
				gpioVolHI = (gpioVol >> 8) & 0xFF;
				gpioVolLO = (gpioVol) & 0xFF;
				regAddr = startReg + gpioId*2;
				AfeSim_Reg[phyDevId][regAddr] = gpioVolHI;
				AfeSim_Reg[phyDevId][regAddr+1] = gpioVolLO;
			}
		}
		break;
	case AFESIM_TYPE_79616:
		startReg = AFESIM_BQ79616_REG_GPIO1_HI;
		for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
		{
			for(gpioId = 0; gpioId < AfeMan->afeMaxGpioNum; gpioId++)
			{
				/*标定GPIO偏移量*/
				gpioVol = AfeSim_Device[phyDevId].gpio[gpioId].inputVoltageBias_100uV;
				gpioVol += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_GPIO_VOLT]);
				gpioVol = (sint32)((float)gpioVol * (100.0 / 152.59));
				/*取值范围限制*/
				gpioVol = gpioVol > 0xFFFF ? 0xFFFF : gpioVol;
				gpioVol = gpioVol < 0 ? 0 : gpioVol;

				/*填写芯片寄存器*/
				gpioVolHI = (gpioVol >> 8) & 0xFF;
				gpioVolLO = (gpioVol) & 0xFF;
				regAddr = startReg + gpioId*2;
				AfeSim_Reg[phyDevId][regAddr] = gpioVolHI;
				AfeSim_Reg[phyDevId][regAddr+1] = gpioVolLO;
			}
			tsrefVolt = AfeSim_Device[phyDevId].power.TSREFbias_100uV;
			tsrefVolt += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_TSREF_VOLT]);
			tsrefVolt = (uint32) ( (float)tsrefVolt * (100.0 / 169.54));
			
			tsrefVoltHI = (tsrefVolt >> 8) & 0xFF;
			tsrefVoltLO = tsrefVolt & 0xFF;
			AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_TSREF_HI] = tsrefVoltHI;
			AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_TSREF_LO] = tsrefVoltLO;
		}
		break;
	}
}

/*AfeDeviceSim_GetDieTemp
 * */
static void AfeDeviceSimX1_GetDieTemp(void)
{
	uint8 phyDevId;
	uint8 dieTempId;
	uint16 startRegAddr;
	uint16 regAddr;
	sint32 dieTemp;
	uint8 dieTempHI;
	uint8 dieTempLO;
	uint16 factor;
	
	RTE_Read_AfeSimUseType(&AfeSim_UseType);
	uint8 simEnable;
	RTE_Read_DeviceRegSimEnable(&simEnable);
	if(FALSE == simEnable)
	{
		return;
	}
	
	if(AfeSim_UseType == AFESIM_TYPE_79718)
	{
		startRegAddr = AFESIM_BQ79718_REG_DIETEMP1_HI;
		factor = 100;
	}
	else
	{
		startRegAddr = AFESIM_BQ79616_REG_DIETEMP1_HI;
		factor = 25;
	}
	
	for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
	{
		for(dieTempId = 0; dieTempId < 2; dieTempId++)
		{
			dieTemp = DieTemp_mC[phyDevId][dieTempId] / factor;
			dieTemp += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_DIETEMP]);
			dieTempHI = (dieTemp >> 8) & 0xFF;
			dieTempLO = dieTemp & 0xFF;
			regAddr = startRegAddr + 2*dieTempId;
			AfeSim_Reg[phyDevId][regAddr]	= dieTempHI;
			AfeSim_Reg[phyDevId][regAddr+1] = dieTempLO;
		}
	}

}


/*AfeDeviceSim_GetCellModuleVoltData
 * X1: Upgraded, support Module volt calibration, VActCellSum calculation and BQ79616 718
 * */
static void AfeDeviceSimX1_GetCellModuleVoltData(void)
{
	int batIndex;
	int cellIndex;
	int devIndex = 1;
	uint16 cellRegBaseAddr;
	uint16 moduleRegBaseAddr;
	uint16 regAddr;
	uint16 regValue;
	//sint32 cellVolt_100uV;
	sint32 cellVoltBias_100uV;
	uint8 cellVoltHigh;
	uint8 cellVoltLow;
	sint32 cellVoltSum_100uV = 0;
	sint32 cellVoltBiasSum_100uV = 0;
	sint32 moduleVolt;
	sint32 moduleVoltBias;
	uint8 moduleVoltHigh;
	uint8 moduleVoltLow;
	uint8 phyDevId;
	uint8 simEnable;
	
	if(AfeSim_UseType == AFESIM_TYPE_79718)
	{
		cellRegBaseAddr = AFESIM_BQ79718_REG_VCELL1_HI;
		moduleRegBaseAddr = AFESIM_BQ79718_REG_BAT_HI;
	}
	else
	{
		cellRegBaseAddr = AFESIM_BQ79616_REG_VCELL1_HI;
		moduleRegBaseAddr = AFESIM_BQ79616_REG_AUX_BAT_HI;
	}
	
	batIndex = 0;
	/* 获取电芯电压累加和,          与模组电压 */
	for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
	{
		cellVoltSum_100uV = 0;
		cellVoltBiasSum_100uV = 0;
		for(cellIndex = 0; cellIndex < AfeSim_Device[phyDevId].activeCell; cellIndex++)
		{
			cellVoltSum_100uV += Bat[batIndex].volt_100uV;
			cellVoltBiasSum_100uV += Bat[batIndex].volt_bias_100uV;
			AfeSim_Device[phyDevId].vc[cellIndex + 1].cellVolt_100uV = Bat[batIndex].volt_100uV;
			AfeSim_Device[phyDevId].vc[cellIndex + 1].cellVoltBias_100uV = Bat[batIndex].volt_bias_100uV;
			batIndex++;
		}

		/* 判断此模组是否被标定 */
		if(AfeCalib[AFECALIB_MODULEVOLT_OFFSET + phyDevId] & AFECALIB_ENABLE_MASK)
		{
			AfeSim_Device[phyDevId].power.VBAT_mV = AfeCalib[AFECALIB_MODULEVOLT_OFFSET + phyDevId] & AFECALIB_DATA_MASK;
		}
		else
		{
			AfeSim_Device[phyDevId].power.VBAT_mV = cellVoltSum_100uV / 10;
		}

		/* 计算电芯电压累加和 */
		AfeSim_Device[phyDevId].power.VActCellSum_mV = cellVoltBiasSum_100uV / 10;

		/* 计算模组电压偏移量 */
		moduleVoltBias = AfeSim_Device[phyDevId].power.VBAT_mV + AfeCalib[AFECALIB_MODULEVOLT_BIAS_OFFSET + phyDevId];
		moduleVoltBias = moduleVoltBias > 65535*2 ? 65535*2 : moduleVoltBias;
		moduleVoltBias = moduleVoltBias < 0 ? 0 : moduleVoltBias;
		AfeSim_Device[phyDevId].power.VBAT_bias_mV = moduleVoltBias;
		AfeSim_Device[phyDevId].power.Aux_BAT_mV = AfeSim_Device[phyDevId].power.VBAT_bias_mV;
	}

	RTE_Read_DeviceRegSimEnable(&simEnable);
	if(FALSE == simEnable)
	{
		return;
	}
	batIndex = 0;
	/* 将电芯电压与模组电压更新到芯片寄存器 */
	switch(AfeSim_UseType)
	{
		case AFESIM_TYPE_79718:
			for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
			{
				for(cellIndex = 0; cellIndex < AfeMan->afeMaxCellNum; cellIndex++)
				{
					if(cellIndex >= AfeMan->cellNum[phyDevId])
					{
						/*超出有效电芯数的通道赋予默认值*/
						regAddr = cellRegBaseAddr - (2 * cellIndex);
						regValue = AfeSim_Device[devIndex].adc[0].defaultVaule;
						cellVoltHigh = (regValue >> 8) & 0xFF;
						cellVoltLow = regValue & 0xFF;
						AfeSim_Reg[phyDevId][regAddr+1] = cellVoltLow;
						AfeSim_Reg[phyDevId][regAddr] = cellVoltHigh;
					}
					else
					{
						/*获取加上电压偏差标定量的电芯电压*/
						cellVoltBias_100uV = AfeSim_Device[phyDevId].vc[cellIndex + 1].cellVoltBias_100uV;
						cellVoltBias_100uV += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_CELL_VOLT]);
						if(cellVoltBias_100uV > 65000)
							cellVoltBias_100uV = 65000;
						if(cellVoltBias_100uV < 0)
							cellVoltBias_100uV = 0;
						
						cellVoltHigh = (cellVoltBias_100uV >> 8) & 0xFF;
						cellVoltLow = cellVoltBias_100uV & 0xFF;

						/*将VC端口电压赋值到虚拟寄存器*/
						regAddr = cellRegBaseAddr - (2 * cellIndex);
						AfeSim_Reg[phyDevId][regAddr+1] = cellVoltLow;
						AfeSim_Reg[phyDevId][regAddr] = cellVoltHigh;
						batIndex++;
					}
				}

				moduleVolt = AfeSim_Device[phyDevId].power.VBAT_bias_mV;
				moduleVolt += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_MODULE_VOLT]/10);
				moduleVolt /= 2;
				moduleVoltHigh = (moduleVolt >> 8) & 0xFF;
				moduleVoltLow  = (moduleVolt     ) & 0xFF;
				regAddr = moduleRegBaseAddr;
				AfeSim_Reg[phyDevId][regAddr+1] = moduleVoltLow;
				AfeSim_Reg[phyDevId][regAddr]   = moduleVoltHigh;

				/* BQ79718 专有寄存器，电芯电压累加和 */
				if(AfeSim_UseType == AFESIM_TYPE_79718)
				{
					moduleVolt = AfeSim_Device[phyDevId].power.VActCellSum_mV / 2;
					moduleVoltHigh = moduleVolt >> 8;
					moduleVoltLow = moduleVolt & 0xFF;
					AfeSim_Reg[phyDevId][AFESIM_BQ79718_REG_VCELL_ACT_SUM_HI] = moduleVoltHigh;
					AfeSim_Reg[phyDevId][AFESIM_BQ79718_REG_VCELL_ACT_SUM_LO] = moduleVoltLow;
				}
			}
		break;
		/* BQ79616 */
		case AFESIM_TYPE_79616:
			for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
			{
				for(cellIndex = 0; cellIndex < AfeMan->afeMaxCellNum; cellIndex++)
				{
					if(cellIndex >= AfeMan->cellNum[phyDevId])
					{
						/*超出有效电芯数的通道赋予默认值*/
						regAddr = cellRegBaseAddr - (2 * cellIndex);
						regValue = AfeSim_Device[devIndex].adc[0].defaultVaule;
						cellVoltHigh = (regValue >> 8) & 0xFF;
						cellVoltLow = regValue & 0xFF;
						AfeSim_Reg[phyDevId][regAddr+1] = cellVoltLow;
						AfeSim_Reg[phyDevId][regAddr] = cellVoltHigh;
					}
					else
					{
						/*获取加上电压偏差标定量的电芯电压
						 * 注意 BQ79616的VCELL LSB = 190.73uV 
						   7FFF * LSB = */
						cellVoltBias_100uV = AfeSim_Device[phyDevId].vc[cellIndex + 1].cellVoltBias_100uV;
						cellVoltBias_100uV += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_CELL_VOLT]);
						if(cellVoltBias_100uV > 62495)
							cellVoltBias_100uV = 62495;
						if(cellVoltBias_100uV < 0)
							cellVoltBias_100uV = 0;
						
						cellVoltBias_100uV = (sint32)( (float)cellVoltBias_100uV * (100.0 / 190.73) );

						cellVoltHigh = (cellVoltBias_100uV >> 8) & 0xFF;
						cellVoltLow = cellVoltBias_100uV & 0xFF;

						/*将VC端口电压赋值到虚拟寄存器*/
						regAddr = cellRegBaseAddr - (2 * cellIndex);
						AfeSim_Reg[phyDevId][regAddr+1] = cellVoltLow;
						AfeSim_Reg[phyDevId][regAddr] = cellVoltHigh;
						batIndex++;
					}
				}

				/* 注意，BQ79616使用AUX ADC采集VBAT，LSB = 3.05mV */
				if(AfeCalib[AFECALIB_AUXVOLT_OFFSET + phyDevId*AFECALIB_AUXVOLT_SINGLE_CHIP_NUM + AFESIM_BQ79616_AUX_BAT] & AFECALIB_ENABLE_MASK)
				{
					moduleVolt = AfeCalib[AFECALIB_AUXVOLT_OFFSET + phyDevId*AFECALIB_AUXVOLT_SINGLE_CHIP_NUM + AFESIM_BQ79616_AUX_BAT] & AFECALIB_DATA_MASK;
				}
				else
				{
					moduleVolt = AfeSim_Device[phyDevId].power.VBAT_bias_mV;
				}
				moduleVolt += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_MODULE_VOLT]/10);
				moduleVolt = (sint32)((float)moduleVolt / 3.05);
				
				moduleVoltHigh = (moduleVolt >> 8) & 0xFF;
				moduleVoltLow  = (moduleVolt     ) & 0xFF;
				regAddr = moduleRegBaseAddr;
				
				AfeSim_Reg[phyDevId][regAddr+1] = moduleVoltLow;
				AfeSim_Reg[phyDevId][regAddr]   = moduleVoltHigh;
			}
		break;
	}

}

void AfeDeviceSim_GetAuxVoltage(void)
{
	if(AfeSim_UseType != AFESIM_TYPE_79616)
	{
		return;
	}
	uint8 auxCell, auxGpio;
	uint8 phyDevId;
	float auxScale;
	sint32 auxVoltPre;
	uint16 auxVolt;
	//sint32 noise;
	uint32 calibOffset;

	uint8 simEnable;
	RTE_Read_DeviceRegSimEnable(&simEnable);
	if(FALSE == simEnable)
	{
		return;
	}
	
	for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
	{
		AFESIM_BQ79616_REG_ADC_CTRL2_RegType* ADC_CTRL2 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ADC_CTRL2];
		AFESIM_BQ79616_REG_ADC_CTRL3_RegType* ADC_CTRL3 = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_ADC_CTRL3];
		calibOffset = AFECALIB_AUXVOLT_BIAS_OFFSET + phyDevId*AFECALIB_AUXVOLT_SINGLE_CHIP_NUM;
		/*AUX_CELL*/
		auxScale = 100.0 / 190.73;
		if(AfeCalib[calibOffset + AFESIM_BQ79616_AUX_CELL] & AFECALIB_ENABLE_MASK)
		{
			auxVoltPre = AfeCalib[calibOffset + AFESIM_BQ79616_AUX_CELL] & AFECALIB_DATA_MASK;
		}
		else if(ADC_CTRL2->reg.field.AUX_CELL_SEL == 0)	/* all channel */
		{
			auxVoltPre = AfeSim_Device[phyDevId].vc[rand()%16 + 1].cellVoltBias_100uV;
		}
		else if(ADC_CTRL2->reg.field.AUX_CELL_SEL == 1) /* aux busbar */
		{
			auxVoltPre = 0;
		}
		else if(ADC_CTRL2->reg.field.AUX_CELL_SEL <= 0x11)	
		{
			auxCell = ADC_CTRL2->reg.field.AUX_CELL_SEL - 1;
			auxVoltPre = AfeSim_Device[phyDevId].vc[auxCell].cellVoltBias_100uV + \
					AfeCalib[calibOffset + AFESIM_BQ79616_AUX_CELL];
		}
		else
		{
			auxVoltPre = 0;
		}
		auxVoltPre += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_CELL_VOLT]);
		auxVoltPre *= auxScale;
		auxVolt = auxVoltPre;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_CELL_HI] = auxVolt >> 8;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_CELL_LO] = auxVolt & 0xFF;

		/*AUX_GPIO*/
		auxScale = 100.0/152.59;
		if(AfeCalib[calibOffset + AFESIM_BQ79616_AUX_GPIO] & AFECALIB_ENABLE_MASK)
		{
			auxVoltPre = AfeCalib[calibOffset + AFESIM_BQ79616_AUX_GPIO] & AFECALIB_DATA_MASK;
		}
		else if(ADC_CTRL3->reg.field.AUX_GPIO_SEL == 0)	/* all channel */
		{
			auxVoltPre = AfeSim_Device[phyDevId].gpio[rand()%8].inputVoltage_100uV;
		}
		else if(ADC_CTRL3->reg.field.AUX_GPIO_SEL <= 8)
		{
			auxGpio = ADC_CTRL3->reg.field.AUX_GPIO_SEL;
			auxVoltPre = AfeSim_Device[phyDevId].gpio[auxGpio - 1].inputVoltage_100uV + \
					AfeCalib[calibOffset + AFESIM_BQ79616_AUX_GPIO];
		}
		else
		{
			auxVoltPre = 0;
		}
		auxVoltPre += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_GPIO_VOLT]);
		auxVoltPre *= auxScale;
		auxVolt = auxVoltPre;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_GPIO_HI] = auxVolt >> 8;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_GPIO_LO] = auxVolt & 0xFF;

		/* SKIP AUX_BAT, acquire at AfeDeviceSimX1_GetCellModuleVoltData */

		/*AUX_REFL*/
		auxScale = 100.0/152.59;
		auxVoltPre = AfeSim_Device[phyDevId].power.Aux_REFL_100uV + \
				AfeCalib[calibOffset + AFESIM_BQ79616_AUX_REFL];
		auxVoltPre += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_INTERNAL_VOLT]);
		auxVoltPre *= auxScale;
		auxVolt = auxVoltPre; 
		
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_REFL_HI] = auxVolt >> 8;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_REFL_LO] = auxVolt & 0xFF;

		/*AUX_VBG2*/
		auxScale = 100.0/152.59;
		auxVoltPre = AfeSim_Device[phyDevId].power.Aux_VBG2_100uV + \
				AfeCalib[calibOffset + AFESIM_BQ79616_AUX_VBG2];
		auxVoltPre += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_INTERNAL_VOLT]);
		auxVoltPre *= auxScale;
		auxVolt = auxVoltPre; 
		
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_VBG2_HI] = auxVolt >> 8;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_VBG2_LO] = auxVolt & 0xFF;

		/*AUX_AVAO_REF*/
		auxScale = 100.0/152.59;
		auxVoltPre = AfeSim_Device[phyDevId].power.Aux_AVAO_REF_100uV + \
				AfeCalib[calibOffset + AFESIM_BQ79616_AUX_AVAO_REF];
		auxVoltPre += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_INTERNAL_VOLT]);
		auxVoltPre *= auxScale;
		auxVolt = auxVoltPre; 

		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_AVAO_REF_HI] = auxVolt >> 8;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_AVAO_REF_LO] = auxVolt & 0xFF;

		/*AUX_AVDD_REF*/
		auxScale = 100.0/152.59;
		auxVoltPre = AfeSim_Device[phyDevId].power.Aux_AVDD_REF_100uV + \
				AfeCalib[calibOffset + AFESIM_BQ79616_AUX_AVDD_REF];
		auxVoltPre += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_INTERNAL_VOLT]);
		auxVoltPre *= auxScale;
		auxVolt = auxVoltPre; 
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_AVDD_REF_HI] = auxVolt >> 8;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_AVDD_REF_LO] = auxVolt & 0xFF;

		/*AUX_OV_DAC*/
		auxScale = 100.0/190.74;
		auxVoltPre = AfeSim_Device[phyDevId].power.Aux_OVDAC_100uV + \
				AfeCalib[calibOffset + AFESIM_BQ79616_AUX_OV_DAC];
		auxVoltPre += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_INTERNAL_VOLT]);
		auxVoltPre *= auxScale;
		auxVolt = auxVoltPre; 
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_OV_DAC_HI] = auxVolt >> 8;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_OV_DAC_LO] = auxVolt & 0xFF;

		/*AUX_UV_DAC*/
		auxScale = 100.0/190.74;
		auxVoltPre = AfeSim_Device[phyDevId].power.Aux_UVDAC_100uV + \
				AfeCalib[calibOffset + AFESIM_BQ79616_AUX_UV_DAC];
		auxVoltPre += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_INTERNAL_VOLT]);
		auxVoltPre *= auxScale;
		auxVolt = auxVoltPre; 
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_UV_DAC_HI] = auxVolt >> 8;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_UV_DAC_LO] = auxVolt & 0xFF;

		/*AUX_OT_OTCB_DAC*/
		auxScale = 100.0/152.59;
		auxVoltPre = AfeSim_Device[phyDevId].power.Aux_OT_OTCB_DAC_100uV + \
				AfeCalib[calibOffset + AFESIM_BQ79616_AUX_OT_OTCB_DAC];
		auxVoltPre += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_INTERNAL_VOLT]);
		auxVoltPre *= auxScale;
		auxVolt = auxVoltPre; 
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_OT_OTCB_DAC_HI] = auxVolt >> 8;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_OT_OTCB_DAC_LO] = auxVolt & 0xFF;

		/*AUX_UT_DAC*/
		auxScale = 100.0/152.59;
		auxVoltPre = AfeSim_Device[phyDevId].power.Aux_UTDAC_100uV + \
				AfeCalib[calibOffset + AFESIM_BQ79616_AUX_UT_DAC];
		auxVoltPre += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_INTERNAL_VOLT]);
		auxVoltPre *= auxScale;
		auxVolt = auxVoltPre; 
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_UT_DAC_HI] = auxVolt >> 8;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_UT_DAC_LO] = auxVolt & 0xFF;

		/*AUX_VCBDONE_DAC*/
		auxScale = 100.0/190.74;
		auxVoltPre = AfeSim_Device[phyDevId].power.Aux_VCBDONE_DAC_100uV + \
				AfeCalib[calibOffset + AFESIM_BQ79616_AUX_VCBDONE_DAC];
		auxVoltPre += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_INTERNAL_VOLT]);
		auxVoltPre *= auxScale;
		auxVolt = auxVoltPre; 
		
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_VCBDONE_HI] = auxVolt >> 8;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_VCBDONE_LO] = auxVolt & 0xFF;

		/*AUX_VCM_DAC*/
		auxScale = 100.0/152.59;
		auxVoltPre = AfeSim_Device[phyDevId].power.Aux_VCM_100uV + \
				AfeCalib[calibOffset + AFESIM_BQ79616_AUX_VCM];
		auxVoltPre += AFEDEVSIM_NOISE(AfeSim_Device[phyDevId].noise.volt[AFESIM_NOISE_TYPE_INTERNAL_VOLT]);
		auxVoltPre *= auxScale;
		auxVolt = auxVoltPre; 
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_VCM_HI] = auxVolt >> 8;
		AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_AUX_VCM_LO] = auxVolt & 0xFF;
	}
}

void AfeDeviceSim_Run100ms(void)
{
	RTE_Read_AfeSimUseType(&AfeSim_UseType);
	AfeDeviceSimX1_GetCellModuleVoltData();
	AfeDeviceSimX1_GetGpioVoltage();
	AfeDeviceSimX1_GetDieTemp();
	AfeDeviceSimX1_GetAfeDevPowerStatus();
	AfeDeviceSim_GetAuxVoltage();
}

void AfeDeviceSim_Run50ms(void)
{

}

void AfeDeviceSim_Run20ms(void)
{

}


void AfeDeviceSim_Run10ms(void)
{
	switch (AfeSim_UseType)
	{
		case AFESIM_TYPE_79718: 
			AfeSim_BQ79718_Fsm();
			AfeDeviceSim_GetBQ79758CurrentData(); 
			break;
		case AFESIM_TYPE_79616: 
			AfeSim_BQ79616_Fsm();
			AfeDeviceSim_GetBQ79656CurrentData(); 
			break;
	}
	
}
