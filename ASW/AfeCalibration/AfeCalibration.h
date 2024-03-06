/*
 * AfeCalibration.h
 *
 *  Created on: 2024��2��4��
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

/*����AFE����*/

/*�궨��оSOX,��ӱ궨��ѹ*/
#define AFECALIB_CELLSOX_OFFSET						0u
#define AFECALIB_CELLSOX_NUM						BAT_MAX_NUM

/*ֱ�ӱ궨��о��ѹ, ��ᵼ��SOX������ͣ*/
#define AFECALIB_CELLVOLT_OFFSET					(AFECALIB_CELLSOX_OFFSET + AFECALIB_CELLSOX_NUM)
#define AFECALIB_CELLVOLT_NUM						BAT_MAX_NUM

/*�궨��о��ѹADC����ƫ����*/
#define AFECALIB_CELLVOLT_BIAS_OFFSET				(AFECALIB_CELLVOLT_OFFSET + AFECALIB_CELLVOLT_NUM)
#define AFECALIB_CELLVOLT_BIAS_NUM					BAT_MAX_NUM

/*ֱ�ӱ궨ģ���ѹ, ��ᵼ��SOX������ͣ*/
#define AFECALIB_MODULEVOLT_OFFSET					(AFECALIB_CELLVOLT_BIAS_OFFSET + AFECALIB_CELLVOLT_BIAS_NUM)
#define AFECALIB_MODULEVOLT_NUM						(AFESIM_MAX_AFE_NUM + 1u)

/*�궨ģ���ѹADC����ƫ����*/
#define AFECALIB_MODULEVOLT_BIAS_OFFSET				(AFECALIB_MODULEVOLT_OFFSET + AFECALIB_MODULEVOLT_NUM)
#define AFECALIB_MODULEVOLT_BIAS_NUM				(AFESIM_MAX_AFE_NUM + 1u)

/*ֱ�ӱ궨GPIO��ѹ, ��ᵼ���ȷ�����ͣ*/
#define AFECALIB_GPIOVOLT_OFFSET					(AFECALIB_MODULEVOLT_BIAS_OFFSET + AFECALIB_MODULEVOLT_BIAS_NUM)
#define AFECALIB_GPIOVOLT_NUM						(AFESIM_MAX_GPIO_NUM * (AFESIM_MAX_AFE_NUM + 1u))

/*�궨GPIO��ѹ����ƫ����*/
#define AFECALIB_GPIOVOLT_BIAS_OFFSET				(AFECALIB_GPIOVOLT_OFFSET + AFECALIB_GPIOVOLT_NUM)
#define AFECALIB_GPIOVOLT_BIAS_NUM					(AFESIM_MAX_GPIO_NUM * (AFESIM_MAX_AFE_NUM + 1u))

/*�궨TSREF��ѹ*/
#define AFECALIB_TSREF_OFFSET						(AFECALIB_GPIOVOLT_BIAS_OFFSET + AFECALIB_GPIOVOLT_BIAS_NUM)
#define AFECALIB_TSREF_NUM							(AFESIM_MAX_AFE_NUM + 1u)

/*�궨TSREF ADC������ѹ*/
#define AFECALIB_TSREF_BIAS_OFFSET					(AFECALIB_TSREF_OFFSET + AFECALIB_TSREF_NUM)
#define AFECALIB_TSREF_BIAS_NUM						(AFESIM_MAX_AFE_NUM + 1u)

/*�궨������ѹ*/
#define AFECALIB_AUXVOLT_SINGLE_CHIP_NUM			(20)
#define AFECALIB_AUXVOLT_OFFSET						(AFECALIB_TSREF_BIAS_OFFSET + AFECALIB_TSREF_BIAS_NUM)
#define AFECALIB_AUXVOLT_NUM						((AFESIM_MAX_AFE_NUM + 1u) * AFECALIB_AUXVOLT_SINGLE_CHIP_NUM)

/*�궨������ѹADC����ƫ��*/
#define AFECALIB_AUXVOLT_BIAS_OFFSET				(AFECALIB_AUXVOLT_OFFSET + AFECALIB_AUXVOLT_NUM)
#define AFECALIB_AUXVOLT_BIAS_NUM					((AFESIM_MAX_AFE_NUM + 1u) * AFECALIB_AUXVOLT_SINGLE_CHIP_NUM)

