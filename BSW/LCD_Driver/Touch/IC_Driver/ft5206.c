//****************************************Copyright (c)***********************************//
//原子哥在线教学平台：www.yuanzige.com
//技术支持：www.openedv.com
//淘宝店铺：http://openedv.taobao.com
//关注微信公众平台微信号："正点原子"，免费获取ZYNQ & FPGA & STM32 & LINUX资料。
//版权所有，盗版必究。
//Copyright(C) 正点原子 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           ft5206.c
// Last modified Date:  2019/07/26 15:59:12
// Last Version:        V1.0
// Descriptions:        7寸电容触摸屏-FT5206/FT5426 驱动代码
//----------------------------------------------------------------------------------------
// Created by:          正点原子
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

//向FT5206写入一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:写数据长度
//返回值:0,成功;1,失败.
u8 FT5206_WR_Reg(u16 reg,u8 *buf,u8 len)
{
    u8 i;
    u8 ret=0;
    IIC_Start();
    IIC_Send_Byte(FT_CMD_WR);   //发送写命令
    IIC_Wait_Ack();
    IIC_Send_Byte(reg&0XFF);    //发送低8位地址
    IIC_Wait_Ack();
    for(i=0; i<len; i++) {
        IIC_Send_Byte(buf[i]);  //发数据
        ret=IIC_Wait_Ack();
        if(ret)break;
    }
    IIC_Stop();                 //产生一个停止条件
    return ret;
}

//从FT5206读出一次数据
//reg:起始寄存器地址
//buf:数据缓缓存区
//len:读数据长度
void FT5206_RD_Reg(u16 reg,u8 *buf,u8 len)
{
    u8 i;
    IIC_Start();
    IIC_Send_Byte(FT_CMD_WR);                 //发送写命令
    IIC_Wait_Ack();
    IIC_Send_Byte(reg&0XFF);                  //发送低8位地址
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(FT_CMD_RD);                 //发送读命令
    IIC_Wait_Ack();
    for(i=0; i<len; i++) {
        buf[i] = IIC_Read_Byte( i == (len-1) ? 0 : 1 ); //发数据
    }
    IIC_Stop();//产生一个停止条件
}

u8 CIP[5]; //用来存放触摸IC-GT911
//初始化FT5206触摸屏
//返回值:0,初始化成功;1,初始化失败
u8 FT5206_Init(void)
{
    u8 temp[5];
    IIC_Init();             //初始化电容屏的I2C总线
    INT_DIR(1);             //TOUCH_INT引脚设置为输出
    INT(1);                 //TOUCH_INT输出为1
    CT_RST(0);              //复位
    delay_ms(20);
    CT_RST(1);              //释放复位
    delay_ms(10);
    INT_DIR(0);
    delay_ms(100);
    temp[0]=0;
    //读取版本号，参考值：0x3003
    FT5206_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);
    if((temp[0]==0X30&&temp[1]==0X03)||temp[1]==0X01||temp[1]==0X02) { //版本:0X3003/0X0001/0X0002
    	xil_printf("CTP ID:%x\r\n",((u16)temp[0]<<8)+temp[1]);
        FT5206_WR_Reg(FT_DEVIDE_MODE,temp,1);   //进入正常操作模式
        FT5206_WR_Reg(FT_ID_G_MODE,temp,1);     //查询模式
        temp[0]=22;                             //触摸有效值，22，越小越灵敏
        FT5206_WR_Reg(FT_ID_G_THGROUP,temp,1);  //设置触摸有效值
        temp[0]=12;                             //激活周期，不能小于12，最大14
        FT5206_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1);
        return 0;
    }
    else  //7寸屏的触摸IC还有GT系列的，所以这里需要兼容
    {
        GT9147_RD_Reg(GT_PID_REG,temp,4); //读取产品ID
        temp[4]=0;
        printf("CTP ID:%s\r\n",temp);  //打印ID
        memcpy(CIP,temp,sizeof(u8)*4);
        temp[0]=0X02;
        GT9147_WR_Reg(GT_CTRL_REG,temp,1);  //软复位GT9XXX
        GT9147_RD_Reg(GT_CFGS_REG,temp,1);  //读取GT_CFGS_REG寄存器
        delay_ms(10);
        temp[0]=0X00;
        GT9147_WR_Reg(GT_CTRL_REG,temp,1); //结束复位, 进入读坐标状态
        return 0;
    }
    return 1;
}

