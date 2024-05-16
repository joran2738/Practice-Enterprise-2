/*
 * brickbreaker.h
 *
 *  Created on: 16 mei 2024
 *      Author: vande
 */

#ifndef INC_BRICKBREAKER_H_
#define INC_BRICKBREAKER_H_

//config

#include <stdint.h>
#include "common.h"

#define EVENTSIZE 100
#define BAR_SIZE 50
#define BRICK_LINES 10
#define MAX_BRICK_LINES 48
#define BRICK_SPEED 140

//Enums

typedef enum {N, NE, E, SE, S, SW, W, NW, still} directions;

//typedefs

//typedef struct {
//    int x;
//    int y;
//} point;

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
void printScreen();
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

#endif /* INC_BRICKBREAKER_H_ */
