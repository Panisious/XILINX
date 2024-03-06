/*
 * AfeCalibration.h
 *
 *  Created on: 2024年2月4日
 *      Author: Administrator
 */

#ifndef SRC_ASW_AFECALIBRATION_AFECALIBRATION_H_
#define SRC_ASW_AFECALIBRATION_AFECALIBRATION_H_

#include "RTE.h"
#include "BatterySim.h"
#include "AfeDeviceSim.h"

#define AFECALIB_MAX_QUEUE_EXP						9
#define AFECALIB_MAX_QUEUE_NUM						(1 << AFECALIB_MAX_QUEUE_EXP)

#define AFECALIB_ENABLE_MASK						0x80000000
#define AFECALIB_DATA_MASK							0x7FFFFFFF

#define AFECALIB_MAX_CALIB_NUM						32768u

/*基础AFE采样*/

/*标定电芯SOX,间接标定电压*/
#define AFECALIB_CELLSOX_OFFSET						0u
#define AFECALIB_CELLSOX_NUM						BAT_MAX_NUM

/*直接标定电芯电压, 这会导致SOX仿真暂停*/
#define AFECALIB_CELLVOLT_OFFSET					(AFECALIB_CELLSOX_OFFSET + AFECALIB_CELLSOX_NUM)
#define AFECALIB_CELLVOLT_NUM						BAT_MAX_NUM

/*标定电芯电压ADC测量偏移量*/
#define AFECALIB_CELLVOLT_BIAS_OFFSET				(AFECALIB_CELLVOLT_OFFSET + AFECALIB_CELLVOLT_NUM)
#define AFECALIB_CELLVOLT_BIAS_NUM					BAT_MAX_NUM

/*直接标定模组电压, 这会导致SOX仿真暂停*/
#define AFECALIB_MODULEVOLT_OFFSET					(AFECALIB_CELLVOLT_BIAS_OFFSET + AFECALIB_CELLVOLT_BIAS_NUM)
#define AFECALIB_MODULEVOLT_NUM						(AFESIM_MAX_AFE_NUM + 1u)

/*标定模组电压ADC测量偏移量*/
#define AFECALIB_MODULEVOLT_BIAS_OFFSET				(AFECALIB_MODULEVOLT_OFFSET + AFECALIB_MODULEVOLT_NUM)
#define AFECALIB_MODULEVOLT_BIAS_NUM				(AFESIM_MAX_AFE_NUM + 1u)

/*直接标定GPIO电压, 这会导致热仿真暂停*/
#define AFECALIB_GPIOVOLT_OFFSET					(AFECALIB_MODULEVOLT_BIAS_OFFSET + AFECALIB_MODULEVOLT_BIAS_NUM)
#define AFECALIB_GPIOVOLT_NUM						(AFESIM_MAX_GPIO_NUM * (AFESIM_MAX_AFE_NUM + 1u))

/*标定GPIO电压测量偏移量*/
#define AFECALIB_GPIOVOLT_BIAS_OFFSET				(AFECALIB_GPIOVOLT_OFFSET + AFECALIB_GPIOVOLT_NUM)
#define AFECALIB_GPIOVOLT_BIAS_NUM					(AFESIM_MAX_GPIO_NUM * (AFESIM_MAX_AFE_NUM + 1u))

/*标定TSREF电压*/
#define AFECALIB_TSREF_OFFSET						(AFECALIB_GPIOVOLT_BIAS_OFFSET + AFECALIB_GPIOVOLT_BIAS_NUM)
#define AFECALIB_TSREF_NUM							(AFESIM_MAX_AFE_NUM + 1u)

/*标定TSREF ADC测量电压*/
#define AFECALIB_TSREF_BIAS_OFFSET					(AFECALIB_TSREF_OFFSET + AFECALIB_TSREF_NUM)
#define AFECALIB_TSREF_BIAS_NUM						(AFESIM_MAX_AFE_NUM + 1u)

