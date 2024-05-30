/*
 * common.c
 *
 *  Created on: 30 May 2024
 *      Author: joran
 */

#include "common.h"

uint16_t bg_color = 0x9696;

void setBgColor(uint16_t color){
	bg_color = color;
}

uint16_t getBgColor(){
	return bg_color;
}

