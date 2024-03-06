/*
 * AfeSim_DaisyChain.c
 *
 *  Created on: 2024年1月28日
 *      Author: Administrator
 */

#include "AfeSim_Device.h"

#include "AfeSim_BQ79718_Type.h"
#include "AfeSim_BQ79616_Type.h"
#include "AfeSim_BQ79600_Type.h"

AfeSim_AfeGeneralType AfeSim_UseType = AFESIM_TYPE_79616;
uint32 AfeSim_DaisyChainNoiseErrRate_1_10000 = 0;

AfeSim_DeviceManagerType AfeSim_DeviceManager =
{
		.cellNum = {0},
		.dev = &AfeSim_Device[0],
		.afeChipRealNum = AFESIM_MAX_AFE_NUM
};
AfeSim_DeviceManagerType* const AfeMan = &AfeSim_DeviceManager;

uint8 AfeSim_Reg[AFESIM_MAX_AFE_NUM + 1][AFESIM_MAX_REG_NUM];

/* 涓嬫爣涓�0鐨勮澶囨槸 BQ79600 */
AfeSim_DeviceManangerType AfeSim_Device[AFESIM_MAX_AFE_NUM + 1];

uint8 AfeSim_DaisyChainCommData[1024];


static void AfeSim_DeviceManagerInit(void);
static void AfeSim_BQ79xxx_RegInit(uint8 devId);
static void AfeSim_BQ79xxx_DeviceInit(uint8 phyDevId);
static void AfeSim_BQ79xxx_Init(void);
static void AfeSim_DeviceManagerInit(void);


void AfeSim_DeviceInit(void)
{
	AfeSim_DeviceManagerInit();
	AfeSim_BQ79600_Init();
	AfeSim_BQ79xxx_Init();
}

/*
 *
 * */
static void AfeSim_BQ79xxx_RegInit(uint8 devId)
{
	memset(AfeSim_Device[devId].reg, 0x00, AFESIM_MAX_REG_NUM);
	if(AfeSim_UseType == AFESIM_TYPE_79718)
	{
		AfeSim_Device[devId].reg[AFESIM_BQ79718_REG_DEV_CONF2] = 0x0C;	/* 18 Cells */
	}
	else
	{
		AfeSim_Device[devId].reg[AFESIM_BQ79616_REG_OV_THRESH] = 0x3F;
		AfeSim_Device[devId].reg[AFESIM_BQ79616_REG_UV_THRESH] = 0x00; /* 16 Cells */
		AfeSim_Device[devId].reg[AFESIM_BQ79616_REG_OTUT_THRESH] = 0xE0;
		AfeSim_Device[devId].reg[AFESIM_BQ79616_REG_DEV_CONF] = 0xA4;
		AfeSim_Device[devId].reg[AFESIM_BQ79616_REG_ACTIVE_CELL] = 0x0A; /* 16 Cells */
		AfeSim_Device[devId].reg[AFESIM_BQ79616_REG_PWR_TRANSIT_CONF] = 0x18;
	}
}

static void AfeSim_BQ79xxx_DeviceInit(uint8 phyDevId)
{
	int i;

	RTE_Read_AfeSimUseType(&AfeSim_UseType);

	if(phyDevId != 0)
	{
		if(AfeSim_UseType == AFESIM_TYPE_79718)
		{
			if(phyDevId == 1)
				AfeSim_Device[phyDevId].afeType = AFESIM_AFE_USE_BQ79758;
			else
				AfeSim_Device[phyDevId].afeType = AFESIM_AFE_USE_BQ79718;
		}
		else
		{
			AfeSim_Device[phyDevId].afeType = AFESIM_AFE_USE_BQ79616;
		}
	}
	else
	{
		AfeSim_Device[phyDevId].afeType = AFESIM_AFE_USE_BQ79600;
	}

	AfeSim_Device[phyDevId].info.devId = 0;
	if(phyDevId != 0)
	{
		AfeSim_Device[phyDevId].info.stackType = AFESIM_DEVICE_IS_STACK;
	}
	else
	{
		AfeSim_Device[phyDevId].info.stackType = AFESIM_DEVICE_IS_BASE;
	}

	AfeSim_Device[phyDevId].reg = &AfeSim_Reg[phyDevId][0];
	AfeSim_BQ79xxx_RegInit(phyDevId);
	if(AfeSim_UseType == AFESIM_TYPE_79718)
	{
		AfeSim_Device[phyDevId].activeCell = 18;
		AfeSim_Device[phyDevId].maxCell = 18;
	}
	else
	{
		AfeSim_Device[phyDevId].activeCell = 16;
		AfeSim_Device[phyDevId].maxCell = 16;
	}
	AfeSim_Device[phyDevId].comh.isBroken = 0;
	AfeSim_Device[phyDevId].coml.isBroken = 0;

	AfeSim_Device[phyDevId].commData.noiseEnable = 0;
	AfeSim_Device[phyDevId].commData.noisePower_dBm = -30.0f;
	AfeSim_Device[phyDevId].commData.signalPower_dBm = 27.0f;
	AfeSim_Device[phyDevId].commData.snr = AfeSim_Device[phyDevId].commData.signalPower_dBm -\
												AfeSim_Device[phyDevId].commData.noisePower_dBm;
	AfeSim_Device[phyDevId].commData.recvBuff = AfeSim_DaisyChainCommData;
	AfeSim_Device[phyDevId].commData.recvSingleBuffLength = 0;
	AfeSim_Device[phyDevId].commData.recvTotalBuffLength = 0;

	AfeSim_Device[phyDevId].power.AVDD_100uV = 0;
	AfeSim_Device[phyDevId].power.AVSS_100uV = 0;
	AfeSim_Device[phyDevId].power.CVDD_100uV = 0;
	AfeSim_Device[phyDevId].power.CVSS_100uV = 0;
	AfeSim_Device[phyDevId].power.DVDD_100uV = 0;
	AfeSim_Device[phyDevId].power.DVSS_100uV = 0;
	AfeSim_Device[phyDevId].power.npnbIsOn = 0;
	AfeSim_Device[phyDevId].power.NPNB_100uV = 0;
	AfeSim_Device[phyDevId].power.LDOIN_100uV = 0;
	AfeSim_Device[phyDevId].power.tsrefIsOn = 0;
	AfeSim_Device[phyDevId].power.TSREF_100uV = 0;
	AfeSim_Device[phyDevId].power.TSREFbias_100uV = 0;
	AfeSim_Device[phyDevId].power.VBAT_mV = 64000;
	AfeSim_Device[phyDevId].power.powerState = AFESIM_DEVICE_POWER_SHUTDOWN;
	
	for(i=0;i<AFESIM_BQ79718_VC_NUM;i++)
	{
		AfeSim_Device[phyDevId].vc[i].internalImpedence_Z.r = 1e5;
		AfeSim_Device[phyDevId].vc[i].internalImpedence_Z.i = 0;
		AfeSim_Device[phyDevId].vc[i].lineImpedence_mZ.r = 1000;
		AfeSim_Device[phyDevId].vc[i].lineImpedence_mZ.i = 100;
		AfeSim_Device[phyDevId].vc[i].resistor_Z.r = 1000;
		AfeSim_Device[phyDevId].vc[i].resistor_Z.i = 0;
		AfeSim_Device[phyDevId].vc[i].outerSinkCurrent_nA = 0;
		AfeSim_Device[phyDevId].vc[i].internalSinkCurrent_nA = 1e3;
		AfeSim_Device[phyDevId].vc[i].isBroken = 0;
		AfeSim_Device[phyDevId].vc[i].isFloating = 0;

		AfeSim_Device[phyDevId].cb[i].cbResistorImpedence_Z.r = 22;
		AfeSim_Device[phyDevId].cb[i].cbResistorImpedence_Z.i = 0;
		AfeSim_Device[phyDevId].cb[i].internalImpedence_Z.r = 100e6;
		AfeSim_Device[phyDevId].cb[i].internalImpedence_Z.i = 100e6;
		AfeSim_Device[phyDevId].cb[i].isBroken = 0;
		AfeSim_Device[phyDevId].cb[i].isFloating = 0;
	}
	for(i=0;i<AFESIM_BQ79718_CELL_NUM;i++)
	{
		AfeSim_Device[phyDevId].cbfet[i].isBroken = 0;
		AfeSim_Device[phyDevId].cbfet[i].isOn = 0;
		AfeSim_Device[phyDevId].cbfet[i].offImpedence_Z.r = 100e6;
		AfeSim_Device[phyDevId].cbfet[i].offImpedence_Z.i = 1e3;
		AfeSim_Device[phyDevId].cbfet[i].onImpedence_Z.r = 1;
		AfeSim_Device[phyDevId].cbfet[i].onImpedence_Z.i = 1;
		AfeSim_Device[phyDevId].cbfet[i].switchOffTime_us = 150;
		AfeSim_Device[phyDevId].cbfet[i].switchOnTime_us = 100;
	}
	for(i=0;i<AFESIM_BQ79718_GPIO_NUM;i++)
	{
		AfeSim_Device[phyDevId].gpio[i].isBroken=0;
		AfeSim_Device[phyDevId].gpio[i].isFloating=0;
		AfeSim_Device[phyDevId].gpio[i].mode = AFESIM_BQ79718_GPIO_OFF;
		AfeSim_Device[phyDevId].gpio[i].pullType = AFESIM_PULL_NONE;
		if(AfeSim_UseType == AFESIM_TYPE_79718)
		{
			if(phyDevId > AFESIM_DEVID_1)
			{
				AfeSim_Device[phyDevId].gpio[i].useType = AfeSim_BQ79718_GpioInitConfig[AFESIM_DEVID_2][i];
			}
			else
			{
				AfeSim_Device[phyDevId].gpio[i].useType = AfeSim_BQ79718_GpioInitConfig[phyDevId][i];
			}
		}
		else
		{
			if(phyDevId >= AFESIM_DEVID_1)
			{
				AfeSim_Device[phyDevId].gpio[i].useType = AfeSim_BQ79616_GpioInitConfig[AFESIM_DEVID_1][i];
			}
			else
			{
				AfeSim_Device[phyDevId].gpio[i].useType = AfeSim_BQ79616_GpioInitConfig[AFESIM_DEVID_0][i];
			}
		}
	}
	for(i=0;i<AFESIM_BQ79718_CELL_NUM;i++)
	{
		AfeSim_Device[phyDevId].adc[i].mode = 0;
		AfeSim_Device[phyDevId].adc[i].defaultVaule = 0xFFFF;
	}
	memset(&AfeSim_Device[phyDevId].diag, 0, sizeof(AfeSim_DiagModel));
}

