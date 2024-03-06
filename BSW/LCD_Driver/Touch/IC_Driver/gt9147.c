// ****************************************Copyright (c)***********************************//
//閸樼喎鐡欓崫銉ユ躬缁炬寧鏆�鐎涳箑閽╅崣甯窗www.yuanzige.com
//閹讹拷閺堫垱鏁幐渚婄窗www.openedv.com
//濞ｆê鐤傛惔妤呮懙閿涙ttp://openedv.taobao.com
//閸忚櫕鏁炲顔讳繆閸忣兛绱獮鍐插酱瀵邦喕淇婇崣鍑ょ窗"濮濓絿鍋ｉ崢鐔风摍"閿涘苯鍘ょ拹纭呭箯閸欐湶YNQ & FPGA & STM32 & LINUX鐠у嫭鏋￠妴锟�
//閻楀牊娼堥幍锟介張澶涚礉閻╂澧楄箛鍛敀閵嗭拷
//Copyright(C) 濮濓絿鍋ｉ崢鐔风摍 2018-2028
//All rights reserved
//----------------------------------------------------------------------------------------
// File name:           gt9147
// Last modified Date:  2019/07/26 16:04:03
// Last Version:        V1.0
// Descriptions:        4.3鐎靛摜鏁哥�圭袝閹界鐫�-GT9147妞瑰崬濮╂禒锝囩垳
//----------------------------------------------------------------------------------------
// Created by:          濮濓絿鍋ｉ崢鐔风摍
// Created date:        2019/07/26 16:04:07
// Version:             V1.0
// Descriptions:        The original version
//
//----------------------------------------------------------------------------------------
//****************************************************************************************//
#include "lcd.h"
#include "gt9147.h"
#include "touch.h"
#include "myiic.h"
#include "delay.h"
#include "string.h"
#include "emio_iic_cfg.h"

