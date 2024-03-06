/*
 * BQ79616_Cfg.h
 *
 *  Created on: 2024Äê1ÔÂ6ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_AFESIM_BQ79616_CFG_H_
#define SRC_AFESIM_BQ79616_CFG_H_

#include "AfeSim_BQ79616_Type.h"

#define AFESIM_BQ79616_CELL_NUM		16
#define AFESIM_BQ79616_GPIO_NUM		8

#define AFESIM_BQ79616_REG_DEV_CONF				(0x0002)
#define AFESIM_BQ79616_REG_ACTIVE_CELL			(0x0003)

#define AFESIM_BQ79616_REG_BBVC_POSN1			(0x0005)
#define AFESIM_BQ79616_REG_BBVC_POSN2			(0x0006)
#define AFESIM_BQ79616_REG_ADC_CONF1			(0x0007)
#define AFESIM_BQ79616_REG_ADC_CONF2			(0x0008)
#define AFESIM_BQ79616_REG_OV_THRESH			(0x0009)
#define AFESIM_BQ79616_REG_UV_THRESH			(0x000A)
#define AFESIM_BQ79616_REG_OTUT_THRESH			(0x000B)
#define AFESIM_BQ79616_REG_UV_DISABLE1			(0x000C)
#define AFESIM_BQ79616_REG_UV_DISABLE2			(0x000D)
#define AFESIM_BQ79616_REG_GPIO_CONF1			(0x000E)
#define AFESIM_BQ79616_REG_GPIO_CONF2			(0x000F)
#define AFESIM_BQ79616_REG_GPIO_CONF3			(0x0010)
#define AFESIM_BQ79616_REG_GPIO_CONF4			(0x0011)
#define AFESIM_BQ79616_REG_FAULT_MSK1			(0x0016)
#define AFESIM_BQ79616_REG_FAULT_MSK2			(0x0017)
#define AFESIM_BQ79616_REG_PWR_TRANSIT_CONF		(0x0018)
#define AFESIM_BQ79616_REG_COMM_TIMEOUT_CONF	(0x0019)
#define AFESIM_BQ79616_REG_MAIN_ADC_CAL1		(0x001B)
#define AFESIM_BQ79616_REG_MAIN_ADC_CAL2		(0x001C)
#define AFESIM_BQ79616_REG_AUX_ADC_CAL1			(0x001D)
#define AFESIM_BQ79616_REG_AUX_ADC_CAL2			(0x001E)
#define AFESIM_BQ79616_REG_BBP_LOC				(0x002A)

#define AFESIM_BQ79616_REG_DIR0_ADDR			(0x0306)
#define AFESIM_BQ79616_REG_DIR1_ADDR			(0x0307)
#define AFESIM_BQ79616_REG_COMM_CTRL			(0x0308)
#define AFESIM_BQ79616_REG_CONTROL1				(0x0309)
#define AFESIM_BQ79616_REG_CONTROL2				(0x030A)
#define AFESIM_BQ79616_REG_ADC_CTRL1			(0x030D)
#define AFESIM_BQ79616_REG_ADC_CTRL2			(0x030E)
#define AFESIM_BQ79616_REG_ADC_CTRL3			(0x030F)
#define AFESIM_BQ79616_REG_CB_CELL16_CTRL		(0x0318)
#define AFESIM_BQ79616_REG_CB_CELL15_CTRL		(0x0319)
#define AFESIM_BQ79616_REG_CB_CELL14_CTRL		(0x031A)
#define AFESIM_BQ79616_REG_CB_CELL13_CTRL		(0x031B)
#define AFESIM_BQ79616_REG_CB_CELL12_CTRL		(0x031C)
#define AFESIM_BQ79616_REG_CB_CELL11_CTRL		(0x031D)
#define AFESIM_BQ79616_REG_CB_CELL10_CTRL		(0x031E)
#define AFESIM_BQ79616_REG_CB_CELL9_CTRL		(0x031F)
#define AFESIM_BQ79616_REG_CB_CELL8_CTRL		(0x0320)
#define AFESIM_BQ79616_REG_CB_CELL7_CTRL		(0x0321)
#define AFESIM_BQ79616_REG_CB_CELL6_CTRL		(0x0322)
#define AFESIM_BQ79616_REG_CB_CELL5_CTRL		(0x0323)
#define AFESIM_BQ79616_REG_CB_CELL4_CTRL		(0x0324)
#define AFESIM_BQ79616_REG_CB_CELL3_CTRL		(0x0325)
#define AFESIM_BQ79616_REG_CB_CELL2_CTRL		(0x0326)
#define AFESIM_BQ79616_REG_CB_CELL1_CTRL		(0x0327)
#define AFESIM_BQ79616_REG_VCB_DONE_THRESH		(0x032A)
#define AFESIM_BQ79616_REG_OTCB_DONE_THRESH		(0x032B)
#define AFESIM_BQ79616_REG_OVUV_CTRL			(0x032C)
#define AFESIM_BQ79616_REG_OTUT_CTRL			(0x032D)
#define AFESIM_BQ79616_REG_BAL_CTRL1			(0x032E)
#define AFESIM_BQ79616_REG_BAL_CTRL2			(0x032F)
#define AFESIM_BQ79616_REG_BAL_CTRL3			(0x0330)
#define AFESIM_BQ79616_REG_FAULT_RST1			(0x0331)
#define AFESIM_BQ79616_REG_FAULT_RST2			(0x0332)
#define AFESIM_BQ79616_REG_DIAG_COMM_CTRL		(0x0336)
#define AFESIM_BQ79616_REG_DIAG_PWR_CTRL		(0x0337)
#define AFESIM_BQ79616_REG_DIAG_CBFET_CTRL1		(0x0338)
#define AFESIM_BQ79616_REG_DIAG_CBFET_CTRL2		(0x0339)
#define AFESIM_BQ79616_REG_DIAG_COMP_CTRL1		(0x033A)
#define AFESIM_BQ79616_REG_DIAG_COMP_CTRL2		(0x033B)
#define AFESIM_BQ79616_REG_DIAG_COMP_CTRL3		(0x033C)
#define AFESIM_BQ79616_REG_DIAG_COMP_CTRL4		(0x033D)
#define AFESIM_BQ79616_REG_DIAG_PROT_CTRL		(0x033E)


#define AFESIM_BQ79616_REG_DIAG_STAT			(0x0526)
#define AFESIM_BQ79616_REG_ADC_STAT1			(0x0527)
#define AFESIM_BQ79616_REG_ADC_STAT2			(0x0528)
#define AFESIM_BQ79616_REG_GPIO_STAT			(0x052A)
#define AFESIM_BQ79616_REG_BAL_STAT				(0x052B)
#define AFESIM_BQ79616_REG_DEV_STAT				(0x052C)
#define AFESIM_BQ79616_REG_FAULT_SUMMARY		(0x052D)
#define AFESIM_BQ79616_REG_FAULT_COMM1			(0x0530)
#define AFESIM_BQ79616_REG_FAULT_COMM2			(0x0531)
#define AFESIM_BQ79616_REG_FAULT_COMM3			(0x0532)
#define AFESIM_BQ79616_REG_FAULT_OTP			(0x0535)
#define AFESIM_BQ79616_REG_FAULT_SYS			(0x0536)
#define AFESIM_BQ79616_REG_FAULT_PROT1			(0x053A)
#define AFESIM_BQ79616_REG_FAULT_PROT2			(0x053B)
#define AFESIM_BQ79616_REG_FAULT_OV1			(0x053C)
#define AFESIM_BQ79616_REG_FAULT_OV2			(0x053D)
#define AFESIM_BQ79616_REG_FAULT_UV1			(0x053E)
#define AFESIM_BQ79616_REG_FAULT_UV2			(0x053F)
#define AFESIM_BQ79616_REG_FAULT_OT				(0x0540)
#define AFESIM_BQ79616_REG_FAULT_UT				(0x0541)
#define AFESIM_BQ79616_REG_FAULT_COMP_GPIO		(0x0543)
#define AFESIM_BQ79616_REG_FAULT_COMP_VCCB1		(0x0545)
#define AFESIM_BQ79616_REG_FAULT_COMP_VCCB2		(0x0546)
#define AFESIM_BQ79616_REG_FAULT_COMP_VCOW1		(0x0548)
#define AFESIM_BQ79616_REG_FAULT_COMP_VCOW2		(0x0549)
#define AFESIM_BQ79616_REG_FAULT_COMP_CBOW1		(0x054B)
#define AFESIM_BQ79616_REG_FAULT_COMP_CBOW2		(0x054C)
#define AFESIM_BQ79616_REG_FAULT_COMP_CBFET1	(0x054E)
#define AFESIM_BQ79616_REG_FAULT_COMP_CBFET2	(0x054F)
#define AFESIM_BQ79616_REG_FAULT_COMP_MISC		(0x0550)
#define AFESIM_BQ79616_REG_FAULT_PWR1			(0x0552)
#define AFESIM_BQ79616_REG_FAULT_PWR2			(0x0553)
#define AFESIM_BQ79616_REG_FAULT_PWR3			(0x0554)
#define AFESIM_BQ79616_REG_CB_COMPLETE1			(0x0556)
#define AFESIM_BQ79616_REG_CB_COMPLETE2			(0x0557)
#define AFESIM_BQ79616_REG_BAL_TIME				(0x0558)

/* Cell Voltage */
#define AFESIM_BQ79616_REG_VCELL16_HI			(0x0568)
#define AFESIM_BQ79616_REG_VCELL16_LO			(0x0569)
#define AFESIM_BQ79616_REG_VCELL15_HI			(0x056A)
#define AFESIM_BQ79616_REG_VCELL15_LO			(0x056B)
#define AFESIM_BQ79616_REG_VCELL14_HI			(0x056C)
#define AFESIM_BQ79616_REG_VCELL14_LO			(0x056D)
#define AFESIM_BQ79616_REG_VCELL13_HI			(0x056E)
#define AFESIM_BQ79616_REG_VCELL13_LO			(0x056F)
#define AFESIM_BQ79616_REG_VCELL12_HI			(0x0570)
#define AFESIM_BQ79616_REG_VCELL12_LO			(0x0571)
#define AFESIM_BQ79616_REG_VCELL11_HI			(0x0572)
#define AFESIM_BQ79616_REG_VCELL11_LO			(0x0573)
#define AFESIM_BQ79616_REG_VCELL10_HI			(0x0574)
#define AFESIM_BQ79616_REG_VCELL10_LO			(0x0575)
#define AFESIM_BQ79616_REG_VCELL9_HI			(0x0576)
#define AFESIM_BQ79616_REG_VCELL9_LO			(0x0577)
#define AFESIM_BQ79616_REG_VCELL8_HI			(0x0578)
#define AFESIM_BQ79616_REG_VCELL8_LO			(0x0579)
#define AFESIM_BQ79616_REG_VCELL7_HI			(0x057A)
#define AFESIM_BQ79616_REG_VCELL7_LO			(0x057B)
#define AFESIM_BQ79616_REG_VCELL6_HI			(0x057C)
#define AFESIM_BQ79616_REG_VCELL6_LO			(0x057D)
#define AFESIM_BQ79616_REG_VCELL5_HI			(0x057E)
#define AFESIM_BQ79616_REG_VCELL5_LO			(0x057F)
#define AFESIM_BQ79616_REG_VCELL4_HI			(0x0580)
#define AFESIM_BQ79616_REG_VCELL4_LO			(0x0581)
#define AFESIM_BQ79616_REG_VCELL3_HI			(0x0582)
#define AFESIM_BQ79616_REG_VCELL3_LO			(0x0583)
#define AFESIM_BQ79616_REG_VCELL2_HI			(0x0584)
#define AFESIM_BQ79616_REG_VCELL2_LO			(0x0585)
#define AFESIM_BQ79616_REG_VCELL1_HI			(0x0586)
#define AFESIM_BQ79616_REG_VCELL1_LO			(0x0587)

