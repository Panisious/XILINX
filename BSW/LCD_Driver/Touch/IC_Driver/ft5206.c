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

#include "lcd.h"
#include "xil_types.h"
#include "ft5206.h"
#include "touch.h"
#include "myiic.h"
#include "delay.h"
#include "emio_iic_cfg.h"

//��FT5206д��һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:д���ݳ���
//����ֵ:0,�ɹ�;1,ʧ��.
u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len)
{
    u8 i;
    u8 ret=0;
    IIC_Start();
    IIC_Send_Byte(FT_CMD_WR);   //����д����
    IIC_Wait_Ack();
    IIC_Send_Byte(reg&0XFF);    //���͵�8λ��ַ
    IIC_Wait_Ack();
    for(i=0; i<len; i++) {
        IIC_Send_Byte(buf[i]);  //������
        ret=IIC_Wait_Ack();
        if(ret)break;
    }
    IIC_Stop();                 //����һ��ֹͣ����
    return ret;
}

//��FT5206����һ������
//reg:��ʼ�Ĵ�����ַ
//buf:���ݻ�������
//len:�����ݳ���
void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len)
{
    u8 i;
    IIC_Start();
    IIC_Send_Byte(FT_CMD_WR);                 //����д����
    IIC_Wait_Ack();
    IIC_Send_Byte(reg&0XFF);                  //���͵�8λ��ַ
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(FT_CMD_RD);                 //���Ͷ�����
    IIC_Wait_Ack();
    for(i=0; i<len; i++) {
        buf[i] = IIC_Read_Byte( i == (len-1) ? 0 : 1 ); //������
    }
    IIC_Stop();//����һ��ֹͣ����
}

u8 CIP[5]; //������Ŵ���IC-GT911
//��ʼ��FT5206������
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ��
u8 FT5206_Init(void)
{
    u8 temp[5];
    IIC_Init();             //��ʼ����������I2C����
    INT_DIR(1);             //TOUCH_INT��������Ϊ���
    INT(1);                 //TOUCH_INT���Ϊ1
    CT_RST(0);              //��λ
    delay_ms(20);
    CT_RST(1);              //�ͷŸ�λ
    delay_ms(10);
    INT_DIR(0);
    delay_ms(100);
    temp[0]=0;
    //��ȡ�汾�ţ��ο�ֵ��0x3003
    FT5206_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);
    if((temp[0]==0X30&&temp[1]==0X03)||temp[1]==0X01||temp[1]==0X02) { //�汾:0X3003/0X0001/0X0002
    	xil_printf("CTP ID:%x\r\n",((u16)temp[0]<<8)+temp[1]);
        FT5206_WR_Reg(FT_DEVIDE_MODE,temp,1);   //������������ģʽ
        FT5206_WR_Reg(FT_ID_G_MODE,temp,1);     //��ѯģʽ
        temp[0]=22;                             //������Чֵ��22��ԽСԽ����
        FT5206_WR_Reg(FT_ID_G_THGROUP,temp,1);  //���ô�����Чֵ
        temp[0]=12;                             //�������ڣ�����С��12�����14
        FT5206_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1);
        return 0;
    }
    else  //7�����Ĵ���IC����GTϵ�еģ�����������Ҫ����
    {
        GT9147_RD_Reg(GT_PID_REG,temp,4); //��ȡ��ƷID
        temp[4]=0;
        printf("CTP ID:%s\r\n",temp);  //��ӡID
        memcpy(CIP,temp,sizeof(u8)*4);
        temp[0]=0X02;
        GT9147_WR_Reg(GT_CTRL_REG,temp,1);  //��λGT9XXX
        GT9147_RD_Reg(GT_CFGS_REG,temp,1);  //��ȡGT_CFGS_REG�Ĵ���
        delay_ms(10);
        temp[0]=0X00;
        GT9147_WR_Reg(GT_CTRL_REG,temp,1); //������λ, ���������״̬
        return 0;
    }
    return 1;
}

const u16 FT5206_TPX_TBL[5]= {FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};

//GT911����GT9xxϵ�У�����ֱ�ӵ���gt9147����غ궨��͵�����غ���
const u16 GT911_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};
uint8_t g_gt_tnum = 5;      //Ĭ��֧�ֵĴ���������(5�㴥��)

