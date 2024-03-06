/*
 * Project_Define.h
 *
 *  Created on: 2024Äê2ÔÂ4ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_PROJECT_DEFINE_H_
#define SRC_PROJECT_DEFINE_H_

#define ZYNQ_DISABLE_DEMO

/* ZYNQ IC Configure */

//#define ZYNQ_USE_7010

#ifndef ZYNQ_USE_7010
#ifndef ZYNQ_USE_7020
#define ZYNQ_USE_7020
#endif
#endif

#define AFESIM_DEBUG

/* OS Configure */
/* Decide if file need to include "FreeRTOS.h" */
#define ZYNQ_USE_FREERTOS


/**/


/* BSW Configure */
#define UARTXPS_USE_GLOBAL_INTR_INIT
#define UARTXPS_USE_OUTER_BUFF
#define CANXPS_USE_GLOBAL_INTR_INIT
//#define CANXPS_USE_OUTER_BUFF

#define ZYNQ_AFESIM_USE_TI_AFE




/* ASW Configure */







#endif /* SRC_PROJECT_DEFINE_H_ */
