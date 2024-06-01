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
#include "common.h"

int total_games = 2;
extern uint8_t connected;
uint8_t last_choice_menu = 40;


void displayMultiplayerIcon(void);
void displayBricksIcon(void);
void displaySpaceIcon(void);

void resetLastChoiceMenu(void){
	last_choice_menu = 40;
}

void displayPauseMenu(uint8_t clear){
	uint16_t back_color = GREY;
	uint16_t continue_color = RED;
	uint16_t triangle_color = WHITE;
	if(choice != last_choice_pause || clear){
		last_choice_pause = choice;
		if(choice == 0){
			back_color = RED;
			continue_color = GREY;
		}
		else if(clear){
			back_color = getBgColor();
			continue_color = getBgColor();
			triangle_color = getBgColor();
		}
		Displ_FillArea(SCREEN_WIDTH * 2 - BUTTON_SIZE - 12, SCREEN_HEIGHT * 2 - BUTTON_SIZE / 2, BUTTON_SIZE, BUTTON_SIZE, back_color);
		Displ_FillArea(SCREEN_WIDTH * 2 + 12, SCREEN_HEIGHT * 2 - BUTTON_SIZE / 2, BUTTON_SIZE, BUTTON_SIZE, continue_color);

		Displ_fillTriangle(SCREEN_WIDTH * 2 + 20, SCREEN_HEIGHT * 2 - BUTTON_SIZE / 2 + 8, SCREEN_WIDTH * 2 + 20, SCREEN_HEIGHT * 2 + BUTTON_SIZE / 2 - 8, SCREEN_WIDTH * 2 + 20 + BUTTON_SIZE - 16, SCREEN_HEIGHT * 2, triangle_color);
		Displ_fillTriangle(SCREEN_WIDTH * 2 - 20, SCREEN_HEIGHT * 2 - BUTTON_SIZE / 2 + 8, SCREEN_WIDTH * 2 - 20, SCREEN_HEIGHT * 2 + BUTTON_SIZE / 2 - 8, SCREEN_WIDTH * 2 - 20 - BUTTON_SIZE + 16, SCREEN_HEIGHT * 2, triangle_color);
	}

}

void displayMenu(){
	int y_offset = 12;
	int x_offset = 12;
	//box around
	if(choice != last_choice_menu){
		last_choice_menu =  choice;
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
    switch (choice) {
    case 0:
    	Displ_WString(8, SCREEN_HEIGHT * 4 - 40, "Multiplayer   ", Font20, 1, WHITE, DARK_GREY);
        break;
    case 1:
    	Displ_WString(8, SCREEN_HEIGHT * 4 - 40, "Brick Breaker ", Font20, 1, WHITE, DARK_GREY);
        break;
    case 2:
    	Displ_WString(8, SCREEN_HEIGHT * 4 - 40, "Space Invaders", Font20, 1, WHITE, DARK_GREY);
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
	uint16_t color = LIGHT_GREEN;
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