static void AfeSim_BQ79xxx_Init(void)
{
	uint8 devId;
	AfeSim_BQ79xxx_DeviceInit(0);
	AfeSim_Device[0].comhDev = &AfeSim_Device[1];
	AfeSim_Device[0].comlDev = 0; //BQ79600
	for(devId = 1; devId < AFESIM_MAX_AFE_NUM; devId++)
	{
		AfeSim_Device[devId].comhDev = &AfeSim_Device[devId+1];
		AfeSim_Device[devId].comlDev = &AfeSim_Device[devId-1];
		AfeSim_BQ79xxx_DeviceInit(devId);
	}
	AfeSim_BQ79xxx_DeviceInit(AFESIM_MAX_AFE_NUM);
	AfeSim_Device[AFESIM_MAX_AFE_NUM].comhDev = 0; //BQ79600
	AfeSim_Device[AFESIM_MAX_AFE_NUM].comlDev = &AfeSim_Device[AFESIM_MAX_AFE_NUM-1];
}


static void AfeSim_DeviceManagerInit(void)
{
	int i;
	RTE_Read_AfeSimUseType(&AfeSim_UseType);
	
	AfeMan->dev = AfeSim_Device;
	AfeMan->reg = AfeSim_Reg;
	AfeMan->afeChipRealNum = AFESIM_MAX_AFE_NUM;

	if(AfeSim_UseType == AFESIM_TYPE_79718)
	{
		AfeMan->afeMaxCellNum = AFESIM_BQ79718_CELL_NUM;
		AfeMan->afeMaxGpioNum = AFESIM_BQ79718_GPIO_NUM;
	}
	else
	{
		AfeMan->afeMaxCellNum = AFESIM_BQ79616_CELL_NUM;
		AfeMan->afeMaxGpioNum = AFESIM_BQ79616_GPIO_NUM;
	}
	
	for(i = 0; i < AFESIM_MAX_AFE_NUM + 1; i++)
	{
		AfeMan->cellNum[i] = 0;
		if(i == 0)
			AfeMan->cellNumSum[i] = 0;
		else
			AfeMan->cellNumSum[i] = AfeMan->cellNumSum[i-1] + AfeMan->cellNum[i];
	}
}


/* AfeSim_AssertDeviceCommDir
 * 判断指定设备的通信方向是否与当前栈通信方向一致 */
uint8 AfeSim_AssertDeviceCommDir(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir)
{
	uint8 commDirMatch = TRUE;
	AFESIM_BQ79600_REG_CONTROL1_RegType CONTROL1;
	CONTROL1.reg.val = AfeSim_Reg[phyDevId][AFESIM_BQ79600_REG_CONTROL1];
	if(CONTROL1.reg.field.DIR_SEL != commDir)
	{
		commDirMatch = FALSE;
		//printf("Device %d comm dir %d mismatch with stack comm dir\r\n", phyDevId, commDir);
	}
	return commDirMatch;
}

/* AfeSim_AssertBaseDeviceCanRW
 * 判断BQ79600能否读写数据(通过SPI读写) */
uint8 AfeSim_AssertBaseDeviceCanRW(void)
{
	uint8 rwEnable = TRUE;
	if(AfeSim_Device[AFESIM_DEVID_0].power.powerState != AFESIM_DEVICE_POWER_WAKE)
	{
		rwEnable = FALSE;
		printf("BQ79600 is not wake, can't read or write\r\n");
	}
	return rwEnable;
}


/* AfeSim_AssertBaseDeviceCanTransmit
 * 判断BQ79600能否传输数据(通过菊花链传输) */
uint8 AfeSim_AssertBaseDeviceCanTransmit(AfeSim_DeviceCommDirectionType commDir)
{
	uint8 transmitEnable = TRUE;
	if(AfeSim_Device[AFESIM_DEVID_0].power.powerState != AFESIM_DEVICE_POWER_WAKE)
	{
		transmitEnable = FALSE;
		printf("BQ79600 is not wake, can't read or write\r\n");
	}
	else
	{
		if(commDir == AFESIM_DEVICE_USE_COMH)
		{
			if(AfeSim_Device[AFESIM_DEVID_0].comh.isBroken)
			{
				transmitEnable = FALSE;
				printf("BQ79600 COMH broken, can't transmit\r\n");
			}
		}
		else
		{
			if(AfeSim_Device[AFESIM_DEVID_0].coml.isBroken)
			{
				transmitEnable = FALSE;
				printf("BQ79600 COML broken, can't transmit\r\n");
			}
		}
	}
	return transmitEnable;
}

/* AfeSim_AssertDeviceIsStackTop
 * 判断是否为栈顶设备 */
uint8 AfeSim_AssertDeviceIsStackTop(uint8 phyDevId)
{
	uint8 isStackTop = FALSE;
	if(AfeSim_Reg[phyDevId][BQ79xxx_REG_COMM_CTRL] & BQ79xxx_REG_COMM_CTRL_TOP_STACK)
	{
		isStackTop = TRUE;
	}
	return isStackTop;
}

/* AfeSim_AssertDeviceCanTransmit
 * 判断设备能否将数据传递给下一个设备
 * 基本仿真:能够传输
 * 逻辑仿真:判断该设备是否正常, IO是否损坏
 * 物理仿真:加入通信误码判断, 并在原数据叠加随机位翻转 */
uint8 AfeSim_AssertDeviceCanTransmit(uint8 phyDevId)
{
	uint8 transmitEnable = TRUE;
#if AFESIM_SIMULEVEL == AFESIM_SIMULEVEL_BASICAL

#elif AFESIM_SIMULEVEL >= AFESIM_SIMULEVEL_LOGICAL
	if(AfeSim_Device[phyDevId].power.powerState != AFESIM_DEVICE_POWER_WAKE)
	{
		transmitEnable = FALSE;
		printf("Device %d is not wake, can't transmit\r\n", phyDevId);
	}
	else if(AfeSim_Device[phyDevId].comh.isBroken)
	{
		transmitEnable = FALSE;
		printf("Device %d COMH broken, can't transmit\r\n", phyDevId);
	}
	else if(AfeSim_Device[phyDevId].coml.isBroken)
	{
		transmitEnable = FALSE;
		printf("Device %d COML broken, can't transmit\r\n", phyDevId);
	}
#endif
	return transmitEnable;
}

