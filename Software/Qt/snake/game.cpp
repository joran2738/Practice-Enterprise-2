//This code is meant to be plain c code so it can be used on an STM32 microcontroller.

#include <stdint.h>
#include "debug.h"
#include "game.h"

uint8_t eventlist[EVENTSIZE];
uint8_t eventindex = 0;
uint32_t game_screen[SCREEN_WIDTH][SCREEN_HEIGHT];
static point person = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 5};
static ballPoint ball = {(SCREEN_WIDTH)/2, SCREEN_HEIGHT - 6, pause};
static brick bricks[10];
uint8_t start = 0;


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
        if(person.x < BAR_SIZE/2) {
            person.x = (BAR_SIZE/2);
        }
        if (ball.dir == pause) {
            ball.x--;
        }
    }
    if(key == right){
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
            ball.dir = NW;
            start = 1;

        }
    }
    playBall();

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
    if (start == 0) {
        for(int i = 0; i < 10; i++) {
            bricks[i].x = i*5;
            bricks[i].y = 2;
            bricks[i].visible = 1;
        }
    }
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            game_screen[x][y] = 0xFF00FF00; //Green
        }
    }

    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            if(x == person.x && y == person.y){
                for(int i = x - BAR_SIZE/2; i <= x + BAR_SIZE/2; i++) {
                    game_screen[i][y] = 0xFFFF0000; //red
                }
            }
            if(x == ball.x && y == ball.y) {
                game_screen[x][y] = 0xFF0000FF; //blue
            }
        }
    }

    for(int i = 0; i < 10; i++) {
        if(bricks[i].visible == 1) {
            for(int l = 0; l < 5; l++) {
                game_screen[bricks[i].x + l][bricks[i].y] = 0xFF000000; //black
            }
        }
    }

}

void playBall() {
    if (ball.y == 0) {
        switch (ball.dir) {
            case N:
                ball.dir = S;
                break;
            case NW:
                ball.dir = SW;
                break;
            case NE:
                ball.dir = SE;
                break;
            default:
                QD << "You shouldn't be here!";
        }
    }
    if (ball.y == SCREEN_HEIGHT - 6) {
        if (ball.x >= person.x - (BAR_SIZE/2) && ball.x <= person.x + (BAR_SIZE/2)) {
            switch (ball.dir) {
                case S:
                    ball.dir = N;
                    break;
                case SE:
                    ball.dir = NE;
                    break;
                case SW:
                    ball.dir = NW   ;
                    break;
                default:
                    QD << "You shouldn't be here!";
            }
        } else {
            ball = {(person.x), SCREEN_HEIGHT - 6, pause};
            start = 0;
        }
    }
    if (ball.x == 0) {
        switch (ball.dir) {
            case NW:
                ball.dir = NE;
                break;
            case SW:
                ball.dir = SE;
                break;
            default:
                QD << "You shouldn't be here!";
        }
    }
    if (ball.x == SCREEN_WIDTH - 1) {
        switch (ball.dir) {
            case NE:
                ball.dir = NW;
                break;
            case SE:
                ball.dir = SW;
                break;
            default:
                QD << "You shouldn't be here!";
        }
    }

    if (ball.y == 5) {
        for (int i = 0; i < 10; i++) {
            if (bricks[ball.x/5].visible == 1) {
                bricks[ball.x/5].visible = 0;
            }
        }
    }

    switch (ball.dir) {
        case N:
            ball.y -= 1;
            break;
        case S:
            ball.y += 1;
            break;
        case NE:
            ball.y -= 1;
            ball.x += 1;
            break;
        case NW:
            ball.y -= 1;
            ball.x -= 1;
            break;
        case SE:
            ball.y += 1;
            ball.x += 1;
            break;
        case SW:
            ball.y += 1;
            ball.x -= 1;
            break;
        case pause:
            ball.y = ball.y;
            ball.x = ball.x;
            break;
        default:
            QD << "You shouldn't be here!";
    }
}