//GT9147闁板秶鐤嗛崣鍌涙殶鐞涳拷
//缁楊兛绔存稉顏勭摟閼哄倷璐熼悧鍫熸拱閸欙拷(0X60),韫囧懘銆忔穱婵婄槈閺傛壆娈戦悧鍫熸拱閸欏嘲銇囨禍搴ｇ搼娴滃定T9147閸愬懘鍎�
//flash閸樼喐婀侀悧鍫熸拱閸欙拷,閹靛秳绱伴弴瀛樻煀闁板秶鐤�.
const u8 GT9147_CFG_TBL[]= {
    0X60,0XE0,0X01,0X20,0X03,0X05,0X35,0X00,0X02,0X08,
    0X1E,0X08,0X50,0X3C,0X0F,0X05,0X00,0X00,0XFF,0X67,
    0X50,0X00,0X00,0X18,0X1A,0X1E,0X14,0X89,0X28,0X0A,
    0X30,0X2E,0XBB,0X0A,0X03,0X00,0X00,0X02,0X33,0X1D,
    0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X32,0X00,0X00,
    0X2A,0X1C,0X5A,0X94,0XC5,0X02,0X07,0X00,0X00,0X00,
    0XB5,0X1F,0X00,0X90,0X28,0X00,0X77,0X32,0X00,0X62,
    0X3F,0X00,0X52,0X50,0X00,0X52,0X00,0X00,0X00,0X00,
    0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
    0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X0F,
    0X0F,0X03,0X06,0X10,0X42,0XF8,0X0F,0X14,0X00,0X00,
    0X00,0X00,0X1A,0X18,0X16,0X14,0X12,0X10,0X0E,0X0C,
    0X0A,0X08,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
    0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
    0X00,0X00,0X29,0X28,0X24,0X22,0X20,0X1F,0X1E,0X1D,
    0X0E,0X0C,0X0A,0X08,0X06,0X05,0X04,0X02,0X00,0XFF,
    0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
    0X00,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,
    0XFF,0XFF,0XFF,0XFF,
};
//閸欐垿锟戒笩T9147闁板秶鐤嗛崣鍌涙殶
//mode:0,閸欏倹鏆熸稉宥勭箽鐎涙ê鍩宖lash
//     1,閸欏倹鏆熸穱婵嗙摠閸掔櫞lash
u8 GT9147_Send_Cfg(u8 mode)
{
    u8 buf[2];
    u8 i=0;
    buf[0]=0;
    buf[1]=mode;    //閺勵垰鎯侀崘娆忓弳閸掔檸T9147 FLASH?  閸楄櫕妲搁崥锔藉竴閻㈠吀绻氱�涳拷
    for(i=0; i<sizeof(GT9147_CFG_TBL); i++)
        buf[0]+=GT9147_CFG_TBL[i]; //鐠侊紕鐣婚弽锟犵崣閸滐拷
    buf[0]=(~buf[0])+1;
    GT9147_WR_Reg(GT_CFGS_REG,(u8*)GT9147_CFG_TBL,sizeof(GT9147_CFG_TBL));//閸欐垿锟戒礁鐦庣�涙ê娅掗柊宥囩枂
    GT9147_WR_Reg(GT_CHECK_REG,buf,2);//閸愭瑥鍙嗛弽锟犵崣閸滐拷,閸滃矂鍘ょ純顔芥纯閺傜増鐖ｇ拋锟�
    return 0;
}
//閸氭厯T9147閸愭瑥鍙嗘稉锟藉▎鈩冩殶閹癸拷
//reg:鐠у嘲顫愮�靛嫬鐡ㄩ崳銊ユ勾閸э拷
//buf:閺佺増宓佺紓鎾剁处鐎涙ê灏�
//len:閸愭瑦鏆熼幑顕�鏆辨惔锟�
//鏉╂柨娲栭崐锟�:0,閹存劕濮�;1,婢惰精瑙�.
u8 GT9147_WR_Reg(u16 reg,u8 *buf,u8 len)
{
    u8 i;
    u8 ret=0;
    IIC_Start();
    IIC_Send_Byte(GT_CMD_WR);   //閸欐垿锟戒礁鍟撻崨鎴掓姢
    IIC_Wait_Ack();
    IIC_Send_Byte(reg>>8);      //閸欐垿锟戒線鐝�8娴ｅ秴婀撮崸锟�
    IIC_Wait_Ack();
    IIC_Send_Byte(reg&0XFF);    //閸欐垿锟戒椒缍�8娴ｅ秴婀撮崸锟�
    IIC_Wait_Ack();
    for(i=0; i<len; i++) {
        IIC_Send_Byte(buf[i]);  //閸欐垶鏆熼幑锟�
        ret=IIC_Wait_Ack();
        if(ret)
            break;
    }
    IIC_Stop();                 //娴溠呮晸娑擄拷娑擃亜浠犲銏℃蒋娴狅拷
    return ret;
}
//娴犲定T9147鐠囪鍤稉锟藉▎鈩冩殶閹癸拷
//reg:鐠у嘲顫愮�靛嫬鐡ㄩ崳銊ユ勾閸э拷
//buf:閺佺増宓佺紓鎾剁处鐎涙ê灏�
//len:鐠囩粯鏆熼幑顕�鏆辨惔锟�
void GT9147_RD_Reg(u16 reg,u8 *buf,u8 len)
{
    u8 i;
    IIC_Start();
    IIC_Send_Byte(GT_CMD_WR);   //閸欐垿锟戒礁鍟撻崨鎴掓姢
    IIC_Wait_Ack();
    IIC_Send_Byte(reg>>8);      //閸欐垿锟戒線鐝�8娴ｅ秴婀撮崸锟�
    IIC_Wait_Ack();
    IIC_Send_Byte(reg&0XFF);    //閸欐垿锟戒椒缍�8娴ｅ秴婀撮崸锟�
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte(GT_CMD_RD);   //閸欐垿锟戒浇顕伴崨鎴掓姢
    IIC_Wait_Ack();
    for(i=0; i<len; i++) {
        buf[i]=IIC_Read_Byte(i==(len-1)?0:1); //閸欐垶鏆熼幑锟�
    }
    IIC_Stop();//娴溠呮晸娑擄拷娑擃亜浠犲銏℃蒋娴狅拷
}
//閸掓繂顫愰崠鏈慣9147鐟欙附鎳滅仦锟�
//鏉╂柨娲栭崐锟�:0,閸掓繂顫愰崠鏍ㄥ灇閸旓拷;1,閸掓繂顫愰崠鏍с亼鐠愶拷
u8 GT9147_Init(void)
{
    u8 temp[5];
    INT_DIR(1);  //TOUCH_INT瀵洝鍓肩拋鍓х枂娑撻缚绶崙锟�
    INT(1);      //TOUCH_INT鏉堟挸鍤稉锟�1
    IIC_Init();         //閸掓繂顫愰崠鏍暩鐎圭懓鐫嗛惃鍑�2C閹崵鍤�
    CT_RST(0);              //婢跺秳缍�
    delay_ms(10);
    CT_RST(1);              //闁插﹥鏂佹径宥勭秴
    delay_ms(10);
    INT_DIR(0);
    delay_ms(100);
    GT9147_RD_Reg(GT_PID_REG,temp,4);//鐠囪褰囨禍褍鎼D
    temp[4]=0;
    xil_printf("CTP ID:%s\r\n",temp);   //閹垫挸宓僆D
    if(strcmp((char*)temp,"9147")==0) { //ID==9147
        temp[0]=0X02;
        GT9147_WR_Reg(GT_CTRL_REG,temp,1);//鏉烆垰顦叉担宀筎9147
        GT9147_RD_Reg(GT_CFGS_REG,temp,1);//鐠囪褰嘒T_CFGS_REG鐎靛嫬鐡ㄩ崳锟�
        if(temp[0]<0X60) { //姒涙顓婚悧鍫熸拱濮ｆ棁绶濇担锟�,闂囷拷鐟曚焦娲块弬鐧磍ash闁板秶鐤�
            if(lcd_id==0X5510)
                GT9147_Send_Cfg(1);//娴狅拷4.3鐎电CU鐏烇拷,閺囧瓨鏌婇獮鏈电箽鐎涙﹢鍘ょ純锟�
        }
        delay_ms(10);
        temp[0]=0X00;
        GT9147_WR_Reg(GT_CTRL_REG,temp,1);//缂佹挻娼径宥勭秴
        return 0;
    }
    return 0;
}