#define AFESIM_BQ79616_REG_BUSBAR_HI			(0x0588)
#define AFESIM_BQ79616_REG_BUSBAR_LO			(0x0589)

#define AFESIM_BQ79616_REG_TSREF_HI				(0x058C)
#define AFESIM_BQ79616_REG_TSREF_LO				(0x058D)


/* GPIO Voltage */
#define AFESIM_BQ79616_REG_GPIO1_HI				(0x058E)
#define AFESIM_BQ79616_REG_GPIO1_LO				(0x058F)
#define AFESIM_BQ79616_REG_GPIO2_HI				(0x0590)
#define AFESIM_BQ79616_REG_GPIO2_LO				(0x0591)
#define AFESIM_BQ79616_REG_GPIO3_HI				(0x0592)
#define AFESIM_BQ79616_REG_GPIO3_LO				(0x0593)
#define AFESIM_BQ79616_REG_GPIO4_HI				(0x0594)
#define AFESIM_BQ79616_REG_GPIO4_LO				(0x0595)
#define AFESIM_BQ79616_REG_GPIO5_HI				(0x0596)
#define AFESIM_BQ79616_REG_GPIO5_LO				(0x0597)
#define AFESIM_BQ79616_REG_GPIO6_HI				(0x0598)
#define AFESIM_BQ79616_REG_GPIO6_LO				(0x0599)
#define AFESIM_BQ79616_REG_GPIO7_HI				(0x059A)
#define AFESIM_BQ79616_REG_GPIO7_LO				(0x059B)
#define AFESIM_BQ79616_REG_GPIO8_HI				(0x059C)
#define AFESIM_BQ79616_REG_GPIO8_LO				(0x059D)

