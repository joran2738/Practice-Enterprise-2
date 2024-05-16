/*
 * menu.h
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */

#ifndef INC_MENU_H_
#define INC_MENU_H_

#include <stdint.h>
#include "common.h"
#include "z_displ_ILI9XXX.h"

#define BUTTON_SIZE 64

#define SPACESHIP_WIDTH 5
#define SPACESHIP_HEIGHT 5

enum gameState{notPlay,inPlay,paused,menu};

extern uint8_t choice;
extern uint8_t last_choice_pause;


void displayPauseMenu(uint8_t);
void displayMenu(void);
void displayMenuIcons();



#endif /* INC_MENU_H_ */