const u16 GT9147_TPX_TBL[5]= {GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};


//閹殿偅寮跨憴锔芥嚋鐏烇拷(闁插洨鏁ら弻銉嚄閺傜懓绱�)
//mode:0,濮濓絽鐖堕幍顐ｅ伎.
//鏉╂柨娲栭崐锟�:瑜版挸澧犵憴锕�鐫嗛悩鑸碉拷锟�.
//0,鐟欙箑鐫嗛弮鐘盒曢幗锟�;1,鐟欙箑鐫嗛張澶幮曢幗锟�
u8 GT9147_Scan(u8 mode)
{
	u8 buf[4];
	u8 i=0;
	u8 res=0;
	u16 temp;
	u16 tempsta;
 	static u8 t=0;//閹貉冨煑閺屻儴顕楅梻鎾,娴犲氦锟藉矂妾锋担宥U閸楃姷鏁ら悳锟�
	t++;
	if((t%10)==0||t<10)//缁屾椽妫介弮锟�,濮ｅ繗绻橀崗锟�10濞嗩搲TP_Scan閸戣姤鏆熼幍宥嗩梾濞达拷1濞嗭拷,娴犲氦锟藉矁濡惇涓哖U娴ｈ法鏁ら悳锟�
	{
		GT9147_RD_Reg(GT_GSTID_REG,&mode,1);	//鐠囪褰囩憴锔芥嚋閻愬湱娈戦悩鑸碉拷锟�
 		if(mode&0X80&&((mode&0XF)<6))
		{
			i=0;
			GT9147_WR_Reg(GT_GSTID_REG,&i,1);//濞撳懏鐖ｈ箛锟�
		}
		if((mode&0XF)&&((mode&0XF)<6))
		{
			temp=0XFFFF<<(mode&0XF);	//鐏忓棛鍋ｉ惃鍕嚋閺佹媽娴嗛幑顫礋1閻ㄥ嫪缍呴弫锟�,閸栧綊鍘p_dev.sta鐎规矮绠�
			tempsta=tp_dev.sta;			//娣囨繂鐡ㄨぐ鎾冲閻ㄥ墖p_dev.sta閸婏拷
			tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES;
			tp_dev.x[4]=tp_dev.x[0];	//娣囨繂鐡ㄧ憴锔惧仯0閻ㄥ嫭鏆熼幑锟�
			tp_dev.y[4]=tp_dev.y[0];
			for(i=0;i<5;i++)
			{
				if(tp_dev.sta&(1<<i))	//鐟欙附鎳滈張澶嬫櫏?
				{
					GT9147_RD_Reg(GT9147_TPX_TBL[i],buf,4);	//鐠囪褰嘪Y閸ф劖鐖ｉ崐锟�
                    if(lcd_id==0X4384)   //4.3鐎碉拷800*480 RGB鐏烇拷
                    {
                        if(tp_dev.touchtype&0X01)//濡亜鐫�
                        {
                            tp_dev.x[i]=((u16)buf[1]<<8)+buf[0];
                            tp_dev.y[i]=((u16)buf[3]<<8)+buf[2];

                        }else
                        {
                            tp_dev.y[i]=((u16)buf[1]<<8)+buf[0];
                            tp_dev.x[i]=480-(((u16)buf[3]<<8)+buf[2]);
                        }
                    }else if(lcd_id==0X4342) //4.3鐎碉拷480*272 RGB鐏烇拷
                    {
                        if(tp_dev.touchtype&0X01)//濡亜鐫�
                        {
                            tp_dev.x[i]=(((u16)buf[1]<<8)+buf[0]);
                            tp_dev.y[i]=(((u16)buf[3]<<8)+buf[2]);
                        }else
                        {
                            tp_dev.y[i]=((u16)buf[1]<<8)+buf[0];
                            tp_dev.x[i]=272-(((u16)buf[3]<<8)+buf[2]);
                        }
                    }
				}
			}
			res=1;
			if(tp_dev.x[0] > vd_mode.height || tp_dev.y[0] > vd_mode.width)  //闂堢偞纭堕弫鐗堝祦(閸ф劖鐖ｇ搾鍛毉娴滐拷)
			{
				if((mode&0XF)>1)		//閺堝鍙炬禒鏍仯閺堝鏆熼幑锟�,閸掓瑥顦茬粭顑跨癌娑擃亣袝閻愬湱娈戦弫鐗堝祦閸掓壆顑囨稉锟芥稉顏囆曢悙锟�.
				{
					tp_dev.x[0]=tp_dev.x[1];
					tp_dev.y[0]=tp_dev.y[1];
					t=0;				//鐟欙箑褰傛稉锟藉▎锟�,閸掓瑤绱伴張锟界亸鎴ｇ箾缂侇厾娲冨ù锟�10濞嗭拷,娴犲氦锟藉本褰佹妯烘嚒娑擃厾宸�
				}else					//闂堢偞纭堕弫鐗堝祦,閸掓瑥鎷烽悾銉︻劃濞嗏剝鏆熼幑锟�(鏉╂ê甯崢鐔告降閻拷)
				{
					tp_dev.x[0]=tp_dev.x[4];
					tp_dev.y[0]=tp_dev.y[4];
					mode=0X80;
					tp_dev.sta=tempsta;	//閹垹顦瞭p_dev.sta
				}
			}else t=0;					//鐟欙箑褰傛稉锟藉▎锟�,閸掓瑤绱伴張锟界亸鎴ｇ箾缂侇厾娲冨ù锟�10濞嗭拷,娴犲氦锟藉本褰佹妯烘嚒娑擃厾宸�
		}
	}
	if((mode&0X8F)==0X80)//閺冪姾袝閹藉摜鍋ｉ幐澶夌瑓
	{
		if(tp_dev.sta&TP_PRES_DOWN)		//娑斿澧犻弰顖濐潶閹稿绗呴惃锟�
		{
			tp_dev.sta&=~TP_PRES_DOWN;	//閺嶅洩顔囬幐澶愭暛閺夋儳绱�
		}else							//娑斿澧犵亸杈ㄧ梾閺堝顫﹂幐澶夌瑓
		{
			tp_dev.x[0]=0xffff;
			tp_dev.y[0]=0xffff;
			tp_dev.sta&=0XE000;			//濞撳懘娅庨悙瑙勬箒閺佸牊鐖ｇ拋锟�
		}
	}
	if(t>240)t=10;//闁插秵鏌婃禒锟�10瀵拷婵顓搁弫锟�
	return res;
}