/* Die Temperature */
#define AFESIM_BQ79616_REG_DIETEMP1_HI			(0x05AE)
#define AFESIM_BQ79616_REG_DIETEMP1_LO			(0x05AF)
#define AFESIM_BQ79616_REG_DIETEMP2_HI			(0x05B0)
#define AFESIM_BQ79616_REG_DIETEMP2_LO			(0x05B1)

/* Aux Voltage */
#define AFESIM_BQ79616_REG_AUX_CELL_HI			(0x05B2)
#define AFESIM_BQ79616_REG_AUX_CELL_LO			(0x05B3)
#define AFESIM_BQ79616_REG_AUX_GPIO_HI			(0x05B4)
#define AFESIM_BQ79616_REG_AUX_GPIO_LO			(0x05B5)
#define AFESIM_BQ79616_REG_AUX_BAT_HI			(0x05B6)
#define AFESIM_BQ79616_REG_AUX_BAT_LO			(0x05B7)
#define AFESIM_BQ79616_REG_AUX_REFL_HI			(0x05B8)
#define AFESIM_BQ79616_REG_AUX_REFL_LO			(0x05B9)
#define AFESIM_BQ79616_REG_AUX_VBG2_HI			(0x05BA)
#define AFESIM_BQ79616_REG_AUX_VBG2_LO			(0x05BB)
#define AFESIM_BQ79616_REG_AUX_AVAO_REF_HI		(0x05BE)
#define AFESIM_BQ79616_REG_AUX_AVAO_REF_LO		(0x05BF)
#define AFESIM_BQ79616_REG_AUX_AVDD_REF_HI		(0x05C0)
#define AFESIM_BQ79616_REG_AUX_AVDD_REF_LO		(0x05C1)
#define AFESIM_BQ79616_REG_AUX_OV_DAC_HI		(0x05C2)
#define AFESIM_BQ79616_REG_AUX_OV_DAC_LO		(0x05C3)
#define AFESIM_BQ79616_REG_AUX_UV_DAC_HI		(0x05C4)
#define AFESIM_BQ79616_REG_AUX_UV_DAC_LO		(0x05C5)
#define AFESIM_BQ79616_REG_AUX_OT_OTCB_DAC_HI	(0x05C6)
#define AFESIM_BQ79616_REG_AUX_OT_OTCB_DAC_LO	(0x05C7)
#define AFESIM_BQ79616_REG_AUX_UT_DAC_HI		(0x05C8)
#define AFESIM_BQ79616_REG_AUX_UT_DAC_LO		(0x05C9)
#define AFESIM_BQ79616_REG_AUX_VCBDONE_HI		(0x05CA)
#define AFESIM_BQ79616_REG_AUX_VCBDONE_LO		(0x05CB)
#define AFESIM_BQ79616_REG_AUX_VCM_HI			(0x05CC)
#define AFESIM_BQ79616_REG_AUX_VCM_LO			(0x05CD)
#define AFESIM_BQ79616_REG_REFOVDAC_HI			(0x05D0)
#define AFESIM_BQ79616_REG_REFOVDAC_LO			(0x05D1)
#define AFESIM_BQ79616_REG_DIAG_MAIN_HI			(0x05D2)
#define AFESIM_BQ79616_REG_DIAG_MAIN_LO			(0x05D3)
#define AFESIM_BQ79616_REG_DIAG_AUX_HI			(0x05D4)
#define AFESIM_BQ79616_REG_DIAG_AUX_LO			(0x05D5)

