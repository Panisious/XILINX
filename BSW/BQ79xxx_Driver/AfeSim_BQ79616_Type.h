/*
 * AfeSim_BQ79616_Type.h
 *
 *  Created on: 2024��1��7��
 *      Author: Administrator
 */

#ifndef SRC_AFESIM_BQ79616_TYPE_H_
#define SRC_AFESIM_BQ79616_TYPE_H_

#include "AfeSim_Device_Type.h"



typedef enum
{
	AFESIM_BQ79616_AUX_CELL = 0,
	AFESIM_BQ79616_AUX_GPIO = 1,
	AFESIM_BQ79616_AUX_BAT = 2,
	AFESIM_BQ79616_AUX_REFL = 3,
	AFESIM_BQ79616_AUX_VBG2 = 4,
	AFESIM_BQ79616_AUX_AVAO_REF = 5,
	AFESIM_BQ79616_AUX_AVDD_REF = 6,
	AFESIM_BQ79616_AUX_OV_DAC = 7,
	AFESIM_BQ79616_AUX_UV_DAC = 8,
	AFESIM_BQ79616_AUX_OT_OTCB_DAC = 9,
	AFESIM_BQ79616_AUX_UT_DAC = 10,
	AFESIM_BQ79616_AUX_VCBDONE_DAC = 11,
	AFESIM_BQ79616_AUX_VCM = 12,
	AFESIM_BQ79616_AUX_MAXNUM = 13,
}AfeSim_BQ79616_AuxVoltMuxType;




#endif /* SRC_AFESIM_BQ79616_TYPE_H_ */