/* 判断设备能否写入数据 */
uint8 AfeSim_AssertDeviceCanWrite(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir)
{
	uint8 writeEnable = TRUE;
#if AFESIM_SIMULEVEL == AFESIM_SIMULEVEL_BASICAL

#elif AFESIM_SIMULEVEL >= AFESIM_SIMULEVEL_LOGICAL
	if(AfeSim_Device[phyDevId].power.powerState != AFESIM_DEVICE_POWER_WAKE)
	{
		writeEnable = FALSE;
		printf("Device %d is not wake, can't write register\r\n", phyDevId);
	}
	else
	{
		if(commDir == AFESIM_DEVICE_USE_COMH)
		{
			if(AfeSim_Device[phyDevId].coml.isBroken)
			{
				writeEnable = FALSE;
				printf("Device %d COML broken, can't write register\r\n", phyDevId);
			}
		}
		else
		{
			if(AfeSim_Device[phyDevId].comh.isBroken)
			{
				writeEnable = FALSE;
				printf("Device %d COMH broken, can't write register\r\n", phyDevId);
			}
		}
	}
#endif
	return writeEnable;
}

/* 判断能否从设备读取数据 */
uint8 AfeSim_AssertDeviceCanRead(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir)
{
	uint8 readEnable = TRUE;
#if AFESIM_SIMULEVEL == AFESIM_SIMULEVEL_BASICAL

#elif AFESIM_SIMULEVEL >= AFESIM_SIMULEVEL_LOGICAL
	if(AfeSim_Device[phyDevId].power.powerState != AFESIM_DEVICE_POWER_WAKE)
	{
		readEnable = FALSE;
		printf("Device %d is not wake, can't read register\r\n", phyDevId);
	}
	else
	{
		if(commDir == AFESIM_DEVICE_USE_COMH)
		{
			if(AfeSim_Device[phyDevId].coml.isBroken)
			{
				readEnable = FALSE;
				printf("Device %d COML broken, can't read register\r\n", phyDevId);
			}
		}
		else
		{
			if(AfeSim_Device[phyDevId].comh.isBroken)
			{
				readEnable = FALSE;
				printf("Device %d COMH broken, can't read register\r\n", phyDevId);
			}
		}
	}
#endif
	return readEnable;
}

/* 判断设备能否接收信号 */
uint8 AfeSim_AssertDeviceCanReceive(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir)
{
	uint8 recvEnable = TRUE;
#if AFESIM_SIMULEVEL == AFESIM_SIMULEVEL_BASICAL

#elif AFESIM_SIMULEVEL >= AFESIM_SIMULEVEL_LOGICAL
	if(commDir == AFESIM_DEVICE_USE_COMH)
	{
		if(AfeSim_Device[phyDevId].coml.isBroken)
		{
			recvEnable = FALSE;
			printf("Device %d COML broken, can't receive\r\n", phyDevId);
		}
	}
	else
	{
		if(AfeSim_Device[phyDevId].comh.isBroken)
		{
			recvEnable = FALSE;
			printf("Device %d COMH broken, can't receive\r\n", phyDevId);
		}
	}
#endif
	return recvEnable;
}

/* 判断设备能否发送信号 */
uint8 AfeSim_AssertDeviceCanSend(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir)
{
	uint8 sendEnable = TRUE;
#if AFESIM_SIMULEVEL == AFESIM_SIMULEVEL_BASICAL

#elif AFESIM_SIMULEVEL >= AFESIM_SIMULEVEL_LOGICAL
	if(AfeSim_Device[phyDevId].power.powerState != AFESIM_DEVICE_POWER_WAKE)
	{
		sendEnable = FALSE;
		printf("Device %d is not wake, can't send\r\n", phyDevId);
	}
	if(commDir == AFESIM_DEVICE_USE_COMH)
	{
		if(AfeSim_Device[phyDevId].comh.isBroken)
		{
			sendEnable = FALSE;
			printf("Device %d COMH broken, can't send\r\n", phyDevId);
		}
	}
	else
	{
		if(AfeSim_Device[phyDevId].coml.isBroken)
		{
			sendEnable = FALSE;
			printf("Device %d COML broken, can't send\r\n", phyDevId);
		}
	}
#endif
	return sendEnable;
}


uint8 AfeSim_AssertDeviceIsBroken(uint8 phyDevId)
{
	uint8 isBroken = FALSE;
	/* 芯片被CAN标定, 导致被毁 */
	if(AfeSim_Device[phyDevId].power.powerState == AFESIM_DEVICE_DESTROYED)
	{
		isBroken = TRUE;
		printf("Device %d is broken\r\n", phyDevId);
	}
	/* 芯片供电电压太低, 无法开机 */
	if(AfeSim_Device[phyDevId].power.powerState == AFESIM_DEVICE_POWER_ON_RESET)
	{
		isBroken = TRUE;
		printf("Device %d is Power On Reset\r\n", phyDevId);
	}
	return isBroken;
}

uint8 AfeSim_AssertDeviceIsAwake(uint8 phyDevId)
{
	uint8 isAwake = TRUE;
	if(AfeSim_Device[phyDevId].power.powerState != AFESIM_DEVICE_POWER_WAKE)
	{
		isAwake = FALSE;
		printf("Device %d is not awake\r\n", phyDevId);
	}
	return isAwake;
}

uint8 AfeSim_AssertDeviceIsSleep(uint8 phyDevId)
{
	uint8 isSleep = TRUE;
	if(AfeSim_Device[phyDevId].power.powerState != AFESIM_DEVICE_POWER_SLEEP)
	{
		isSleep = FALSE;
		printf("Device %d is not sleep\r\n", phyDevId);
	}
	return isSleep;
}

uint8 AfeSim_AssertDeviceIsShutdown(uint8 phyDevId)
{
	uint8 isShutdown = TRUE;
	if(AfeSim_Device[phyDevId].power.powerState != AFESIM_DEVICE_POWER_SHUTDOWN)
	{
		isShutdown = FALSE;
		printf("Device %d is not shutdown\r\n", phyDevId);
	}
	return isShutdown;
}


void AfeSim_DeviceTransmitFaultCmd(uint8 commDir, uint8 startPhyDevId)
{
	//if(TRUE == AfeSim_AssertDeviceCanSend(uint8 phyDevId, AfeSim_DeviceCommDirectionType commDir))
}

void AfeSim_DeviceTransmitFaultResponse(uint8 commDir, uint8 startPhyDevId)
{

}

uint8 AfeSim_AssertDeviceTransmitNoise(void)
{
	if(AfeSim_DaisyChainNoiseErrRate_1_10000)
	{
		uint32 dice = rand() % 10000;
		if(dice < AfeSim_DaisyChainNoiseErrRate_1_10000)
		{
			AfeMan->noiseFailCnt++;
			return AFESIM_COMM_ERR_NOISE_BIT_FLIP;
		}
	}
	return AFESIM_COMM_NO_ERR;
}

uint8 AfeSim_DeviceTransmitErrorHandler(uint8 afeType, uint8 phyDevId, uint8 errorType, uint8 commDir)
{
	/*
	3 = 发送响应帧失败 FAULT_COMM2 COMH_TR
	2 = 接收响应帧失败 FAULT_COMM2 COMH_RR
	1 = 接收指令帧失败 FAULT_COMM2 COMH_RC
	0 = Bit故障          FAULT_COMM2 COMH_BIT
	*/
	if(afeType == 1)
	{
		AfeSim_BQ79616_CommErrorHandler(phyDevId, errorType, commDir);
	}
	else
	{
	
	}
}

/* 唤醒一个虚拟AFE
 * 进行初始参数配置 */