/* 0x0002 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 HB_EN          : 1;
			uint8 FTONE_EN       : 1;
			uint8 NFAULT_EN      : 1;
			uint8 TWO_STOP_EN    : 1;
			uint8 FCOMM_EN       : 1;
			uint8 MULTIDROP_EN   : 1;
			uint8 NO_ADJ_CB      : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_DEV_CONF_RegType;


/* 0x0003 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 NUM_CELL  : 4;
		}field;
	}reg;
}AFESIM_BQ79616_REG_ACTIVE_CELL_RegType;


/* 0x0005 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 CELL1  : 1;
			uint8 CELL2  : 1;
			uint8 CELL3  : 1;
			uint8 CELL4  : 1;
			uint8 CELL5  : 1;
			uint8 CELL6  : 1;
			uint8 CELL7  : 1;
			uint8 CELL8  : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_BBVC_POSN1_RegType;


/* 0x0006 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 CELL9  : 1;
			uint8 CELL10  : 1;
			uint8 CELL11  : 1;
			uint8 CELL12  : 1;
			uint8 CELL13  : 1;
			uint8 CELL14  : 1;
			uint8 CELL15  : 1;
			uint8 CELL16  : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_BBVC_POSN2_RegType;

/* 0x0008 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 ADC_DLY   : 6;
		}field;
	}reg;
}AFESIM_BQ79616_REG_ADC_CONF2_RegType;

/* 0x0009 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 OV_THR   : 6;
		}field;
	}reg;
}AFESIM_BQ79616_REG_OV_THRESH_RegType;

/* 0x000A */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 UV_THR   : 6;
		}field;
	}reg;
}AFESIM_BQ79616_REG_UV_THRESH_RegType;

