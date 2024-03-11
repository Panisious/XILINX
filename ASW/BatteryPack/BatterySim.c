/*
 * BatterySim.c
 *
 *  Created on: 2024年1月27日
 *      Author: Administrator
 */

#include "BatterySim.h"
#include "math.h"

/* 影响模组散热 */
float ModulePack_HeatConductFactor = 0.3;
float CMCPack_HeatConductFactor = 0.03;
float DieCMC_HeatConductFactor = 0.1;
float PackAir_HeatConductFactor = 0.01;
float ShuntPack_HeatConductFactor = 0.5;

/* 影响模组温度增量 */
float BatteryMass_g = 3000;
float BatteryHeatCap_J_gC = 1;
/* 影响均衡温度增量 */
float PCBMass_g = 200;
float PCBHeatCap_J_gC = 0.5;
/* 影响芯片温度增量 */

/* 影响Shunt温度增量 */

/* 影响Pack温度增量 */


/* 外界输入信号, BMS系统总电流, 单位mA, 影响SoC和电芯电压
 * 正值代表放电, 负值代表充电 */
sint32 BatteryBusCurrent_mA = 0;	/* 10A */

/* 虚拟电芯 */
BatteryType Bat[BAT_MAX_NUM];

/* 模组温度 */
sint32 ModuleTemp_mC[AFESIM_MAX_AFE_NUM+1][AFESIM_MAX_GPIO_NUM];

/* 均衡温度 */
sint32 BalanceTemp_mC[AFESIM_MAX_AFE_NUM+1][AFESIM_MAX_GPIO_NUM];

sint32 DieTemp_mC[AFESIM_MAX_AFE_NUM+1][AFESIM_MAX_DT_NUM];

sint32 ShuntTemp_mC[AFESIM_MAX_ST_NUM];

/* 空气温度, Pack温度 */
sint32 AirTemp_mC;
sint32 PackTempDiff_mC;
sint32 PackTemp_mC;


/* 客制化 SOC-电压查找表 */
sint32 Cust_SocTableX[BATSIM_TABLE_MAX_LEN] =
{
	BAT_SOC_AH(0), 		BAT_SOC_AH(1), 		BAT_SOC_AH(3), 		BAT_SOC_AH(6), 		BAT_SOC_AH(10),
	BAT_SOC_AH(15), 	BAT_SOC_AH(20), 	BAT_SOC_AH(25), 	BAT_SOC_AH(30), 	BAT_SOC_AH(35),
	BAT_SOC_AH(40), 	BAT_SOC_AH(45), 	BAT_SOC_AH(50), 	BAT_SOC_AH(55), 	BAT_SOC_AH(60),
	BAT_SOC_AH(65), 	BAT_SOC_AH(70), 	BAT_SOC_AH(75), 	BAT_SOC_AH(80), 	BAT_SOC_AH(85),
	BAT_SOC_AH(90), 	BAT_SOC_AH(92), 	BAT_SOC_AH(94), 	BAT_SOC_AH(96), 	BAT_SOC_AH(98),
	BAT_SOC_AH(100), 	BAT_SOC_AH(101), 	BAT_SOC_AH(102), 	BAT_SOC_AH(103), 	BAT_SOC_AH(104),
	BAT_SOC_AH(105), 	0x7FFFFFFF
};
sint32 Cust_SocTableY[BATSIM_TABLE_MAX_LEN] =
{
	0,					1000,				2000,				5000,				10000,
	31600,				32000,				32125,				32250,				32375,
	32500,				32750,				32630,				32680,				32750,
	32860,				33000,				33125,				33250,				33375,
	33500,				33540,				33580,				33630,				33900,
	36500,				38500,				42000,				45000,				52000,
	58000,				65500,
};