//ɨ�败����(���ò�ѯ��ʽ)
//mode:0,����ɨ��.
//����ֵ:��ǰ����״̬.
//0,�����޴���;1,�����д���
u8 FT5206_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	static u8 t=0;//���Ʋ�ѯ���,�Ӷ�����CPUռ����
	t++;
    u16 tempsta;

	if((t%10)==0||t<10)//����ʱ,ÿ����10��CTP_Scan�����ż��1��,�Ӷ���ʡCPUʹ����
	{
        if(strcmp((char *)CIP,"911")==0) //����IC 911
        {
            GT9147_RD_Reg(GT_GSTID_REG,&mode,1);      //��ȡ������״̬
            if((mode&0X80)&&((mode&0XF)<=g_gt_tnum))
            {
                i = 0;
                GT9147_WR_Reg(GT_GSTID_REG,&i,1); /* ���־ */
            }
        }
        else    //����IC FT5206
        {
            FT5206_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//��ȡ�������״̬
        }

		if((mode&0XF)&&((mode&0XF)<=g_gt_tnum))
		{
			temp=0XFF<<(mode&0XF);//����ĸ���ת��Ϊ1��λ��,ƥ��tp_dev.sta����
            tempsta=tp_dev.sta;    //���浱ǰ��tp_dev.staֵ
            tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES;
            tp_dev.x[g_gt_tnum-1]=tp_dev.x[0];//���津��0������,���������һ����
            tp_dev.y[g_gt_tnum-1]=tp_dev.y[0];

            delay_ms(3); //��Ҫ����ʱ,��������Ϊ�а�������.

			for(i=0;i<g_gt_tnum;i++)
			{
				if(tp_dev.sta&(1<<i))		//������Ч?
				{
                    if(strcmp((char *)CIP,"911")==0) //����IC 911
                    {
                        GT9147_RD_Reg(GT911_TPX_TBL[i],buf,4);   //��ȡXY����ֵ
                        if(tp_dev.touchtype&0X01) //����
                        {
                            tp_dev.y[i]=(((u16)buf[3]<<8)+buf[2]);
                            tp_dev.x[i]=(((u16)buf[1]<<8)+buf[0]);
                        }
                        else
                        {
                            tp_dev.x[i]=vd_mode.height - (((u16)buf[3]<<8)+buf[2]);
                            tp_dev.y[i]=(((u16)buf[1]<<8)+buf[0]);
                            xil_printf("x=%d,y=%d\n",tp_dev.x[i],tp_dev.y[i]);
                        }
                    }
                    else
                    {
                        FT5206_RD_Reg(FT5206_TPX_TBL[i],buf,4);	//��ȡXY����ֵ
                        if(tp_dev.touchtype&0X01)//����
                        {
                            tp_dev.y[i]=((u16)(buf[0]&0X0F)<<8)+buf[1];
                            tp_dev.x[i]=((u16)(buf[2]&0X0F)<<8)+buf[3];
                        }else
                        {
                            tp_dev.x[i]=vd_mode.height-(((u16)(buf[0]&0X0F)<<8)+buf[1]);
                            tp_dev.y[i]=((u16)(buf[2]&0X0F)<<8)+buf[3];
                        }
                    }
 					//printf("x[%d]:%d,y[%d]:%d\r\n",i,tp_dev.x[i],i,tp_dev.y[i]);
				}
			}

			res=1;
			if(tp_dev.x[0]==0 && tp_dev.y[0]==0)
				mode=0;	//���������ݶ���0,����Դ˴�����
			t=0;		//����һ��,��������������10��,�Ӷ����������
		}
	}

    if(strcmp((char *)CIP,"911")==0) //����IC 911
    {
        if((mode&0X8F)==0X80) //�޴����㰴��
        {
            if(tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
            {
                tp_dev.sta&=~TP_PRES_DOWN; //��ǰ����ɿ�
            }
            else						//֮ǰ��û�б�����
            {
                tp_dev.x[0]=0xffff;
                tp_dev.y[0]=0xffff;
                tp_dev.sta&=0XE0;	//�������Ч���
            }
        }
    }
    else
    {
        if((mode&0X1F)==0) //�޴����㰴��
        {
            if(tp_dev.sta&TP_PRES_DOWN)	//֮ǰ�Ǳ����µ�
            {
                tp_dev.sta&=~TP_PRES_DOWN; //��ǰ����ɿ�
            }
            else						//֮ǰ��û�б�����
            {
                tp_dev.x[0]=0xffff;
                tp_dev.y[0]=0xffff;
                tp_dev.sta&=0XE0;	//�������Ч���
            }
        }
    }

	if(t>240)t=10;//���´�10��ʼ����
	return res;
}