/*�궨ģ���¶�*/
#define AFECALIB_MODULETEMP_OFFSET					(AFECALIB_AUXVOLT_BIAS_OFFSET + AFECALIB_AUXVOLT_BIAS_NUM)
#define AFECALIB_MODULETEMP_NUM						(AFESIM_MAX_GPIO_NUM * (AFESIM_MAX_AFE_NUM + 1u))

/*�궨�����¶�*/
#define AFECALIB_BALANCETEMP_OFFSET					(AFECALIB_MODULETEMP_OFFSET + AFECALIB_MODULETEMP_NUM)
#define AFECALIB_BALANCETEMP_NUM					(AFESIM_MAX_GPIO_NUM * (AFESIM_MAX_AFE_NUM + 1u))

/*�궨оƬ�¶�*/
#define AFECALIB_DIETEMP_OFFSET						(AFECALIB_BALANCETEMP_OFFSET + AFECALIB_BALANCETEMP_NUM)
#define AFECALIB_DIETEMP_NUM						(AFESIM_MAX_DT_NUM * (AFESIM_MAX_AFE_NUM + 1u))

/*�궨Shunt�¶ȣ�BQ79758����*/
#define AFECALIB_SHUNTTEMP_OFFSET					(AFECALIB_DIETEMP_OFFSET + AFECALIB_DIETEMP_NUM)
#define AFECALIB_SHUNTTEMP_NUM						AFESIM_MAX_ST_NUM

/*�궨Pack�¶�*/
#define AFECALIB_PACKTEMP_OFFSET					(AFECALIB_SHUNTTEMP_OFFSET + AFECALIB_SHUNTTEMP_NUM)
#define AFECALIB_PACKTEMP_NUM						(1u)
/*�궨�����¶�*/
#define AFECALIB_AIRTEMP_OFFSET						(AFECALIB_PACKTEMP_OFFSET + AFECALIB_PACKTEMP_NUM)
#define AFECALIB_AIRTEMP_NUM						(1u)

#define AFECALIB_1stCURRENT_OFFSET					(AFECALIB_AIRTEMP_OFFSET + AFECALIB_AIRTEMP_NUM)
#define AFECALIB_1stCURRENT_NUM						(1u)
#define AFECALIB_2ndCURRENT_OFFSET					(AFECALIB_1stCURRENT_OFFSET + AFECALIB_1stCURRENT_NUM)
#define AFECALIB_2ndCURRENT_NUM						(1u)

/*������������궨*/
#define AFECALIB_PACK_HEAT_SINK_FACTOR_OFFSET		(AFECALIB_2ndCURRENT_OFFSET + AFECALIB_2ndCURRENT_NUM)
#define AFECALIB_MODULE_HEAT_SINK_FACTOR_OFFSET		(AFECALIB_MODULE_HEAT_SINK_FACTOR_OFFSET + 1u)
#define AFECALIB_PCB_HEAT_SINK_FACTOR_OFFSET		(AFECALIB_MODULE_HEAT_SINK_FACTOR_OFFSET + 1u)

/*���ϱ궨*/

/*��Դ���ϲ���*/
#define AFECALIB_PWR_FAULT_OFFSET
#define AFECALIB_AVDD_FAULT_OFFSET
#define AFECALIB_DVDD_FAULT_OFFSET
#define AFECALIB_CVDD_FAULT_OFFSET
/*��϶�ο���ѹ*/
#define AFECALIB_VBGREF_FAULT_OFFSET
/*ADC�ο���ѹ*/
#define AFECALIB_VREFH_FAULT_OFFSET

/*ͨ�Ź��ϲ���*/
/*0����79600������AFEX���ϣ�1����79600������AFE1���ϣ�2����AFE1��AFE2���ϣ��Դ�����*/
#define AFECALIB_DAISYCHAIN_DISABLE_OFFSET			10000
#define AFECALIB_DAISYCHAIN_DISABLE_NUM				((AFESIM_MAX_AFE_NUM + 1u))
#define AFECALIB_DAISYCHAIN_NOISE_OFFSET			(AFECALIB_DAISYCHAIN_DISABLE_OFFSET + AFECALIB_DAISYCHAIN_DISABLE_NUM)
#define AFECALIB_DAISYCHAIN_NOISE_NUM				(1u)
/*0����79600���ϣ�1����AFE1���ϣ��Դ�����*/
#define AFECALIB_DEVICE_DISABLE_OFFSET				(AFECALIB_DAISYCHAIN_NOISE_OFFSET + AFECALIB_DAISYCHAIN_NOISE_NUM)
#define AFECALIB_DEVICE_DISABLE_NUM					(AFESIM_MAX_AFE_NUM + 1u)