/*标定辅助电压*/
#define AFECALIB_AUXVOLT_SINGLE_CHIP_NUM			(20)
#define AFECALIB_AUXVOLT_OFFSET						(AFECALIB_TSREF_BIAS_OFFSET + AFECALIB_TSREF_BIAS_NUM)
#define AFECALIB_AUXVOLT_NUM						((AFESIM_MAX_AFE_NUM + 1u) * AFECALIB_AUXVOLT_SINGLE_CHIP_NUM)

/*标定辅助电压ADC测量偏差*/
#define AFECALIB_AUXVOLT_BIAS_OFFSET				(AFECALIB_AUXVOLT_OFFSET + AFECALIB_AUXVOLT_NUM)
#define AFECALIB_AUXVOLT_BIAS_NUM					((AFESIM_MAX_AFE_NUM + 1u) * AFECALIB_AUXVOLT_SINGLE_CHIP_NUM)

/*标定模组温度*/
#define AFECALIB_MODULETEMP_OFFSET					(AFECALIB_AUXVOLT_BIAS_OFFSET + AFECALIB_AUXVOLT_BIAS_NUM)
#define AFECALIB_MODULETEMP_NUM						(AFESIM_MAX_GPIO_NUM * (AFESIM_MAX_AFE_NUM + 1u))

/*标定均衡温度*/
#define AFECALIB_BALANCETEMP_OFFSET					(AFECALIB_MODULETEMP_OFFSET + AFECALIB_MODULETEMP_NUM)
#define AFECALIB_BALANCETEMP_NUM					(AFESIM_MAX_GPIO_NUM * (AFESIM_MAX_AFE_NUM + 1u))

/*标定芯片温度*/
#define AFECALIB_DIETEMP_OFFSET						(AFECALIB_BALANCETEMP_OFFSET + AFECALIB_BALANCETEMP_NUM)
#define AFECALIB_DIETEMP_NUM						(AFESIM_MAX_DT_NUM * (AFESIM_MAX_AFE_NUM + 1u))

/*标定Shunt温度，BQ79758采样*/
#define AFECALIB_SHUNTTEMP_OFFSET					(AFECALIB_DIETEMP_OFFSET + AFECALIB_DIETEMP_NUM)
#define AFECALIB_SHUNTTEMP_NUM						AFESIM_MAX_ST_NUM

/*标定Pack温度*/
#define AFECALIB_PACKTEMP_OFFSET					(AFECALIB_SHUNTTEMP_OFFSET + AFECALIB_SHUNTTEMP_NUM)
#define AFECALIB_PACKTEMP_NUM						(1u)
/*标定环境温度*/
#define AFECALIB_AIRTEMP_OFFSET						(AFECALIB_PACKTEMP_OFFSET + AFECALIB_PACKTEMP_NUM)
#define AFECALIB_AIRTEMP_NUM						(1u)

#define AFECALIB_1stCURRENT_OFFSET					(AFECALIB_AIRTEMP_OFFSET + AFECALIB_AIRTEMP_NUM)
#define AFECALIB_1stCURRENT_NUM						(1u)
#define AFECALIB_2ndCURRENT_OFFSET					(AFECALIB_1stCURRENT_OFFSET + AFECALIB_1stCURRENT_NUM)
#define AFECALIB_2ndCURRENT_NUM						(1u)

/*环境物理参数标定*/
#define AFECALIB_PACK_HEAT_SINK_FACTOR_OFFSET		(AFECALIB_2ndCURRENT_OFFSET + AFECALIB_2ndCURRENT_NUM)
#define AFECALIB_MODULE_HEAT_SINK_FACTOR_OFFSET		(AFECALIB_MODULE_HEAT_SINK_FACTOR_OFFSET + 1u)
#define AFECALIB_PCB_HEAT_SINK_FACTOR_OFFSET		(AFECALIB_MODULE_HEAT_SINK_FACTOR_OFFSET + 1u)

/*故障标定*/