uint8 AfeSim_OneDeviceWakeup(uint8 phyDevId)
{
	int i;
	uint8 wakeStatus = TRUE;
	if(TRUE == AfeSim_AssertDeviceIsBroken(phyDevId))
	{
		wakeStatus = FALSE;
		printf("Wake Device %d fail\r\n", phyDevId);
	}
	else
	{
		RTE_Read_AfeSimUseType(&AfeSim_UseType);
		if(AfeSim_UseType == AFESIM_TYPE_79718)
		{
			AfeSim_Device[phyDevId].power.powerState = AFESIM_DEVICE_POWER_WAKE;
			AfeSim_Device[phyDevId].power.VBAT_mV = 50000;
			AfeSim_Device[phyDevId].power.npnbIsOn = 1;
			AfeSim_Device[phyDevId].power.NPNB_100uV = 6700;
			AfeSim_Device[phyDevId].power.LDOIN_100uV = 6000;
			AfeSim_Device[phyDevId].power.AVDD_100uV = 5000;
			AfeSim_Device[phyDevId].power.AVSS_100uV = 0;
			AfeSim_Device[phyDevId].power.CVDD_100uV = 5000;
			AfeSim_Device[phyDevId].power.CVSS_100uV = 0;
			AfeSim_Device[phyDevId].power.DVDD_100uV = 1800;
			AfeSim_Device[phyDevId].power.DVSS_100uV = 0;
			
			if(phyDevId != BQ79600_DEVID)
			{
				AfeMan->cellNum[phyDevId] = 18;
				AfeSim_Device[phyDevId].activeCell = 18;
			}
			AfeSim_Device[phyDevId].power.tsrefIsOn = 1;
			AfeSim_Device[phyDevId].power.TSREF_100uV = 40000;
			//AfeSim_Device[phyDevId].power.TSREFbias_100uV = AfeSim_Device[phyDevId].power.TSREF_100uV;
		}
		else
		if(AfeSim_UseType == AFESIM_TYPE_79616)
		{
			AfeSim_Device[phyDevId].power.powerState = AFESIM_DEVICE_POWER_WAKE;
			AfeSim_Device[phyDevId].power.VBAT_mV = 50000;
			AfeSim_Device[phyDevId].power.npnbIsOn = 1;
			AfeSim_Device[phyDevId].power.NPNB_100uV = 6700;
			AfeSim_Device[phyDevId].power.LDOIN_100uV = 6000;
			AfeSim_Device[phyDevId].power.AVDD_100uV = 5000;
			AfeSim_Device[phyDevId].power.AVSS_100uV = 0;
			AfeSim_Device[phyDevId].power.CVDD_100uV = 5000;
			AfeSim_Device[phyDevId].power.CVSS_100uV = 0;
			AfeSim_Device[phyDevId].power.DVDD_100uV = 1800;
			AfeSim_Device[phyDevId].power.DVSS_100uV = 0;
			if(phyDevId != BQ79600_DEVID)
			{
				AfeMan->cellNum[phyDevId] = 16;
				AfeSim_Device[phyDevId].activeCell = 16;
			}
			AfeSim_Device[phyDevId].power.tsrefIsOn = 0;
			AfeSim_Device[phyDevId].power.TSREF_100uV = 0;
			AfeSim_BQ79616_ResetAuxVolt(phyDevId);
			
		}
		/* 每次唤醒设备, 都会刷新电芯统计情况 */
		for(i = 0; i < AFESIM_MAX_AFE_NUM + 1; i++)
		{
			if(i == 0)
				AfeMan->cellNumSum[i] = 0;
			else
				AfeMan->cellNumSum[i] = AfeMan->cellNumSum[i-1] + AfeMan->cellNum[i];
		}
	}
	return wakeStatus;
}

uint8 AfeSim_OneDeviceSleep(uint8 phyDevId)
{
	uint8 sleepStatus = TRUE;
	if(TRUE == AfeSim_AssertDeviceIsBroken(phyDevId))
	{
		sleepStatus = FALSE;
		printf("Device %d sleep fail", phyDevId);
	}
	else
	{
		AfeSim_Device[phyDevId].power.powerState = AFESIM_DEVICE_POWER_SLEEP;
	}
	return sleepStatus;
}

uint8 AfeSim_OneDeviceShutdown(uint8 phyDevId)
{
	uint8 shutdownStatus = TRUE;
	if(TRUE == AfeSim_AssertDeviceIsBroken(phyDevId))
	{
		shutdownStatus = FALSE;
		printf("Device %d shutdown fail", phyDevId);
	}
	else
	{
		AfeSim_Device[phyDevId].power.powerState = AFESIM_DEVICE_POWER_SHUTDOWN;
		AfeSim_Device[phyDevId].power.tsrefIsOn = 0;
		AfeSim_Device[phyDevId].power.TSREF_100uV = 0;
		//AfeSim_Device[phyDevId].power.TSREFbias_100uV = AfeSim_Device[phyDevId].power.TSREF_100uV;
	}
	return shutdownStatus;
}

/* 设备唤醒API
 * 触发条件: BQ79600收到唤醒全部AFE的指令
 * 返回值：成功唤醒的AFE数量 */
uint8 AfeSim_DeviceWakeUp(AfeSim_DeviceCommDirectionType commDir, AfeSim_DevicePowerStatusChangeType wakeType)
{
	uint8 wakeupNum = 0;
	sint8 phyDevId;
	sint8 devStart;
	sint8 devEnd;
	sint8 devInc;

	printf("AFE Wake All Devices\r\n");
	if(FALSE == AfeSim_AssertBaseDeviceCanTransmit(commDir))
	{
		printf("AFE Wake FAIL\r\n");
	}
	else
	{
		/*选择通信方向*/
		if(commDir == AFESIM_DEVICE_USE_COMH)	/* AFE 1 - 15 */
		{
			devStart = 1;
			devInc = 1;
			devEnd = AfeMan->afeChipRealNum + devInc;
		}
		else	/* AFE 15 - 1*/
		{
			devStart = AfeMan->afeChipRealNum;
			devInc = -1;
			devEnd = 1 + devInc;
		}
		
		for(phyDevId = devStart; phyDevId != devEnd; phyDevId += devInc)
		{
			/* 如果菊花链断开, 则无法接收到唤醒信号, 唤醒终止 */
			if(FALSE == AfeSim_AssertDeviceCanReceive(phyDevId, commDir))
			{
				printf("Device %d can't receive\r\n", phyDevId);
				break;
			}
			/* 如果设备损毁, 则无法响应唤醒信号, 唤醒终止 */
			if(TRUE == AfeSim_AssertDeviceIsBroken(phyDevId))
			{
				printf("Device %d is Broken\r\n", phyDevId);
				break;
			}
			/* 若设备未被唤醒, 则判断该设备是否符合唤醒要求
			 * 否则跳过该设备 */
			if(FALSE == AfeSim_AssertDeviceIsAwake(phyDevId))
			{
				/* 如果设备收到SleepToActive但不处于Sleep, 唤醒终止
				 * 处于Sleep, 允许唤醒 */
				if(wakeType == AFESIM_SLEEP_TO_ACTIVE)
				{
					if(FALSE == AfeSim_AssertDeviceIsSleep(phyDevId))
					{
						printf("Device %d is not Sleep\r\n", phyDevId);
						break;
					}
					wakeupNum++;
					AfeSim_OneDeviceWakeup(phyDevId);
					printf("Device %d Wake\r\n", phyDevId);
				}
				/* 如果设备收到WakeTone但不处于Shutdown, 唤醒终止
				 * 处于Shutdown, 允许唤醒 */
				else if(wakeType == AFESIM_SHUTDOWN_TO_ACTIVE)
				{
					if(FALSE == AfeSim_AssertDeviceIsShutdown(phyDevId))
					{
						printf("Device %d is not Shutdown\r\n", phyDevId);
						break;
					}
					wakeupNum++;
					AfeSim_OneDeviceWakeup(phyDevId);
					printf("Device %d Wake\r\n", phyDevId);
				}
			}
			if(FALSE == AfeSim_AssertDeviceCanSend(phyDevId, commDir))
			{
				printf("Device %d can't send to next Device\r\n", phyDevId);
				break;
			}
		}
	}
	return wakeupNum;
}

/* 设备下电API
 * 触发条件: BQ79600收到下电全部AFE的指令
 * 返回值：成功下电的AFE数量 */
