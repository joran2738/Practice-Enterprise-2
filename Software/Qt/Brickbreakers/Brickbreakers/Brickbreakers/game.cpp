#include <QDateTime>
#include "debug.h"
#include "game.h"
#include "displayText.h"
#include "brickbreaker.h"

uint8_t eventlist[EVENTSIZE];
uint8_t eventindex = 0;
uint32_t game_screen[SCREEN_WIDTH][SCREEN_HEIGHT];

point person = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 5};
ballPoint ball = {(SCREEN_WIDTH)/2, SCREEN_HEIGHT - 6, pause};
uint8_t start = 0;
uint8_t guardianTimer = 100;
uint8_t delay = BRICK_SPEED;
uint8_t guardian = 0;

void init (void) {

    updateScreen();
}

void loop (void) {
    int key = readInput();
    if(key == left) {
        person.x--;
        if(person.x < BAR_SIZE/2) {
            person.x = (BAR_SIZE/2);
        }
        if (ball.dir == pause) {
            ball.x--;
        }
    }
    if(key == right) {
        person.x++;
        if(person.x > SCREEN_WIDTH - (BAR_SIZE/2) - 1) {
            person.x = SCREEN_WIDTH - (BAR_SIZE/2) - 1;
        }
        if (ball.dir == pause) {
            ball.x++;
        }
    }
    if(key == action) {
        if (ball.dir == pause) {
            ball.dir = NE;
            start = 1;

        }
    }

    playBall();

    if (start == 1 && ball.dir != pause) {
        if (delay <= 0) {
            lowerBricks();
            delay = BRICK_SPEED;
        } else {
            delay--;
        }
        checkGameOver();
    }

    if (guardian == 1) {
        if (guardianTimer <= 0) {
            guardian = 0;
            guardianTimer = 100;
        } else {
            guardianTimer--;
        }
    }
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
            game_screen[x][y] = GREEN;
        }
    }
    printScreen(game_screen);
}
