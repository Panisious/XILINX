/*
 * AfeDeviceSim.h
 *
 *  Created on: 2024年1月27日
 *      Author: Administrator
 */

#include "RTE.h"
#include "AfeCalibration.h"
#include "AfeSim_BQ79600.h"
#include "AfeSim_BQ79616.h"
#include "AfeSim_BQ79718.h"

#define AFEDEVSIM_TEMP_RT_TABLE_SIZE		32

#define AFEDEVSIM_NOISE_LOW		3
#define AFEDEVSIM_NOISE_MID		5
#define AFEDEVSIM_NOISE_HIGH	10
#define AFEDEVSIM_NOISE(x)		((rand()%(2*x)) - x)

extern sint32 ModuleNTCtable_temp_mC_Default1[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 ModuleNTCtable_volt_Default1[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 ModuleNTCtable_temp_mC_Cust[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 ModuleNTCtable_volt_Cust[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 (*ModuleNTCtable_temp_mC)[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 (*ModuleNTCtable_volt)[AFEDEVSIM_TEMP_RT_TABLE_SIZE];

extern sint32 BalanceNTCtable_temp_mC_Default1[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 BalanceNTCtable_volt_Default1[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 BalanceNTCtable_temp_mC_Cust[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 BalanceNTCtable_volt_Cust[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 (*BalanceNTCtable_temp_mC)[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 (*BalanceNTCtable_volt)[AFEDEVSIM_TEMP_RT_TABLE_SIZE];

extern sint32 ShuntNTCtable_temp_mC_Default1[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 ShuntNTCtable_volt_Default1[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 ShuntNTCtable_temp_mC_Cust[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 ShuntNTCtable_volt_Cust[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 (*ShuntNTCtable_temp_mC)[AFEDEVSIM_TEMP_RT_TABLE_SIZE];
extern sint32 (*ShuntNTCtable_volt)[AFEDEVSIM_TEMP_RT_TABLE_SIZE];

extern sint32 AfeDeviceSim_ModuleVolt_mV[AFESIM_MAX_AFE_NUM];
extern sint32 AfeDeviceSim_VCellActSum_mV[AFESIM_MAX_AFE_NUM];
/* 采样 诊断 均衡 */

extern void AfeDeviceSim_Run100ms(void);
extern void AfeDeviceSim_Run50ms(void);
extern void AfeDeviceSim_Run20ms(void);
extern void AfeDeviceSim_Run10ms(void);