uint8 AfeSim_DeviceSleepShutdown(AfeSim_DeviceCommDirectionType commDir, AfeSim_DevicePowerStatusChangeType shutdownType)
{
	uint8 breakFlag = 0;
	uint8 shutdownNum = 0;
	sint8 phyDevId;
	sint8 devStart;
	sint8 devEnd;
	sint8 devInc;
	
	if(FALSE == AfeSim_AssertBaseDeviceCanTransmit(commDir))
	{
		printf("AFE Sleep Shutdown FAIL\r\n");
	}
	else
	{
		/*选择通信方向*/
		if(commDir == AFESIM_DEVICE_USE_COMH)	/* AFE 1 - 15 */
		{
			devStart = 1;
			devEnd = AfeMan->afeChipRealNum + 1;
			devInc = 1;
		}
		else	/* AFE 15 - 1*/
		{
			devStart = AfeMan->afeChipRealNum;
			devEnd = 0;
			devInc = -1;
		}
		for(phyDevId = devStart; phyDevId != devEnd; phyDevId += devInc)
		{
			/* 如果不能接收，则此设备无法休眠 */
			if(FALSE == AfeSim_AssertDeviceCanReceive(phyDevId, commDir))
			{
				printf("Device %d can't receive\r\n", phyDevId);
				break;
			}
			if(FALSE == AfeSim_AssertDeviceIsAwake(phyDevId))
			{
				printf("Device %d is Broken\r\n", phyDevId);
				break;
			}
			/* 如果不能发送，则此设备可休眠，但下一个设备无法休眠 */
			if(!AfeSim_AssertDeviceCanSend(phyDevId, commDir))
			{
				breakFlag = 1;
			}
			/* 如果是栈顶设备，则休眠到此为止 */
			if(AfeSim_AssertDeviceIsStackTop(phyDevId))
			{
				breakFlag = 1;
			}

			if(shutdownType == AFESIM_ACTIVE_TO_SLEEP)
			{
				shutdownNum++;
				AfeSim_OneDeviceSleep(phyDevId);
			}
			else if(shutdownType == AFESIM_ACTIVE_TO_SHUTDOWN)
			{
				shutdownNum++;
				AfeSim_OneDeviceShutdown(phyDevId);
			}

			if(breakFlag)
			{
				break;
			}
		}
	}
	return shutdownNum;
}


/* AfeSim_DeviceCreateSingleReplyBuff
 * 栈设备生成回复帧用 */
uint8 AfeSim_DeviceCreateSingleReplyBuff(uint8 phyDevId, uint8* buff, uint8 stackDevId, uint8 rwBytes, 
								uint16 rwRegAddr, uint8* data, uint16 *offset)
{
	uint8 loop = 0u;
	uint16 crc16 = 0u;
	uint16 dataCnt = *offset;
	uint8 crcError = AFESIM_COMM_NO_ERR;
	
	buff[dataCnt++] = rwBytes - 1;
	buff[dataCnt++] = stackDevId;
	buff[dataCnt++] = (rwRegAddr >> 8u) & 0xFF;
	buff[dataCnt++] = rwRegAddr & 0xFF;
	for(loop = 0u; loop < rwBytes; loop++)
	{
		buff[dataCnt++] = data[loop];
	}
	crc16 = CRC16_Check_Modbus(&buff[*offset], (dataCnt - *offset));

	if(AfeSim_UseType == AFESIM_TYPE_79616)
	{
		AFESIM_BQ79616_REG_DIAG_COMM_CTRL_RegType* DIAG_COMM_CTRL = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DIAG_COMM_CTRL];
		AFESIM_BQ79616_REG_DEV_STAT_RegType* DEV_STAT = &AfeSim_Reg[phyDevId][AFESIM_BQ79616_REG_DEV_STAT];
		if(DIAG_COMM_CTRL->reg.field.FLIP_TR_CRC)
		{
			if(FALSE == AfeSim_Device[phyDevId].diag.bist.bist_WillFail[AFESIM_BQ79616_DIAG_CRC_FLIP_FAULT])
			{
				crc16 ^= 0xFFFF;
				crcError = AFESIM_COMM_ERR_CRC_FLIP;
			}
		}
		DEV_STAT->reg.field.CUST_CRC_DONE = 1;
		DEV_STAT->reg.field.FACT_CRC_DONE = 1;
	}

	buff[dataCnt++] = (crc16 >> 8u) & 0xFF;
	buff[dataCnt++] = crc16 & 0xFF;
	*offset = dataCnt;
	
	return crcError;
}


void AfeSim_DeviceSingleCommandResponse(uint8 phyDevId, uint16 currentReg, AfeSim_DeviceCommDirectionType commDir)
{
	if(phyDevId == AFESIM_DEVID_0)
	{
		switch(currentReg)
		{
		case BQ79600_REG_DIR0_ADDR:	/*正链自动编址时才会写入*/
			AfeSimX1_BQ79600_CommandResp_DIR0_ADDR(phyDevId, commDir);
			break;
		case BQ79600_REG_DIR1_ADDR:	/*反链自动编址时才会写入*/
			AfeSim_BQ79600_CommandResp_DIR1_ADDR(phyDevId, commDir);
			break;
		case AFESIM_BQ79616_REG_COMM_CTRL:
			AfeSim_BQ79600_CommandResp_COMM_CTRL(phyDevId, commDir);
			break;
		case AFESIM_BQ79600_REG_CONTROL1:
			AfeSimX1_BQ79600_CommandResp_CONTROL1(phyDevId, commDir);
			break;
		}
	}
	else
	{
		if(AfeSim_UseType == AFESIM_TYPE_79718)
		{
			switch(currentReg)
			{
			case BQ79600_REG_DIR0_ADDR:	/*正链自动编址时才会写入*/
				AfeSimX1_BQ79600_CommandResp_DIR0_ADDR(phyDevId, commDir);
				break;
			case BQ79600_REG_DIR1_ADDR:	/*反链自动编址时才会写入*/
				AfeSim_BQ79600_CommandResp_DIR1_ADDR(phyDevId, commDir);
				break;
			case AFESIM_BQ79616_REG_COMM_CTRL:
				AfeSim_BQ79718_WriteCommandResponse_COMM_CTRL(phyDevId, commDir);
				break;
			case AFESIM_BQ79600_REG_CONTROL1:
				AfeSimX1_BQ79600_CommandResp_CONTROL1(phyDevId, commDir);
				break;
			case AFESIM_BQ79718_REG_DEV_CONF2:
				AfeSim_BQ79718_WriteCommandResponse_DEV_CONF2(phyDevId);
				break;
			case AFESIM_BQ79718_REG_ADC_CTRL1:
				break;
			case AFESIM_BQ79718_REG_ADC_CTRL2:
				AfeSimX1_BQ79718_WriteCommandResponse_ADC_CTRL2(phyDevId);
				break;
			}
		}
		else if(AfeSim_UseType == AFESIM_TYPE_79616)
		{
			switch(currentReg)
			{
			case AFESIM_BQ79616_REG_ACTIVE_CELL: /* 0x0003 */
				AfeSim_BQ79616_WriteCommandResponse_ACTIVE_CELL(phyDevId);
				break;
			case AFESIM_BQ79616_REG_BBVC_POSN1: /* 0x0005 */
				AfeSim_BQ79616_WriteCommandResponse_BBVC_POSN1(phyDevId);
				break;
			case AFESIM_BQ79616_REG_BBVC_POSN2:  /* 0x0006 */
				AfeSim_BQ79616_WriteCommandResponse_BBVC_POSN2(phyDevId);
				break;
			case AFESIM_BQ79616_REG_GPIO_CONF1: /* 0x000E */
				AfeSim_BQ79616_WriteCommandResponse_GPIO_CONF1(phyDevId);
				break;
			case AFESIM_BQ79616_REG_GPIO_CONF2: /* 0x000F */
				AfeSim_BQ79616_WriteCommandResponse_GPIO_CONF2(phyDevId);
				break;
			case AFESIM_BQ79616_REG_GPIO_CONF3: /* 0x0010 */
				AfeSim_BQ79616_WriteCommandResponse_GPIO_CONF3(phyDevId);
				break;
			case AFESIM_BQ79616_REG_GPIO_CONF4: /* 0x0011 */
				AfeSim_BQ79616_WriteCommandResponse_GPIO_CONF4(phyDevId);
				break;
			case BQ79600_REG_DIR0_ADDR:	/* 0x0306 正链自动编址时才会写入*/
				AfeSimX1_BQ79600_CommandResp_DIR0_ADDR(phyDevId, commDir);
				break;
			case BQ79600_REG_DIR1_ADDR:	/* 0x0307 反链自动编址时才会写入*/
				AfeSim_BQ79600_CommandResp_DIR1_ADDR(phyDevId, commDir);
				break;
			case AFESIM_BQ79616_REG_COMM_CTRL: /* 0x0308 */
				AfeSim_BQ79718_WriteCommandResponse_COMM_CTRL(phyDevId, commDir);
				break;
			case AFESIM_BQ79600_REG_CONTROL1: /* 0x0309 */
				AfeSimX1_BQ79600_CommandResp_CONTROL1(phyDevId, commDir);
				break;
			case AFESIM_BQ79616_REG_CONTROL2:  /* 0x030A*/
				AfeSim_BQ79616_WriteCommandResponse_CONTROL2(phyDevId);
				break;
			case AFESIM_BQ79616_REG_ADC_CTRL1: /* 0x030D */
				AfeSim_BQ79616_WriteCommandResponse_ADC_CTRL1(phyDevId);
				break;
			case AFESIM_BQ79616_REG_ADC_CTRL2: /* 0x030E */
				AfeSim_BQ79616_WriteCommandResponse_ADC_CTRL2(phyDevId);
				break;
			case AFESIM_BQ79616_REG_ADC_CTRL3: /* 0x030F */
				AfeSim_BQ79616_WriteCommandResponse_ADC_CTRL3(phyDevId);
				break;
			case AFESIM_BQ79616_REG_OVUV_CTRL: /* 0x032C */
				AfeSim_BQ79616_WriteCommandResponse_OVUV_CTRL(phyDevId);
				break;
			case AFESIM_BQ79616_REG_OTUT_CTRL: /* 0x032D */
				AfeSim_BQ79616_WriteCommandResponse_OTUT_CTRL(phyDevId);
				break;
			case AFESIM_BQ79616_REG_BAL_CTRL1: /* 0x032E */
				AfeSim_BQ79616_WriteCommandResponse_BAL_CTRL1(phyDevId);
				break;
			case AFESIM_BQ79616_REG_BAL_CTRL2: /* 0x032F */
				AfeSim_BQ79616_WriteCommandResponse_BAL_CTRL2(phyDevId);
				break;
			case AFESIM_BQ79616_REG_FAULT_RST1: /* 0x0331*/
				AfeSim_BQ79616_WriteCommandResponse_FAULT_RST1(phyDevId);
				break;
			case AFESIM_BQ79616_REG_FAULT_RST2: /* 0x0332*/
				AfeSim_BQ79616_WriteCommandResponse_FAULT_RST2(phyDevId);
				break;
			case AFESIM_BQ79616_REG_DIAG_PWR_CTRL: /* 0x0337 */
			 	AfeSim_BQ79616_WriteCommandResponse_DIAG_PWR_CTRL(phyDevId);
			 	break;
			case AFESIM_BQ79616_REG_DIAG_COMP_CTRL3: /* 0x033C */
				AfeSim_BQ79616_WriteCommandResponse_DIAG_COMP_CTRL3(phyDevId);
				break;
			case AFESIM_BQ79616_REG_DIAG_COMP_CTRL4: /* 0x033D */
			 	AfeSim_BQ79616_WriteCommandResponse_DIAG_COMP_CTRL4(phyDevId);
			 	break;
			}
		}
	}
}