const u16 FT5206_TPX_TBL[5]= {FT_TP1_REG,FT_TP2_REG,FT_TP3_REG,FT_TP4_REG,FT_TP5_REG};

//GT911属于GT9xx系列，所以直接调用gt9147的相关宏定义和调用相关函数
const u16 GT911_TPX_TBL[5]={GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};
uint8_t g_gt_tnum = 5;      //默认支持的触摸屏点数(5点触摸)

//扫描触摸屏(采用查询方式)
//mode:0,正常扫描.
//返回值:当前触屏状态.
//0,触屏无触摸;1,触屏有触摸
u8 FT5206_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u8 temp;
	static u8 t=0;//控制查询间隔,从而降低CPU占用率
	t++;
    u16 tempsta;

	if((t%10)==0||t<10)//空闲时,每进入10次CTP_Scan函数才检测1次,从而节省CPU使用率
	{
        if(strcmp((char *)CIP,"911")==0) //触摸IC 911
        {
            GT9147_RD_Reg(GT_GSTID_REG,&mode,1);      //读取触摸点状态
            if((mode&0X80)&&((mode&0XF)<=g_gt_tnum))
            {
                i = 0;
                GT9147_WR_Reg(GT_GSTID_REG,&i,1); /* 清标志 */
            }
        }
        else    //触摸IC FT5206
        {
            FT5206_RD_Reg(FT_REG_NUM_FINGER,&mode,1);//读取触摸点的状态
        }

		if((mode&0XF)&&((mode&0XF)<=g_gt_tnum))
		{
			temp=0XFF<<(mode&0XF);//将点的个数转换为1的位数,匹配tp_dev.sta定义
            tempsta=tp_dev.sta;    //保存当前的tp_dev.sta值
            tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES;
            tp_dev.x[g_gt_tnum-1]=tp_dev.x[0];//保存触点0的数据,保存在最后一个上
            tp_dev.y[g_gt_tnum-1]=tp_dev.y[0];

            delay_ms(3); //必要的延时,否则老认为有按键按下.

			for(i=0;i<g_gt_tnum;i++)
			{
				if(tp_dev.sta&(1<<i))		//触摸有效?
				{
                    if(strcmp((char *)CIP,"911")==0) //触摸IC 911
                    {
                        GT9147_RD_Reg(GT911_TPX_TBL[i],buf,4);   //读取XY坐标值
                        if(tp_dev.touchtype&0X01) //横屏
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
                        FT5206_RD_Reg(FT5206_TPX_TBL[i],buf,4);	//读取XY坐标值
                        if(tp_dev.touchtype&0X01)//横屏
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
				mode=0;	//读到的数据都是0,则忽略此次数据
			t=0;		//触发一次,则会最少连续监测10次,从而提高命中率
		}
	}

    if(strcmp((char *)CIP,"911")==0) //触摸IC 911
    {
        if((mode&0X8F)==0X80) //无触摸点按下
        {
            if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
            {
                tp_dev.sta&=~TP_PRES_DOWN; //标记按键松开
            }
            else						//之前就没有被按下
            {
                tp_dev.x[0]=0xffff;
                tp_dev.y[0]=0xffff;
                tp_dev.sta&=0XE0;	//清除点有效标记
            }
        }
    }
    else
    {
        if((mode&0X1F)==0) //无触摸点按下
        {
            if(tp_dev.sta&TP_PRES_DOWN)	//之前是被按下的
            {
                tp_dev.sta&=~TP_PRES_DOWN; //标记按键松开
            }
            else						//之前就没有被按下
            {
                tp_dev.x[0]=0xffff;
                tp_dev.y[0]=0xffff;
                tp_dev.sta&=0XE0;	//清除点有效标记
            }
        }
    }

	if(t>240)t=10;//重新从10开始计数
	return res;
}
