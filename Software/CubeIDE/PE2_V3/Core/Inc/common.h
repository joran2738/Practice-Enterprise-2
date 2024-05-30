/*
 * common.h
 *
 *  Created on: May 3, 2024
 *      Author: joran
 */

#ifndef INC_COMMON_H_
#define INC_COMMON_H_

#include "stdint.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 60
#define RANDOM(MIN, MAX) (rand() % ((MAX) - (MIN) + 1) + (MIN))


typedef struct {
    int8_t x;
    int8_t y;
} point;

void setBgColor(uint16_t);

uint16_t getBgColor(void);


#endif /* INC_COMMON_H_ */
