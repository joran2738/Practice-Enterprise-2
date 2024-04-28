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

void displayPauseMenu(uint32_t game_screen[][SCREEN_HEIGHT]){
    int xcord_offset = (BUTTON_OFFSET + 2) + ((BUTTON_SIZE + 4) * choice);

    //box around
    for(int i = BUTTON_OFFSET; i < SCREEN_WIDTH - BUTTON_OFFSET; i++){
        for(int j = ycord_offset; j < ycord_offset + BUTTON_SIZE; j++){
            game_screen[i][j] = GRAY;
        }
    }

    //selector
    for(int i = xcord_offset; i < xcord_offset + BUTTON_SIZE; i++){
        for(int j = ycord_offset; j < ycord_offset + BUTTON_SIZE; j++){
            game_screen[i][j] = RED;
        }
    }

    //play
    for (int i = 0; i < BUTTON_SIZE - 2; i++){
        for (int j = i; j < BUTTON_SIZE - i; j++){
            game_screen[xplay_offset + i][ycord_offset + j] = LIGHT_GREEN;
        }
    }

    // go back
    for (int i = 0; i < BUTTON_SIZE - 2; i++){
        for (int j = i; j < BUTTON_SIZE - i; j++){
            game_screen[xback_offset + i][ycord_offset + j] = LIGHT_GREEN;
        }
    }
}

void displayMenu(uint32_t game_screen[][SCREEN_HEIGHT]){
    int y_offset = 0;
    int x_offset = 0;
    //box around
    for(int i = 0; i < total_games + 1 && i < 8; i++){
        for (int j = 3; j < 3 + BUTTON_SIZE; j++){
            for (int k = 3; k < 3 + BUTTON_SIZE; k++){
                if(i == choice){
                    game_screen[x_offset + j][y_offset + k] = RED;
                }else{
                    game_screen[x_offset + j][y_offset + k] = GRAY;
                }

            }
        }


        if((i + 1)% 4 == 0){
            x_offset = 0;
            y_offset += BUTTON_SIZE + 3;
            continue;
        }
        x_offset += BUTTON_SIZE + 3;
    }
    displayMenuIcons(game_screen);

    // text

    for (int i = 0; i < SCREEN_WIDTH; i++){
        for (int j = SCREEN_HEIGHT - 16; j < SCREEN_HEIGHT; j++){
            game_screen[i][j] = BLACK;
        }
    }

    switch (choice) {
    case 0:
        displayText(game_screen,"Multiplayer", 2, SCREEN_HEIGHT - 16, WHITE);
        break;
    case 1:
        displayText(game_screen,"Brick Breaker", 2, SCREEN_HEIGHT - 16, WHITE);
        break;
    case 2:
        displayText(game_screen,"Space Invaders", 2, SCREEN_HEIGHT - 16, WHITE);
        break;
    default:
        break;
    }
}

void displayMenuIcons(uint32_t game_screen[][SCREEN_HEIGHT]){

    // multiplayer
    if(connected){
        for(int j = 5; j <= BUTTON_SIZE; j++){
            game_screen[j][5] = LIGHT_GREEN;
        }
        for(int j = 1; j < 3; j++){
            game_screen[5][2 + j] = LIGHT_GREEN;
            game_screen[BUTTON_SIZE][2 + j] = LIGHT_GREEN;
        }
        for (int j = 0; j < 2; j++){
            for(int k = 7; k < BUTTON_SIZE; k += 2){
                game_screen[2 + j + BUTTON_SIZE/2][k] = LIGHT_GREEN;
            }

        }
    }
    for(int j = 5; j <= BUTTON_SIZE; j++){
        game_screen[j][BUTTON_SIZE] = LIGHT_GREEN;
    }
    for(int j = 1; j < 3; j++){
        game_screen[5][BUTTON_SIZE + j] = LIGHT_GREEN;
        game_screen[BUTTON_SIZE][BUTTON_SIZE + j] = LIGHT_GREEN;
    }

    for (int j = 0; j < 2; j++){
        for(int k = 6; k < BUTTON_SIZE; k += 2){
            game_screen[2 + j + BUTTON_SIZE/2][k] = LIGHT_GREEN;
        }

    }

    // brick
    for(int j = 10; j < 15; j++){
        game_screen[BUTTON_SIZE + j][BUTTON_SIZE] = LIGHT_GREEN;
    }
    for(int j = 5; j < BUTTON_SIZE; j++){
        game_screen[BUTTON_SIZE + 7][j] = LIGHT_GREEN;
        game_screen[BUTTON_SIZE * 2 + 4][j] = LIGHT_GREEN;
    }
    game_screen[BUTTON_SIZE + 10][BUTTON_SIZE - 5] = LIGHT_GREEN;

    //spaceinvaders
    uint8_t spaceShipX = BUTTON_SIZE * 2 + 15;
    uint8_t spaceShipY = BUTTON_SIZE - 2;

    for (int i = spaceShipX - SPACESHIP_WIDTH / 2; i <= spaceShipX + SPACESHIP_WIDTH / 2; i++) {
        for (int j = spaceShipY - SPACESHIP_HEIGHT / 2; j <= spaceShipY + SPACESHIP_HEIGHT / 2; j++) {
            if (i == spaceShipX && j != spaceShipY + SPACESHIP_HEIGHT / 2){
                game_screen[i][j] = LIGHT_GREEN;
            }else if((i == spaceShipX - SPACESHIP_WIDTH / 2 || i == spaceShipX + SPACESHIP_WIDTH / 2) && j != spaceShipY - SPACESHIP_HEIGHT / 2){
                game_screen[i][j] = LIGHT_GREEN;
            }else if((i == spaceShipX || spaceShipX ) && j == spaceShipY - (SPACESHIP_HEIGHT / 2) + 2){
                game_screen[i][j] = LIGHT_GREEN;
            }
        }
    }
    for (int i = 0; i < 3; i++){
        for (int j = 6; j < 9; j++){
            game_screen[BUTTON_SIZE * 3 + i][j] = LIGHT_GREEN;
        }
    }



}

