//****************************************Copyright (c)***********************************//
//ԭ�Ӹ����߽�ѧƽ̨��www.yuanzige.com
//����֧�֣�www.openedv.com
//�Ա����̣�http://openedv.taobao.com 
//��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡZYNQ & FPGA & STM32 & LINUX���ϡ�
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           delay.h
// Last modified Date:  2019/07/26 9:53:48
// Last Version:        V1.0
// Descriptions:        ��ʱ����ͷ�ļ�
//----------------------------------------------------------------------------------------
// Created by:          ����ԭ��
// Created date:        2019/07/26 9:53:52
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//

#ifndef DELAY_H_
#define DELAY_H_

#include "Project_Define.h"
#include "xil_types.h"
#include "sleep.h"

void delay_ms(u32 n);       //������ʱ
void delay_us(u32 n);       //΢����ʱ

#endif /* DELAY_H_ */