/*电源故障部分*/
#define AFECALIB_PWR_FAULT_OFFSET
#define AFECALIB_AVDD_FAULT_OFFSET
#define AFECALIB_DVDD_FAULT_OFFSET
#define AFECALIB_CVDD_FAULT_OFFSET
/*带隙参考电压*/
#define AFECALIB_VBGREF_FAULT_OFFSET
/*ADC参考电压*/
#define AFECALIB_VREFH_FAULT_OFFSET

/*通信故障部分*/
/*0代表79600反链到AFEX故障，1代表79600正链到AFE1故障，2代表AFE1到AFE2故障，以此类推*/
#define AFECALIB_DAISYCHAIN_DISABLE_OFFSET			10000
#define AFECALIB_DAISYCHAIN_DISABLE_NUM				((AFESIM_MAX_AFE_NUM + 1u))
#define AFECALIB_DAISYCHAIN_NOISE_OFFSET			(AFECALIB_DAISYCHAIN_DISABLE_OFFSET + AFECALIB_DAISYCHAIN_DISABLE_NUM)
#define AFECALIB_DAISYCHAIN_NOISE_NUM				(1u)
/*0代表79600故障，1代表AFE1故障，以此类推*/
#define AFECALIB_DEVICE_DISABLE_OFFSET				(AFECALIB_DAISYCHAIN_NOISE_OFFSET + AFECALIB_DAISYCHAIN_NOISE_NUM)
#define AFECALIB_DEVICE_DISABLE_NUM					(AFESIM_MAX_AFE_NUM + 1u)

/*器件故障部分*/
/*低通滤波器故障*/
#define AFECALIB_ADC_LFP_FAULT_OFFSET


/*线路故障部分*/
#define AFECALIB_VCCB_SHORT_FAULT_OFFSET
#define AFECALIB_VC_OPEN_FAULT_OFFSET
#define AFECALIB_CB_OPEN_FAULT_OFFSET
#define AFECALIB_CBFET_FAULT_OFFSET
#define AFECALIB_GPIO_OPEN_FAULT_OFFSET
#define AFECALIB_GPIO_ADJSHORT_FAULT_OFFSET
#define AFECALIB_GPIO_COMP_FAULT_OFFSET
#define AFECALIB_BUSBAR_FAULT_OFFSET


/*诊断故障部分*/
/*故障注入故障*/
#define AFECALIB_FAULTINJBIST_FAULT_OFFSET
/*诊断比较器异常退出故障*/
#define AFECALIB_ADC_COMP_ABORT_OFFSET
#define AFECALIB_CRCFLIP_FAULT_OFFSET
#define AFECALIB_OVDAC_FAULT_OFFSET

#if (AFECALIB_PCB_HEAT_SINK_FACTOR_OFFSET) >= AFECALIB_MAX_CALIB_NUM
#error "Fuck AfeCalib Array Overflow !!!"
#endif

