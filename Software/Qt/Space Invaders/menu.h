#ifndef MENU_H
#define MENU_H

#include <stdint.h>

#define BUTTON_OFFSET 20
#define BUTTON_SIZE 16
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 60

enum menu_State {in,none};

extern uint8_t choice;

void displayPauseMenu(uint32_t game_screen[][SCREEN_HEIGHT],menu_State);


#endif // MENU_H
