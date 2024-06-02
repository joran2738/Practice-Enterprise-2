#ifndef BRICKBREAKER_H
#define BRICKBREAKER_H

//config

#include <stdint.h>

#define EVENTSIZE 100
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 60
#define BAR_SIZE 79
#define BRICK_LINES 10
#define MAX_BRICK_LINES 48
#define BRICK_SPEED 140

//Enums

enum directions {N, NE, E, SE, S, SW, W, NW, pause};

//typedefs

typedef struct {
    int x;
    int y;
} point;

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


//function declarations

void initBrickbreaker(void);
void printScreen(uint32_t game_screen[][SCREEN_HEIGHT]);
void playBall(void);
void moveBall(void);
void changeDirection(directions);
void gameEnd(void);
void lowerBricks(void);
void checkGameOver(void);
void checkBrickHit(void);
void dropPowerUp(int, int);
void checkGuardianTimer(void);
void explodeBricks(int, int);
int setBrickToughness(void);

#endif // BRICKBREAKER_H
