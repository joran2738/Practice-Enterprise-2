//This code is meant to be plain c code so it can be used on an STM32 microcontroller.

#include <stdint.h>
#include "debug.h"
#include "game.h"
#include "displayText.h"

uint8_t eventlist[EVENTSIZE];
uint8_t eventindex = 0;
uint32_t game_screen[SCREEN_WIDTH][SCREEN_HEIGHT];
static snake person = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2, pause};

void init (void){
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            game_screen[x][y] = 0xFF00FF00; //Green
        }
    }
}

void loop (void){
    int key = readInput();
    if(key == left){
        person.x--;
        if(person.x < 0) person.x = SCREEN_WIDTH - 1;
    }
    if(key == up){
        person.y--;
        if(person.y < 0) person.y = SCREEN_HEIGHT - 1;
    }
    if(key == right){
        person.x++;
        if(person.x >= SCREEN_WIDTH) person.x = 0;
    }
    if(key == down){
        person.y++;
        if(person.y >= SCREEN_HEIGHT) person.y = 0;
    }
    updateScreen();
}

int readInput()
{
    int newKey = 0;
    if(eventindex > 0){
        newKey = eventlist[0];
        //QD << newKey << eventindex;
        for(int i=0; i < eventindex; i++){
            eventlist[i] = eventlist[i+1];
        }
        eventindex--;
    }
    return newKey;
}

void updateScreen()
{
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            if(x == person.x && y == person.y){
                game_screen[x][y] = RED;
            }else{
                game_screen[x][y] = GREEN;
            }
        }
    }
}
