
#include "AfeCalibration.h"
#include "Comm_Decode.h"
#include "BatterySim.h"
#include "AfeSim.h"

/*	AFE 数据标定管理
 * 	用 LCD/CAN/UART 标定基本采样数据，AFE仿真数据，功能安全数据
 * */
 
sint32 AfeCalib[AFECALIB_MAX_CALIB_NUM];
sint32 AfeCalibLatch[AFECALIB_MAX_CALIB_NUM];

AfeCalib_ManagerType AfeCalib_Manager;


void AfeCalib_LatchCellVolt(void);
void AfeCalib_LatchGpioVolt(void);


void AfeCalibration_Init(void)
{

}



void AfeCalib_CanMsgDecode_ClearCalib(uint8* data)
{
	uint8 phyDevId;
	taskENTER_CRITICAL();
	switch(data[0])
	{
	case CAN_CALIB_ClearCalib_B0_Ignore:
		break;
		
	case CAN_CALIB_ClearCalib_B0_AllCalib:
		RTE_Write_BatterySOXsimEnable(1);
		RTE_Write_AfeGpioSimEnable(1);
		RTE_Write_BatterySOXsimEnable(1);
		RTE_Write_BatterySOXsimEnable(1);
		RTE_Write_BatterySOXsimEnable(1);
		memset(AfeCalib, 0, sizeof(AfeCalib));
		AfeSim_BQ79616_ResetAuxVolt(0xFF);
		break;
		
	case CAN_CALIB_ClearCalib_B0_ChainBroken:
		memset(&AfeCalib[AFECALIB_DAISYCHAIN_DISABLE_OFFSET], 0, sizeof(sint32)*(AFECALIB_DAISYCHAIN_DISABLE_NUM + AFECALIB_DAISYCHAIN_NOISE_NUM));
		break;
	case CAN_CALIB_ClearCalib_B0_DeviceBroken:
		memset(&AfeCalib[AFECALIB_DEVICE_DISABLE_OFFSET], 0, sizeof(sint32)*AFECALIB_DEVICE_DISABLE_NUM);
		break;
	case CAN_CALIB_ClearCalib_B0_SampleNoise:
		for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
		{
			memset(&AfeSim_Device[phyDevId].noise, 0, sizeof(AfeSim_NoiseModel));
		}
		break;
		
	case CAN_CALIB_ClearCalib_B0_CellSOX:
		RTE_Write_BatterySOXsimEnable(1);
		memset(&AfeCalib[AFECALIB_CELLSOX_OFFSET], 0, sizeof(sint32)*(AFECALIB_CELLSOX_NUM));
		break;
	case CAN_CALIB_ClearCalib_B0_CellVolt:
		RTE_Write_BatterySOXsimEnable(1);
		memset(&AfeCalib[AFECALIB_CELLVOLT_OFFSET], 0, sizeof(sint32)*AFECALIB_CELLVOLT_NUM);
		memset(&AfeCalib[AFECALIB_CELLVOLT_BIAS_OFFSET], 0, sizeof(sint32)*AFECALIB_CELLVOLT_BIAS_NUM);
		break;
	case CAN_CALIB_ClearCalib_B0_ModuleVolt:
		RTE_Write_BatterySOXsimEnable(1);
		memset(&AfeCalib[AFECALIB_MODULEVOLT_OFFSET], 0, sizeof(sint32)*AFECALIB_MODULEVOLT_NUM);
		memset(&AfeCalib[AFECALIB_MODULEVOLT_BIAS_OFFSET], 0, sizeof(sint32)*AFECALIB_MODULEVOLT_BIAS_NUM);
		break;
	case CAN_CALIB_ClearCalib_B0_GpioVolt:
		RTE_Write_AfeGpioSimEnable(1);
		RTE_Write_BatteryTempSimEnable(1);
		memset(&AfeCalib[AFECALIB_GPIOVOLT_OFFSET], 0, sizeof(sint32)*AFECALIB_GPIOVOLT_NUM);
		memset(&AfeCalib[AFECALIB_GPIOVOLT_BIAS_OFFSET], 0, sizeof(sint32)*AFECALIB_GPIOVOLT_BIAS_NUM);
		break;
	case CAN_CALIB_ClearCalib_B0_Tsref:
		RTE_Write_AfeGpioSimEnable(1);
		RTE_Write_BatteryTempSimEnable(1);
		memset(&AfeCalib[AFECALIB_TSREF_OFFSET], 0, sizeof(sint32)*AFECALIB_TSREF_NUM);
		memset(&AfeCalib[AFECALIB_TSREF_BIAS_OFFSET], 0, sizeof(sint32)*AFECALIB_TSREF_BIAS_NUM);
		break;
	case CAN_CALIB_ClearCalib_B0_AuxVolt:
		RTE_Write_AfeGpioSimEnable(1);
		memset(&AfeCalib[AFECALIB_AUXVOLT_OFFSET], 0, sizeof(sint32)*AFECALIB_AUXVOLT_NUM);
		memset(&AfeCalib[AFECALIB_AUXVOLT_BIAS_OFFSET], 0, sizeof(sint32)*AFECALIB_AUXVOLT_BIAS_NUM);
		AfeSim_BQ79616_ResetAuxVolt(0xFF);
		break;

		
	case CAN_CALIB_ClearCalib_B0_ModuleTemp:
		RTE_Write_AfeGpioSimEnable(1);
		RTE_Write_BatteryTempSimEnable(1);
		memset(&AfeCalib[AFECALIB_MODULETEMP_OFFSET], 0, sizeof(sint32)*AFECALIB_MODULETEMP_NUM);
		break;
	case CAN_CALIB_ClearCalib_B0_BalanceTemp:
		RTE_Write_AfeGpioSimEnable(1);
		RTE_Write_BatteryTempSimEnable(1);
		memset(&AfeCalib[AFECALIB_BALANCETEMP_OFFSET], 0, sizeof(sint32)*AFECALIB_BALANCETEMP_NUM);
		break;
	case CAN_CALIB_ClearCalib_B0_DieTemp:
		RTE_Write_AfeGpioSimEnable(1);
		RTE_Write_BatteryTempSimEnable(1);
		memset(&AfeCalib[AFECALIB_DIETEMP_OFFSET], 0, sizeof(sint32)*AFECALIB_DIETEMP_NUM);
		break;
	case CAN_CALIB_ClearCalib_B0_ShuntTemp:
		RTE_Write_AfeGpioSimEnable(1);
		RTE_Write_BatteryTempSimEnable(1);
		memset(&AfeCalib[AFECALIB_SHUNTTEMP_OFFSET], 0, sizeof(sint32)*AFECALIB_SHUNTTEMP_NUM);
		break;
	case CAN_CALIB_ClearCalib_B0_PackTemp:
		RTE_Write_AfeGpioSimEnable(1);
		RTE_Write_BatteryTempSimEnable(1);
		memset(&AfeCalib[AFECALIB_PACKTEMP_OFFSET], 0, sizeof(sint32)*AFECALIB_PACKTEMP_NUM);
		break;
	case CAN_CALIB_ClearCalib_B0_AirTemp:
		RTE_Write_AfeGpioSimEnable(1);
		RTE_Write_BatteryTempSimEnable(1);
		memset(&AfeCalib[AFECALIB_AIRTEMP_OFFSET], 0, sizeof(sint32)*AFECALIB_AIRTEMP_NUM);
		break;
	case CAN_CALIB_ClearCalib_B0_AllTemp:
		RTE_Write_AfeGpioSimEnable(1);
		RTE_Write_BatteryTempSimEnable(1);
		memset(&AfeCalib[AFECALIB_MODULETEMP_OFFSET], 0, sizeof(sint32)*AFECALIB_MODULETEMP_NUM);
		memset(&AfeCalib[AFECALIB_BALANCETEMP_OFFSET], 0, sizeof(sint32)*AFECALIB_BALANCETEMP_NUM);
		memset(&AfeCalib[AFECALIB_DIETEMP_OFFSET], 0, sizeof(sint32)*AFECALIB_DIETEMP_NUM);
		memset(&AfeCalib[AFECALIB_SHUNTTEMP_OFFSET], 0, sizeof(sint32)*AFECALIB_SHUNTTEMP_NUM);
		memset(&AfeCalib[AFECALIB_PACKTEMP_OFFSET], 0, sizeof(sint32)*AFECALIB_PACKTEMP_NUM);
		memset(&AfeCalib[AFECALIB_AIRTEMP_OFFSET], 0, sizeof(sint32)*AFECALIB_AIRTEMP_NUM);
		break;

		
	case CAN_CALIB_ClearCalib_B0_Current:
		AfeCalib[AFECALIB_1stCURRENT_OFFSET] = 0;
		AfeCalib[AFECALIB_2ndCURRENT_OFFSET] = 0;
		break;
		
	case CAN_CALIB_ClearFunctionSafety_B0_All:
		for(phyDevId = 0; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
		{
			memset(&AfeSim_Device[phyDevId].diag, 0, sizeof(AfeSim_DiagModel));
		}
		break;
	}
	taskEXIT_CRITICAL();

}



void AfeCalib_CanMsgDecode_AfeSimControl(uint8* data)
{
	switch(data[0])
	{
	case CAN_CALIB_AfeSimControl_B0_Ignore:
		break;
	case CAN_CALIB_AfeSimControl_B0_Reset:
		taskENTER_CRITICAL();
		memset(AfeCalib, 0, sizeof(AfeCalib));
		BatterySim_Init();
		AfeSim_Init();
		RTE_Write_BatterySOXsimEnable(1);
		RTE_Write_BatteryTempSimEnable(1);
		RTE_Write_BatteryBalSimEnable(1);
		RTE_Write_AfeGpioSimEnable(1);
		RTE_Write_DeviceRegSimEnable(1);
		taskEXIT_CRITICAL();
		break;
	case CAN_CALIB_AfeSimControl_B0_StartAll:
		RTE_Write_BatterySOXsimEnable(1);
		RTE_Write_BatteryTempSimEnable(1);
		RTE_Write_BatteryBalSimEnable(1);
		RTE_Write_AfeGpioSimEnable(1);
		RTE_Write_DeviceRegSimEnable(1);
		break;
	case CAN_CALIB_AfeSimControl_B0_PauseAll:
		AfeCalib_LatchCellVolt();
		AfeCalib_LatchGpioVolt();
		RTE_Write_BatterySOXsimEnable(0);
		RTE_Write_BatteryTempSimEnable(0);
		RTE_Write_BatteryBalSimEnable(0);
		RTE_Write_AfeGpioSimEnable(0);
		RTE_Write_DeviceRegSimEnable(0);
		break;
	case CAN_CALIB_AfeSimControl_B0_StartSOX:
		RTE_Write_BatterySOXsimEnable(1);
		break;
	case CAN_CALIB_AfeSimControl_B0_PauseSOX:
		AfeCalib_LatchCellVolt();
		RTE_Write_BatterySOXsimEnable(0);
		break;
	case CAN_CALIB_AfeSimControl_B0_StartHeat:
		RTE_Write_BatteryTempSimEnable(1);
		break;
	case CAN_CALIB_AfeSimControl_B0_PauseHeat:
		AfeCalib_LatchGpioVolt();
		RTE_Write_BatteryTempSimEnable(0);
		break;
	case CAN_CALIB_AfeSimControl_B0_StartBal:
		RTE_Write_BatteryBalSimEnable(1);
		break;
	case CAN_CALIB_AfeSimControl_B0_PauseBal:
		RTE_Write_BatteryBalSimEnable(0);
		break;
	case CAN_CALIB_AfeSimControl_B0_StartGpio:
		RTE_Write_AfeGpioSimEnable(1);
		break;
	case CAN_CALIB_AfeSimControl_B0_PauseGpio:
		AfeCalib_LatchGpioVolt();
		RTE_Write_AfeGpioSimEnable(0);
		break;
	case CAN_CALIB_AfeSimControl_B0_StartReg:
		RTE_Write_DeviceRegSimEnable(1);
		break;
	case CAN_CALIB_AfeSimControl_B0_PauseReg:
		RTE_Write_DeviceRegSimEnable(0);
		break;
		
	case CAN_CALIB_AfeSimControl_B0_SOXuseCust:
		BatSim_SoxTableX = &Cust_SocTableX;
		BatSim_SoxTableY = &Cust_SocTableY;
		break;
	case CAN_CALIB_AfeSimControl_B0_SOXuseNCM:
		BatSim_SoxTableX = &NCM_SocTableX;
		BatSim_SoxTableY = &NCM_SocTableY;
		break;
	case CAN_CALIB_AfeSimControl_B0_SOXuseLFP:
		BatSim_SoxTableX = &LFP_SocTableX;
		BatSim_SoxTableY = &LFP_SocTableY;
		break;

	case CAN_CALIB_AfeSimControl_B0_ModuleTempUseCust:
		ModuleNTCtable_temp_mC = &ModuleNTCtable_temp_mC_Cust;
		ModuleNTCtable_volt = &ModuleNTCtable_volt_Cust;
		break;
	case CAN_CALIB_AfeSimControl_B0_ModuleTempUseDefault1:
		ModuleNTCtable_temp_mC = &ModuleNTCtable_temp_mC_Default1;
		ModuleNTCtable_volt = &ModuleNTCtable_volt_Default1;
		break;
	case CAN_CALIB_AfeSimControl_B0_BalTempUseCust:
		BalanceNTCtable_temp_mC = &BalanceNTCtable_temp_mC_Cust;
		BalanceNTCtable_volt = &BalanceNTCtable_volt_Cust;
		break;
	case CAN_CALIB_AfeSimControl_B0_BalTempUseDefault1:
		BalanceNTCtable_temp_mC = &BalanceNTCtable_temp_mC_Default1;
		BalanceNTCtable_volt = &BalanceNTCtable_volt_Default1;
		break;
	case CAN_CALIB_AfeSimControl_B0_ShuntTempUseCust:
		ShuntNTCtable_temp_mC = &ShuntNTCtable_temp_mC_Cust;
		ShuntNTCtable_volt = &ShuntNTCtable_volt_Cust;
		break;
	case CAN_CALIB_AfeSimControl_B0_ShuntTempUseDefault1:
		ShuntNTCtable_temp_mC = &ShuntNTCtable_temp_mC_Default1;
		ShuntNTCtable_volt = &ShuntNTCtable_volt_Default1;
		break;
	}
}


/* AfeCalib_CanMsgDecode_RealAfeCellNum
 * 接收CAN报文 0x002, 设定仿真AFE最大数量和仿真电芯最大数量 */
void AfeCalib_CanMsgDecode_RealAfeCellNum(uint8* data)
{
	uint8 loop;
	uint8 afeNum = (data[0]);	/* 1 - 15 */
	uint16 cellNum = ((data[2]<<8) + data[1]);
	if(afeNum < 1)
		afeNum = 1;
	if(afeNum > AFESIM_MAX_AFE_NUM)
		afeNum = AFESIM_MAX_AFE_NUM;
	if(cellNum < AFESIM_MAX_CELL_NUM)
		cellNum = AFESIM_MAX_CELL_NUM;
	if(cellNum > BAT_MAX_NUM)
		cellNum = BAT_MAX_NUM;
	AfeMan->afeChipRealNum = afeNum;
	BatMan->maxBatNum = cellNum;
	for(loop = 0; loop <= AFESIM_MAX_AFE_NUM; loop++)
	{
		if(loop <= AfeMan->afeChipRealNum)
		{
			if(AfeSim_Device[loop].power.powerState == AFESIM_DEVICE_DESTROYED)
			{
				AfeSim_Device[loop].power.powerState = AFESIM_DEVICE_POWER_SHUTDOWN;
			}
		}
		else
		{
			AfeSim_Device[loop].power.powerState = AFESIM_DEVICE_DESTROYED;
		}
	}
}


/* AfeCalib_ChangeAfeUseType
 * 接收CAN报文 0x003，选用指定类型的AFE，需要重新初始化AFE仿真驱动 */
void AfeCalib_ChangeAfeUseType(uint8* data)
{
	taskENTER_CRITICAL();
	uint8 afeUseType = data[0];
	afeUseType = afeUseType > 1 ? 1 : afeUseType;
	RTE_Write_AfeSimUseType(afeUseType);
	memset(AfeCalib, 0, sizeof(AfeCalib));
	BatterySim_Init();
	AfeSim_Init();
	RTE_Write_BatterySOXsimEnable(1);
	RTE_Write_BatteryTempSimEnable(1);
	RTE_Write_BatteryBalSimEnable(1);
	RTE_Write_AfeGpioSimEnable(1);
	RTE_Write_DeviceRegSimEnable(1);
	taskEXIT_CRITICAL();
}




/* AfeCalib_CanMsgDecode_CustSocTable
   接收CAN报文 0x014 设置外部SOC-电压查找表
*/
void AfeCalib_CanMsgDecode_CustSocTable(uint8* data)
{
	uint8 index = data[0];
	uint16 cellVolt = (data[2]<<8) + data[1];
	sint32 soc = (data[4]<<8) + data[3];

	index = index >= BATSIM_TABLE_MAX_LEN ? BATSIM_TABLE_MAX_LEN-1 : index;
	soc = soc > 500 ? 500 : soc;

	Cust_SocTableX[index] = BAT_SOC_AH(soc);
	Cust_SocTableY[index] = cellVolt;
}


/* AfeCalib_CanMsgDecode_CustModuleTempTable
   接收CAN报文 0x015 设置外部模组温度-电压查找表
*/
void AfeCalib_CanMsgDecode_CustModuleTempTable(uint8* data)
{
	uint8 index = data[0];
	uint16 gpioVolt = (data[2]<<8) + data[1];
	sint16 temp = (data[4]<<8) + data[3];

	index = index >= BATSIM_TABLE_MAX_LEN ? BATSIM_TABLE_MAX_LEN-1 : index;
	gpioVolt = gpioVolt > 10000 ? 10000 : gpioVolt;
	temp = temp > 150 ? 150 : temp ;
	temp = temp < -50 ? -50 : temp ;
	
	ModuleNTCtable_temp_mC_Cust[index] = (sint32)temp * 1000L;
	ModuleNTCtable_volt_Cust[index] = gpioVolt;
}


/* AfeCalib_CanMsgDecode_CustBalanceTempTable
   接收CAN报文 0x016 设置外部均衡温度-电压查找表
*/
void AfeCalib_CanMsgDecode_CustBalanceTempTable(uint8* data)
{
	uint8 index = data[0];
	uint16 gpioVolt = (data[2]<<8) + data[1];
	sint16 temp = (data[4]<<8) + data[3];

	index = index >= BATSIM_TABLE_MAX_LEN ? BATSIM_TABLE_MAX_LEN-1 : index;
	gpioVolt = gpioVolt > 10000 ? 10000 : gpioVolt;
	temp = temp > 150 ? 150 : temp ;
	temp = temp < -50 ? -50 : temp ;
	
	BalanceNTCtable_temp_mC_Cust[index] = (sint32)temp * 1000L;
	BalanceNTCtable_volt_Cust[index] = gpioVolt;
}


/* AfeCalib_CanMsgDecode_CustShuntTempTable
   接收CAN报文 0x017 设置外部Shunt温度-电压查找表
*/
void AfeCalib_CanMsgDecode_CustShuntTempTable(uint8* data)
{
	uint8 index = data[0];
	uint16 gpioVolt = (data[2]<<8) + data[1];
	sint16 temp = (data[4]<<8) + data[3];
	
	index = index >= BATSIM_TABLE_MAX_LEN ? BATSIM_TABLE_MAX_LEN-1 : index;
	gpioVolt = gpioVolt > 10000 ? 10000 : gpioVolt;
	temp = temp > 150 ? 150 : temp ;
	temp = temp < -50 ? -50 : temp ;
	
	ShuntNTCtable_temp_mC_Cust[index] = (sint32)temp * 1000L;
	ShuntNTCtable_volt_Cust[index] = gpioVolt;
}


void AfeCalib_CanMsgDecode_SampleNoise(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	AfeSim_NoiseType noiseType = data[1];
	uint16 noiseVolt = (data[3]<<8) + data[2];

	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	noiseType = noiseType >= AFESIM_NOISE_TYPE_NUM ? AFESIM_NOISE_TYPE_NUM-1 : noiseType;
	noiseVolt = noiseVolt >= 10000 ? 10000 : noiseVolt;

	if(isDevGlobal)
	{
		for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
		{
			AfeSim_Device[phyDevId].noise.volt[noiseType] = noiseVolt;
		}
	}
	else
	{
		AfeSim_Device[phyDevId].noise.volt[noiseType] = noiseVolt;
	}
	
}

/* AfeCalib_CanMsgDecode_DaisyChainBroken
 * 接收CAN报文 0x020, 将指定AFE的COMH或COML通信端口断开 */
void AfeCalib_CanMsgDecode_DaisyChainBroken(uint8* data)
{
	uint8 phyDevId = (data[0]);
	uint8 side = (data[1]) > 0 ? 1 : 0;
	uint8 onOff = (data[2]) > 0 ? 1 : 0;
	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	if(side == 0)
	{
		if(onOff)
			AfeCalib[AFECALIB_DAISYCHAIN_DISABLE_OFFSET + phyDevId] |= (1L<<0L);
		else
			AfeCalib[AFECALIB_DAISYCHAIN_DISABLE_OFFSET + phyDevId] &= ~(1L<<0L);
	}
	else
	{
		if(onOff)
			AfeCalib[AFECALIB_DAISYCHAIN_DISABLE_OFFSET + phyDevId] |= (1L<<1L);
		else
			AfeCalib[AFECALIB_DAISYCHAIN_DISABLE_OFFSET + phyDevId] &= ~(1L<<1L);
	}
}


/* AfeCalib_CanMsgDecode_DaisyChainNoise
 * 接收CAN报文 0x021, 设定菊花链每次通信的误码率（万分之一） */
void AfeCalib_CanMsgDecode_DaisyChainNoise(uint8* data)
{
	uint16 noise = (data[1]<<8) + data[0];
	noise = noise > 10000 ? 10000 : noise;
	RTE_Write_DaisyChainErrRate(noise);
}


/* AfeCalib_CanMsgDecode_DeviceBroken
 * 接收CAN报文 0x022, 将指定AFE设定为损毁状态
 * 被损毁的设备无法通信，无法唤醒　*/
void AfeCalib_CanMsgDecode_DeviceBroken(uint8* data)
{
	uint8 phyDevId = (data[0]);
	uint8 isBroken = (data[1]);
	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	AfeCalib[AFECALIB_DEVICE_DISABLE_OFFSET + phyDevId] = isBroken;
}


void AfeCalib_CanMsgDecode_CellSOX(uint8* data)
{
	uint8 isDevGlobal = 	data[0] == 0xFF ? 1 : 0;	/* 是否改变全部电芯*/
	uint8 useAbsCellPos = 	0;							/* 是否使用绝对电芯位置 */
	uint8 afePos = 			data[0];					/* AFE位置 */
	uint16 cellPos =        data[1] & 0x1F;				/* 电芯位置*/
	uint16 afeCellPos;
	uint16 initCellPos;
	uint8 isCellGlobal =    data[1] == 0xFF ? 1 : 0;	/*是否标定全部电芯电压*/
	uint16 sox = 			(data[3]<<8) + data[2];		/*SOX标定量*/

	RTE_Write_BatterySOXsimEnable(1);

	/* 使用AFE位置+电芯位置 */
	switch(AfeSim_UseType)
	{
	case AFESIM_TYPE_79718: cellPos = cellPos > AFESIM_BQ79718_CELL_NUM ? AFESIM_BQ79718_CELL_NUM : cellPos; break; /*1-18*/
	case AFESIM_TYPE_79616: cellPos = cellPos > AFESIM_BQ79616_CELL_NUM ? AFESIM_BQ79616_CELL_NUM : cellPos; break; /*1-16*/
	}
	afePos = afePos > AfeMan->afeChipRealNum ? AfeMan->afeChipRealNum : afePos;
	afePos = afePos == 0 ? 1 : afePos; /*1-15*/
	sox = sox > 500 ? 500 : sox;
	if(isDevGlobal)
	{
		if(isCellGlobal)
		{
			/*同时标定全部电芯*/
			for(afeCellPos = 0; afeCellPos < BAT_MAX_NUM; afeCellPos++)
			{
				AfeCalib[AFECALIB_CELLSOX_OFFSET + afeCellPos] = sox;
				Bat[afeCellPos].soc_mC = BAT_SOC_AH(AfeCalib[AFECALIB_CELLSOX_OFFSET + afeCellPos]); /*输入单位是安时, 需要转换成毫库伦*/
			}
		}
		else
		{
			/*同时标定全部AFE的第n个电芯*/
			for(afePos = 1; afePos <= AfeMan->afeChipRealNum; afePos++)
			{
				if(AfeSim_Device[afePos].activeCell >= cellPos)
				{
					afeCellPos = AfeMan->cellNumSum[afePos-1] + cellPos-1;
					AfeCalib[AFECALIB_CELLSOX_OFFSET + afeCellPos] = sox;
					Bat[afeCellPos].soc_mC = BAT_SOC_AH(AfeCalib[AFECALIB_CELLSOX_OFFSET + afeCellPos]); /*输入单位是安时, 需要转换成毫库伦*/
				}
			}
		}
	}
	else
	{
		if(isCellGlobal)
		{
			/*同时标定AFEx的所有电芯*/
			for(cellPos = 1; cellPos <= AfeSim_Device[afePos].activeCell ; cellPos++)
			{
				afeCellPos = AfeMan->cellNumSum[afePos-1] + cellPos-1;
				AfeCalib[AFECALIB_CELLSOX_OFFSET + afeCellPos] = sox;
				Bat[afeCellPos].soc_mC = BAT_SOC_AH(AfeCalib[AFECALIB_CELLSOX_OFFSET + afeCellPos]);
			}
		}
		else
		{
			/*标定AFEx的电芯y*/
			afeCellPos = AfeMan->cellNumSum[afePos-1] + cellPos-1;
			AfeCalib[AFECALIB_CELLSOX_OFFSET + afeCellPos] = sox;
			Bat[afeCellPos].soc_mC = BAT_SOC_AH(AfeCalib[AFECALIB_CELLSOX_OFFSET + afeCellPos]);	 /*输入单位是安时, 需要转换成毫库伦*/
		}		
	}
}


void AfeCalib_LatchCellVolt(void)
{
	uint16 loop;
	/*将当前电压存入标定数组*/
	for(loop = 0; loop < BAT_MAX_NUM; loop++)
	{
		AfeCalib[AFECALIB_CELLVOLT_OFFSET + loop] = Bat[loop].volt_100uV;
	}
}

void AfeCalib_CanMsgDecode_CellVolt(uint8* data)
{
	uint16 loop;
	uint8 isDevGlobal = 	data[0] == 0xFF ? 1 : 0;	/* 是否改变全部电芯*/
	uint8 afePos = 			data[0];					/* bit3-0 AFE位置 */
	uint8 isCellGlobal =    data[1] == 0xFF ? 1 : 0;
	uint16 cellPos =        data[1];					/*绝对电芯位置*/
	uint16 afeCellPos = 	cellPos & 0x1F;				/*AFEx的电芯位置y*/
	uint16 cellVolt = 		(data[3]<<8) + data[2];	    /*cellVolt标定量*/
	
	sint32 cellVolt_100uV = (sint32)cellVolt | AFECALIB_ENABLE_MASK;
	
	//RTE_Write_BatterySOXsimEnable(0);
	//AfeCalib_LatchCellVolt();
	
	/* 使用AFE位置+电芯位置 */
	switch(AfeSim_UseType)
	{
	case AFESIM_TYPE_79718: cellPos = cellPos > AFESIM_BQ79718_CELL_NUM ? AFESIM_BQ79718_CELL_NUM : cellPos; break; /*1-18*/
	case AFESIM_TYPE_79616: cellPos = cellPos > AFESIM_BQ79616_CELL_NUM ? AFESIM_BQ79616_CELL_NUM : cellPos; break; /*1-16*/
	}
	afePos = afePos > AfeMan->afeChipRealNum ? AfeMan->afeChipRealNum : afePos;
	afePos = afePos == 0 ? 1 : afePos; /*1-15*/
	
	if(isDevGlobal)
	{
		if(isCellGlobal)
		{
			/*同时标定全部电芯*/
			for(afeCellPos = 0; afeCellPos < BAT_MAX_NUM; afeCellPos++)
			{
				AfeCalib[AFECALIB_CELLVOLT_OFFSET + afeCellPos] = cellVolt_100uV;
			}
		}
		else
		{
			/*同时标定全部AFE的第n个电芯*/
			for(afePos = 1; afePos <= AfeMan->afeChipRealNum; afePos++)
			{
				if(AfeSim_Device[afePos].activeCell >= cellPos)
				{
					afeCellPos = AfeMan->cellNumSum[afePos-1] + cellPos-1;
					AfeCalib[AFECALIB_CELLVOLT_OFFSET + afeCellPos] = cellVolt_100uV;
				}
			}
		}
	}
	else
	{
		if(isCellGlobal)
		{
			/*同时标定AFEx的所有电芯*/
			for(cellPos = 1; cellPos <= AfeSim_Device[afePos].activeCell ; cellPos++)
			{
				afeCellPos = AfeMan->cellNumSum[afePos-1] + cellPos-1;
				AfeCalib[AFECALIB_CELLVOLT_OFFSET + afeCellPos] = cellVolt_100uV;
			}
		}
		else
		{
			/*标定AFEx的电芯y*/
			afeCellPos = AfeMan->cellNumSum[afePos-1] + cellPos-1;
			AfeCalib[AFECALIB_CELLVOLT_OFFSET + afeCellPos] = cellVolt_100uV;
		}		
	}
}

void AfeCalib_CanMsgDecode_CellVoltBias(uint8* data)
{
	uint16 loop;
	uint8 isDevGlobal = 	data[0] == 0xFF ? 1 : 0;	/* 是否改变全部电芯*/
	uint8 afePos = 			data[0];					/* bit3-0 AFE位置 */
	uint16 cellPos =        data[1];					/*绝对电芯位置*/
	uint8 isCellGlobal =    data[1] == 0xFF ? 1 : 0;
	uint16 afeCellPos;									/*AFEx的电芯位置y*/
	sint16 cellVoltBias = 	(data[3]<<8) + data[2];	    /*cellVolt标定量*/
	sint32 cellVoltBias_100uV;
	cellVoltBias = cellVoltBias > 6553 ? 6553 : cellVoltBias;
	cellVoltBias = cellVoltBias < -6553 ? -6553 : cellVoltBias;
	cellVoltBias_100uV = (sint32)cellVoltBias * 10;

	/* 使用AFE位置+电芯位置 */
	switch(AfeSim_UseType)
	{
	case AFESIM_TYPE_79718: cellPos = cellPos > AFESIM_BQ79718_CELL_NUM ? AFESIM_BQ79718_CELL_NUM : cellPos; break; /*1-18*/
	case AFESIM_TYPE_79616: cellPos = cellPos > AFESIM_BQ79616_CELL_NUM ? AFESIM_BQ79616_CELL_NUM : cellPos; break; /*1-16*/
	}
	afePos = afePos > AfeMan->afeChipRealNum ? AfeMan->afeChipRealNum : afePos;
	afePos = afePos == 0 ? 1 : afePos; /*1-15*/
	
	if(isDevGlobal)
	{
		if(isCellGlobal)
		{
			/*同时标定全部电芯*/
			for(afeCellPos = 0; afeCellPos < BAT_MAX_NUM; afeCellPos++)
			{
				AfeCalib[AFECALIB_CELLVOLT_BIAS_OFFSET + afeCellPos] = cellVoltBias_100uV;
			}
		}
		else
		{
			/*同时标定全部AFE的第n个电芯*/
			for(afePos = 1; afePos <= AfeMan->afeChipRealNum; afePos++)
			{
				if(AfeSim_Device[afePos].activeCell >= cellPos)
				{
					afeCellPos = AfeMan->cellNumSum[afePos-1] + cellPos-1;
					AfeCalib[AFECALIB_CELLVOLT_BIAS_OFFSET + afeCellPos] = cellVoltBias_100uV;
				}
			}
		}
	}
	else
	{
		if(isCellGlobal)
		{
			/*同时标定AFEx的所有电芯*/
			for(cellPos = 1; cellPos <= AfeSim_Device[afePos].activeCell ; cellPos++)
			{
				afeCellPos = AfeMan->cellNumSum[afePos-1] + cellPos-1;
				AfeCalib[AFECALIB_CELLVOLT_BIAS_OFFSET + afeCellPos] = cellVoltBias_100uV;
			}
		}
		else
		{
			/*标定AFEx的电芯y*/
			afeCellPos = AfeMan->cellNumSum[afePos-1] + cellPos-1;
			AfeCalib[AFECALIB_CELLVOLT_BIAS_OFFSET + afeCellPos] = cellVoltBias_100uV;
		}		
	}
}

void AfeCalib_LatchGpioVolt(void)
{
	uint8 phyDevId;
	uint8 gpioId;
	sint32 gpioVolt;
	for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
	{
		for(gpioId = 0; gpioId < AFESIM_MAX_GPIO_NUM; gpioId++)
		{
			gpioVolt = AfeSim_Device[phyDevId].gpio[gpioId].inputVoltage_100uV;
			AfeCalib[AFECALIB_GPIOVOLT_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId] = gpioVolt;
		}
	}
}

void AfeCalib_CanMsgDecode_GpioVolt(uint8* data)
{
	uint8 phyDevId	   = data[0];
	uint8 isDevGlobal  = data[0] == 0xFF ? 1 : 0;
	uint8 gpioId 	   = data[1];
	uint8 isGpioGlobal = data[1] == 0xFF ? 1 : 0;
	uint16 gpioVolt    = (data[3]<<8) + data[2];
	sint32 gpioVolt_100uV = (sint32)gpioVolt | AFECALIB_ENABLE_MASK;
	
	//RTE_Write_AfeGpioSimEnable(0);
	
	/* 将所有GPIO电压存入标定数组 */
	//AfeCalib_LatchGpioVolt();

	/* 1 <= phyDevId <= 15 */
	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	phyDevId = phyDevId < 1 ? 1 : phyDevId;
	/* 1 <= gpioId <= 11 */
	gpioId = gpioId > AFESIM_MAX_GPIO_NUM ? AFESIM_MAX_GPIO_NUM : gpioId;
	gpioId = gpioId < 1 ? 1 : gpioId;

	if(isDevGlobal)
	{
		if(isGpioGlobal)
		{
			for(gpioId = 1; gpioId <= AFESIM_MAX_GPIO_NUM; gpioId++)
			{
				for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
				{
					AfeCalib[AFECALIB_GPIOVOLT_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId-1] = gpioVolt_100uV;
				}
			}
		}
		else
		{
			for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
			{
				AfeCalib[AFECALIB_GPIOVOLT_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId-1] = gpioVolt_100uV;
			}
		}

	}
	else
	{
		if(isGpioGlobal)
		{
			for(gpioId = 1; gpioId <= AFESIM_MAX_GPIO_NUM; gpioId++)
			{
				AfeCalib[AFECALIB_GPIOVOLT_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId-1] = gpioVolt_100uV;
			}
		}
		else
		{
			AfeCalib[AFECALIB_GPIOVOLT_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId-1] = gpioVolt_100uV;
		}
	}
}

void AfeCalib_CanMsgDecode_GpioVoltBias(uint8* data)
{
	uint8 phyDevId      = data[0];
	uint8 isDevGlobal   = data[0] == 0xFF ? 1 : 0;
	uint8 gpioId        = data[1];
	uint8 isGpioGlobal  = data[1] == 0xFF ? 1 : 0;
	sint16 gpioVoltBias = (data[3]<<8) + data[2];
	sint32 gpioVoltBias_100uV;
	/* 1 <= phyDevId <= 15 */
	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	phyDevId = phyDevId < 1 ? 1 : phyDevId;
	/* 1 <= gpioId <= 11 */
	gpioId = gpioId > AFESIM_MAX_GPIO_NUM ? AFESIM_MAX_GPIO_NUM : gpioId;
	gpioId = gpioId < 1 ? 1 : gpioId;

	gpioVoltBias = gpioVoltBias > 6553 ? 6553 : gpioVoltBias;
	gpioVoltBias = gpioVoltBias < -6553 ? -6553 : gpioVoltBias;
	gpioVoltBias_100uV = (sint32)gpioVoltBias * 10;
	
	if(isDevGlobal)
	{
		if(isGpioGlobal)
		{
			for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
			{
				for(gpioId = 1; gpioId <= AFESIM_MAX_GPIO_NUM; gpioId++)
				{
					AfeCalib[AFECALIB_GPIOVOLT_BIAS_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId-1] = gpioVoltBias_100uV;
				}
			}
		}
		else
		{
			for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
			{
				AfeCalib[AFECALIB_GPIOVOLT_BIAS_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId-1] = gpioVoltBias_100uV;
			}
		}
	}
	else
	{
		if(isGpioGlobal)
		{
			for(gpioId = 1; gpioId <= AFESIM_MAX_GPIO_NUM; gpioId++)
			{
				AfeCalib[AFECALIB_GPIOVOLT_BIAS_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId-1] = gpioVoltBias_100uV;
			}
		}
		else
		{
			AfeCalib[AFECALIB_GPIOVOLT_BIAS_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId-1] = gpioVoltBias_100uV;
		}
	}
}


void AfeCalib_CanMsgDecode_ModuleVolt(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint16 moduleVolt = (data[2]<<8) + data[1];

	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	phyDevId = phyDevId < 1 ? 1 : phyDevId;
	moduleVolt = moduleVolt > 1310 ? 1310 : moduleVolt;
	
	sint32 calibModuleVolt_mV = (100 * moduleVolt) | AFECALIB_ENABLE_MASK;

	if(isDevGlobal)
	{
		for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
		{
			AfeCalib[AFECALIB_MODULEVOLT_OFFSET + phyDevId] = calibModuleVolt_mV;
		}
	}
	else
	{
		AfeCalib[AFECALIB_MODULEVOLT_OFFSET + phyDevId] = calibModuleVolt_mV;
	}
}

void AfeCalib_CanMsgDecode_ModuleVoltBias(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	sint16 moduleVoltBias = (data[2]<<8) + data[1];
	
	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	phyDevId = phyDevId < 1 ? 1 : phyDevId;

	moduleVoltBias = moduleVoltBias > 1310 ? 1310 : moduleVoltBias;
	moduleVoltBias = moduleVoltBias < -1310 ? -1310: moduleVoltBias;
	
	sint32 calibModuleVoltBias_mV = (100 * moduleVoltBias);
	if(isDevGlobal)
	{
		for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
		{
			AfeCalib[AFECALIB_MODULEVOLT_BIAS_OFFSET + phyDevId] = calibModuleVoltBias_mV;
		}
	}
	else
	{
		AfeCalib[AFECALIB_MODULEVOLT_BIAS_OFFSET + phyDevId] = calibModuleVoltBias_mV;
	}
}


void AfeCalib_CanMsgDecode_TSREFVolt(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint16 tsrefVolt = (data[2]<<8) + data[1];
	
	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	phyDevId = phyDevId < 1 ? 1 : phyDevId;

	sint32 calibTsrefVolt_100uV = tsrefVolt | AFECALIB_ENABLE_MASK;
	if(isDevGlobal)
	{
		for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
		{
			AfeCalib[AFECALIB_TSREF_OFFSET + phyDevId] = calibTsrefVolt_100uV;
		}
	}
	else
	{
		AfeCalib[AFECALIB_TSREF_OFFSET + phyDevId] = calibTsrefVolt_100uV;
	}
}

void AfeCalib_CanMsgDecode_TSREFVoltBias(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	sint16 tsrefVoltBias = (data[2]<<8) + data[1];
	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	phyDevId = phyDevId < 1 ? 1 : phyDevId;

	tsrefVoltBias = tsrefVoltBias > 6553 ? 6553: tsrefVoltBias;
	tsrefVoltBias = tsrefVoltBias < -6553 ? -6553: tsrefVoltBias;

	sint32 calibTsrefVoltBias_100uV = tsrefVoltBias * 10;
	if(isDevGlobal)
	{
		for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
		{
			AfeCalib[AFECALIB_TSREF_BIAS_OFFSET + phyDevId] = calibTsrefVoltBias_100uV;
		}
	}
	else
	{
		AfeCalib[AFECALIB_TSREF_BIAS_OFFSET + phyDevId] = calibTsrefVoltBias_100uV;
	}
}

void AfeCalib_CanMsgDecode_AuxVolt(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint8 auxType = data[1];
	uint32 auxVolt_100uV = (data[4]<<16) + (data[3]<<8) + data[2];

	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	phyDevId = phyDevId < 1 ? 1 : phyDevId;
	auxType = auxType >= AFESIM_BQ79616_AUX_MAXNUM ? AFESIM_BQ79616_AUX_MAXNUM - 1 : auxType;
	if(isDevGlobal)
	{
		for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
		{
			AfeCalib[AFECALIB_AUXVOLT_OFFSET + phyDevId*AFECALIB_AUXVOLT_SINGLE_CHIP_NUM + auxType] = 
						AFECALIB_ENABLE_MASK | auxVolt_100uV;
		}
	}
	else
	{
		AfeCalib[AFECALIB_AUXVOLT_OFFSET + phyDevId*AFECALIB_AUXVOLT_SINGLE_CHIP_NUM + auxType] = 
					AFECALIB_ENABLE_MASK | auxVolt_100uV;
	}
	
}

void AfeCalib_CanMsgDecode_AuxVoltBias(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint8 auxType = data[1];
	sint32 auxVoltBias_100uV = (data[5]<<24) + (data[4]<<16) + (data[3]<<8) + data[2];

	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	phyDevId = phyDevId < 1 ? 1 : phyDevId;
	auxType = auxType >= AFESIM_BQ79616_AUX_MAXNUM ? AFESIM_BQ79616_AUX_MAXNUM - 1 : auxType;
	if(isDevGlobal)
	{
		for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
		{
			AfeCalib[AFECALIB_AUXVOLT_BIAS_OFFSET + phyDevId*AFECALIB_AUXVOLT_SINGLE_CHIP_NUM + auxType] = auxVoltBias_100uV;
		}
	}
	else
	{
		AfeCalib[AFECALIB_AUXVOLT_BIAS_OFFSET + phyDevId*AFECALIB_AUXVOLT_SINGLE_CHIP_NUM + auxType] = auxVoltBias_100uV;
	}
}


void AfeCalib_CanMsgDecode_ModuleTemp(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint8 moduleTempId = data[1];
	sint16 moduleTemp_100mC = (data[3]<<8) + data[2];
	uint8 isTempGlobal = data[1] == 0xFF ? 1 : 0;

	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	phyDevId = phyDevId < 1 ? 1 : phyDevId;
	moduleTempId = moduleTempId > AFESIM_MAX_GPIO_NUM ? AFESIM_MAX_GPIO_NUM : moduleTempId;
	moduleTempId = moduleTempId < 1 ? 1 : moduleTempId;
	moduleTemp_100mC = moduleTemp_100mC > 1500 ? 1500 : moduleTemp_100mC;
	moduleTemp_100mC = moduleTemp_100mC < -500 ? -500 : moduleTemp_100mC;

	sint32 calibModuleTemp_mC = (((sint32)moduleTemp_100mC+1000) * 100) | AFECALIB_ENABLE_MASK;
	if(isDevGlobal)
	{
		for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
		{
			if(isTempGlobal)
			{
				for(moduleTempId = 1; moduleTempId <= AFESIM_MAX_MT_NUM; moduleTempId++)
				{
					AfeCalib[AFECALIB_MODULETEMP_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + (moduleTempId - 1)] = calibModuleTemp_mC;
				}
			}
			else
			{
				AfeCalib[AFECALIB_MODULETEMP_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + (moduleTempId - 1)] = calibModuleTemp_mC;
			}
		}
	}
	else
	{
		if(isTempGlobal)
		{
			for(moduleTempId = 1; moduleTempId <= AFESIM_MAX_MT_NUM; moduleTempId++)
			{
				AfeCalib[AFECALIB_MODULETEMP_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + (moduleTempId - 1)] = calibModuleTemp_mC;
			}
		}
		else
		{
			AfeCalib[AFECALIB_MODULETEMP_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + (moduleTempId - 1)] = calibModuleTemp_mC;
		}

	}
}

void AfeCalib_CanMsgDecode_BalanceTemp(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint8 balanceTempId = data[1];
	sint16 balanceTemp_100mC = (data[3]<<8) + data[2];
	uint8 isTempGlobal = data[1] == 0xFF ? 1 : 0;

	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	phyDevId = phyDevId < 1 ? 1 : phyDevId;
	balanceTempId = balanceTempId > AFESIM_MAX_GPIO_NUM ? AFESIM_MAX_GPIO_NUM : balanceTempId;
	balanceTempId = balanceTempId < 1 ? 1 : balanceTempId;
	balanceTemp_100mC = balanceTemp_100mC > 1500 ? 1500 : balanceTemp_100mC;
	balanceTemp_100mC = balanceTemp_100mC < -500 ? -500 : balanceTemp_100mC;

	sint32 calibBalanceTemp_mC = (((sint32)balanceTemp_100mC+1000) * 100) | AFECALIB_ENABLE_MASK;
	if(isDevGlobal)
	{
		for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
		{
			if(isTempGlobal)
			{
				for(balanceTempId = 7; balanceTempId <= 6 + AFESIM_MAX_BT_NUM; balanceTempId++)
				{
					AfeCalib[AFECALIB_BALANCETEMP_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + (balanceTempId - 1)] = calibBalanceTemp_mC;
				}
			}
			else
			{
				AfeCalib[AFECALIB_BALANCETEMP_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + (balanceTempId - 1)] = calibBalanceTemp_mC;
			}
		}
	}
	else
	{
		if(isTempGlobal)
		{
			for(balanceTempId = 7; balanceTempId <= 6 + AFESIM_MAX_BT_NUM; balanceTempId++)
			{
				AfeCalib[AFECALIB_BALANCETEMP_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + (balanceTempId - 1)] = calibBalanceTemp_mC;
			}
		}
		else
		{
			AfeCalib[AFECALIB_BALANCETEMP_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + (balanceTempId - 1)] = calibBalanceTemp_mC;
		}
	}
}

void AfeCalib_CanMsgDecode_DieTemp(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint8 dieTempId = data[1];
	uint8 isTempGlobal = data[1] == 0xFF ? 1 : 0;
	sint16 dieTemp_100mC = (data[3]<<8) + data[2];

	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	phyDevId = phyDevId < 1 ? 1 : phyDevId;
	dieTempId = dieTempId > AFESIM_MAX_DT_NUM ? AFESIM_MAX_DT_NUM : dieTempId;
	dieTempId = dieTempId < 1 ? 1 : dieTempId;
	dieTemp_100mC = dieTemp_100mC > 1500 ? 1500 : dieTemp_100mC;
	dieTemp_100mC = dieTemp_100mC < -500 ? -500 : dieTemp_100mC;

	sint32 calibDieTemp_mC = (((sint32)dieTemp_100mC+1000) * 100) | AFECALIB_ENABLE_MASK;
	if(isDevGlobal)
	{
		for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
		{
			if(isTempGlobal)
			{
				for(dieTempId = 1; dieTempId <= AFESIM_MAX_DT_NUM; dieTempId++)
				{
					AfeCalib[AFECALIB_DIETEMP_OFFSET + phyDevId*AFESIM_MAX_DT_NUM + (dieTempId - 1)] = calibDieTemp_mC;
				}
			}
			else
			{
				AfeCalib[AFECALIB_DIETEMP_OFFSET + phyDevId*AFESIM_MAX_DT_NUM + (dieTempId - 1)] = calibDieTemp_mC;
			}
		}
	}
	else
	{
		if(isTempGlobal)
		{
			for(dieTempId = 1; dieTempId <= AFESIM_MAX_DT_NUM; dieTempId++)
			{
				AfeCalib[AFECALIB_DIETEMP_OFFSET + phyDevId*AFESIM_MAX_DT_NUM + (dieTempId - 1)] = calibDieTemp_mC;
			}
		}
		else
		{
			AfeCalib[AFECALIB_DIETEMP_OFFSET + phyDevId*AFESIM_MAX_DT_NUM + (dieTempId - 1)] = calibDieTemp_mC;
		}

	}
}

void AfeCalib_CanMsgDecode_ShuntTemp(uint8* data)
{
	//uint8 phyDevId = data[0];
	//uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint8 shuntTempId = data[0];
	sint16 shuntTemp_100mC = (data[2]<<8) + data[1];
	uint8 isTempGlobal = data[0] == 0xFF ? 1 : 0;

	//phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	//phyDevId = phyDevId < 1 ? 1 : phyDevId;
	shuntTempId = shuntTempId > AFESIM_MAX_ST_NUM ? AFESIM_MAX_ST_NUM : shuntTempId;
	shuntTempId = shuntTempId < 1 ? 1 : shuntTempId;
	shuntTemp_100mC = shuntTemp_100mC > 1500 ? 1500 : shuntTemp_100mC;
	shuntTemp_100mC = shuntTemp_100mC < -500 ? -500 : shuntTemp_100mC;

	sint32 calibShuntTemp_mC = (((sint32)shuntTemp_100mC+1000) * 100) | AFECALIB_ENABLE_MASK;
	if(isTempGlobal)
	{
		AfeCalib[AFECALIB_SHUNTTEMP_OFFSET + 1] = calibShuntTemp_mC;
		AfeCalib[AFECALIB_SHUNTTEMP_OFFSET + 0] = calibShuntTemp_mC;
	}
	else
	{
		AfeCalib[AFECALIB_SHUNTTEMP_OFFSET + shuntTempId - 1] = calibShuntTemp_mC;
	}
}

void AfeCalib_CanMsgDecode_PackTemp(uint8* data)
{
	sint16 packTemp_100mC = (data[1]<<8) + data[0];

	packTemp_100mC = packTemp_100mC > 1500 ? 1500 : packTemp_100mC;
	packTemp_100mC = packTemp_100mC < -500 ? -500 : packTemp_100mC;

	sint32 calibPackTemp_mC = (((sint32)packTemp_100mC+1000) * 100) | AFECALIB_ENABLE_MASK;
	AfeCalib[AFECALIB_PACKTEMP_OFFSET] = calibPackTemp_mC;
}

void AfeCalib_CanMsgDecode_AirTemp(uint8* data)
{
	sint16 airTemp_100mC = (data[1]<<8) + data[0];

	airTemp_100mC = airTemp_100mC > 1500 ? 1500 : airTemp_100mC;
	airTemp_100mC = airTemp_100mC < -500 ? -500 : airTemp_100mC;

	sint32 calibAirTemp_mC = (((sint32)airTemp_100mC+1000) * 100) | AFECALIB_ENABLE_MASK;
	AfeCalib[AFECALIB_AIRTEMP_OFFSET] = calibAirTemp_mC;
}

void AfeCalib_BQ79616_DiagBistFaultInject(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint8 bistType = data[1];
	uint32 faultMask = (data[3]<<8) + data[2];
	bistType = bistType >= AFESIM_DIAG_BIST_TYPE_NUM ? AFESIM_DIAG_BIST_TYPE_NUM - 1 : bistType;

	if(isDevGlobal)
	{
		for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
		{
			AfeSim_Device[phyDevId].diag.bist.bist_WillFail[bistType] = faultMask;
		}
	}
	else
	{
		AfeSim_Device[phyDevId].diag.bist.bist_WillFail[bistType] = faultMask;
	}
}




void AfeCalib_BQ79616_DiagCycleFaultInject(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint8 diagType = data[1];
	uint32 faultMask = (data[3]<<8) + data[2];
	diagType = diagType >= AFESIM_DIAG_CYCLE_TYPE_NUM ? AFESIM_DIAG_CYCLE_TYPE_NUM - 1 : diagType;

	if(isDevGlobal)
	{
		for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
		{
			AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[diagType] = faultMask;
		}
	}
	else
	{
		AfeSim_Device[phyDevId].diag.cycle.diag_WillFail[diagType] = faultMask;
	}
}



void AfeCalib_CanMsgDecode_RandRegReadA(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint8 regNum = data[1];
	uint16 startRegAddr = (data[3]<<8) + data[2];
	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	startRegAddr = startRegAddr + regNum > AFESIM_MAX_REG_NUM ? AFESIM_MAX_REG_NUM - regNum: startRegAddr;
	AfeCalib_Manager.randRegHandler.readPhyDevId = phyDevId;
	AfeCalib_Manager.randRegHandler.readEnable = 1;
	if(isDevGlobal)
	{
		AfeCalib_Manager.randRegHandler.readLen = ((uint16)regNum + 1 >= 16) ? (16) : ((uint16)regNum + 1);
	}
	else
	{
		AfeCalib_Manager.randRegHandler.readLen = (uint16)regNum + 1;
	}
	AfeCalib_Manager.randRegHandler.readStartRegPos = startRegAddr;
	AfeCalib_Manager.randRegHandler.isGlobalR = isDevGlobal;
}

void AfeCalib_CanMsgDecode_RandRegWriteA(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint8 regNum = data[1];
	uint16 startRegAddr = (data[3]<<8) + data[2];
	uint16 loop;
	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	regNum = regNum > 4 ? 4 : regNum;
	regNum = regNum < 1 ? 1 : regNum;
	startRegAddr = startRegAddr + regNum > AFESIM_MAX_REG_NUM ? AFESIM_MAX_REG_NUM - regNum: startRegAddr;
	if(isDevGlobal)
	{
		for(phyDevId = 0; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
		{
			for(loop = 0; loop < regNum; loop++)
			{
				AfeSim_Reg[phyDevId][startRegAddr + loop] = data[4 + loop];
			}
		}
	}
	else
	{
		for(loop = 0; loop < regNum; loop++)
		{
			AfeSim_Reg[phyDevId][startRegAddr + loop] = data[4 + loop];
		}

	}
}

void AfeCalib_CanMsgDecode_RandRegWriteB(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint8 regNum = data[1];
	uint8 loopSingleLen = data[2];
	uint16 startRegAddr = (data[4]<<8) + data[3];
	uint16 loop;
	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	loopSingleLen = loopSingleLen > 3 ? 3 : loopSingleLen;
	loopSingleLen = loopSingleLen < 1 ? 1 : loopSingleLen;
	startRegAddr = startRegAddr + regNum > AFESIM_MAX_REG_NUM ? AFESIM_MAX_REG_NUM - regNum: startRegAddr;
	if(isDevGlobal)
	{
		for(phyDevId = 0; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
		{
			for(loop = 0; loop <= regNum; loop++)
			{
				AfeSim_Reg[phyDevId][startRegAddr + loop] = data[5 + (loop % loopSingleLen)];
			}
		}
	}
	else
	{
		for(loop = 0; loop <= regNum; loop++)
		{
			AfeSim_Reg[phyDevId][startRegAddr + loop] = data[5 + (loop % loopSingleLen)];
		}
	}

}

void AfeCalib_CanMsgDecode_RandRegWriteC1(uint8* data)
{
	uint8 phyDevId = data[0];
	uint8 isDevGlobal = data[0] == 0xFF ? 1 : 0;
	uint8 regNum = data[1];
	uint16 startRegAddr = (data[3]<<8) + data[2];
	phyDevId = phyDevId > AFESIM_MAX_AFE_NUM ? AFESIM_MAX_AFE_NUM : phyDevId;
	regNum = regNum > 8 ? 8 : regNum;
	regNum = regNum < 1 ? 1 : regNum;
	startRegAddr = startRegAddr + regNum > AFESIM_MAX_REG_NUM ? AFESIM_MAX_REG_NUM - regNum: startRegAddr;
	AfeCalib_Manager.randRegHandler.writeEnable = 1;
	AfeCalib_Manager.randRegHandler.writeContinuousSingleLen = regNum;
	AfeCalib_Manager.randRegHandler.writePhyDevId = phyDevId;
	AfeCalib_Manager.randRegHandler.writeStartRegPos = startRegAddr;
	AfeCalib_Manager.randRegHandler.writeLoopLen = 0;
	AfeCalib_Manager.randRegHandler.isGlobalW = isDevGlobal;
}

void AfeCalib_CanMsgDecode_RandRegWriteC2(uint8* data)
{
	uint8 phyDevId = AfeCalib_Manager.randRegHandler.writePhyDevId;
	uint16 regAddr = AfeCalib_Manager.randRegHandler.writeStartRegPos + AfeCalib_Manager.randRegHandler.writeLoopLen;
	uint8 loop;
	if(AfeCalib_Manager.randRegHandler.writeEnable)
	{
		if(AfeCalib_Manager.randRegHandler.isGlobalW)
		{
			for(phyDevId = 0; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
			{
				for(loop = 0; loop < AfeCalib_Manager.randRegHandler.writeContinuousSingleLen; loop++)
				{
					AfeSim_Reg[phyDevId][regAddr + loop] = data[loop];
				}
			}
		}
		else
		{
			for(loop = 0; loop < AfeCalib_Manager.randRegHandler.writeContinuousSingleLen; loop++)
			{
				AfeSim_Reg[phyDevId][regAddr + loop] = data[loop];
			}
		}
	}
	AfeCalib_Manager.randRegHandler.writeLoopLen += AfeCalib_Manager.randRegHandler.writeContinuousSingleLen;
}


void AfeCalib_CanMsgDecode(CAN_MessageType* canMsg)
{
	uint16 msgId = canMsg->msgId.field.msgId;
	uint8* data = canMsg->data.data8;
	uint32* buff = canMsg->data.data32;
	uint8 cellPos;
	switch(msgId)
	{
	/* AFE仿真器控制标定 */
	case CAN_CALIB_ClearCalib_MSGID:
		AfeCalib_CanMsgDecode_ClearCalib(data);
		break;
	case CAN_CALIB_AfeSimControl_MSGID:
		AfeCalib_CanMsgDecode_AfeSimControl(data);
		break;
	case CAN_CALIB_RealAfeCellNum_MSGID:
		AfeCalib_CanMsgDecode_RealAfeCellNum(data);
		break;
	case CAN_CALIB_AfeUseType_MSGID:
		AfeCalib_ChangeAfeUseType(data);
		break;

	/* 硬件标定 */
	case CAN_CALIB_SOXCurve_MSGID:
		AfeCalib_CanMsgDecode_CustSocTable(data);
		break;
	case CAN_CALIB_ModuleTempRT_MSGID:
		AfeCalib_CanMsgDecode_CustModuleTempTable(data);
		break;
	case CAN_CALIB_BalanceTempRT_MSGID:
		AfeCalib_CanMsgDecode_CustBalanceTempTable(data);
		break;
	case CAN_CALIB_ShuntTempRT_MSGID:
		AfeCalib_CanMsgDecode_CustShuntTempTable(data);
		break;
		
	/* 通用故障标定 */
	case CAN_CALIB_DaisyChainBroken_MSGID:
		AfeCalib_CanMsgDecode_DaisyChainBroken(data);
		break;
	case CAN_CALIB_DaisyChainNoise_MSGID:
		AfeCalib_CanMsgDecode_DaisyChainNoise(data);
		break;
	case CAN_CALIB_DeviceBroken_MSGID:
		AfeCalib_CanMsgDecode_DeviceBroken(data);
		break;
	case CAN_CALIB_SampleNoise_MSGID:
		AfeCalib_CanMsgDecode_SampleNoise(data);
		break;

	/* 电压和电流标定 */
	case CAN_CALIB_CellSOX_MSGID:
		AfeCalib_CanMsgDecode_CellSOX(data);
		break;
	case CAN_CALIB_CellVolt_MSGID:
		AfeCalib_CanMsgDecode_CellVolt(data);
		break;
	case CAN_CALIB_CellVoltBias_MSGID:
		AfeCalib_CanMsgDecode_CellVoltBias(data);
		break;

	case CAN_CALIB_GpioVolt_MSGID:
		AfeCalib_CanMsgDecode_GpioVolt(data);
		break;
	case CAN_CALIB_GpioVoltBias_MSGID:
		AfeCalib_CanMsgDecode_GpioVoltBias(data);
		break;

	case CAN_CALIB_ModuleVolt_MSGID:
		AfeCalib_CanMsgDecode_ModuleVolt(data);
		break;
	case CAN_CALIB_ModuleVoltBias_MSGID:
		AfeCalib_CanMsgDecode_ModuleVoltBias(data);
		break;
	case CAN_CALIB_TSREF_MSGID:
		AfeCalib_CanMsgDecode_TSREFVolt(data);
		break;
	case CAN_CALIB_TSREFBias_MSGID:
		AfeCalib_CanMsgDecode_TSREFVoltBias(data);
		break;
	case CAN_CALIB_AuxVolt_MSGID:
		AfeCalib_CanMsgDecode_AuxVolt(data);
		break;
	case CAN_CALIB_AuxVoltBias_MSGID:
		AfeCalib_CanMsgDecode_AuxVoltBias(data);
		break;

	case CAN_CALIB_ModuleTemp_MSGID:
		AfeCalib_CanMsgDecode_ModuleTemp(data);
		break;
	case CAN_CALIB_BalTemp_MSGID:
		AfeCalib_CanMsgDecode_BalanceTemp(data);
		break;
	case CAN_CALIB_DieTemp_MSGID:
		AfeCalib_CanMsgDecode_DieTemp(data);
		break;
	case CAN_CALIB_ShuntTemp_MSGID:
		AfeCalib_CanMsgDecode_ShuntTemp(data);
		break;
	case CAN_CALIB_PackTemp_MSGID:
		AfeCalib_CanMsgDecode_PackTemp(data);
		break;
	case CAN_CALIB_AirTemp_MSGID:
		AfeCalib_CanMsgDecode_AirTemp(data);
		break;

	/* 功能安全 */
	case CAN_CALIB_BQ79616_DiagBistFaultInject_MSGID:
		AfeCalib_BQ79616_DiagBistFaultInject(data);
		break;
	case CAN_CALIB_BQ79616_DiagKeyOnFaultInject_MSGID:
		//AfeCalib_BQ79616_DiagBistFaultInject(data);
		break;
	case CAN_CALIB_BQ79616_DiagCycleFaultInject_MSGID:
		AfeCalib_BQ79616_DiagCycleFaultInject(data);
		break;

	case CAN_CALIB_RandomRegReadA_MSGID:
		AfeCalib_CanMsgDecode_RandRegReadA(data);
		break;
	case CAN_CALIB_RandomRegWriteA_MSGID:
		AfeCalib_CanMsgDecode_RandRegWriteA(data);
		break;
	case CAN_CALIB_RandomRegWriteB_MSGID:
		AfeCalib_CanMsgDecode_RandRegWriteB(data);
		break;
	case CAN_CALIB_RandomRegWriteC1_MSGID:
		AfeCalib_CanMsgDecode_RandRegWriteC1(data);
		break;
	case CAN_CALIB_RandomRegWriteC2_MSGID:
		AfeCalib_CanMsgDecode_RandRegWriteC2(data);
		break;
		
	case CAN_CALIB_1stCurrent_MSGID:
		AfeCalib[AFECALIB_1stCURRENT_OFFSET] = (data[3]<<24) + (data[2]<<16) + (data[1]<<8) + data[0];
		break;
	case CAN_CALIB_2ndCurrentBias_MSGID:
		AfeCalib[AFECALIB_2ndCURRENT_OFFSET] = (data[3]<<24) + (data[2]<<16) + (data[1]<<8) + data[0];
		break;
	/* 温度标定 */
	}
}

void AfeCalibration_Run100ms(void)
{
	int i,j;
	uint8 phyDevId;
	uint8 gpioId;
	uint8 auxId;
	uint8 tempId;
	uint8 soxSimEnable;
	uint8 tempSimEnable;
	uint8 balSimEnable;
	uint8 gpioSimEnable;
	sint32* auxPtr;
	uint32 index;
	RTE_Read_BatterySOXsimEnable(&soxSimEnable);
	RTE_Read_BatteryTempSimEnable(&tempSimEnable);
	RTE_Read_BatteryBalSimEnable(&balSimEnable);
	RTE_Read_AfeGpioSimEnable(&gpioSimEnable);
	
	/* 处理电芯电压, 模组电压标定量 */
	if(FALSE == soxSimEnable)
	{
		for(i = 0; i < BatMan->maxBatNum; i++)
		{
			Bat[i].volt_100uV = AfeCalib[AFECALIB_CELLVOLT_OFFSET + i] & AFECALIB_DATA_MASK;
		}
	}

	/* 处理GPIO电压标定量*/
	if(FALSE == tempSimEnable || FALSE == gpioSimEnable)
	{
		for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
		{
			for(gpioId = 0; gpioId < AfeMan->afeMaxGpioNum; gpioId++)
			{
				AfeSim_Device[phyDevId].gpio[gpioId].inputVoltage_100uV = AfeCalib[AFECALIB_GPIOVOLT_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId] & AFECALIB_DATA_MASK;
			}
		}
	}

	/* 处理辅助电压标定量 */
	for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum ; phyDevId++)
	{
		if(AfeCalib[AFECALIB_TSREF_OFFSET + phyDevId] & AFECALIB_ENABLE_MASK)
		{
			AfeSim_Device[phyDevId].power.TSREF_100uV = AfeCalib[AFECALIB_TSREF_OFFSET + phyDevId] & AFECALIB_DATA_MASK;
		}
	}
	
	/* 电芯电压偏移标定量 */
	for(i = 0; i < BatMan->maxBatNum; i++)
	{
		Bat[i].volt_bias_100uV = Bat[i].volt_100uV + AfeCalib[AFECALIB_CELLVOLT_BIAS_OFFSET + i];
	}

	/* 模组电压偏移标定量 */

	/* GPIO电压偏移标定量 */
	for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
	{
		for(gpioId = 0; gpioId <= AFESIM_MAX_GPIO_NUM; gpioId++)
		{
			AfeSim_Device[phyDevId].gpio[gpioId].inputVoltageBias_100uV = \
				AfeSim_Device[phyDevId].gpio[gpioId].inputVoltage_100uV + AfeCalib[AFECALIB_GPIOVOLT_BIAS_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + gpioId];
		}
	}

	/*处理AUX电压偏移标定量*/
	for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
	{
		AfeSim_Device[phyDevId].power.TSREFbias_100uV = AfeSim_Device[phyDevId].power.TSREF_100uV + AfeCalib[AFECALIB_TSREF_BIAS_OFFSET + phyDevId];
		for(auxId = 0; auxId < AFESIM_BQ79616_AUX_MAXNUM; auxId++)
		{
			if(AfeCalib[AFECALIB_AUXVOLT_OFFSET + phyDevId*AFECALIB_AUXVOLT_SINGLE_CHIP_NUM + auxId] & AFECALIB_ENABLE_MASK)
			{
				auxPtr = &AfeSim_Device[phyDevId].power.Aux_CELL_100uV;
				auxPtr[auxId] = AfeCalib[AFECALIB_AUXVOLT_OFFSET + phyDevId*AFECALIB_AUXVOLT_SINGLE_CHIP_NUM + auxId] & AFECALIB_DATA_MASK;
			}
		}
	}

	/* 处理温度标定量 */
	for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
	{
		for(tempId = 0; tempId < AFESIM_MAX_GPIO_NUM; tempId++)
		{
			index = AFECALIB_MODULETEMP_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + tempId;
			if(AfeCalib[index] & AFECALIB_ENABLE_MASK)
			{
				ModuleTemp_mC[phyDevId][tempId] = (AfeCalib[index] & AFECALIB_DATA_MASK) - 100000;
			}
			index = AFECALIB_BALANCETEMP_OFFSET + phyDevId*AFESIM_MAX_GPIO_NUM + tempId;
			if(AfeCalib[index] & AFECALIB_ENABLE_MASK)
			{
				BalanceTemp_mC[phyDevId][tempId] = (AfeCalib[index] & AFECALIB_DATA_MASK) - 100000;
			}
		}
		for(tempId = 1; tempId <= AFESIM_MAX_DT_NUM; tempId++)
		{
			index = AFECALIB_DIETEMP_OFFSET + phyDevId*AFESIM_MAX_DT_NUM + tempId - 1;
			if(AfeCalib[index] & AFECALIB_ENABLE_MASK)
			{
				DieTemp_mC[phyDevId][tempId - 1] = (AfeCalib[index] & AFECALIB_DATA_MASK) - 100000;
			}
		}
	}
	for(tempId = 1; tempId <= AFESIM_MAX_ST_NUM; tempId++)
	{
		index = AFECALIB_SHUNTTEMP_OFFSET + tempId-1;
		if(AfeCalib[index] & AFECALIB_ENABLE_MASK)
		{
			ShuntTemp_mC[tempId - 1] = (AfeCalib[index] & AFECALIB_DATA_MASK) - 100000;
		}
	}
	if(AfeCalib[AFECALIB_PACKTEMP_OFFSET] & AFECALIB_ENABLE_MASK)
	{
		PackTemp_mC = (AfeCalib[AFECALIB_PACKTEMP_OFFSET] & AFECALIB_DATA_MASK) - 100000;
	}
	if(AfeCalib[AFECALIB_AIRTEMP_OFFSET] & AFECALIB_ENABLE_MASK)
	{
		AirTemp_mC = (AfeCalib[AFECALIB_AIRTEMP_OFFSET] & AFECALIB_DATA_MASK) - 100000;
	}
	
	/* 处理通信标定量 */
	for(phyDevId = 0; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
	{
		AfeSim_Device[phyDevId].coml.isBroken = (AfeCalib[AFECALIB_DAISYCHAIN_DISABLE_OFFSET + phyDevId] & (1<<0)) ? 1 : 0;
		AfeSim_Device[phyDevId].comh.isBroken = (AfeCalib[AFECALIB_DAISYCHAIN_DISABLE_OFFSET + phyDevId] & (1<<1)) ? 1 : 0;

		if(AfeCalib[AFECALIB_DEVICE_DISABLE_OFFSET + phyDevId])
		{
			AfeSim_Device[phyDevId].power.powerState = AFESIM_DEVICE_DESTROYED;
			memset(AfeSim_Device[phyDevId].reg, 0, AFESIM_MAX_REG_NUM);
		}
		else
		{
			if(AfeSim_Device[phyDevId].power.powerState == AFESIM_DEVICE_DESTROYED)
			{
				AfeSim_Device[phyDevId].power.powerState = AFESIM_DEVICE_POWER_SHUTDOWN;
			}
		}
	}

}

void AfeCalibration_Run200ms(void)
{
	uint8 phyDevId;
	uint8 cellId;
	uint16 cellPackId;
	uint8 gpioId;
	CAN_MessageType txMsg[16] = {0};
	uint8 msgIndex;
	uint8 msgSubIndex;
	uint8 temp1, temp2, temp3, temp4;
	uint16 regIndex;
	uint8 regCnt;
	sint16 stemp;
	uint16 utemp;
	sint32 current;
	
	/* 通信标定量输出 */
	txMsg[0].msgId.field.msgId = CAN_OUT_DaisyChainBroken_MSGID;
	txMsg[0].msgDlc.field.len = 8;
	txMsg[1].msgId.field.msgId = CAN_OUT_DeviceBroken_MSGID;
	txMsg[1].msgDlc.field.len = 8;
	txMsg[2].msgId.field.msgId = CAN_OUT_DeviceStackRole_MSGID;
	txMsg[2].msgDlc.field.len = 8;
	txMsg[3].msgId.field.msgId = CAN_OUT_DeviceStackID_MSGID;
	txMsg[3].msgDlc.field.len = 8;
	for(phyDevId = 0; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
	{
		msgIndex = phyDevId / 2;
		msgSubIndex = phyDevId % 2;
		if(msgSubIndex == 0)
		{
			txMsg[0].data.data8[msgIndex] = (AfeSim_Device[phyDevId].coml.isBroken << 1) | (AfeSim_Device[phyDevId].comh.isBroken);
			txMsg[1].data.data8[msgIndex] = (AfeSim_Device[phyDevId].power.powerState);
			if(TRUE == AfeSim_AssertDeviceCommDir(phyDevId, AFESIM_DEVICE_USE_COMH))
			{
				temp1 = 0x01;
				temp2 = AfeSim_Reg[phyDevId][BQ79600_REG_DIR0_ADDR];
			}
			else
			{
				temp1 = 0x0A;
				temp2 = AfeSim_Reg[phyDevId][BQ79600_REG_DIR1_ADDR];
			}
			if(TRUE == AfeSim_AssertDeviceIsStackTop(phyDevId))
			{
				temp1 += 0x01;
			}
			txMsg[2].data.data8[msgIndex] = temp1;
			txMsg[3].data.data8[msgIndex] = temp2;
		}
		else
		{
			txMsg[0].data.data8[msgIndex] |= (AfeSim_Device[phyDevId].coml.isBroken << 5) | (AfeSim_Device[phyDevId].comh.isBroken << 4);
			txMsg[1].data.data8[msgIndex] |= (AfeSim_Device[phyDevId].power.powerState << 4);
			if(TRUE == AfeSim_AssertDeviceCommDir(phyDevId, AFESIM_DEVICE_USE_COMH))
			{
				temp1 = 0x10;
				temp2 = AfeSim_Reg[phyDevId][BQ79600_REG_DIR0_ADDR] << 4;
			}
			else
			{
				temp1 = 0xA0;
				temp2 = AfeSim_Reg[phyDevId][BQ79600_REG_DIR1_ADDR] << 4;
			}
			if(TRUE == AfeSim_AssertDeviceIsStackTop(phyDevId))
			{
				temp1 += 0x10;
			}
			txMsg[2].data.data8[msgIndex] |= temp1;
			txMsg[3].data.data8[msgIndex] |= temp2;
		}
	}
	txMsg[2].data.data8[0] &= 0xF0;
	txMsg[3].data.data8[0] &= 0xF0;
	CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[0]);
	CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[1]);
	CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[2]);
	CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[3]);

	/*物理量输出*/
	txMsg[0].msgId.field.msgId = CAN_OUT_CellVolt_MSGID;
	txMsg[0].msgDlc.field.len = 8;
	txMsg[1].msgId.field.msgId = CAN_OUT_CellVolt_MSGID+1;
	txMsg[1].msgDlc.field.len = 8;
	txMsg[2].msgId.field.msgId = CAN_OUT_CellVolt_MSGID+2;
	txMsg[2].msgDlc.field.len = 8;
	txMsg[3].msgId.field.msgId = CAN_OUT_CellVolt_MSGID+3;
	txMsg[3].msgDlc.field.len = 8;
	txMsg[4].msgId.field.msgId = CAN_OUT_CellVolt_MSGID+4;
	txMsg[4].msgDlc.field.len = 4;
	cellPackId = 0;
	for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
	{
		for(cellId = 0; cellId < AFESIM_MAX_CELL_NUM; cellId++)
		{
			if(cellId < AfeSim_Device[phyDevId].activeCell)
			{
				txMsg[cellId / 4].data.data16[cellId % 4] = Bat[cellPackId++].volt_100uV;
			}
			else
			{
				txMsg[cellId / 4].data.data16[cellId % 4] = 0xFFFF;
			}
		}
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[0]);
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[1]);
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[2]);
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[3]);
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[4]);
		txMsg[0].msgId.field.msgId+=5;
		txMsg[1].msgId.field.msgId+=5;
		txMsg[2].msgId.field.msgId+=5;
		txMsg[3].msgId.field.msgId+=5;
		txMsg[4].msgId.field.msgId+=5;
	}
	txMsg[5].msgId.field.msgId = CAN_OUT_ModuleVolt_MSGID;
	txMsg[5].msgDlc.field.len = 8;
	temp1 = 0;
	for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM + 1; phyDevId++)
	{
		if(phyDevId <= AfeMan->afeChipRealNum)
		{
			txMsg[5].data.data16[(phyDevId - 1) % 4] = AfeSim_Device[phyDevId].power.VBAT_mV / 100;
		}
		else
		{
			txMsg[5].data.data16[(phyDevId - 1) % 4] = 0xFFFF;
		}
		temp1++;
		if(temp1 > 3)
		{
			CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[5]);
			txMsg[5].msgId.field.msgId++;
			temp1 = 0;
		}
	}
	txMsg[6].msgId.field.msgId = CAN_OUT_GpioVolt_MSGID;
	txMsg[6].msgDlc.field.len = 8;
	txMsg[7].msgId.field.msgId = CAN_OUT_GpioVolt_MSGID+1;
	txMsg[7].msgDlc.field.len = 8;
	txMsg[8].msgId.field.msgId = CAN_OUT_GpioVolt_MSGID+2;
	txMsg[8].msgDlc.field.len = 2;
	for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
	{
		for(gpioId = 0; gpioId < AFESIM_MAX_GPIO_NUM; gpioId++)
		{
			if(AfeSim_Device[phyDevId].gpio[gpioId].useType != AFESIM_DEVICE_GPIO_NOUSE)
			{
				txMsg[6 + gpioId / 4].data.data16[gpioId % 4] = AfeSim_Device[phyDevId].gpio[gpioId].inputVoltage_100uV;
			}
			else
			{
				txMsg[6 + gpioId / 4].data.data16[gpioId % 4] = 0xFFFF;
			}
		}
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[6]);
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[7]);
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[8]);
		txMsg[6].msgId.field.msgId+=3;
		txMsg[7].msgId.field.msgId+=3;
		txMsg[8].msgId.field.msgId+=3;
	}
	
	txMsg[9].msgId.field.msgId = CAN_OUT_ModuleTemp_MSGID;
	txMsg[9].msgDlc.field.len = 8;
	txMsg[10].msgId.field.msgId = CAN_OUT_ModuleTemp_MSGID+1;
	txMsg[10].msgDlc.field.len = 4;
	txMsg[11].msgId.field.msgId = CAN_OUT_BalanceTemp_MSGID;
	txMsg[11].msgDlc.field.len = 4;
	txMsg[12].msgId.field.msgId = CAN_OUT_DieTemp_MSGID;
	txMsg[12].msgDlc.field.len = 4;
	for(phyDevId = 1; phyDevId <= AFESIM_MAX_AFE_NUM; phyDevId++)
	{
		for(temp1 = 0; temp1 < AFESIM_MAX_MT_NUM; temp1++)
		{
			stemp = ModuleTemp_mC[phyDevId][temp1] / 100.0;
			txMsg[9 + temp1 / 4].data.data16[temp1 % 4] = stemp;
		}
		for(temp2 = 0; temp2 < AFESIM_MAX_BT_NUM; temp2++)
		{
			stemp = BalanceTemp_mC[phyDevId][temp2 + AFESIM_MAX_MT_NUM] / 100.0;
			txMsg[11].data.data16[temp2 % 4] = stemp;
		}
		for(temp3 = 0; temp3 < AFESIM_MAX_DT_NUM; temp3++)
		{
			stemp = DieTemp_mC[phyDevId][temp3] / 100.0;
			txMsg[12].data.data16[temp3 % 4] = stemp;
		}
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[9]);
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[10]);
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[11]);
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[12]);
		txMsg[9].msgId.field.msgId+=2;
		txMsg[10].msgId.field.msgId+=2;
		txMsg[11].msgId.field.msgId+=1;
		txMsg[12].msgId.field.msgId+=1;
	}
	txMsg[13].msgId.field.msgId = CAN_OUT_Shunt_MSGID;
	txMsg[13].msgDlc.field.len = 8;
	RTE_Read_BatterySysCurrent(&current);
	txMsg[13].data.data32[0] = current;
	stemp = ShuntTemp_mC[0] / 100.0;
	txMsg[13].data.data16[2] = stemp;
	stemp = ShuntTemp_mC[1] / 100.0;
	txMsg[13].data.data16[3] = stemp;
	CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[13]);

	txMsg[14].msgId.field.msgId = CAN_OUT_PackAir_MSGID;
	txMsg[14].msgDlc.field.len = 4;
	stemp = PackTemp_mC / 100.0;
	txMsg[14].data.data16[0] = stemp;
	stemp = AirTemp_mC / 100.0;
	txMsg[14].data.data16[1] = stemp;
	CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[14]);

	txMsg[0].msgId.field.msgId = CAN_OUT_AuxVolt_MSGID;
	txMsg[0].msgDlc.field.len = 8;
	txMsg[1].msgId.field.msgId = CAN_OUT_AuxVolt_MSGID+1;
	txMsg[1].msgDlc.field.len = 8;
	txMsg[2].msgId.field.msgId = CAN_OUT_AuxVolt_MSGID+2;
	txMsg[2].msgDlc.field.len = 8;
	txMsg[3].msgId.field.msgId = CAN_OUT_AuxVolt_MSGID+3;
	txMsg[3].msgDlc.field.len = 8;
	for(phyDevId = 1; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
	{
		txMsg[0].data.data16[0] = (uint16)AfeSim_Device[phyDevId].power.Aux_CELL_100uV;
		txMsg[0].data.data16[1] = (uint16)AfeSim_Device[phyDevId].power.Aux_GPIO_100uV;
		txMsg[0].data.data16[2] = (uint16)(AfeSim_Device[phyDevId].power.Aux_BAT_mV/100);
		txMsg[0].data.data16[3] = (uint16)AfeSim_Device[phyDevId].power.Aux_REFL_100uV;
		txMsg[1].data.data16[0] = (uint16)AfeSim_Device[phyDevId].power.Aux_VBG2_100uV;
		txMsg[1].data.data16[1] = (uint16)AfeSim_Device[phyDevId].power.Aux_AVAO_REF_100uV;
		txMsg[1].data.data16[2] = (uint16)(AfeSim_Device[phyDevId].power.Aux_AVDD_REF_100uV);
		txMsg[1].data.data16[3] = (uint16)AfeSim_Device[phyDevId].power.Aux_OVDAC_100uV;
		txMsg[2].data.data16[0] = (uint16)AfeSim_Device[phyDevId].power.Aux_UVDAC_100uV;
		txMsg[2].data.data16[1] = (uint16)AfeSim_Device[phyDevId].power.Aux_OT_OTCB_DAC_100uV;
		txMsg[2].data.data16[2] = (uint16)(AfeSim_Device[phyDevId].power.Aux_UTDAC_100uV);
		txMsg[2].data.data16[3] = (uint16)AfeSim_Device[phyDevId].power.Aux_VCBDONE_DAC_100uV;
		txMsg[3].data.data16[0] = (uint16)AfeSim_Device[phyDevId].power.Aux_VCM_100uV;
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[0]);
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[1]);
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[2]);
		CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[3]);
		txMsg[0].msgId.field.msgId+=4;
		txMsg[1].msgId.field.msgId+=4;
		txMsg[2].msgId.field.msgId+=4;
		txMsg[3].msgId.field.msgId+=4;
	}
	
	/* 任意寄存器标定输出 */
	if(AfeCalib_Manager.randRegHandler.readEnable)
	{
		txMsg[0].msgId.field.msgId = CAN_OUT_RandomReg_MSGID;
		txMsg[0].msgDlc.field.len = 8;
		regCnt = 0;
		if(AfeCalib_Manager.randRegHandler.isGlobalR)
		{
			for(phyDevId = 0; phyDevId <= AfeMan->afeChipRealNum; phyDevId++)
			{
				for(regIndex = 0; regIndex < AfeCalib_Manager.randRegHandler.readLen; regIndex++)
				{
					txMsg[0].data.data8[regIndex % 8] = AfeSim_Reg[AfeCalib_Manager.randRegHandler.readPhyDevId][AfeCalib_Manager.randRegHandler.readStartRegPos + regIndex];
					regCnt++;
					if(regCnt >= 8)
					{
						txMsg[0].msgDlc.field.len = 8;
						CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[0]);
						txMsg[0].msgId.field.msgId++;
						txMsg[0].data.data32[0] = 0;
						txMsg[0].data.data32[1] = 0;
						regCnt = 0;
					}
				}
				if(regCnt > 0)
				{
					txMsg[0].msgDlc.field.len = regCnt;
					CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[0]);
					txMsg[0].msgId.field.msgId++;
					txMsg[0].data.data32[0] = 0;
					txMsg[0].data.data32[1] = 0;
				}
			}
		}
		else
		{
			for(regIndex = 0; regIndex < AfeCalib_Manager.randRegHandler.readLen; regIndex++)
			{
				txMsg[0].data.data8[regIndex % 8] = AfeSim_Reg[AfeCalib_Manager.randRegHandler.readPhyDevId][AfeCalib_Manager.randRegHandler.readStartRegPos + regIndex];
				regCnt++;
				if(regCnt >= 8)
				{
					txMsg[0].msgDlc.field.len = 8;
					CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[0]);
					txMsg[0].msgId.field.msgId++;
					txMsg[0].data.data32[0] = 0;
					txMsg[0].data.data32[1] = 0;
					regCnt = 0;
				}
			}
		}
		if(regCnt > 0)
		{
			txMsg[0].msgDlc.field.len = regCnt;
			CAN_InsertTxFrame(CAN0_DEVICE_ID, &txMsg[0]);
			txMsg[0].msgId.field.msgId++;
			txMsg[0].data.data32[0] = 0;
			txMsg[0].data.data32[1] = 0;
		}
	}
	
	CAN_StartTransmitTxFrame(CAN0_DEVICE_ID);
}