/* 磷酸铁锂电池 SOC-电压查找表, SOC单位毫库伦, 电压单位微伏 */
sint32 LFP_SocTableX[BATSIM_TABLE_MAX_LEN] = 
{
	BAT_SOC_AH(0), 		BAT_SOC_AH(1), 		BAT_SOC_AH(3), 		BAT_SOC_AH(6), 		BAT_SOC_AH(10),
	BAT_SOC_AH(15), 	BAT_SOC_AH(20), 	BAT_SOC_AH(25), 	BAT_SOC_AH(30), 	BAT_SOC_AH(35),
	BAT_SOC_AH(40), 	BAT_SOC_AH(45), 	BAT_SOC_AH(50), 	BAT_SOC_AH(55), 	BAT_SOC_AH(60),
	BAT_SOC_AH(65), 	BAT_SOC_AH(70), 	BAT_SOC_AH(75), 	BAT_SOC_AH(80), 	BAT_SOC_AH(85),
	BAT_SOC_AH(90), 	BAT_SOC_AH(92), 	BAT_SOC_AH(94), 	BAT_SOC_AH(96), 	BAT_SOC_AH(98),
	BAT_SOC_AH(100), 	BAT_SOC_AH(101), 	BAT_SOC_AH(102), 	BAT_SOC_AH(103), 	BAT_SOC_AH(104),
	BAT_SOC_AH(105), 	0x7FFFFFFF
};
sint32 LFP_SocTableY[BATSIM_TABLE_MAX_LEN] = 
{
	15000,				25000,				26500,				28500,				30500,
	31600,				32000,				32125,				32250,				32375,
	32500,				32750,				32630,				32680,				32750,
	32860,				33000,				33125,				33250,				33375,
	33500,				33540,				33580,				33630,				33900,
	36500,				38500,				42000,				45000,				52000,
	58000,				65500,
};

/* 三元锂电池 SOC-电压查找表 */
sint32 NCM_SocTableX[BATSIM_TABLE_MAX_LEN] = 
{
	BAT_SOC_AH(0), 		BAT_SOC_AH(1), 		BAT_SOC_AH(3), 		BAT_SOC_AH(6), 		BAT_SOC_AH(10),
	BAT_SOC_AH(15), 	BAT_SOC_AH(20), 	BAT_SOC_AH(25), 	BAT_SOC_AH(30), 	BAT_SOC_AH(35),
	BAT_SOC_AH(40), 	BAT_SOC_AH(45), 	BAT_SOC_AH(50), 	BAT_SOC_AH(55), 	BAT_SOC_AH(60),
	BAT_SOC_AH(65), 	BAT_SOC_AH(70), 	BAT_SOC_AH(75), 	BAT_SOC_AH(80), 	BAT_SOC_AH(85),
	BAT_SOC_AH(90), 	BAT_SOC_AH(92), 	BAT_SOC_AH(94), 	BAT_SOC_AH(96), 	BAT_SOC_AH(98),
	BAT_SOC_AH(100), 	BAT_SOC_AH(101), 	BAT_SOC_AH(102), 	BAT_SOC_AH(103), 	BAT_SOC_AH(104),
	BAT_SOC_AH(105), 	0x7FFFFFFF
};
sint32 NCM_SocTableY[BATSIM_TABLE_MAX_LEN] = 
{
	30000,				33670,				34150,				34560,				34980,
	35380,				35660,				35880,				36060,				36230,
	36410,				36630,				36880,				37180,				37540,
	37950,				38400,				38900,				39440,				40010,
	40600,				40850,				41090,				41340,				41590,
	41820,				42500,				45000,				50000,				54000,
	58000,				65500,
};



sint32 (*BatSim_SoxTableX)[BATSIM_TABLE_MAX_LEN] = &LFP_SocTableX;
sint32 (*BatSim_SoxTableY)[BATSIM_TABLE_MAX_LEN] = &LFP_SocTableY;

BatteryManagerType BatteryManager =
{
		.bat = &Bat[0],
		.maxBatNum = BAT_MAX_NUM,
};
BatteryManagerType* const BatMan = &BatteryManager;

void BatterySim_Init(void)
{
	int i,j;
	/* 初始化系统电流 */
	RTE_Write_BatterySysCurrent(0);
	/* 初始化电芯SOC和内阻参数*/
	for(i=0;i<BAT_MAX_NUM;i++)
	{
		Bat[i].soc_mC = BAT_mAH(90*1000 + (rand()%5000));
		Bat[i].res_uR = 950 + (rand()%100);
	}

	/* 初始化模组和均衡温度 */
	for(i=1;i<=AFESIM_MAX_AFE_NUM;i++)
	{
		for(j=0;j<AFESIM_MAX_MT_NUM;j++)
		{
			ModuleTemp_mC[i][j] = 25000;
		}
		for(j=6;j<6+AFESIM_MAX_BT_NUM;j++)
		{
			BalanceTemp_mC[i][j] = 30000;
		}
		for(j=0;j<AFESIM_MAX_DT_NUM;j++)
		{
			DieTemp_mC[i][j] = 35000;
		}
	}
	for(i=0;i<AFESIM_MAX_ST_NUM;i++)
	{
		ShuntTemp_mC[i] = 15500;
	}
	/*初始化环境温度*/
	AirTemp_mC = 25000;
	PackTemp_mC = 25000;
}

