//This code is meant to be plain c code so it can be used on an STM32 microcontroller.

#include <stdint.h>
#include "debug.h"
#include "game.h"
#include "displayText.h"

uint8_t eventlist[EVENTSIZE];
uint8_t eventindex = 0;
uint32_t game_screen[SCREEN_WIDTH][SCREEN_HEIGHT];
static point person = {SCREEN_WIDTH/2, SCREEN_HEIGHT/2};

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
        if(person.x < 0) person.x = 0;
    }
    if(key == up){
        person.y--;
        if(person.y < 0) person.y = 0;
    }
    if(key == right){
        person.x++;
        if(person.x > 255) person.x = 255;
    }
    if(key == down){
        person.y++;
        if(person.y > 255) person.y = 255;
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
