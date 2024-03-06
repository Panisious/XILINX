//****************************************Copyright (c)***********************************//
//ԭ�Ӹ����߽�ѧƽ̨��www.yuanzige.com
//����֧�֣�www.openedv.com
//�Ա����̣�http://openedv.taobao.com
//��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡZYNQ & FPGA & STM32 & LINUX���ϡ�
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           touch
// Last modified Date:  2019/07/26 16:04:03
// Last Version:        V1.0
// Descriptions:        ��������������
//----------------------------------------------------------------------------------------
// Created by:          ����ԭ��
// Created date:        2019/07/26 16:04:07
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//

#include "touch.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "xil_types.h"
#include "emio_iic_cfg.h"
#include "lcd.h"

_m_tp_dev tp_dev= {
    TP_Init,
    TP_Scan,
    0,
    0,
    0,
    0,
};

//��������ɨ��
//tp:0,��Ļ����;1,��������(У׼�����ⳡ����)
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
u8 TP_Scan(void)
{
    if( INT_RD() == 0 )  //����а�������
    {
        if( (tp_dev.sta & TP_PRES_DOWN) == 0 )    //֮ǰû�б�����
        {
            tp_dev.sta = TP_PRES_DOWN | TP_CATH_PRES ;   //��sta�����λ�͵�2��λ ����1    //��������

            tp_dev.x[4] = tp_dev.x[0];                //��¼��һ�ΰ���ʱ������
            tp_dev.y[4] = tp_dev.y[0];
        }
        printf("tp_dev.sta = %X\r\n",tp_dev.sta);
    }
    else  //����ް�������
    {
        if(tp_dev.sta & TP_PRES_DOWN)   //֮ǰ�Ǳ����µ�
        {
            tp_dev.sta &= ~(1<<7) ;                    //��ǰ����ɿ�
        }
        else
        {                                    //֮ǰ��û�б�����
            tp_dev.x[4]=0;
            tp_dev.y[4]=0;
            tp_dev.x[0]=0xffff;
            tp_dev.y[0]=0xffff;
        }
    }
    printf("tp_dev.sta = %X\r\n",tp_dev.sta);
    return tp_dev.sta & TP_PRES_DOWN ;  //���ص�ǰ�Ĵ���״̬
}

//��������ʼ��
void TP_Init(void)
{
	if(   lcd_id==0X5510 ||  lcd_id==0X4342 || lcd_id==0X4384 )  //���ݴ�����,4.3����
	{

		GT9147_Init();
        tp_dev.scan=GT9147_Scan;               //ɨ�躯��ָ��GT9147������ɨ��
        tp_dev.touchtype|=0X80;                //������
    }
    //SSD1963 7�������� 7��800*480/1024*600 RGB��
	else if(   lcd_id==0X1963||   lcd_id==0X7084||   lcd_id==0X7016) {
        FT5206_Init();
        tp_dev.scan=FT5206_Scan;            //ɨ�躯��ָ��FT5206������ɨ��
        tp_dev.touchtype|=0X80;             //������
	}
	else if(   lcd_id==0X1018) {
        GT9271_Init();
        tp_dev.scan=GT9271_Scan;            //ɨ�躯��ָ��GT9271������ɨ��
        tp_dev.touchtype|=0X80;             //������
    }
}
