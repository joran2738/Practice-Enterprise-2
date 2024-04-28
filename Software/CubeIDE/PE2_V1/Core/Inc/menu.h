/*
 * menu.h
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include <stdint.h>

#define BUTTON_OFFSET 20
#define BUTTON_SIZE 16
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 60

#define SPACESHIP_WIDTH 5
#define SPACESHIP_HEIGHT 5

#define ycord_offset ((SCREEN_HEIGHT / 2) - (BUTTON_SIZE / 2) - 2)
#define xplay_offset ((BUTTON_OFFSET + 2) + ((BUTTON_SIZE + 4) * 1) + BUTTON_SIZE/4)
#define xback_offset ((BUTTON_OFFSET + 2) + BUTTON_SIZE/4)

enum gameState{notPlay,inPlay,paused,menu};

extern uint8_t choice;

void displayPauseMenu(uint32_t game_screen[][SCREEN_HEIGHT]);
void displayMenu(uint32_t game_screen[][SCREEN_HEIGHT]);
void displayMenuIcons(uint32_t game_screen[][SCREEN_HEIGHT]);



#endif /* INC_MENU_H_ */
