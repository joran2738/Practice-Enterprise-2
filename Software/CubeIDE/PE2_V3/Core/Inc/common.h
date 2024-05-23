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

typedef struct {
    uint8_t x;
    uint8_t y;
} point;

#define RANDOM(MIN, MAX) (rand() % ((MAX) - (MIN) + 1) + (MIN))

#endif /* INC_COMMON_H_ */
