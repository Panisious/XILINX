/*
 * lcd.h
 *
 *  Created on: 2024骞�1鏈�27鏃�
 *      Author: Administrator
 */

#ifndef SRC_LCD_DRIVER_LCD_LCD_H_
#define SRC_LCD_DRIVER_LCD_LCD_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ZYNQ_BasicType.h"

#include "xaxivdma.h"
#include "xgpio.h"

#include "emio_iic_cfg.h"
#include "vdma_api.h"
#include "display_ctrl.h"
#include "lcd_modes.h"
#include "touch.h"

#include "font.h"

#include "RTE.h"

#define BYTES_PIXEL        3                          //閸嶅繒绀岀�涙濡弫甯礉RGB888閸楋拷3娑擃亜鐡ч懞锟�
#define FRAME_BUFFER_NUM   3
#define DYNCLK_BASEADDR    XPAR_AXI_DYNCLK_0_BASEADDR //閸斻劍锟戒焦妞傞柦鐔风唨閸︽澘娼�
#define VDMA_ID            XPAR_AXIVDMA_0_DEVICE_ID   //VDMA閸ｃ劋娆D
#define DISP_VTC_ID        XPAR_VTC_0_DEVICE_ID       //VTC閸ｃ劋娆D
#define AXI_GPIO_0_ID      XPAR_AXI_GPIO_1_DEVICE_ID  //PL缁旓拷  AXI GPIO 0(lcd_id)閸ｃ劋娆D
#define AXI_GPIO_0_CHANEL  1                          //PL閹稿鏁担璺ㄦ暏AXI GPIO(lcd_id)闁岸浜�1

#define FRAME_BUFF_ADDR			(0x10000000 + 0x01000000)

#define MLCD_WHITE        0XFFFF
#define MLCD_BLACK        0X0000
#define MLCD_BLUE         0X001F
#define MLCD_BRED         0XF81F
#define MLCD_GRED         0XFFE0
#define MLCD_GBLUE        0X07FF
#define MLCD_RED          0XF800
#define MLCD_MAGENTA      0XF81F
#define MLCD_GREEN        0X07E0
#define MLCD_CYAN         0X7FFF
#define MLCD_YELLOW       0XFFE0
#define MLCD_BROWN        0XBC40 //閿熸枻鎷疯壊
#define MLCD_BRRED        0XFC07 //閿熸埅鐚存嫹鑹�
#define MLCD_GRAY         0X8430 //閿熸枻鎷疯壊

#define MLCD_DARKBLUE     0X01CF //閿熸枻鎷烽敓鏂ゆ嫹鑹�
#define MLCD_LIGHTBLUE    0X7D7C //娴呴敓鏂ゆ嫹鑹�
#define MLCD_GRAYBLUE     0X5458 //閿熸枻鎷烽敓鏂ゆ嫹鑹�

#define MLCD_LIGHTGREEN   0X841F //娴呴敓鏂ゆ嫹鑹�
#define MLCD_LGRAY        0XC618 //娴呴敓鏂ゆ嫹鑹�(PANNEL),閿熸枻鎷烽敓钘夎儗閿熸枻鎷疯壊

#define MLCD_LGRAYBLUE    0XA651 //娴呴敓鏂ゆ嫹閿熸枻鎷疯壊(閿熷彨纭锋嫹閿熸枻鎷烽敓缂达拷)
#define MLCD_LBBLUE       0X2B12 //娴呴敓鏂ゆ嫹閿熸枻鎷疯壊(閫夐敓鏂ゆ嫹閿熸枻鎷风洰閿熶茎鍑ゆ嫹鑹�)

extern const uint32 frame_buffer_addr;
extern uint32 		lcd_id;
extern VideoMode    vd_mode;


void LCD_Init(void);
void LCD_Run50ms(void);

void LCD_SetBackGroundColor24(uint32 color);
void LCD_SetBackGroundColor(uint8 r, uint8 g, uint8 b);
void LCD_SetPointColor24(uint32 color);
void LCD_SetPointColor(uint8 r, uint8 g, uint8 b);
void LCD_Clear(void);

void LCD_FillArea(uint16 sx, uint16 sy, uint16 ex, uint16 ey);
void LCD_ClearArea(uint16 sx, uint16 sy, uint16 ex, uint16 ey);
void LCD_DrawPoint(uint16 x, uint16 y, uint8 r, uint8 g, uint8 b);
void LCD_DrawLine(uint16 sx, uint16 sy, uint16 ex, uint16 ey);
void LCD_DrawChar16(uint16 sx, uint16 sy, uint8 ch);
void LCD_DrawString2(uint16 sx, uint16 sy, uint8 *str, u16 dx);
void LCD_DrawString(uint16 sx, uint16 sy, uint8 *str);

#endif /* SRC_LCD_DRIVER_LCD_LCD_H_ */