/* AfeSimX1_DeviceSingleWrite
 * 对单一设备寄存器写入数据
 *  */
uint8 AfeSimX1_DeviceSingleWrite(uint8 stackDevId, uint16 regAddrStart, uint8* data, uint8 len, AfeSim_DeviceCommDirectionType commDir)
{
	uint8 retVal = FALSE;
	sint8 phyDevId = 0;
	sint8 phyTargetDevId = 0;
	sint8 devStart = 0;
	sint8 devEnd = 0;
	sint8 devInc = 1;
	uint8 offset = 0;
	uint16 currentReg = 0;
	uint8 transmitNoiseFail = FALSE;
	uint8 transmitNoiseStart = 0;

	RTE_Read_DaisyChainErrRate(&AfeSim_DaisyChainNoiseErrRate_1_10000);
	
	if(stackDevId == AFESIM_DEVID_0)
	{
		/* 判断79600能否写入数据 */
		if(FALSE == AfeSim_AssertBaseDeviceCanRW())
		{
			retVal = FALSE;
			printf("Single Write Fail, BQ79600 Power Down\r\n");
		}
		else
		{
			phyDevId = AFESIM_DEVID_0;
			for(offset = 0; offset < len; offset++)
			{
				currentReg = regAddrStart + offset;
				AfeSim_Reg[phyDevId][currentReg] = data[offset];
				AfeSim_DeviceSingleCommandResponse(phyDevId, currentReg, commDir);
			}
			retVal = TRUE;
		}
	}
	else
	{
		/* 判断79600能否传输数据 */
		if(FALSE == AfeSim_AssertBaseDeviceCanTransmit(commDir))
		{
			retVal = FALSE;
			printf("Single Write Fail, BQ79600 Daisy Chain Broken\r\n");
		}
		/* 选择通信方向*/
		if(commDir == AFESIM_DEVICE_USE_COMH)
		{
			devInc = 1;
			devStart = 1;
			phyTargetDevId = stackDevId;
			devEnd = AfeMan->afeChipRealNum + devInc;
		}
		else
		{
			devInc = -1;
			devStart = AfeMan->afeChipRealNum;
			phyTargetDevId = AfeMan->afeChipRealNum - stackDevId + 1;
			devEnd = 1 + devInc;
		}

		for(phyDevId = devStart; phyDevId != devEnd; phyDevId += devInc)
		{
#if 1
			if(phyDevId != BQ79600_DEVID)
			{
				/* 判断设备通信方向一致性 */
				if(FALSE == AfeSim_AssertDeviceCommDir(phyDevId, commDir))
				{
					retVal = FALSE;
					printf("Stack Write Fail, Path Device %d is Not in Stack %d\r\n", phyDevId, commDir);
					break;
				}
			}
#endif
			#if 1
			/*判断设备是否接收失败*/
			transmitNoiseFail |= AfeSim_AssertDeviceTransmitNoise();
			if(transmitNoiseFail & AFESIM_COMM_ERR_NOISE_BIT_FLIP)
			{
				AfeSim_DeviceTransmitErrorHandler(AfeSim_UseType, phyDevId, AFESIM_COMM_RC_FAULT, commDir);
				break;
			}
			#endif
			/* 非目标设备 */
			if(phyDevId != phyTargetDevId)
			{
				/* 判断此设备能否传输数据到下一个设备 */
				if(FALSE == AfeSim_AssertDeviceCanTransmit(phyDevId))
				{
					retVal = FALSE;
					printf("Single Write Fail, Path Device %d in Stack %d is Broken\r\n", phyDevId, commDir);
					break;
				}
				/* 已经是栈顶设备了，还未找到目标设备 */
				if(TRUE == AfeSim_AssertDeviceIsStackTop(phyDevId))
				{
					retVal = FALSE;
					printf("Single Write Fail, Device %d Not Found in Stack %d\r\n", phyDevId, commDir);
					break;
				}
			}
			else /* 目标设备 */
			{
				/* 判断设备能否写入数据 */
				if(transmitNoiseFail == 0 && AfeSim_AssertDeviceCanWrite(phyDevId, commDir))
				{
					for(offset = 0; offset < len; offset++)
					{
						currentReg = regAddrStart + offset;
						AfeSim_Reg[phyDevId][currentReg] = data[offset];
						AfeSim_DeviceSingleCommandResponse(phyDevId, currentReg, commDir);
					}
					retVal = TRUE;
					printf("Single Write Device %d %04X in Stack %d\r\n", phyDevId, regAddrStart, commDir);
				}
				/* 写入失败 */
				else
				{
					retVal = FALSE;
					printf("Single Write Fail, Device %d is broken\r\n", phyDevId);
					break;
				}
				break;
			}
		}
	}
	return retVal;
}

/* AfeSimX1_DeviceStackWrite
 * 对设备进行栈写入或广播写入 */