/* 0x000B */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 OT_THR   : 5;
			uint8 UT_THR   : 3;
		}field;
	}reg;
}AFESIM_BQ79616_REG_OTUT_THRESH_RegType;



/* 0x000E */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 GPIO1       : 3;
			uint8 GPIO2       : 3;
			uint8 SPI_EN      : 1;
			uint8 FAULT_IN_EN : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_GPIO_CONF1_RegType;

/* 0x000F */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 GPIO3       : 3;
			uint8 GPIO4       : 3;
		}field;
	}reg;
}AFESIM_BQ79616_REG_GPIO_CONF2_RegType;

/* 0x0010 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 GPIO5       : 3;
			uint8 GPIO6       : 3;
		}field;
	}reg;
}AFESIM_BQ79616_REG_GPIO_CONF3_RegType;

/* 0x0011 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 GPIO7       : 3;
			uint8 GPIO8       : 3;
		}field;
	}reg;
}AFESIM_BQ79616_REG_GPIO_CONF4_RegType;

/* 0x002A */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 LOC       : 5;
		}field;
	}reg;
}AFESIM_BQ79616_REG_BBP_LOC_RegType;


/* 0x030A */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 TSREF_EN      : 1;
			uint8 SEND_HW_RESET : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_CONTROL2_RegType;


/* 0x030D */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 MAIN_MODE      : 2;
			uint8 MAIN_GO        : 1;
			uint8 LPF_VCELL_EN   : 1;
			uint8 LPF_BB_EN      : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_ADC_CTRL1_RegType;

/* 0x030E */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 AUX_CELL_SEL   : 5;
			uint8 AUX_CELL_ALIGN : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_ADC_CTRL2_RegType;



/* 0x030F */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 AUX_MODE       : 2;
			uint8 AUX_GO         : 1;
			uint8 AUX_GPIO_SEL   : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_ADC_CTRL3_RegType;


/* 0x030F */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 CB_THR       : 6;
		}field;
	}reg;
}AFESIM_BQ79616_REG_VCB_DONE_THRESH_RegType;

/* 0x032C */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 OVUV_MODE        : 2;
			uint8 OVUV_GO          : 1;
			uint8 OVUV_LOCK        : 4;
			uint8 VCBDONE_THR_LOCK : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_OVUV_CTRL_RegType;

/* 0x032D */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 OTUT_MODE      : 2;
			uint8 OTUT_GO        : 1;
			uint8 OTUT_LOCK      : 3;
			uint8 OTCB_THR_LOCK  : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_OTUT_CTRL_RegType;

/* 0x032E */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 DUTY   : 3;
		}field;
	}reg;
}AFESIM_BQ79616_REG_BAL_CTRL1_RegType;

/* 0x032F */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 AUTO_BAL   : 1;
			uint8 BAL_GO     : 1;
			uint8 BAL_ACT    : 2;
			uint8 OTCB_EN    : 1;
			uint8 FLTSTOP_EN : 1;
			uint8 CB_PAUSE   : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_BAL_CTRL2_RegType;


