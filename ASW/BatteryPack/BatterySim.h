/*
 * BatterySim.h
 *
 *  Created on: 2024��1��27��
 *      Author: Administrator
 */

#ifndef SRC_APPLICATION_BATTERYPACK_BATTERYSIM_H_
#define SRC_APPLICATION_BATTERYPACK_BATTERYSIM_H_

#include "ZYNQ_BasicType.h"
#include "RTE.h"
#include "AfeCalibration.h"

#define BATSIM_TABLE_MAX_LEN		32

#define BAT_MAX_NUM		(18*15) /*270*/

#define BAT_MAX_mAH		550*1000
#define BAT_MIN_mAH		100

/* ��ǰÿ50ms����һ��SOC */
#define SIM_BASE_ms			100
#define TIM_BASE_ms			1000
#define SIM_BASE_FACTOR		((float)SIM_BASE_ms / (float)TIM_BASE_ms)

#define BAT_SOC_AH(x)		(sint32)(((sint32)x) * (sint32)1000 * (u32)3600)
#define BAT_mAH(x)			(sint32)(((sint32)x) * (sint32)3600)
#define BAT_AH_LOSS(x)		(sint32)((((float)x) * 1000 * (float)SIM_BASE_ms) / (float)TIM_BASE_ms)
#define BAT_mAH_LOSS(x)		(sint32)((((float)x) * SIM_BASE_FACTOR))

typedef struct
{
	sint32 soc_mC;				/* SOC, ��λ ������ */
	sint32 res_uR;				/* ����, ��λ ΢ŷ */
	sint32 volt_100uV;			/* ��ѹ, ��λ 100΢�� */
	sint32 volt_bias_100uV;		/* ����ADC����ƫ��ĵ�ѹ����ƫ����������ģ���ѹ���� */
	sint32 currentExtra_uA;		/* ����������, ���Թ���/����/оƬ����, ��λ ΢�� */
	sint32 heat_mJ;				/* ��о����, ��λ ������ */
	sint32 mass_mg;				/* ��о����, ��λ ���� */
	sint32 temp_mK;				/* ��о�¶�, ��λ ��������*/
}BatteryType;

typedef struct
{
	BatteryType* bat;
	uint16 maxBatNum;
}BatteryManagerType;

extern sint32 Cust_SocTableX[BATSIM_TABLE_MAX_LEN];
extern sint32 Cust_SocTableY[BATSIM_TABLE_MAX_LEN];
extern sint32 LFP_SocTableX[BATSIM_TABLE_MAX_LEN];
extern sint32 LFP_SocTableY[BATSIM_TABLE_MAX_LEN];
extern sint32 NCM_SocTableX[BATSIM_TABLE_MAX_LEN];
extern sint32 NCM_SocTableY[BATSIM_TABLE_MAX_LEN];
extern sint32 (*BatSim_SoxTableX)[BATSIM_TABLE_MAX_LEN];
extern sint32 (*BatSim_SoxTableY)[BATSIM_TABLE_MAX_LEN];

extern BatteryManagerType* const BatMan;
extern BatteryType Bat[BAT_MAX_NUM];
extern sint32 ModuleTemp_mC[AFESIM_MAX_AFE_NUM+1][AFESIM_MAX_GPIO_NUM];
extern sint32 BalanceTemp_mC[AFESIM_MAX_AFE_NUM+1][AFESIM_MAX_GPIO_NUM];
extern sint32 DieTemp_mC[AFESIM_MAX_AFE_NUM+1][AFESIM_MAX_DT_NUM];
extern sint32 ShuntTemp_mC[AFESIM_MAX_ST_NUM];
extern sint32 AirTemp_mC;
extern sint32 PackTemp_mC;

void BatterySim_Init(void);
void BatterySim_NormalRun(void);

#endif /* SRC_APPLICATION_BATTERYPACK_BATTERYSIM_H_ */
