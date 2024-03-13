//This code is meant to be plain c code so it can be used on an STM32 microcontroller.

#include <stdint.h>
#include "debug.h"
#include "game.h"

#define DEBUG 1

uint8_t eventlist[EVENTSIZE];
uint8_t eventindex = 0;
uint32_t game_screen[SCREEN_WIDTH][SCREEN_HEIGHT];
static point person = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 5};
bullets ammo;

uint8_t delay = 0;


void init (void) {
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            game_screen[x][y] = 0xFF00FF00; //Green
        }
    }
    ammo.in_play=0;
    for (int i = 0;i<MAX_BULLETS;i++){
        ammo.bullet[i].x = -1;
        ammo.bullet[i].y = -1;
    }
}

void loop (void) {
    int key = readInput();
    if(key == left) {
        person.x--;
        if(person.x < SPACESHIP_WIDTH/2) {
            person.x = (SPACESHIP_WIDTH/2);
        }
    }
    if(key == right) {
        person.x++;
        if(person.x > SCREEN_WIDTH - (SPACESHIP_WIDTH/2) - 1) {
            person.x = SCREEN_WIDTH - (SPACESHIP_WIDTH/2) - 1;
        }
    }
    if(key == action) {

        if(ammo.in_play < MAX_BULLETS && delay == 0){
            ammo.bullet[ammo.in_play].x = person.x;
            ammo.bullet[ammo.in_play].y = person.y - 3;
            ammo.in_play++;
            delay = MAX_DELAY;
        }
    }
    moveBullets();
    updateScreen();
}

int readInput()
{
    int newKey = 0;
    if(eventindex > 0) {
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

    for(int x = 0; x < SCREEN_WIDTH; x++) {
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            game_screen[x][y] = 0xFF00FF00; //Green
        }
    }

    for (int i = person.x - SPACESHIP_WIDTH / 2; i <= person.x + SPACESHIP_WIDTH / 2; i++) {
        for (int j = person.y - SPACESHIP_HEIGHT / 2; j <= person.y + SPACESHIP_HEIGHT / 2; j++) {
            if (i == person.x && j != person.y + SPACESHIP_HEIGHT / 2){
                game_screen[i][j] = 0xFFFF0000; //RED
            }else if((i == person.x - SPACESHIP_WIDTH / 2 || i == person.x + SPACESHIP_WIDTH / 2) && j != person.y - SPACESHIP_HEIGHT / 2){
                game_screen[i][j] = 0xFFFF0000; //RED
            }else if((i == person.x + 1 || person.x - 1 ) && j == person.y - (SPACESHIP_HEIGHT / 2) + 2){
                game_screen[i][j] = 0xFFFF0000; //RED
            }
        }
    }
    for(int i = 0; i < ammo.in_play; i++){
        game_screen[ammo.bullet[i].x][ammo.bullet[i].y] = 0xFFFF0000; //RED
    }
    if (delay > 0){
        delay--;
    }
}

void moveBullets(){
    for(int i = 0; i < ammo.in_play; i++){
        ammo.bullet[i].y--;
        if (ammo.bullet[i].y < 0){
            for(int j = i; j < ammo.in_play - 1; j++){
                if (j != MAX_BULLETS - 1 ){
                    ammo.bullet[j].x = ammo.bullet[j+1].x;
                    ammo.bullet[j].y = ammo.bullet[j+1].y;
                }
            }
            ammo.in_play--;
        }
    }
}

