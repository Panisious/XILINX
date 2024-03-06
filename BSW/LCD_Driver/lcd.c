/*
 * lcd.c
 *
 *  Created on: 2024年1月27日
 *      Author: Administrator
 */

#include "lcd.h"

XAxiVdma     vdma;
DisplayCtrl  dispCtrl;
XGpio        axi_gpio_inst;
VideoMode    vd_mode;
uint32 lcd_id = 0;

static uint8 BG_DEFAULT = 0x00;
static uint8 BG_R, BG_G, BG_B;
static uint8 PT_R, PT_G, PT_B;

const uint32 frame_buffer_addr = FRAME_BUFF_ADDR;


void LCD_Init(void)
{
	XGpio_Initialize(&axi_gpio_inst,AXI_GPIO_0_ID);                //GPIO初始化
	XGpio_SetDataDirection(&axi_gpio_inst,AXI_GPIO_0_CHANEL,0x07); //设置AXI GPIO为输入
	lcd_id = LTDC_PanelID_Read(&axi_gpio_inst,AXI_GPIO_0_CHANEL);  //获取LCD的ID
	XGpio_SetDataDirection(&axi_gpio_inst,AXI_GPIO_0_CHANEL,0x00); //设置AXI GPIO为输出
	xil_printf("LCD ID: %x\r\n",lcd_id);
	switch(lcd_id){
		case 0x4342 : vd_mode = VMODE_480x272; break;  //4.3寸屏,480*272分辨率
		case 0x4384 : vd_mode = VMODE_800x480; break;  //4.3寸屏,800*480分辨率
		case 0x7084 : vd_mode = VMODE_800x480; break;  //7寸屏,800*480分辨率
		case 0x7016 : vd_mode = VMODE_1024x600; break; //7寸屏,1024*600分辨率
		case 0x1018 : vd_mode = VMODE_1280x800; break; //10.1寸屏,1280*800分辨率
		default : vd_mode = VMODE_800x480; break;
	}
	emio_init();
	run_vdma_frame_buffer(&vdma, VDMA_ID, vd_mode.width, vd_mode.height,
							frame_buffer_addr,0, 0,ONLY_READ);

	LCD_Clear();

	DisplayInitialize(&dispCtrl, DISP_VTC_ID, DYNCLK_BASEADDR);
	//设置VideoMode
	DisplaySetMode(&dispCtrl, &vd_mode);
	DisplayStart(&dispCtrl);

	LCD_SetPointColor24(0x00FF00);
	LCD_SetBackGroundColor24(0x000000);

	tp_dev.init();
}

void LCD_SetBackGroundColor24(uint32 color)
{
	BG_R = (uint8)(color >> 16);
	BG_G = (uint8)(color >> 8);
	BG_B = (uint8)(color >> 0);
}

void LCD_SetBackGroundColor(uint8 r, uint8 g, uint8 b)
{
	BG_R = r;
	BG_G = g;
	BG_B = b;
}

void LCD_SetPointColor24(uint32 color)
{
	PT_R = (uint8)(color >> 16);
	PT_G = (uint8)(color >> 8);
	PT_B = (uint8)(color >> 0);
}

void LCD_SetPointColor(uint8 r, uint8 g, uint8 b)
{
	PT_R = r;
	PT_G = g;
	PT_B = b;
}

void LCD_Clear(void)
{
	memset((u8*)frame_buffer_addr, BG_DEFAULT, vd_mode.height*vd_mode.width*FRAME_BUFFER_NUM*BYTES_PIXEL);
	Xil_DCacheFlush();
}

void LCD_DrawPoint(uint16 x, uint16 y, uint8 r, uint8 g, uint8 b)
{
	static uint8 (*frame)[480][800][3] = FRAME_BUFF_ADDR;
	(*frame)[y][x][0] = b;
	(*frame)[y][x][1] = g;
	(*frame)[y][x][2] = r;
}

void LCD_FillArea(uint16 sx, uint16 sy, uint16 ex, uint16 ey)
{
	uint16 x,y;
	for(y=sy; y<ey; y++)
	{
		for(x = sx; x < ex; x++)
		{
			LCD_DrawPoint(x, y, PT_R, PT_G, PT_B);
		}
	}
}