void BatterySim_SocLoss(BatteryType* bat, sint32 current_mA)
{
	/* 充电放电 */
	bat->soc_mC -= BAT_mAH_LOSS(current_mA);

	/* SOC上下限制 */
	if(bat->soc_mC < BAT_mAH(BAT_MIN_mAH))
	{
		bat->soc_mC = BAT_mAH(BAT_MIN_mAH);
	}
	else if(bat->soc_mC > BAT_mAH(BAT_MAX_mAH))
	{
		bat->soc_mC = BAT_mAH(BAT_MAX_mAH);
	}
}

void BatterySim_SocToVolt(BatteryType* bat, uint16 cellPos)
{
	sint32 soc_uC = bat->soc_mC;
	sint32 volt_temp;
	sint32 volt_100uV;
	sint32 volt_loss_100uV;
	static uint32 size = BATSIM_TABLE_MAX_LEN;
	uint32 index = size / 2;
	uint32 div = size / 4;

	if(AfeCalib[AFECALIB_CELLVOLT_OFFSET + cellPos] & AFECALIB_ENABLE_MASK)
	{
		volt_100uV = AfeCalib[AFECALIB_CELLVOLT_OFFSET + cellPos] & AFECALIB_DATA_MASK;
		if(volt_100uV< 0)
			bat->volt_100uV = 0;
		else if(volt_100uV > 65535)
			bat->volt_100uV = 65535;
		else
			bat->volt_100uV = volt_100uV;
		return;
	}
	
	/* 二分法查表获取SoC对应表下标*/
	while(div)
	{
		if((*BatSim_SoxTableX)[index] > soc_uC)
		{
			index -= div;
		}
		else
		{
			index += div;
		}
		div = div >> 1;
	}
	if((*BatSim_SoxTableX)[index] > soc_uC)
	{
		index -= 1;
	}

	/* 线性插值计算电压 */
	float linear = (float)(soc_uC - (*BatSim_SoxTableX)[index]) /
			(float)((*BatSim_SoxTableX)[index + 1] - (*BatSim_SoxTableX)[index]);

	volt_100uV = (*BatSim_SoxTableY)[index] +
			(sint32)(linear * ((*BatSim_SoxTableY)[index + 1] - (*BatSim_SoxTableY)[index]));

	volt_loss_100uV = (sint32)( (float)bat->res_uR * ((1000.0 * (float)BatteryBusCurrent_mA) + (float)bat->currentExtra_uA) / (1000000.0*100.0) );
	volt_temp = volt_100uV - volt_loss_100uV;
	
	if(volt_temp < 0)
		bat->volt_100uV = 0;
	else if(volt_temp > 65535)
		bat->volt_100uV = 65535;
	else
		bat->volt_100uV = volt_temp;
}


/* Private - BatterySim_ModuleTemp
 * 计算模组温度
 * 需要升级计算公式 */
static void BatterySim_ModuleTemp(sint32 moduleTempArray_mC[][AFESIM_MAX_GPIO_NUM], uint16 afeId, uint16 tempId)
{
	sint32 moduleTemp_mC = moduleTempArray_mC[afeId][tempId];
	float tempDiff_mC = ModulePack_HeatConductFactor * SIM_BASE_FACTOR * (moduleTemp_mC - PackTemp_mC);
	int batPos = AfeMan->cellNumSum[afeId - 1] + (tempId * 3);
	float heat_mJ = (((float)BatteryBusCurrent_mA * (float)BatteryBusCurrent_mA) * Bat[batPos].res_uR) * SIM_BASE_FACTOR / 1e9;
	float tempIncrease_mC = 500 * heat_mJ / (BatteryMass_g * BatteryHeatCap_J_gC);

	moduleTemp_mC += (tempIncrease_mC - tempDiff_mC);
	PackTempDiff_mC += tempDiff_mC;

	moduleTempArray_mC[afeId][tempId] = moduleTemp_mC;
}