/* 0x0330 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 BAL_TIME_GO   : 1;
			uint8 BAL_TIME_SEL  : 4;
		}field;
	}reg;
}AFESIM_BQ79616_REG_BAL_CTRL3_RegType;

/* 0x0331 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 RST_PWR    : 1;
			uint8 RST_SYS    : 1;
			uint8 RST_COMP   : 1;
			uint8 RST_OV     : 1;
			uint8 RST_UV     : 1;
			uint8 RST_OT     : 1;
			uint8 RST_UT     : 1;
			uint8 RST_PROT   : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_FAULT_RST1_RegType;

/* 0x0332 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 RST_COMM1       : 1;
			uint8 RST_COMM2       : 1;
			uint8 RST_COMM3_HB    : 1;
			uint8 RST_COMM3_FTONE : 1;
			uint8 RST_COMM3_FCOMM : 1;
			uint8 RST_OTP_DATA    : 1;
			uint8 RST_OTP_CRC     : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_FAULT_RST2_RegType;


/* 0x0336 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 FLIP_TR_CRC : 1;
			uint8 SPI_LOOPBACK : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_DIAG_COMM_CTRL_RegType;

/* 0x0337 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 PWR_BIST_GO : 1;
			uint8 BIST_NO_RST : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_DIAG_PWR_CTRL_RegType;

/* 0x033A */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 BB_THR   : 3;
			uint8 VCCB_THR : 5;
		}field;
	}reg;
}AFESIM_BQ79616_REG_DIAG_COMP_CTRL1_RegType;

/* 0x033B */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 OW_THR   : 4;
			uint8 GPIO_THR : 3;
		}field;
	}reg;
}AFESIM_BQ79616_REG_DIAG_COMP_CTRL2_RegType;


/* 0x033C */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 COMP_ADC_GO   : 1;
			uint8 COMP_ADC_SEL  : 3;
			uint8 OW_SINK       : 2;
			uint8 CBFET_CTRL_GO : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_DIAG_COMP_CTRL3_RegType;

/* 0x033D */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 LPF_FAULT_INJ  : 1;
			uint8 COMP_FAULT_INJ : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_DIAG_COMP_CTRL4_RegType;

/* 0x033E */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 PROT_BIST_NO_RST  : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_DIAG_PROT_CTRL_RegType;

/* 0x0526 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 DRDY_BIST_PWR   : 1;
			uint8 DRDY_BIST_OVUV  : 1;
			uint8 DRDY_BIST_OTUT  : 1;
			uint8 DRDY_OVUV       : 1;
			uint8 DRDY_OTUT       : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_DIAG_STAT_RegType;

/* 0x0527 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 DRDY_MAIN_ADC  : 1;
			uint8 DRDY_AUX_MISC  : 1;
			uint8 DRDY_AUX_CELL  : 1;
			uint8 DRDY_AUX_GPIO  : 1;
			uint8 DRDY_CS_ADC    : 1;
		}field;
		struct
		{
			uint8 MAIN_GO_Clear : 1;
			uint8 AUX_GO_Clear  : 3;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_ADC_STAT1_RegType;

/* 0x0528 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 DRDY_VCCB  : 1;
			uint8 DRDY_CBFET : 1;
			uint8 DRDY_CBOW  : 1;
			uint8 DRDY_VCOW  : 1;
			uint8 DRDY_GPIO  : 1;
			uint8 DRDY_LPF   : 1;
		}field;
		struct
		{
			uint8 COMP_ADC_GO_Clear : 5;
			uint8 MAIN_GO_Clear : 1;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_ADC_STAT2_RegType;


/* 0x052A */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 GPIO1  : 1;
			uint8 GPIO2  : 1;
			uint8 GPIO3  : 1;
			uint8 GPIO4  : 1;
			uint8 GPIO5  : 1;
			uint8 GPIO6  : 1;
			uint8 GPIO7  : 1;
			uint8 GPIO8  : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_GPIO_STAT_RegType;