uint8 AfeSimX1_DeviceStackWrite(uint16 regAddrStart, uint8* data, uint8 len, AfeSim_DeviceCommDirectionType commDir, uint8 isBroadcast)
{
	uint8 retVal = FALSE;
	uint8 writeCnt = 0;
	sint8 phyDevId = 0;
	sint8 devStart = 0;
	sint8 devEnd = 0;
	sint8 devInc = 1;
	uint8 offset = 0;
	uint16 currentReg = 0;
	uint8 transmitNoiseFail = 0;
	uint8 transmitNoiseStart = 0;
	RTE_Read_DaisyChainErrRate(&AfeSim_DaisyChainNoiseErrRate_1_10000);

	if(isBroadcast)
	{
		/* 判断79600能否写入数据 */
		if(FALSE == AfeSim_AssertBaseDeviceCanRW())
		{
			retVal = FALSE;
			printf("Broadcast Write Fail, BQ79600 power down\r\n");
		}
		else
		{
			for(offset = 0; offset < len; offset++)
			{
				currentReg = regAddrStart + offset;
				AfeSim_Reg[AFESIM_DEVID_0][currentReg] = data[offset];
				AfeSim_DeviceSingleCommandResponse(phyDevId, currentReg, commDir);
			}
			printf("Broadcast Write Device %d %04X\r\n", AFESIM_DEVID_0, regAddrStart);
			writeCnt++;
		}
	}

	if(FALSE == AfeSim_AssertBaseDeviceCanTransmit(commDir))
	{
		retVal = FALSE;
		printf("Stack Write Fail, BQ79600 Daisy Chain Broken\r\n");
	}
	else
	{
		if(commDir == AFESIM_DEVICE_USE_COMH)
		{
			devInc = 1;
			devStart = 1;
			devEnd = AfeMan->afeChipRealNum + devInc;
		}
		else
		{
			devInc = -1;
			devStart = AfeMan->afeChipRealNum;
			devEnd = 1 + devInc;
		}
		for(phyDevId = devStart; phyDevId != devEnd; phyDevId += devInc)
		{
			/* 判断设备能否接收菊花链信号 */
			if(FALSE == AfeSim_AssertDeviceCanReceive(phyDevId, commDir))
			{
				retVal = FALSE;
				printf("Stack Write Fail, Path Device %d in Stack %d is Broken\r\n", phyDevId, commDir);
				break;
			}
#if 1
			/* 如果不是Broadcast Reverse Write, 需要判断设备通信方向一致性 */
			if(isBroadcast != 2)
			{
				/* 正向写, 要求AFE芯片通信方向与BQ79600一致 */
				if(FALSE == AfeSim_AssertDeviceCommDir(phyDevId, commDir))
				{
					retVal = FALSE;
					printf("Stack Write Fail, Path Device %d is Not in Stack %d\r\n", phyDevId, commDir);
					break;
				}
			}
#endif
#if 1
			/*判断设备是否接收失败*/
			transmitNoiseFail |= AfeSim_AssertDeviceTransmitNoise();
			if(transmitNoiseFail & AFESIM_COMM_ERR_NOISE_BIT_FLIP)
			{
				AfeSim_DeviceTransmitErrorHandler(AfeSim_UseType, phyDevId, AFESIM_COMM_RC_FAULT, commDir);
				break;
			}
#endif

			/* 判断设备能否写入数据 */
			if(transmitNoiseFail == 0 && AfeSim_AssertDeviceCanWrite(phyDevId, commDir))
			{
				for(offset = 0; offset < len; offset++)
				{
					currentReg = regAddrStart + offset;
					AfeSim_Reg[phyDevId][currentReg] = data[offset];
					AfeSim_DeviceSingleCommandResponse(phyDevId, currentReg, commDir);
				}
				printf("Stack Write Device %d %04X \r\n",phyDevId, regAddrStart);
				writeCnt++;
			}
			/* 判断设备是否已经是栈顶 */
			if(TRUE == AfeSim_AssertDeviceIsStackTop(phyDevId))
			{
				retVal = TRUE;
				break;
			}
			/* 判断设备能否传输菊花链信号 */
			if(FALSE == AfeSim_AssertDeviceCanSend(phyDevId, commDir))
			{
				retVal = FALSE;
				printf("Stack Write Fail, Path Device %d in Stack %d is Broken\r\n", phyDevId, commDir);
				break;
			}
		}
	}
	return retVal;
}


/* AfeSimX1_DeviceSingleRead
 * 单体设备读取数据
 * stackDevId 是原指令的栈设备ID，若当前使用反链，需要映射到物理设备ID
 * 返回的buff中，设备ID还是targetDevId */
uint8 AfeSimX1_DeviceSingleRead(uint8 stackDevId, uint16 regAddrStart, uint8* buff, uint8 rwBytes, AfeSim_DeviceCommDirectionType commDir)
{
	uint8 retVal = FALSE;
	uint8 phyDevId = 0;
	uint8 phyTargetDevId;
	sint8 devStart = 0;
	sint8 devInc = 1;
	sint8 devEnd = AfeMan->afeChipRealNum + devInc;
	uint16 offset = 0;
	uint8 transmitNoiseFail = 0;
	uint8 transmitFail = 0;
	uint8 oneDevTransFail = 0;
	uint8 targetIdPos = 0;
	RTE_Read_DaisyChainErrRate(&AfeSim_DaisyChainNoiseErrRate_1_10000);

	if(stackDevId == AFESIM_DEVID_0)
	{
		/* 判断79600能否写入数据 */
		if(FALSE == AfeSim_AssertBaseDeviceCanRW())
		{
			retVal = FALSE;
			printf("Single Read Fail, BQ79600 Power Down\r\n");
		}
		else
		{
			phyDevId = 0;
			transmitFail = AfeSim_DeviceCreateSingleReplyBuff(
					phyDevId,
					buff,
					stackDevId,
					rwBytes,
					regAddrStart,
					&AfeSim_Reg[stackDevId][regAddrStart],
					&offset);
			retVal = TRUE;
			printf("Single Read BQ79600 %04X\r\n", regAddrStart);
		}
	}
	else
	{
		if(FALSE == AfeSim_AssertBaseDeviceCanTransmit(commDir))
		{
			retVal = FALSE;
			printf("Single Read Fail, BQ79600 Daisy Chain Broken\r\n");
		}
		else
		{
			if(commDir == AFESIM_DEVICE_USE_COMH)
			{
				devInc = 1;
				devStart = 1;
				devEnd = AfeMan->afeChipRealNum + devInc;
				targetIdPos = AfeMan->afeChipRealNum;
				phyTargetDevId = stackDevId;
			}
			else
			{
				devInc = -1;
				devStart = AfeMan->afeChipRealNum;
				devEnd = 1 + devInc;
				targetIdPos = 1;
				phyTargetDevId = AfeMan->afeChipRealNum - stackDevId + 1;
			}
			for(phyDevId = devStart; phyDevId != devEnd; phyDevId += devInc)
			{
#if 1
				/* 判断设备通信方向一致性 */
				if(FALSE == AfeSim_AssertDeviceCommDir(phyDevId, commDir))
				{
					retVal = FALSE;
					printf("Stack Write Fail, Path Device %d is Not in Stack %d\r\n", phyDevId, commDir);
					break;
				}
#endif
#if 1
				/*判断设备是否接收失败*/
				transmitNoiseFail |= AfeSim_AssertDeviceTransmitNoise();
				if(transmitNoiseFail & AFESIM_COMM_ERR_NOISE_BIT_FLIP)
				{
					AfeSim_DeviceTransmitErrorHandler(AfeSim_UseType, phyDevId, AFESIM_COMM_RC_FAULT, commDir);
					break;
				}
#endif

				/* 非目标设备 */
				if(phyDevId != phyTargetDevId)
				{
					/* 判断此设备能否传输数据到下一个设备，不能则break */
					if(FALSE == AfeSim_AssertDeviceCanTransmit(phyDevId))
					{
						retVal = FALSE;
						printf("Single Read Fail, Path Device %d in Stack %d is Broken\r\n", phyDevId, commDir);
						break;
					}
					/* 判断是否是栈顶设备，是则break */
					if(TRUE == AfeSim_AssertDeviceIsStackTop(phyDevId))
					{
						targetIdPos = phyDevId;
						retVal = FALSE;
						printf("Single Read Fail, Device %d is Not Found in Stack %d\r\n", phyDevId, commDir);
						break;
					}
				}
				else /* 目标设备 */
				{
					targetIdPos = phyDevId;
					/* 判断设备能否读取数据 */
					if(TRUE == AfeSim_AssertDeviceCanRead(phyDevId, commDir))
					{
						transmitFail = AfeSim_DeviceCreateSingleReplyBuff(
								phyDevId,
								buff,
								stackDevId,
								rwBytes,
								regAddrStart,
								&AfeSim_Reg[phyDevId][regAddrStart],
								&offset);
						/* transmitFail = 1 代表发生了CRC翻转 */
						retVal = TRUE;
						printf("Single Read Device %d %04X in Stack %d\r\n", phyDevId, regAddrStart, AfeSim_Reg[phyDevId][regAddrStart], commDir);
					}
					/* 写入失败 */
					else
					{
						retVal = FALSE;
						printf("Single Read Fail, Target Device %d in Stack %d is Broken\r\n", phyDevId, commDir);
					}
					break;
				}
			}
		}
	}
	/*回传响应帧
	若正链回传，则接收方用COMH收
	若反链回传，则接收方用COML收 */
	if(retVal == TRUE && stackDevId != BQ79600_DEVID)
	{
		if(commDir == AFESIM_DEVICE_USE_COMH)
		{
			devInc = 1;
			devEnd = 1 - devInc;
		}
		else
		{
			devInc = -1;
			devEnd = AfeMan->afeChipRealNum - devInc;
		}

		uint16 buffTotalLen = rwBytes + 6;
		for(phyDevId = targetIdPos; phyDevId != devEnd; phyDevId -= devInc)
		{
			if(phyDevId == targetIdPos)
			{
				if(transmitFail)
				{
				#ifdef AFESIM_DEBUG
				#else
					AfeSim_DeviceTransmitErrorHandler(AfeSim_UseType, phyDevId, AFESIM_COMM_TR_FAULT, commDir);
				#endif
				}
			}
			else
			{
				/* 响应帧经过虚拟菊花链信道 */
				oneDevTransFail = AfeSim_AssertDeviceTransmitNoise();
				if(oneDevTransFail & AFESIM_COMM_ERR_NOISE_BIT_FLIP)
				{
					buff[rand() % buffTotalLen] ^= (1 << (rand()%8));
				}
				/* 设备接收到虚拟菊花链信号并故障处理 */
				transmitFail |= oneDevTransFail;
				if(transmitFail & AFESIM_COMM_ERR_CRC_FLIP)
				{
					AfeSim_DeviceTransmitErrorHandler(AfeSim_UseType, phyDevId, AFESIM_COMM_RR_FAULT, commDir);
				}
				else if(transmitFail & AFESIM_COMM_ERR_NOISE_BIT_FLIP)
				{
					AfeSim_DeviceTransmitErrorHandler(AfeSim_UseType, phyDevId, AFESIM_COMM_BIT_FLIP, commDir);
				}
			}
		}
	}
	return retVal;
}