/*�������ϲ���*/
/*��ͨ�˲�������*/
#define AFECALIB_ADC_LFP_FAULT_OFFSET


/*��·���ϲ���*/
#define AFECALIB_VCCB_SHORT_FAULT_OFFSET
#define AFECALIB_VC_OPEN_FAULT_OFFSET
#define AFECALIB_CB_OPEN_FAULT_OFFSET
#define AFECALIB_CBFET_FAULT_OFFSET
#define AFECALIB_GPIO_OPEN_FAULT_OFFSET
#define AFECALIB_GPIO_ADJSHORT_FAULT_OFFSET
#define AFECALIB_GPIO_COMP_FAULT_OFFSET
#define AFECALIB_BUSBAR_FAULT_OFFSET


/*��Ϲ��ϲ���*/
/*����ע�����*/
#define AFECALIB_FAULTINJBIST_FAULT_OFFSET
/*��ϱȽ����쳣�˳�����*/
#define AFECALIB_ADC_COMP_ABORT_OFFSET
#define AFECALIB_CRCFLIP_FAULT_OFFSET
#define AFECALIB_OVDAC_FAULT_OFFSET

#if (AFECALIB_PCB_HEAT_SINK_FACTOR_OFFSET) >= AFECALIB_MAX_CALIB_NUM
#error "Fuck AfeCalib Array Overflow !!!"
#endif

