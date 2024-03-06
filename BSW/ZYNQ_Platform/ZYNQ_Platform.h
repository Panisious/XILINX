/*
 * AfeSim_ZYNQ_Platform.h
 *
 *  Created on: 2024Äê1ÔÂ7ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_AFESIM_ZYNQ_PLATFORM_H_
#define SRC_AFESIM_ZYNQ_PLATFORM_H_

#include "Project_Define.h"

#include "ZYNQ_BasicType.h"
#include "Crc16.h"
#include "Interrupts.h"
#include "Uart.h"

#include "Xti_spi_if.h"

/**/

/* */

/* */

/* Hardware Abstract Layer Functions */
extern void ZYNQ_Platform_Init(void);
extern void ZYNQ_BQ79600_Init(void);

extern uint8 ZYNQ_BQ79600_MosiGetDataStatus(void);
extern uint8 ZYNQ_BQ79600_MosiDataRead(uint8 *data);

extern void ZYNQ_BQ79600_MisoDataWrite(uint8 *data, int length);
extern void ZYNQ_BQ79600_MisoFF(void);
extern void ZYNQ_BQ79600_MisoData(void);

extern void ZYNQ_BQ79600_SetRdyPinHigh(void);
extern void ZYNQ_BQ79600_SetRdyPinLow(void);

extern void ZYNQ_BQ79600_SetInhPinHigh(void);
extern void ZYNQ_BQ79600_SetInhPinLow(void);

#endif /* SRC_AFESIM_ZYNQ_PLATFORM_H_ */
