/*
 * TouchMenu.h
 *
 *  Created on: 2024Äê2ÔÂ4ÈÕ
 *      Author: Administrator
 */

#ifndef SRC_ASW_INFODISPLAY_TOUCHMENU_H_
#define SRC_ASW_INFODISPLAY_TOUCHMENU_H_

#include "TouchKey.h"


typedef struct
{
	TouchKeyType Prev;


	TouchKeyType Next;


}TouchMenuManagerType;

extern void TouchMenu_Init(void);

extern void TouchMenu_Run200ms(void); /**/

#endif /* SRC_ASW_INFODISPLAY_TOUCHMENU_H_ */