/* CAN���ı궨, ����ID���� */
typedef enum
{
	/* AFE���������Ʊ궨 */

		/* 000 ����궨�� */
		CAN_CALIB_ClearCalib_MSGID = 0x000,
		/* 001 AFE������� */
		CAN_CALIB_AfeSimControl_MSGID,
		/* 002 ��ʵAFE�͵�о���� */
		CAN_CALIB_RealAfeCellNum_MSGID,
		/* 003 AFE����ѡ�� */
		CAN_CALIB_AfeUseType_MSGID,
		/* 004 AFE���ͱ궨 */
		CAN_CALIB_AfeChipType_MSGID,

	/* Ӳ���궨 */

		/* 010 ģ���¶�λ�ñ궨 */
		CAN_CALIB_ModuleTempPos_MSGID = 0x010,
		/* 011 �����¶�λ�ñ궨 */
		CAN_CALIB_BalTempPos_MSGID,
		/* 012 Shunt�¶�λ�ñ궨 */
		CAN_CALIB_ShuntTempPos_MSGID,
		/* 013 Shunt�ڶ�����������λ�ñ궨 */
		CAN_CALIB_GpioCurrentPos_MSGID,
		/* 014 �ⲿ����SOX���ұ� */
		CAN_CALIB_SOXCurve_MSGID,
		/* 015 �ⲿ����ģ���¶Ȳ��ұ� */
		CAN_CALIB_ModuleTempRT_MSGID,
		/* 016 �ⲿ��������¶Ȳ��ұ� */
		CAN_CALIB_BalanceTempRT_MSGID,
		/* 017 �ⲿ����Shunt�¶Ȳ��ұ� */
		CAN_CALIB_ShuntTempRT_MSGID,

	/* ͨ�ű궨 */

		/* 020 �ջ���ͨ�ϱ궨 */
		CAN_CALIB_DaisyChainBroken_MSGID = 0x020,
		/* 021 �ջ��������궨 */
		CAN_CALIB_DaisyChainNoise_MSGID,
		/* 022 оƬ��ٱ궨 */
		CAN_CALIB_DeviceBroken_MSGID,
		/* 023 ��ѹ�������� */
		CAN_CALIB_SampleNoise_MSGID,

	/* ��ѹ�궨 */

		/* 030 ��оSOX�궨 */
		CAN_CALIB_CellSOX_MSGID = 0x030,
		/* 031 ��о��ѹ�궨, ע��, �궨��ѹ��Ҫ��ͣSOX���� */
		CAN_CALIB_CellVolt_MSGID,
		/* 032 ��о��ѹƫ�����궨, ������ͣSOX���� */
		CAN_CALIB_CellVoltBias_MSGID,

		/* 033 GPIO��ѹ�궨 */
		CAN_CALIB_GpioVolt_MSGID,
		/* 034 GPIO��ѹƫ�Ʊ궨 */
		CAN_CALIB_GpioVoltBias_MSGID,
		/* 035 ģ���ѹ�궨 */
		CAN_CALIB_ModuleVolt_MSGID,
		/* 036 ģ���ѹƫ�Ʊ궨 */
		CAN_CALIB_ModuleVoltBias_MSGID,
		/* 037 TSREF�궨 */
		CAN_CALIB_TSREF_MSGID,
		/* 038 TSREFƫ��궨 */
		CAN_CALIB_TSREFBias_MSGID,
		/* 039 ������ѹ�궨 */
		CAN_CALIB_AuxVolt_MSGID,
		/* 03A ������ѹ�궨 */
		CAN_CALIB_AuxVoltBias_MSGID,

	/* �¶ȱ궨 */

		/* 040 ģ���¶ȱ궨 */
		CAN_CALIB_ModuleTemp_MSGID = 0x040,
		/* 041 �����¶ȱ궨 */
		CAN_CALIB_BalTemp_MSGID,
		/* 042 оƬ�¶ȱ궨 */
		CAN_CALIB_DieTemp_MSGID,
		/* 043 Shunt�¶ȱ궨 */
		CAN_CALIB_ShuntTemp_MSGID,
		/* 044 Pack�¶ȱ궨 */
		CAN_CALIB_PackTemp_MSGID,
		/* 045 �����¶ȱ궨 */
		CAN_CALIB_AirTemp_MSGID,

	/* �����궨 */
		/* 050 ��һ�����궨 */
		CAN_CALIB_1stCurrent_MSGID = 0x050,
		/* 051 �ڶ�����ƫ�����궨 */
		CAN_CALIB_2ndCurrentBias_MSGID,

	/* ���ܰ�ȫ�궨 */
		/* 060 79616�ϵ��Լ����ע��*/
		CAN_CALIB_BQ79616_DiagBistFaultInject_MSGID = 0x060,
		/* 061 79616�ϵ���Ϲ���ע��*/
		CAN_CALIB_BQ79616_DiagKeyOnFaultInject_MSGID = 0x061,
		/* 062 79616������Ϲ���ע��*/
		CAN_CALIB_BQ79616_DiagCycleFaultInject_MSGID = 0x062,
		
	/* �����ѱ궨 */

	/* ����Ĵ����궨 */
		/* 0E0 A�� ����Ĵ�����ȡ */
		CAN_CALIB_RandomRegReadA_MSGID = 0x0E0,

		/* 0F0 A�� ����Ĵ����궨 */
		CAN_CALIB_RandomRegWriteA_MSGID = 0x0F0,
		/* 0F1 B�� ����Ĵ����궨 */
		CAN_CALIB_RandomRegWriteB_MSGID,
		/* 0F2 C�� ����Ĵ����궨 ����֡ */
		CAN_CALIB_RandomRegWriteC1_MSGID,
		/* 0F3 C�� ����Ĵ����궨 ����֡ */
		CAN_CALIB_RandomRegWriteC2_MSGID,

	/* ��� */
		/* ͨ����� */
		CAN_OUT_DaisyChainBroken_MSGID = 0x100,
		CAN_OUT_DeviceBroken_MSGID,
		CAN_OUT_DeviceStackRole_MSGID,
		CAN_OUT_DeviceStackID_MSGID,
		CAN_OUT_DaisyChainNoise_MSGID,

		/*���������*/
		CAN_OUT_CellVolt_MSGID = 0x200,		/* 15AFE 18ͨ��, 5*15���� */
		CAN_OUT_ModuleVolt_MSGID = 0x250,	/* 15AFE */
		CAN_OUT_GpioVolt_MSGID = 0x260,   	/* 15AFE 11ͨ��, 3*15���� */
		CAN_OUT_AuxVolt_MSGID = 0x290,
		CAN_OUT_ModuleTemp_MSGID = 0x300, 	/* 15AFE 6ͨ��, 2*15���� */
		CAN_OUT_BalanceTemp_MSGID = 0x320, 	/* 15AFE 2ͨ��, 1*15���� */
		CAN_OUT_DieTemp_MSGID = 0x330, 		/*15AFE 2ͨ��, 1*15���� */
		CAN_OUT_Shunt_MSGID = 0x340,
		CAN_OUT_PackAir_MSGID,

		
		/*�Ĵ������*/
		CAN_OUT_RandomReg_MSGID = 0x400,
		
		/*���ܰ�ȫ���*/

}CAN_CALIB_MSGID_Type;