/* CAN报文标定, 报文ID定义 */
typedef enum
{
	/* AFE仿真器控制标定 */

		/* 000 清除标定量 */
		CAN_CALIB_ClearCalib_MSGID = 0x000,
		/* 001 AFE仿真控制 */
		CAN_CALIB_AfeSimControl_MSGID,
		/* 002 真实AFE和电芯数量 */
		CAN_CALIB_RealAfeCellNum_MSGID,
		/* 003 AFE种类选择 */
		CAN_CALIB_AfeUseType_MSGID,
		/* 004 AFE类型标定 */
		CAN_CALIB_AfeChipType_MSGID,

	/* 硬件标定 */

		/* 010 模组温度位置标定 */
		CAN_CALIB_ModuleTempPos_MSGID = 0x010,
		/* 011 均衡温度位置标定 */
		CAN_CALIB_BalTempPos_MSGID,
		/* 012 Shunt温度位置标定 */
		CAN_CALIB_ShuntTempPos_MSGID,
		/* 013 Shunt第二电流传感器位置标定 */
		CAN_CALIB_GpioCurrentPos_MSGID,
		/* 014 外部定义SOX查找表 */
		CAN_CALIB_SOXCurve_MSGID,
		/* 015 外部定义模组温度查找表 */
		CAN_CALIB_ModuleTempRT_MSGID,
		/* 016 外部定义均衡温度查找表 */
		CAN_CALIB_BalanceTempRT_MSGID,
		/* 017 外部定义Shunt温度查找表 */
		CAN_CALIB_ShuntTempRT_MSGID,

	/* 通信标定 */

		/* 020 菊花链通断标定 */
		CAN_CALIB_DaisyChainBroken_MSGID = 0x020,
		/* 021 菊花链噪声标定 */
		CAN_CALIB_DaisyChainNoise_MSGID,
		/* 022 芯片损毁标定 */
		CAN_CALIB_DeviceBroken_MSGID,
		/* 023 电压采样噪声 */
		CAN_CALIB_SampleNoise_MSGID,

	/* 电压标定 */

		/* 030 电芯SOX标定 */
		CAN_CALIB_CellSOX_MSGID = 0x030,
		/* 031 电芯电压标定, 注意, 标定电压需要暂停SOX仿真 */
		CAN_CALIB_CellVolt_MSGID,
		/* 032 电芯电压偏移量标定, 无需暂停SOX仿真 */
		CAN_CALIB_CellVoltBias_MSGID,

		/* 033 GPIO电压标定 */
		CAN_CALIB_GpioVolt_MSGID,
		/* 034 GPIO电压偏移标定 */
		CAN_CALIB_GpioVoltBias_MSGID,
		/* 035 模组电压标定 */
		CAN_CALIB_ModuleVolt_MSGID,
		/* 036 模组电压偏移标定 */
		CAN_CALIB_ModuleVoltBias_MSGID,
		/* 037 TSREF标定 */
		CAN_CALIB_TSREF_MSGID,
		/* 038 TSREF偏差标定 */
		CAN_CALIB_TSREFBias_MSGID,
		/* 039 辅助电压标定 */
		CAN_CALIB_AuxVolt_MSGID,
		/* 03A 辅助电压标定 */
		CAN_CALIB_AuxVoltBias_MSGID,

	/* 温度标定 */

		/* 040 模组温度标定 */
		CAN_CALIB_ModuleTemp_MSGID = 0x040,
		/* 041 均衡温度标定 */
		CAN_CALIB_BalTemp_MSGID,
		/* 042 芯片温度标定 */
		CAN_CALIB_DieTemp_MSGID,
		/* 043 Shunt温度标定 */
		CAN_CALIB_ShuntTemp_MSGID,
		/* 044 Pack温度标定 */
		CAN_CALIB_PackTemp_MSGID,
		/* 045 环境温度标定 */
		CAN_CALIB_AirTemp_MSGID,

	/* 电流标定 */
		/* 050 第一电流标定 */
		CAN_CALIB_1stCurrent_MSGID = 0x050,
		/* 051 第二电流偏移量标定 */
		CAN_CALIB_2ndCurrentBias_MSGID,

	/* 功能安全标定 */
		/* 060 79616上电自检故障注入*/
		CAN_CALIB_BQ79616_DiagBistFaultInject_MSGID = 0x060,
		/* 061 79616上电诊断故障注入*/
		CAN_CALIB_BQ79616_DiagKeyOnFaultInject_MSGID = 0x061,
		/* 062 79616周期诊断故障注入*/
		CAN_CALIB_BQ79616_DiagCycleFaultInject_MSGID = 0x062,
		
	/* 反向唤醒标定 */

	/* 任意寄存器标定 */
		/* 0E0 A类 任意寄存器读取 */
		CAN_CALIB_RandomRegReadA_MSGID = 0x0E0,

		/* 0F0 A类 任意寄存器标定 */
		CAN_CALIB_RandomRegWriteA_MSGID = 0x0F0,
		/* 0F1 B类 任意寄存器标定 */
		CAN_CALIB_RandomRegWriteB_MSGID,
		/* 0F2 C类 任意寄存器标定 控制帧 */
		CAN_CALIB_RandomRegWriteC1_MSGID,
		/* 0F3 C类 任意寄存器标定 数据帧 */
		CAN_CALIB_RandomRegWriteC2_MSGID,

	/* 输出 */
		/* 通信输出 */
		CAN_OUT_DaisyChainBroken_MSGID = 0x100,
		CAN_OUT_DeviceBroken_MSGID,
		CAN_OUT_DeviceStackRole_MSGID,
		CAN_OUT_DeviceStackID_MSGID,
		CAN_OUT_DaisyChainNoise_MSGID,

		/*物理量输出*/
		CAN_OUT_CellVolt_MSGID = 0x200,		/* 15AFE 18通道, 5*15报文 */
		CAN_OUT_ModuleVolt_MSGID = 0x250,	/* 15AFE */
		CAN_OUT_GpioVolt_MSGID = 0x260,   	/* 15AFE 11通道, 3*15报文 */
		CAN_OUT_AuxVolt_MSGID = 0x290,
		CAN_OUT_ModuleTemp_MSGID = 0x300, 	/* 15AFE 6通道, 2*15报文 */
		CAN_OUT_BalanceTemp_MSGID = 0x320, 	/* 15AFE 2通道, 1*15报文 */
		CAN_OUT_DieTemp_MSGID = 0x330, 		/*15AFE 2通道, 1*15报文 */
		CAN_OUT_Shunt_MSGID = 0x340,
		CAN_OUT_PackAir_MSGID,

		
		/*寄存器输出*/
		CAN_OUT_RandomReg_MSGID = 0x400,
		
		/*功能安全输出*/

}CAN_CALIB_MSGID_Type;

