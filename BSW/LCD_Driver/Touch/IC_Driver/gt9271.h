//****************************************Copyright (c)***********************************//
//ԭ�Ӹ����߽�ѧƽ̨��www.yuanzige.com
//����֧�֣�www.openedv.com
//�Ա����̣�http://openedv.taobao.com
//��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡZYNQ & FPGA & STM32 & LINUX���ϡ�
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           gt9271
// Last modified Date:  2019/07/26 16:04:03
// Last Version:        V1.0
// Descriptions:        10.1����ݴ�����-GT9271 ��������
//----------------------------------------------------------------------------------------
// Created by:          ����ԭ��
// Created date:        2019/07/26 16:04:07
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//

#ifndef __GT9271_H
#define __GT9271_H
#include "xil_types.h"

//IO��������
#define GT9271_RST(x)   	IOWR_ALTERA_AVALON_PIO_DATA(TOUCH_TCS_BASE,x) 		//GT9147��λ����
#define GT9271_INT(x)   	IOWR_ALTERA_AVALON_PIO_DATA(TOUCH_INT_BASE,x)     	//GT9147�ж�����
#define GT9271_INT_DIR(x)	IOWR_ALTERA_AVALON_PIO_DIRECTION(TOUCH_INT_BASE,x)  // ����PIO�ķ���

//I2C��д����
#define GT9271_CMD_WR       0X28        //д����
#define GT9271_CMD_RD       0X29        //������

//GT9271 ���ּĴ�������
#define GT9271_CTRL_REG     0X8040      //GT9271���ƼĴ���
#define GT9271_CFGS_REG     0X8047      //GT9271������ʼ��ַ�Ĵ���
#define GT9271_CHECK_REG    0X80FF      //GT9271У��ͼĴ���
#define GT9271_PID_REG      0X8140      //GT9271��ƷID�Ĵ���

#define GT9271_GSTID_REG    0X814E      //GT9271��ǰ��⵽�Ĵ������
#define GT9271_TP1_REG      0X8150      //��һ�����������ݵ�ַ
#define GT9271_TP2_REG      0X8158      //�ڶ������������ݵ�ַ
#define GT9271_TP3_REG      0X8160      //���������������ݵ�ַ
#define GT9271_TP4_REG      0X8168      //���ĸ����������ݵ�ַ
#define GT9271_TP5_REG      0X8170      //��������������ݵ�ַ
#define GT9271_TP6_REG      0X8178      //���������������ݵ�ַ
#define GT9271_TP7_REG      0X8180      //���߸����������ݵ�ַ
#define GT9271_TP8_REG      0X8188      //�ڰ˸����������ݵ�ַ
#define GT9271_TP9_REG      0X8190      //�ھŸ����������ݵ�ַ
#define GT9271_TP10_REG     0X8198      //��ʮ�����������ݵ�ַ


u8 GT9271_Send_Cfg(u8 mode);
u8 GT9271_WR_Reg(u16 reg,u8 *buf,u8 len);
void GT9271_RD_Reg(u16 reg,u8 *buf,u8 len);
u8 GT9271_Init(void);
u8 GT9271_Scan(u8 mode);
#endif
