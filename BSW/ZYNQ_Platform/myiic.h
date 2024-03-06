//****************************************Copyright (c)***********************************//
//ԭ�Ӹ����߽�ѧƽ̨��www.yuanzige.com
//����֧�֣�www.openedv.com
//�Ա����̣�http://openedv.taobao.com
//��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡZYNQ & FPGA & STM32 & LINUX���ϡ�
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           myiic.h
// Last modified Date:  2019/07/26 16:04:03
// Last Version:        V1.0
// Descriptions:        IIC ���������ͷ�ļ�
//----------------------------------------------------------------------------------------
// Created by:          ����ԭ��
// Created date:        2019/07/26 16:04:07
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//

#ifndef __MYIIC_H
#define __MYIIC_H

#include "Project_Define.h"
#include "delay.h"

//IIC���в�������
void IIC_Init(void);                    //��ʼ��IIC��IO��
void IIC_Start(void);                   //����IIC��ʼ�ź�
void IIC_Stop(void);                    //����IICֹͣ�ź�
void IIC_Send_Byte(u8 txd);             //IIC����һ���ֽ�
u8   IIC_Read_Byte(unsigned char ack);  //IIC��ȡһ���ֽ�
u8   IIC_Wait_Ack(void);                //IIC�ȴ�ACK�ź�
void IIC_Ack(void);                     //IIC����ACK�ź�
void IIC_NAck(void);                    //IIC������ACK�ź�
#endif