/* 报文ID 000，清除指定的标定量 */
typedef enum
{
	CAN_CALIB_ClearCalib_B0_Ignore = 0x00,		/* 忽略此报文 */
	CAN_CALIB_ClearCalib_B0_AllCalib,

	CAN_CALIB_ClearCalib_B0_ChainBroken = 0x10,
	CAN_CALIB_ClearCalib_B0_DeviceBroken,
	CAN_CALIB_ClearCalib_B0_CircuitFault,
	CAN_CALIB_ClearCalib_B0_ChipFault,
	CAN_CALIB_ClearCalib_B0_SampleNoise = 0x1F,
	
	CAN_CALIB_ClearCalib_B0_CellSOX = 0x20,
	CAN_CALIB_ClearCalib_B0_CellVolt,
	CAN_CALIB_ClearCalib_B0_ModuleVolt,
	CAN_CALIB_ClearCalib_B0_GpioVolt,
	CAN_CALIB_ClearCalib_B0_Tsref,
	CAN_CALIB_ClearCalib_B0_AuxVolt,			/* ONLY BQ79616 */
	

	CAN_CALIB_ClearCalib_B0_ModuleTemp = 0x30,
	CAN_CALIB_ClearCalib_B0_BalanceTemp,
	CAN_CALIB_ClearCalib_B0_DieTemp,
	CAN_CALIB_ClearCalib_B0_ShuntTemp,			/* ONLY BQ79656, BQ79758 */
	CAN_CALIB_ClearCalib_B0_PackTemp,			/* 内部环境温度 */
	CAN_CALIB_ClearCalib_B0_AirTemp,			/* 外界环境温度 */
	CAN_CALIB_ClearCalib_B0_AllTemp,

	CAN_CALIB_ClearCalib_B0_Current = 0x40,		/* ONLY BQ79656, BQ79758 */

	CAN_CALIB_ClearFunctionSafety_B0_All = 0x50,
}CAN_CALIB_ClearCalib_Type;

