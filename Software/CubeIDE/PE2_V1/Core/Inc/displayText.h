/*
 * displayText.h
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */

#ifndef INC_DISPLAYTEXT_H_
#define INC_DISPLAYTEXT_H_

#include <stdint.h>
#include "ledFont5x8.h"

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 60

#define RED 0xFFFF0000
#define GREEN 0xFF00FF00
#define BLUE 0xFF0000FF
#define LIGHT_BLUE 0xFF02f7db
#define BLACK 0xFF000000
#define DARK_GRAY 0xFF2E2C2C
#define YELLOW 0xFFEDE609
#define GRAY 0xFF969696
#define LIGHT_GREEN 0xFF02FA3C
#define WHITE 0xFFFFFFFF

void displayText(uint32_t[][SCREEN_HEIGHT], char*, int, int, uint32_t);


#endif /* INC_DISPLAYTEXT_H_ */
