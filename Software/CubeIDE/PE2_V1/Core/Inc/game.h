/*
 * game.h
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */

#ifndef INC_GAME_H_
#define INC_GAME_H_

#include <stdint.h>
//Configs
#define EVENTSIZE 100
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 60


//Enums
enum keys { left=1, up, right, down, action};


//Function declaration
    void init (void);
    int loop (int);
    void updateScreen(void);


#endif /* INC_GAME_H_ */