void LCD_ClearArea(uint16 sx, uint16 sy, uint16 ex, uint16 ey)
{
	uint16 x,y;
	for(y = sy; y <= ey; y++)
	{
		for(x = sx; x <= ex; x++)
		{
			LCD_DrawPoint(x, y, BG_R, BG_G, BG_B);
		}
	}
}

void LCD_DrawLine(uint16 sx, uint16 sy, uint16 ex, uint16 ey)
{
	uint16 x = sx;
	uint16 y = sy;
	float k = (float)(ey - sy) / (float)(ex - sx);
	float kinv = 1.0 / k;
	float yk = (float)y;
	float xk = (float)x;
	if(k < 0.5)
	{
		if(ex > sx)
		{
			for(; x <= ex ; x++)
			{
				y = (uint16)yk;
				LCD_DrawPoint(x, y, PT_R, PT_G, PT_B);
				yk += k;
			}
		}
		else
		{
			for(; x >= ex ; x--)
			{
				y = (uint16)yk;
				LCD_DrawPoint(x, y, PT_R, PT_G, PT_B);
				yk += k;
			}
		}
	}
	else
	{
		if(ey > sy)
		{
			for(; y <= ey ; y++)
			{
				x = (uint16)xk;
				LCD_DrawPoint(x, y, PT_R, PT_G, PT_B);
				xk += kinv;
			}
		}
		else
		{
			for(; y >= ey ; y--)
			{
				x = (uint16)xk;
				LCD_DrawPoint(x, y, PT_R, PT_G, PT_B);
				xk += kinv;
			}
		}
	}
}

void LCD_DrawChar16(uint16 sx, uint16 sy, uint8 ch)
{
	int i, j, x, y;

	for(i=0; i<16; i++)
	{
		y = sy + i;
		for(j=0; j<8; j++)
		{
			x = sx + j;
			if(Font_16_8[ch][i] & (1<<j))
			{
				LCD_DrawPoint(x, y, PT_R, PT_G, PT_B);
			}
			else
			{
				LCD_DrawPoint(x, y, BG_R, BG_G, BG_B);
			}
		}
	}
}

void LCD_DrawString2(uint16 sx, uint16 sy, uint8 *str, u16 dx)
{
	int x=sx, y=sy;
	uint8 ch = *str;
	while(ch)
	{
		if(ch == '\r')
			x = sx;
		else if(ch == '\n')
			y += 20;
		else if(ch == '\t')
		{
			x = sx + 80 * ((x-sx)/80 + 1);
		}
		else
		{
			LCD_DrawChar16(x,y,ch);
			x+=8;
		}
		if(x>800-8)
		{
			x=sx;
			y+=20;
		}
		if(y>480-16)
		{
			return;
		}
		str++;
		ch = *str;
	}
	while(x - sx < dx)
	{
		LCD_DrawChar16(x,y,0);
		x+=8;
	}
}

void LCD_DrawString(uint16 sx, uint16 sy, uint8 *str)
{
	int x=sx, y=sy;
	uint8 ch = *str;
	while(ch)
	{
		if(ch == '\r')
			x = sx;
		else if(ch == '\n')
			y += 20;
		else if(ch == '\t')
		{
			x = sx + 80 * ((x-sx)/80 + 1);
		}
		else
		{
			LCD_DrawChar16(x,y,ch);
			x+=8;
		}
		if(x>800-8)
		{
			x=sx;
			y+=20;
		}
		if(y>480-16)
		{
			return;
		}
		str++;
		ch = *str;
	}
}

void LCD_Run50ms(void)
{
	sint16 x, y;
	static sint16 x_first = 0, y_first = 0;
	sint16 dx, dy;
	static uint8 firstTouch = 0;
	tp_dev.scan();

	/* 调整方向 */
	x = tp_dev.y[0];
	y = 480 - tp_dev.x[0];

	if(tp_dev.sta & 1)
	{
		RTE_Write_LCDTouchCoordinate(x, y);
		if(firstTouch == 0)
		{
			x_first = x;
			y_first = y;
			firstTouch = 1;
			RTE_Write_LCDTouchState(TRUE);
		}
		else
		{
			dx = x - x_first;
			dy = y_first - y;
			RTE_Write_LCDTouchDxDy(dx, dy);
		}
	}
	else
	{
		firstTouch = 0;
		dx = 0;
		dy = 0;
		RTE_Write_LCDTouchState(FALSE);
		RTE_Write_LCDTouchDxDy(dx, dy);
	}
}