/* 报文ID 001，使能或暂停指定的仿真 */
typedef enum
{
	CAN_CALIB_AfeSimControl_B0_Ignore = 0x00,	/* 忽略此报文 */
	CAN_CALIB_AfeSimControl_B0_Reset,			/* 所有仿真量赋初值 */
	CAN_CALIB_AfeSimControl_B0_StartAll,		/* 启动全仿真 */
	CAN_CALIB_AfeSimControl_B0_PauseAll,
	CAN_CALIB_AfeSimControl_B0_StartSOX,		/* 电芯电压和容量随电流变化仿真 */
	CAN_CALIB_AfeSimControl_B0_PauseSOX,
	CAN_CALIB_AfeSimControl_B0_StartHeat,		/* 模组温度、Pack温度随电流变化仿真 */
	CAN_CALIB_AfeSimControl_B0_PauseHeat,
	CAN_CALIB_AfeSimControl_B0_StartBal,		/* 均衡电压、温度随均衡指令变化仿真 */
	CAN_CALIB_AfeSimControl_B0_PauseBal,
	CAN_CALIB_AfeSimControl_B0_StartGpio,		/* GPIO电压随温度变化仿真 */
	CAN_CALIB_AfeSimControl_B0_PauseGpio,
	CAN_CALIB_AfeSimControl_B0_StartReg,		/* 寄存器值随物理量变化仿真 */
	CAN_CALIB_AfeSimControl_B0_PauseReg,

	CAN_CALIB_AfeSimControl_B0_SOXuseCust = 0x80,	/* 使用外部SOX表 */
	CAN_CALIB_AfeSimControl_B0_SOXuseNCM ,		/* 使用默认三元锂电池SOX表 */
	CAN_CALIB_AfeSimControl_B0_SOXuseLFP,				/* 使用默认磷酸铁锂电池SOX表 */
	

	CAN_CALIB_AfeSimControl_B0_ModuleTempUseCust = 0x90,	/* 使用外部模组温度NTC RT 表*/
	CAN_CALIB_AfeSimControl_B0_ModuleTempUseDefault1,		/* 使用默认模组温度NTC RT 表 */

	CAN_CALIB_AfeSimControl_B0_BalTempUseCust = 0xA0,		/* 使用外部均衡温度NTC RT 表*/
	CAN_CALIB_AfeSimControl_B0_BalTempUseDefault1,			/* 使用默认均衡温度NTC RT 表 */

	CAN_CALIB_AfeSimControl_B0_ShuntTempUseCust = 0xB0,		/* 使用外部Shunt温度NTC RT 表*/
	CAN_CALIB_AfeSimControl_B0_ShuntTempUseDefault1,			/* 使用默认Shunt温度NTC RT 表 */
}CAN_CALIB_AfeSimControl;

typedef struct
{
	uint32 calibType[1024];
	uint32 calibValue[1024];
	struct
	{
		uint32 mask : AFECALIB_MAX_QUEUE_EXP;
	}readIndex;
	struct
	{
		uint32 mask : AFECALIB_MAX_QUEUE_EXP;
	}writeIndex;
}AfeCalib_QueueType;

typedef struct
{

}AfeCalib_ChainListType;

typedef struct
{
	uint8 readEnable;
	uint8 writeEnable;

	uint8 isGlobalR;
	uint8 isGlobalW;
	
	uint8 readPhyDevId;
	uint8 writePhyDevId;
	
	uint16 readStartRegPos;
	uint16 writeStartRegPos;
	
	uint16 readLen;
	uint16 writeLen;
	uint16 writeLoopLen;
	uint16 writeContinuousSingleLen;
}AfeCalib_RandomRegOpHandlerType;

typedef struct
{
	AfeCalib_RandomRegOpHandlerType randRegHandler;
}AfeCalib_ManagerType;

typedef struct
{

}AfeCalib_RteRequestType;


extern sint32 AfeCalib[AFECALIB_MAX_CALIB_NUM];
extern sint32 AfeCalibLatch[AFECALIB_MAX_CALIB_NUM];

extern void AfeCalibration_Run100ms(void);
extern void AfeCalibration_Run200ms(void);

#endif /* SRC_ASW_AFECALIBRATION_AFECALIBRATION_H_ */
