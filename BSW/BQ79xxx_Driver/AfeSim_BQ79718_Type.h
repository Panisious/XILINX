/*
 * AfeSim_BQ79718_Type.h
 *
 *  Created on: 2024�??1�??7�??
 *      Author: Administrator
 */

#ifndef SRC_AFESIM_BQ79718_TYPE_H_
#define SRC_AFESIM_BQ79718_TYPE_H_

#include "AfeSim_Device_Type.h"
#include "AfeSim_BQ79718_Cfg.h"


typedef enum
{
	AFESIM_BQ79718_GPIO_OFF,
	AFESIM_BQ79718_GPIO_ADC_INPUT
}AfeSim_BQ79718_GpioModeType;

typedef struct
{
	/* Physical Parameters */
	sint32 cellVolt_100uV;
	sint32 cellVoltBias_100uV;			/* 鍙洿鎺ュ啓鍏ヨ姱鐗囧瘎瀛樺櫒鐨勭數鍘嬪�� */
	cplx32 internalImpedence_Z;
	cplx32 lineImpedence_mZ;
	cplx32 resistor_Z;

	/* Diagnostic Parameters */
	uint32 pinCapacitor_pF;
	uint16 outerSinkCurrent_nA;
	uint16 internalSinkCurrent_nA;

	/* Connectivity Parameters */
	uint8 isBroken;
	uint8 isFloating;
	uint8 shortTo[2];
}AfeSim_BQ79718_VCPin_Model;

typedef struct
{
	/* Physical Parameters */
	cplx32 internalImpedence_Z;		/*内阻*/
	cplx32 lineImpedence_Z;			/*均衡线路阻抗*/
	cplx32 cbResistorImpedence_Z;	/*均衡电阻*/

	/* Diagnostic Parameters */
	uint32 pinCapacitor_pF;			/*端口电容*/
	uint16 sinkCurrent_nA;			/*下拉电流�??*/

	uint8 isBroken;					/*是否彻底损坏*/
	uint8 isFloating;				/*浮空虚焊*/
	uint8 shortTo[2];				/*短接到相邻pin*/
}AfeSim_BQ79718_CBPin_Model;

typedef struct
{
	/* Physical Parameters */
	uint16 switchOnTime_us;
	uint16 switchOffTime_us;
	cplx32 offImpedence_Z;
	cplx32 onImpedence_Z;

	/* Logic Parameters */
	uint8 isBroken;
	uint8 isOn;

}AfeSim_BQ79718_CBFET_Model;

typedef struct
{
	sint32 maxVoltage_uV;
	sint32 inputVoltage_100uV;
	sint32 inputVoltageBias_100uV;
	sint32 outputVoltage_uV;
	cplx32 impedence_Z;

	uint8 pullType;
	uint32 pullUpRes;
	uint32 pullDownRes;
	uint8 outputLevel;
	AfeSim_BQ79718_GpioModeType mode;	/* GPIO ģʽ�����Ҫ��? */
	uint8 isBroken;					/*是否彻底损坏*/
	uint8 isFloating;				/*浮空虚焊*/
	uint8 shortTo[2];				/*短接到相邻pin*/

	AfeSim_DeviceGpioUseType useType;
}AfeSim_BQ79xxx_GPIO_Model;

typedef struct
{
	/*share*/
	sint32 VBAT_mV;
	sint32 VBAT_bias_mV;

	sint32 NPNB_100uV;
	sint32 LDOIN_100uV;

	sint32 CVDD_100uV;	/*FAULT_PWR1 - bit4-3*/
	sint32 CVSS_100uV;	/*FAULT_PWR1 - bit7*/

	sint32 AVDD_100uV;	/*FAULT_PWR1 - bit1-0, FAULT_PWR3 - bit0 */
	sint32 AVSS_100uV;

	sint32 DVDD_100uV;	/*FAULT_PWR1 - bit2*/
	sint32 DVSS_100uV;	/*FAULT_PWR1 - bit6*/

	sint32 TSREF_100uV;	
	sint32 TSREFbias_100uV; /*FAULT_PWR2 - bit2-0,
		79616 = 4.975 - 5.025 V, 79718=40000*/
	
	
	/*BQ796xx*/
	sint32 NEG5V_100uV;	/*FAULT_PWR2 - bit3*/
	sint32 REFHM_100uV;	/*FAULT_PWR1 - bit5, FAULT_PWR2 - bit4 */
	sint32 REFHP_100uV;
	sint32 Aux_CELL_100uV;
	sint32 Aux_GPIO_100uV;
	sint32 Aux_BAT_mV;
	sint32 Aux_REFL_100uV;			/* 1.092 - 1.106 V */
	sint32 Aux_VBG2_100uV;			/* 1.092 - 1.106 V */
	sint32 Aux_AVAO_REF_100uV;		/* 2.400 - 2.550 V */
	sint32 Aux_AVDD_REF_100uV;  	/* 2.400 - 2.500 V */
	sint32 Aux_OVDAC_100uV;			/* 4.450 - 4.500 V */
	sint32 Aux_UVDAC_100uV;			/* 3.095 - 3.150 V */
	sint32 Aux_OT_OTCB_DAC_100uV;	/* 1.900 - 2.000 V */
	sint32 Aux_UTDAC_100uV;			/* 3.950 - 4.050 V */
	sint32 Aux_VCBDONE_DAC_100uV; 	/* 3.950 - 4.050 V */
	sint32 Aux_VCM_100uV;			/* 2.400 - 2.550 V */
	/*BQ797xx*/
	sint32 VActCellSum_mV;

	/* Logic */
	uint8 npnbIsOn;
	uint8 tsrefIsOn;

	AfeSim_DevicePowerType powerState;
}AfeSim_ChipPower_Model;