/* 0x052B */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 CB_DONE         : 1;
			uint8 MB_DONE         : 1;
			uint8 ABORTFLT        : 1;
			uint8 CB_RUN          : 1;
			uint8 MB_RUN          : 1;
			uint8 CB_INPAUSE      : 1;
			uint8 OT_PAUSE_DET    : 1;
			uint8 INVALID_CBCONF  : 1;
		}field;
		struct
		{
			uint8 BAL_GO_Clear : 3;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_BAL_STAT_RegType;

/* 0x0526 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 MAIN_RUN       : 1;
			uint8 AUX_RUN        : 1;
			uint8 RSVD           : 1;
			uint8 OVUV_RUN       : 1;
			uint8 OTUT_RUN       : 1;
			uint8 CUST_CRC_DONE  : 1;
			uint8 FACT_CRC_DONE  : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_DEV_STAT_RegType;


/* 0x0526 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 PROT_BIST_NO_RST  : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_FACT_TEST_MODE_RegType;

/* 0x052D */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 FAULT_PWR       : 1;
			uint8 FAULT_SYS       : 1;
			uint8 FAULT_OVUV      : 1;
			uint8 FAULT_OTUT      : 1;
			uint8 FAULT_COMM      : 1;
			uint8 FAULT_OTP       : 1;
			uint8 FAULT_COMP_ADC  : 1;
			uint8 FAULT_PROT      : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_FAULT_SUMMARY_RegType;

/* 0x0531 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 COMH_BIT : 1;
			uint8 COMH_RC  : 1;
			uint8 COMH_RR  : 1;
			uint8 COMH_TR  : 1;
			uint8 COML_BIT : 1;
			uint8 COML_RC  : 1;
			uint8 COML_RR  : 1;
			uint8 COML_TR  : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_FAULT_COMM2_RegType;

/* 0x0532 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 HB_FAST : 1;
			uint8 HB_FAIL  : 1;
			uint8 FTONE_DET  : 1;
			uint8 FCOMM_DET  : 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_FAULT_COMM3_RegType;

/* 0x0543 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 GPIO1_FAIL : 1;
			uint8 GPIO2_FAIL : 1;
			uint8 GPIO3_FAIL : 1;
			uint8 GPIO4_FAIL : 1;
			uint8 GPIO5_FAIL : 1;
			uint8 GPIO6_FAIL : 1;
			uint8 GPIO7_FAIL : 1;
			uint8 GPIO8_FAIL : 1;
		}field;
		struct
		{
			uint8 BIST : 8;
		}bist;
		struct
		{
			uint8 CLR: 8;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_FAULT_COMP_GPIO_RegType;

/* 0x0545 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 CELL9_FAIL : 1;
			uint8 CELL10_FAIL : 1;
			uint8 CELL11_FAIL : 1;
			uint8 CELL12_FAIL : 1;
			uint8 CELL13_FAIL : 1;
			uint8 CELL14_FAIL : 1;
			uint8 CELL15_FAIL : 1;
			uint8 CELL16_FAIL : 1;
		}field;
		struct
		{
			uint8 BIST : 8;
		}bist;
		struct
		{
			uint8 CLR: 8;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_FAULT_COMP_VCCB1_RegType;

/* 0x0546 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 CELL1_FAIL : 1;
			uint8 CELL2_FAIL : 1;
			uint8 CELL3_FAIL : 1;
			uint8 CELL4_FAIL : 1;
			uint8 CELL5_FAIL : 1;
			uint8 CELL6_FAIL : 1;
			uint8 CELL7_FAIL : 1;
			uint8 CELL8_FAIL : 1;

		}field;
		struct
		{
			uint8 BIST : 8;
		}bist;
		struct
		{
			uint8 CLR: 8;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_FAULT_COMP_VCCB2_RegType;

/* 0x0548 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 VCOW9_FAIL : 1;
			uint8 VCOW10_FAIL : 1;
			uint8 VCOW11_FAIL : 1;
			uint8 VCOW12_FAIL : 1;
			uint8 VCOW13_FAIL : 1;
			uint8 VCOW14_FAIL : 1;
			uint8 VCOW15_FAIL : 1;
			uint8 VCOW16_FAIL : 1;
		}field;
		struct
		{
			uint8 BIST : 8;
		}bist;
		struct
		{
			uint8 CLR: 8;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_FAULT_COMP_VCOW1_RegType;

/* 0x0549 */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 VCOW1_FAIL : 1;
			uint8 VCOW2_FAIL : 1;
			uint8 VCOW3_FAIL : 1;
			uint8 VCOW4_FAIL : 1;
			uint8 VCOW5_FAIL : 1;
			uint8 VCOW6_FAIL : 1;
			uint8 VCOW7_FAIL : 1;
			uint8 VCOW8_FAIL : 1;

		}field;
		struct
		{
			uint8 BIST : 8;
		}bist;
		struct
		{
			uint8 CLR: 8;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_FAULT_COMP_VCOW2_RegType;


/* 0x054B */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 CBOW9_FAIL : 1;
			uint8 CBOW10_FAIL : 1;
			uint8 CBOW11_FAIL : 1;
			uint8 CBOW12_FAIL : 1;
			uint8 CBOW13_FAIL : 1;
			uint8 CBOW14_FAIL : 1;
			uint8 CBOW15_FAIL : 1;
			uint8 CBOW16_FAIL : 1;
		}field;
		struct
		{
			uint8 BIST : 8;
		}bist;
		struct
		{
			uint8 CLR: 8;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_FAULT_COMP_CBOW1_RegType;

/* 0x054C */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 CBOW1_FAIL : 1;
			uint8 CBOW2_FAIL : 1;
			uint8 CBOW3_FAIL : 1;
			uint8 CBOW4_FAIL : 1;
			uint8 CBOW5_FAIL : 1;
			uint8 CBOW6_FAIL : 1;
			uint8 CBOW7_FAIL : 1;
			uint8 CBOW8_FAIL : 1;

		}field;
		struct
		{
			uint8 BIST : 8;
		}bist;
		struct
		{
			uint8 CLR: 8;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_FAULT_COMP_CBOW2_RegType;

/* 0x054E */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 CBFET9_FAIL : 1;
			uint8 CBFET10_FAIL : 1;
			uint8 CBFET11_FAIL : 1;
			uint8 CBFET12_FAIL : 1;
			uint8 CBFET13_FAIL : 1;
			uint8 CBFET14_FAIL : 1;
			uint8 CBFET15_FAIL : 1;
			uint8 CBFET16_FAIL : 1;
		}field;
		struct
		{
			uint8 BIST : 8;
		}bist;
		struct
		{
			uint8 CLR: 8;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_FAULT_COMP_CBFET1_RegType;

/* 0x054F */
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 CBFET1_FAIL : 1;
			uint8 CBFET2_FAIL : 1;
			uint8 CBFET3_FAIL : 1;
			uint8 CBFET4_FAIL : 1;
			uint8 CBFET5_FAIL : 1;
			uint8 CBFET6_FAIL : 1;
			uint8 CBFET7_FAIL : 1;
			uint8 CBFET8_FAIL : 1;

		}field;
		struct
		{
			uint8 BIST : 8;
		}bist;
		struct
		{
			uint8 CLR: 8;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_FAULT_COMP_CBFET2_RegType;



/*0x0550*/
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 LPF_FAIL 			: 1;
			uint8 COMP_ADC_ABORT  	: 1;
		}field;
	}reg;
}AFESIM_BQ79616_REG_FAULT_COMP_MISC_RegType;

