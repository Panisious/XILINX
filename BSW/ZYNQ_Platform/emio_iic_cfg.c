//****************************************Copyright (c)***********************************//
//ԭ�Ӹ����߽�ѧƽ̨��www.yuanzige.com
//����֧�֣�www.openedv.com
//�Ա����̣�http://openedv.taobao.com
//��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡZYNQ & FPGA & STM32 & LINUX���ϡ�
//��Ȩ���У�����ؾ���
//Copyright(C) ����ԭ�� 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           emio_iic_cfg.c
// Last modified Date:  2019/07/26 15:59:46
// Last Version:        V1.0
// Descriptions:        iic����
//----------------------------------------------------------------------------------------
// Created by:          ����ԭ��
// Created date:        2019/07/26 15:59:52
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//

#include"emio_iic_cfg.h"

#define  GPIOPS_ID  XPAR_XGPIOPS_0_DEVICE_ID  //PS �� GPIO ���� ID

static  XGpioPs  gpiops_inst; //PS �� GPIO ����ʵ��

//EMIO��ʼ��
void emio_init(void)
{
	XGpioPs_Config *gpiops_cfg_ptr; //PS �� GPIO ������Ϣ

	//�������� ID ����������Ϣ
	gpiops_cfg_ptr = XGpioPs_LookupConfig(GPIOPS_ID);

	//��ʼ����������
	XGpioPs_CfgInitialize(&gpiops_inst, gpiops_cfg_ptr, gpiops_cfg_ptr->BaseAddr);

	//���� iic�˿� Ϊ���
	XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_SCL_NUM, 1);
	XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_SDA_NUM, 1);
	XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_CT_RST_NUM, 1);

	//ʹ��iic�˿� ���
	XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_SCL_NUM, 1);
	XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_SDA_NUM, 1);
	XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_CT_RST_NUM, 1);

	//��iic��SCLK��SDA������
	XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, 1);
	XGpioPs_WritePin(&gpiops_inst, EMIO_SDA_NUM, 1);
}

//���� iic��SDA�˿�Ϊ����
void  SDA_IN( void )
{
	XGpioPs_SetOutputEnablePin (&gpiops_inst, EMIO_SDA_NUM, 0);
	XGpioPs_SetDirectionPin    (&gpiops_inst, EMIO_SDA_NUM, 0);
}

//���� iic��SDA�˿�Ϊ���
void  SDA_OUT( void )
{
	XGpioPs_SetDirectionPin    (&gpiops_inst, EMIO_SDA_NUM, 1);  //SDA����Ϊ���
	XGpioPs_SetOutputEnablePin (&gpiops_inst, EMIO_SDA_NUM, 1);
}

//��ֵ iic��SCL�˿�
void  IIC_SCL( u8 x )
{
	XGpioPs_WritePin(&gpiops_inst, EMIO_SCL_NUM, x);
}

//��ֵ iic��SDA�˿�
void  IIC_SDA( u8 x )
{
	XGpioPs_WritePin(&gpiops_inst, EMIO_SDA_NUM, x);
}

//��ȡ iic��SDA�˿�
u8  READ_SDA ( void )
{
	return  XGpioPs_ReadPin(&gpiops_inst, EMIO_SDA_NUM) ;
} 

//��ֵ ����оƬ��RST�˿�
void  CT_RST( u8 x )
{
	XGpioPs_WritePin(&gpiops_inst, EMIO_CT_RST_NUM, x);
}

//���ô���оƬ��INT�˿ڵķ���
void  INT_DIR( u8 x )
{

	XGpioPs_SetDirectionPin(&gpiops_inst, EMIO_CT_INT_NUM, x);
	XGpioPs_SetOutputEnablePin(&gpiops_inst, EMIO_CT_INT_NUM, x);
}

//��ֵ����оƬ��INT�˿�
void  INT( u8 x )
{
	XGpioPs_WritePin(&gpiops_inst, EMIO_CT_INT_NUM, x);
}

//��ȡ����оƬ��INT�˿�
u8  INT_RD( void )
{
	return  XGpioPs_ReadPin(&gpiops_inst, EMIO_CT_INT_NUM) ;
}