/* ����ID 000�����ָ���ı궨�� */
typedef enum
{
	CAN_CALIB_ClearCalib_B0_Ignore = 0x00,		/* ���Դ˱��� */
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
	CAN_CALIB_ClearCalib_B0_PackTemp,			/* �ڲ������¶� */
	CAN_CALIB_ClearCalib_B0_AirTemp,			/* ��绷���¶� */
	CAN_CALIB_ClearCalib_B0_AllTemp,

	CAN_CALIB_ClearCalib_B0_Current = 0x40,		/* ONLY BQ79656, BQ79758 */

	CAN_CALIB_ClearFunctionSafety_B0_All = 0x50,
}CAN_CALIB_ClearCalib_Type;

/* ����ID 001��ʹ�ܻ���ָͣ���ķ��� */
typedef enum
{
	CAN_CALIB_AfeSimControl_B0_Ignore = 0x00,	/* ���Դ˱��� */
	CAN_CALIB_AfeSimControl_B0_Reset,			/* ���з���������ֵ */
	CAN_CALIB_AfeSimControl_B0_StartAll,		/* ����ȫ���� */
	CAN_CALIB_AfeSimControl_B0_PauseAll,
	CAN_CALIB_AfeSimControl_B0_StartSOX,		/* ��о��ѹ������������仯���� */
	CAN_CALIB_AfeSimControl_B0_PauseSOX,
	CAN_CALIB_AfeSimControl_B0_StartHeat,		/* ģ���¶ȡ�Pack�¶�������仯���� */
	CAN_CALIB_AfeSimControl_B0_PauseHeat,
	CAN_CALIB_AfeSimControl_B0_StartBal,		/* �����ѹ���¶������ָ��仯���� */
	CAN_CALIB_AfeSimControl_B0_PauseBal,
	CAN_CALIB_AfeSimControl_B0_StartGpio,		/* GPIO��ѹ���¶ȱ仯���� */
	CAN_CALIB_AfeSimControl_B0_PauseGpio,
	CAN_CALIB_AfeSimControl_B0_StartReg,		/* �Ĵ���ֵ���������仯���� */
	CAN_CALIB_AfeSimControl_B0_PauseReg,

	CAN_CALIB_AfeSimControl_B0_SOXuseCust = 0x80,	/* ʹ���ⲿSOX�� */
	CAN_CALIB_AfeSimControl_B0_SOXuseNCM ,		/* ʹ��Ĭ����Ԫ﮵��SOX�� */
	CAN_CALIB_AfeSimControl_B0_SOXuseLFP,				/* ʹ��Ĭ��������﮵��SOX�� */
	

	CAN_CALIB_AfeSimControl_B0_ModuleTempUseCust = 0x90,	/* ʹ���ⲿģ���¶�NTC RT ��*/
	CAN_CALIB_AfeSimControl_B0_ModuleTempUseDefault1,		/* ʹ��Ĭ��ģ���¶�NTC RT �� */

	CAN_CALIB_AfeSimControl_B0_BalTempUseCust = 0xA0,		/* ʹ���ⲿ�����¶�NTC RT ��*/
	CAN_CALIB_AfeSimControl_B0_BalTempUseDefault1,			/* ʹ��Ĭ�Ͼ����¶�NTC RT �� */

	CAN_CALIB_AfeSimControl_B0_ShuntTempUseCust = 0xB0,		/* ʹ���ⲿShunt�¶�NTC RT ��*/
	CAN_CALIB_AfeSimControl_B0_ShuntTempUseDefault1,			/* ʹ��Ĭ��Shunt�¶�NTC RT �� */
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