/*0x0552*/
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 AVDD_OV 		: 1;
			uint8 AVDD_OSC  	: 1;
			uint8 DVDD_OV 		: 1;
			uint8 CVDD_OV  		: 1;
			uint8 CVDD_UV 		: 1;
			uint8 REFHM_OPEN  	: 1;
			uint8 DVSS_OPEN 	: 1;
			uint8 CVSS_OPEN  	: 1;
		}field;
		struct
		{
			uint8 BIST : 8;
		}bist;
		struct
		{
			uint8 CLR : 8;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_FAULT_PWR1_RegType;

/*0x0553*/
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 TSREF_OV 		: 1;
			uint8 TSREF_UV  	: 1;
			uint8 TSREF_OSC 	: 1;
			uint8 NEG5V_UV  	: 1;
			uint8 REFH_OSC 		: 1;
			uint8 RSVD  		: 1;
			uint8 PWRBIST_FAIL 	: 1;
		}field;
		struct
		{
			uint8 BIST : 5;
			uint8 RSVD : 1;
			uint8 FAIL : 1;
		}bist;
		struct
		{
			uint8 CLR : 8;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_FAULT_PWR2_RegType;

/*0x0554*/
typedef struct
{
	union
	{
		uint8 val;
		struct
		{
			uint8 AVDDUV_DRST 	: 1;
		}field;
		struct
		{
			uint8 CLR : 8;
		}clear;
	}reg;
}AFESIM_BQ79616_REG_FAULT_PWR3_RegType;



#endif /* SRC_AFESIM_BQ79616_CFG_H_ */
