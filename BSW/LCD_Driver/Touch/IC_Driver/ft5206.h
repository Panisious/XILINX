//****************************************Copyright (c)***********************************//
//ԭ�Ӹ����߽�ѧƽ̨��www.yuanzige.com
//����֧�֣�www.openedv.com
//�Ա����̣�http://openedv.taobao.com 
//��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡZYNQ & FPGA & STM32 & LINUX���ϡ�
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           ft5206.c
// Last modified Date:  2019/07/26 15:59:12
// Last Version:        V1.0
// Descriptions:        7����ݴ�����-FT5206/FT5426 ��������
//----------------------------------------------------------------------------------------
// Created by:          ����ԭ��
// Created date:        2019/07/26 15:59:18
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//

#ifndef __FT5206_H
#define __FT5206_H

#include "xil_types.h"

   
//I2C��д����   
#define FT_CMD_WR               0X70        //д����
#define FT_CMD_RD               0X71        //������
  
//FT5206 ���ּĴ������� 
#define FT_DEVIDE_MODE          0x00        //FT5206ģʽ���ƼĴ���
#define FT_REG_NUM_FINGER       0x02        //����״̬�Ĵ���

#define FT_TP1_REG              0X03        //��һ�����������ݵ�ַ
#define FT_TP2_REG              0X09        //�ڶ������������ݵ�ַ
#define FT_TP3_REG              0X0F        //���������������ݵ�ַ
#define FT_TP4_REG              0X15        //���ĸ����������ݵ�ַ
#define FT_TP5_REG              0X1B        //��������������ݵ�ַ  
 

#define FT_ID_G_LIB_VERSION     0xA1        //�汾        
#define FT_ID_G_MODE            0xA4        //FT5206�ж�ģʽ���ƼĴ���
#define FT_ID_G_THGROUP         0x80        //������Чֵ���üĴ���
#define FT_ID_G_PERIODACTIVE    0x88        //����״̬�������üĴ���


u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len);
void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len);
u8 FT5206_Init(void);
u8 FT5206_Scan(u8 mode);

#endif
