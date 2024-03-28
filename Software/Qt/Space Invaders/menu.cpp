#include "menu.h"



void displayPauseMenu(uint32_t game_screen[][SCREEN_HEIGHT],menu_State state){
    int ycord_offset = (SCREEN_HEIGHT / 2) - (BUTTON_SIZE / 2) - 2;
    int xplay_offset = (BUTTON_OFFSET + 2) + ((BUTTON_SIZE + 4) * 1) + BUTTON_SIZE/4;
    int xback_offset = (BUTTON_OFFSET + 2) + BUTTON_SIZE/4;
    int xcord_offset;
    if(state == none){
        choice = 1;
    }
    xcord_offset = (BUTTON_OFFSET + 2) + ((BUTTON_SIZE + 4) * choice);

    //box around
    for(int i = BUTTON_OFFSET; i < SCREEN_WIDTH - BUTTON_OFFSET; i++){
        for(int j = ycord_offset; j < ycord_offset + BUTTON_SIZE; j++){
            game_screen[i][j] = 0x969696;
        }
    }

    //selector
    for(int i = xcord_offset; i < xcord_offset + BUTTON_SIZE; i++){
        for(int j = ycord_offset; j < ycord_offset + BUTTON_SIZE; j++){
            game_screen[i][j] = 0xFFFF0000;
        }
    }

    //play
    for (int i = 0; i < BUTTON_SIZE - 2; i++){
        for (int j = i; j < BUTTON_SIZE - i; j++){
            game_screen[xplay_offset + i][ycord_offset + j] = 0x02fa3c;
        }
    }

    // go back
    for (int i = 0; i < BUTTON_SIZE - 2; i++){
        for (int j = i; j < BUTTON_SIZE - i; j++){
            game_screen[xback_offset + i][ycord_offset + j] = 0x02fa3c;
        }
    }

}