/* Public - AfeSim_DeviceStackRead
 * 栈读取、全局读取
 * X1 已升级支持双链 */
uint8 AfeSimX1_DeviceStackRead(uint16 regAddrStart, uint8* buff, uint8 rwBytes, AfeSim_DeviceCommDirectionType commDir, uint8 isBroadcast)
{
	uint8 retVal = FALSE;
	uint8 readEnable = FALSE;
	uint8 stackTop = 0;
	sint8 stackDevId;
	uint8 phyDevId;
	sint8 devStart;
	sint8 devEnd;
	sint8 devInc = 1;
	uint16 offset = 0;
	uint8 transmitNoiseFail = 0;
	uint8 transmitFail = 0;
	uint8 oneDevTransFail = 0;
	RTE_Read_DaisyChainErrRate(&AfeSim_DaisyChainNoiseErrRate_1_10000);

	/* 判断BQ79600能否传输数据 */
	if(FALSE == AfeSim_AssertBaseDeviceCanRW())
	{
		retVal = FALSE;
		printf("Stack Read Fail, BQ79600 is Broken\r\n");
	}
	else if(FALSE == AfeSim_AssertBaseDeviceCanTransmit(commDir))
	{
		retVal = FALSE;
		printf("Stack Read Fail, BQ79600 Daisy Chain is Broken\r\n");
	}
	else
	{
		if(commDir == AFESIM_DEVICE_USE_COMH)
		{
			devInc = 1;
			devStart = 1;
			devEnd = AfeMan->afeChipRealNum + devInc;
		}
		else
		{
			devInc = -1;
			devStart = AfeMan->afeChipRealNum;
			devEnd = 1 + devInc;
		}
		for(phyDevId = devStart; phyDevId != devEnd; phyDevId += devInc)
		{
#if 1
			/* 判断通信方向一致性 */
			if(FALSE == AfeSim_AssertDeviceCommDir(phyDevId, commDir))
			{
				retVal = FALSE;
				printf("Stack Read Fail, Path Device %d is Not in Stack %d\r\n", phyDevId, commDir);
				break;
			}
#endif
#if 1
			/*判断设备是否接收失败*/
			transmitNoiseFail |= AfeSim_AssertDeviceTransmitNoise();
			if(transmitNoiseFail & AFESIM_COMM_ERR_NOISE_BIT_FLIP)
			{
				AfeSim_DeviceTransmitErrorHandler(AfeSim_UseType, phyDevId, AFESIM_COMM_RC_FAULT, commDir);
				break;
			}
#endif

			/* 判断栈顶设备能否回读数据 */
			if(FALSE == AfeSim_AssertDeviceIsStackTop(phyDevId))
			{
				if(FALSE == AfeSim_AssertDeviceCanTransmit(phyDevId))
				{
					readEnable = FALSE;
					retVal = FALSE;
					printf("Stack Read Fail, Top Device %d in Stack %d is Broken\r\n", phyDevId, commDir);
					break;
				}
			}
			/* 判断栈设备能否传输数据 */
			else
			{
				if(TRUE == AfeSim_AssertDeviceCanRead(phyDevId, commDir))
				{
					readEnable = TRUE;
					stackTop = phyDevId;
					retVal = TRUE;
				}
				else
				{
					readEnable = FALSE;
					retVal = FALSE;
					printf("Stack Read Fail, Path Device %d in Stack %d is Broken\r\n", phyDevId, commDir);
				}
				break;
			}
		}
		
		/* 从栈顶向下回读数据 */
		if(readEnable)
		{
			if(commDir == AFESIM_DEVICE_USE_COMH)
			{
				devInc = -1;
				devStart = stackTop;
				devEnd = 1 + devInc;
			}
			else
			{
				devInc = 1;
				devStart = stackTop;
				devEnd = AfeMan->afeChipRealNum + devInc;
			}
			uint8 buffSingleLen = (rwBytes + 6);
			uint16 buffTotalLen = 0;
			for(phyDevId = devStart; phyDevId != devEnd; phyDevId += devInc)
			{
				buffTotalLen += buffSingleLen;
				if(commDir == AFESIM_DEVICE_USE_COMH)
				{
					stackDevId = phyDevId;
				}
				else
				{
					stackDevId = AfeMan->afeChipRealNum - phyDevId + 1;
				}

				/*收到来自上一设备的错误回复帧*/
				if(transmitFail)
				{
					if(transmitFail & AFESIM_COMM_ERR_CRC_FLIP)
					{
						AfeSim_DeviceTransmitErrorHandler(AfeSim_UseType, phyDevId, AFESIM_COMM_RR_FAULT, commDir);
					}
					else if(transmitFail & AFESIM_COMM_ERR_NOISE_BIT_FLIP)
					{
						AfeSim_DeviceTransmitErrorHandler(AfeSim_UseType, phyDevId, AFESIM_COMM_BIT_FLIP, commDir);
					}
				}
				
				transmitFail |= AfeSim_DeviceCreateSingleReplyBuff(
						phyDevId,
						buff,
						stackDevId,
						rwBytes,
						regAddrStart,
						&AfeSim_Reg[phyDevId][regAddrStart],
						&offset);
				
				oneDevTransFail = AfeSim_AssertDeviceTransmitNoise();
				if(oneDevTransFail)
				{
					buff[rand() % buffTotalLen] ^= (1 << (rand()%8));
				}
				transmitFail |= oneDevTransFail;

				printf("Stack %d Read Device %d %04X\r\n", commDir, phyDevId, regAddrStart);
			}
			if(isBroadcast)
			{
				phyDevId = 0;
				if(transmitFail & AFESIM_COMM_ERR_CRC_FLIP)
				{
					AfeSim_BQ79600_CommErrorHandler(commDir, 1);
				}
				else if(transmitFail & AFESIM_COMM_ERR_NOISE_BIT_FLIP)
				{
					AfeSim_BQ79600_CommErrorHandler(commDir, 0);
				}
				
				AfeSim_DeviceCreateSingleReplyBuff(
						phyDevId,
						buff,
						AFESIM_DEVID_0,
						rwBytes,
						regAddrStart,
						&AfeSim_Reg[AFESIM_DEVID_0][regAddrStart],
						&offset);
				printf("Broadcast Read BQ79600 %04X\r\n", regAddrStart);
			}
		}
	}
	/*回传响应帧
	若正链回传，则接收方用COMH收
	若反链回传，则接收方用COML收 */
	return retVal;
}
