/*
 * AfeSim.c
 *
 *  Created on: 2024Äê1ÔÂ27ÈÕ
 *      Author: Administrator
 */

#include "AfeSim.h"

void AfeSim_Init(void)
{
	AfeSim_HAL_Init();
	AfeSim_DeviceInit();
#if 0
	AfeSim_BQ79600_Init();
	AfeSim_BQ79xxx_Init();
	AfeSim_BQ79616_Init();
	AfeSim_BQ79718_Init();
#endif
}

void AfeSim_MainFunction(void)
{

}
