/*
 * menu.c
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */

#include "menu.h"
#include "displayText.h"
#include <stdint.h>
#include "debug.h"

int total_games = 2;
extern uint8_t connected;
uint8_t last_choice = 5;

void displayMultiplayerIcon(void);
void displayBricksIcon(void);
void displaySpaceIcon(void);

void displayPauseMenu(uint8_t clear){
	uint16_t back_color = GREY;
	uint16_t continue_color = RED;
	if(choice == 0){
		back_color = RED;
		continue_color = GREY;
	}
	else if(clear){
		back_color = DARK_GREY;
		continue_color = DARK_GREY;
	}
	Displ_FillArea(SCREEN_WIDTH * 2 - BUTTON_SIZE - 12, SCREEN_HEIGHT * 2 - BUTTON_SIZE / 2, BUTTON_SIZE, BUTTON_SIZE, back_color);
	Displ_FillArea(SCREEN_WIDTH * 2 + 12, SCREEN_HEIGHT * 2 - BUTTON_SIZE / 2, BUTTON_SIZE, BUTTON_SIZE, continue_color);

	Displ_fillTriangle(SCREEN_WIDTH * 2 + 20, SCREEN_HEIGHT * 2 - BUTTON_SIZE / 2 + 8, SCREEN_WIDTH * 2 + 20, SCREEN_HEIGHT * 2 + BUTTON_SIZE / 2 - 8, SCREEN_WIDTH * 2 + 20 + BUTTON_SIZE - 8, SCREEN_HEIGHT * 2, WHITE);

}

void displayMenu(){
	int y_offset = 12;
	int x_offset = 12;
	//box around
	if(choice != last_choice){
		last_choice =  choice;
		for(int i = 0; i < total_games + 1 && i < 8; i++){
			if(i == choice){
				Displ_FillArea(x_offset, y_offset,BUTTON_SIZE , BUTTON_SIZE, RED);
			}else{
				Displ_FillArea(x_offset, y_offset,BUTTON_SIZE , BUTTON_SIZE, GREY);
			}if((i + 1)% 4 == 0){
				x_offset = 0;
				y_offset += BUTTON_SIZE + 12;
				continue;
			}
			x_offset += BUTTON_SIZE + 12;
		}
	}
    displayMenuIcons();

    // text
/*
    for (int i = 0; i < SCREEN_WIDTH; i++){
        for (int j = SCREEN_HEIGHT - 16; j < SCREEN_HEIGHT; j++){
            game_screen[i][j] = ENC_BLACK;
        }
    }
*/
    switch (choice) {
    case 0:
        //displayText(game_screen,"Multiplayer", 2, SCREEN_HEIGHT - 16, WHITE);
        break;
    case 1:
        //displayText(game_screen,"Brick Breaker", 2, SCREEN_HEIGHT - 16, WHITE);
        break;
    case 2:
        //displayText(game_screen,"Space Invaders", 2, SCREEN_HEIGHT - 16, WHITE);
        break;
    default:
        break;
    }
}

void displayMenuIcons(){

    // multiplayer
	displayMultiplayerIcon();

    // brick
	displayBricksIcon();

    //spaceinvaders
	displaySpaceIcon();
}

void displayMultiplayerIcon(){
	if(connected){
		Displ_FillArea(20, 20, BUTTON_SIZE - 16, 4, LIGHT_GREEN);
		Displ_FillArea(20, 12, 4, 8, LIGHT_GREEN);
		Displ_FillArea(BUTTON_SIZE, 12, 4, 8, LIGHT_GREEN);
		Displ_FillArea(BUTTON_SIZE/2 + 8, 24, 8, BUTTON_SIZE - 24, LIGHT_GREEN);
	}
	Displ_FillArea(20, BUTTON_SIZE, BUTTON_SIZE - 16, 4, LIGHT_GREEN);
	Displ_FillArea(20, BUTTON_SIZE+4, 4, 8, LIGHT_GREEN);
	Displ_FillArea(BUTTON_SIZE, BUTTON_SIZE+4, 4, 8, LIGHT_GREEN);
	if(!connected){
		for(int k = 24; k < BUTTON_SIZE; k += 8){
			Displ_FillArea(BUTTON_SIZE/2 + 8, k, 8, 4, LIGHT_GREEN);
		}
	}
}

void displayBricksIcon(){
	Displ_FillArea(40+BUTTON_SIZE, BUTTON_SIZE, 20, 4, LIGHT_GREEN);
	Displ_FillArea(28+BUTTON_SIZE, 12, 4, BUTTON_SIZE, LIGHT_GREEN);
	Displ_FillArea((8+BUTTON_SIZE)*2, 12, 4, BUTTON_SIZE, LIGHT_GREEN);
	Displ_FillArea(40+BUTTON_SIZE, BUTTON_SIZE - 20, 4, 4, LIGHT_GREEN);
}

void displaySpaceIcon(){
	Displ_FillArea((BUTTON_SIZE+12)*3 - 48, BUTTON_SIZE -12, 4, 16, LIGHT_GREEN);
	Displ_FillArea((BUTTON_SIZE+12)*3 - 32, BUTTON_SIZE -12, 4, 16, LIGHT_GREEN);
	Displ_FillArea((BUTTON_SIZE+12)*3 - 44, BUTTON_SIZE -8, 12, 4, LIGHT_GREEN);
	Displ_FillArea((BUTTON_SIZE+12)*3 - 40, BUTTON_SIZE -16, 4, 16, LIGHT_GREEN);

	Displ_FillArea((BUTTON_SIZE+12)*3 - 36, BUTTON_SIZE -40, 12, 12, LIGHT_GREEN);
}
