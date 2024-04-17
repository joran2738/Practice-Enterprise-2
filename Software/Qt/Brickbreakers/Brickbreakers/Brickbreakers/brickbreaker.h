#ifndef BRICKBREAKER_H
#define BRICKBREAKER_H

#include "game.h"

//config

#define BAR_SIZE 17
#define BRICK_LINES 10
#define MAX_BRICK_LINES 38
#define BRICK_SPEED 140

//function declarations

void playBall(void);
void moveBall(void);
void changeDirection(directions);
void gameEnd(void);
void lowerBricks(void);
void checkGameOver(void);
void checkBrickHit(void);
void dropPowerUp(int, int);

//typedefs

typedef struct {
    int x;
    int y;
    directions dir;
} ballPoint;

typedef struct {
    int x;
    int y;
    int visible;
    int toughness;
} brick;

#endif // BRICKBREAKER_H