/* Private - BatterySim_BalanceTemp
 * 计算均衡温度
 * 需要升级计算公式 */
static void BatterySim_BalanceTemp(sint32 balanceTempArray_mC[][AFESIM_MAX_GPIO_NUM], uint16 afeId, uint16 tempId)
{
	sint32 balanceTemp_mC = balanceTempArray_mC[afeId][tempId];
	sint32 diff = CMCPack_HeatConductFactor * SIM_BASE_FACTOR * (balanceTemp_mC - PackTemp_mC);
	sint32 heat = 30;

	balanceTemp_mC += (heat - diff);
	PackTempDiff_mC += diff;
	balanceTempArray_mC[afeId][tempId] = balanceTemp_mC;
}

/* Private - BatterySim_BalanceTemp
 * 计算均衡温度
 * 需要升级计算公式 */
static void BatterySim_DieTemp(sint32 dieTempArray_mC[][AFESIM_MAX_DT_NUM], uint16 afeId, uint16 tempId)
{
	sint32 dieTemp_mC = dieTempArray_mC[afeId][tempId];
	sint32 diff = DieCMC_HeatConductFactor * SIM_BASE_FACTOR * (dieTemp_mC - PackTemp_mC);
	sint32 heat = 50;

	dieTemp_mC += (heat - diff);
	PackTempDiff_mC += diff;
	dieTempArray_mC[afeId][tempId] = dieTemp_mC;
}

/* Private - BatterySim_ShuntTemp
 * 计算Shunt温度
 * 需要升级计算公式 */
static void BatterySim_ShuntTemp(sint32 shuntTempArray_mC[AFESIM_MAX_ST_NUM], uint16 tempId)
{
	sint32 shuntTemp_mC = shuntTempArray_mC[tempId];
	sint32 diff = ShuntPack_HeatConductFactor * SIM_BASE_FACTOR * (shuntTemp_mC - PackTemp_mC);
	sint32 heat = BatteryBusCurrent_mA * BatteryBusCurrent_mA / 1000000 * 100 / 1000000;

	shuntTemp_mC += (heat - diff);
	PackTempDiff_mC += diff;
	shuntTempArray_mC[tempId] = shuntTemp_mC;
}


/* API Task - BatterySim_NormalRun
 * */
void BatterySim_NormalRun(void)
{
	int i,j;
	uint16 xy[2];
	uint8 soxSimEnable;
	uint8 tempSimEnable;
	uint8 balSimEnable;
	RTE_Read_BatterySysCurrent(&BatteryBusCurrent_mA);
	RTE_Read_BatterySOXsimEnable(&soxSimEnable);
	RTE_Read_BatteryTempSimEnable(&tempSimEnable);
	RTE_Read_BatteryBalSimEnable(&balSimEnable);

	if(soxSimEnable)
	{
		for(i = 0;i < BatMan->maxBatNum; i++)
		{
			/* SOC 损耗仿真 */
			BatterySim_SocLoss(&Bat[i], BatteryBusCurrent_mA + Bat[i].currentExtra_uA/1000);

			/* SOC - 电压转换仿真 */
			BatterySim_SocToVolt(&Bat[i], i);
		}
	}
	if(tempSimEnable)
	{
		PackTempDiff_mC = 0;
		for(i = 1; i <= AfeMan->afeChipRealNum; i++)
		{
			/* 电芯温度仿真 */
			for(j = 0; j < AFESIM_MAX_MT_NUM; j++)
			{
				BatterySim_ModuleTemp(ModuleTemp_mC, i, j);
			}
			/* 均衡温度仿真 */
			for(j = 6; j < 6 + AFESIM_MAX_BT_NUM; j++)
			{
				BatterySim_BalanceTemp(BalanceTemp_mC, i, j);
			}
			for(j = 0; j < AFESIM_MAX_DT_NUM ; j++)
			{
				BatterySim_DieTemp(DieTemp_mC, i, j);
			}
		}
		for(i = 0; i < AFESIM_MAX_ST_NUM; i++)
		{
			 BatterySim_ShuntTemp(ShuntTemp_mC, i);
		}
		PackTemp_mC += PackTempDiff_mC / BatMan->maxBatNum;
		PackTemp_mC = PackTemp_mC - (PackAir_HeatConductFactor * SIM_BASE_FACTOR * (PackTemp_mC - AirTemp_mC));
	}
}