typedef struct
{
	uint8 mode;
	uint16 defaultVaule;
}AfeSim_BQ79718_ADC_Model;

typedef struct
{
	uint32 ovThresh_mV;
	uint32 uvThresh_mV;
	uint32 otThresh_mV;
	uint32 utThresh_mV;

	uint32 uvDisableMsk;

}AfeSim_PROT_Model;

typedef enum
{
	AFESIM_BQ79616_DIAG_PWR_BIST = 0,
	AFESIM_BQ79616_DIAG_VCCB_FAULT_INJ_BIST,
	AFESIM_BQ79616_DIAG_VCOW_FAULT_INJ_BIST,
	AFESIM_BQ79616_DIAG_CBOW_FAULT_INJ_BIST,
	AFESIM_BQ79616_DIAG_CBFET_FAULT_INJ_BIST,
	AFESIM_BQ79616_DIAG_GPIO_FAULT_INJ_BIST,
	AFESIM_BQ79616_DIAG_LPF_FAULT_INJ_BIST,
	AFESIM_BQ79616_DIAG_CRC_FLIP_FAULT,
	AFESIM_BQ79616_DIAG_VCM_FAULT,
	AFESIM_DIAG_BIST_TYPE_NUM,
}AfeSim_DiagBistType;

typedef enum
{
	AFESIM_BQ79616_DIAG_VCCB_FAULT,
	AFESIM_BQ79616_DIAG_VCOW_FAULT,
	AFESIM_BQ79616_DIAG_CBOW_FAULT,
	AFESIM_BQ79616_DIAG_CBFET_FAULT,
	AFESIM_BQ79616_DIAG_GPIO_FAULT,
	AFESIM_DIAG_CYCLE_TYPE_NUM,
}AfeSim_DiagType;

typedef struct
{
	uint32 bist_WillFail[16];		/* Interrupt From Calibration, make one dev one type bist fail */
	AfeSim_DiagBistType type;
	uint8 step;					/* In Case If Some Bist Need Time to Finish */

	/* BQ79616 */
}AfeSim_DiagBistModel;

typedef struct
{
	uint8 diagEnable;
	uint32 diag_WillFail[16];
	AfeSim_DiagType type;
	uint8 step;
	/* BQ79616 */
}AfeSim_DiagNormalModel;

typedef struct
{
	uint8 diagIsBist;
	AfeSim_DiagBistModel bist;
	AfeSim_DiagNormalModel cycle;
	
}AfeSim_DiagModel;

typedef enum
{
	AFESIM_NOISE_TYPE_CELL_VOLT,
	AFESIM_NOISE_TYPE_MODULE_VOLT,
	AFESIM_NOISE_TYPE_GPIO_VOLT,
	AFESIM_NOISE_TYPE_TSREF_VOLT,
	AFESIM_NOISE_TYPE_INTERNAL_VOLT,
	AFESIM_NOISE_TYPE_DIETEMP,
	AFESIM_NOISE_TYPE_NUM
}AfeSim_NoiseType;

typedef struct
{
	uint16 volt[16];
}AfeSim_NoiseModel;

typedef struct
{
	uint8 busbar_enable;
	uint8 bbp_loc;
	uint8 bbvc_enable;
	uint16 bbvc_posn;
}AfeSim_BusbarModel;

/*此manager�??要同时�?�虑BQ79616和BQ79718*/
typedef struct AfeSim_DeviceManangerType_Tag
{
	/* 79600, 79616, 79656, 79718, 79758�?? 79631, 79731 */
	AfeSim_AfeUseType afeType;

	/* 设备ID�?? �??/栈顶 */
	AfeSim_DeviceInfomationType info;

	/* 菊花链设备（链表�?? */
	struct AfeSim_DeviceManagerType_Tag * comhDev;
	struct AfeSim_DeviceManagerType_Tag * comlDev;

	/* 菊花链�?�信物理模型  */
	AfeSim_DaisyChain_Model comh;
	AfeSim_DaisyChain_Model coml;

	/* 菊花链�?�信数据 */
	AfeSim_CommDataType commData;

	uint8 * reg;

	/* 电芯数量配置 */
	uint8 activeCell;
	uint8 maxCell;

	/* 电源仿真模型 */
	AfeSim_ChipPower_Model power;

	/* VCCB Pin 仿真模型*/
	AfeSim_BQ79718_VCPin_Model vc[AFESIM_BQ79718_VC_NUM];
	AfeSim_BQ79718_CBPin_Model cb[AFESIM_BQ79718_CB_NUM];

	/* CBFET 仿真模型 */
	AfeSim_BQ79718_CBFET_Model cbfet[AFESIM_BQ79718_CELL_NUM];

	/* GPIO Pin 仿真模型 */
	AfeSim_BQ79xxx_GPIO_Model gpio[AFESIM_BQ79718_GPIO_NUM];

	/* ADC ����ģ�ͣ�BQ79718��ȫ����BQ79616�õ�1����*/
	AfeSim_BQ79718_ADC_Model adc[AFESIM_BQ79718_CELL_NUM];

	/* BQ79616 Aux ADC */
	u8 auxAdc;

	/* CS ADC仿真模型 */
	u8 csAdc;

	/* 滤波器仿真模�?? */
	u8 lpf;

	/* 功能安全模型 */
	AfeSim_PROT_Model prot;

	AfeSim_DiagModel diag;

	AfeSim_NoiseModel noise;

	AfeSim_BusbarModel busbar;
}AfeSim_DeviceManangerType;


#endif /* SRC_AFESIM_BQ79718_TYPE_H_ */
