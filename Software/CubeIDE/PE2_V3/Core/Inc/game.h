/*
 * game.h
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include <stdint.h>
#include "spaceInvaders.h"
#include "brickbreaker.h"
#include "z_displ_ILI9XXX.h"
#include "stm32f3xx_hal.h"
//Configs
#define EVENTSIZE 100



//Enums
enum keys { left=1, up, right, down, action};


//Function declaration
    void init (void);
    int loop (int);
    void updateScreen(void);


#endif /* INC_GAME_H_ */
