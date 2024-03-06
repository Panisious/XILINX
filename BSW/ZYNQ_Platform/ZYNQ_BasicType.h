/*
 * AfeSim_ZYNQ_BasicType.h
 *
 *  Created on: 2024Äê1ÔÂ20ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_ZYNQ_PLATFORM_CPU0_ZYNQ_BASICTYPE_CPU0_H_
#define SRC_ZYNQ_PLATFORM_CPU0_ZYNQ_BASICTYPE_CPU0_H_

#include "Project_Define.h"

#include <stdio.h>
#include <stdint.h>
#include "xil_types.h"
#include "xil_printf.h"

//#define PRINT_DEBUG
#ifdef PRINT_DEBUG
#define printf xil_printf
#else
#define printf (void)
#endif

typedef uint8_t 		uint8;
typedef uint16_t		uint16;
typedef uint32_t 		uint32;
typedef uint64_t 		uint64;

typedef char 			sint8;
typedef short 			sint16;
typedef int 			sint32;
typedef long long 		sint64;

typedef float			float32;
typedef double			float64;

#endif /* SRC_ZYNQ_PLATFORM_CPU0_ZYNQ_BASICTYPE_CPU0_H_ */
