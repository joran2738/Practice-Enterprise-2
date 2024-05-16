/*
 * spaceInvaders.h
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */
#ifndef INC_SPACEINVADERS_H_
#define INC_SPACEINVADERS_H_

#include <stdint.h>
#include "common.h"
//#include <QDateTime> // tic thing

#define SPACESHIP_WIDTH 5
#define SPACESHIP_HEIGHT 5

#define PLAYABLE_OFFSET 5
#define PLAYABLE_MAX 75

#define MAX_BULLETS 6
#define MAX_DELAY_BULLETS 10

#define MAX_COMETS 10
#define MIN_COMET_SIZE 2
#define MAX_COMET_SIZE 4

#define MAX_STARS 10

#define HEARTS_Y 2
#define HEARTS_X 2
#define HEARTS_OFFSET 3


//Typedefs
typedef struct {
    uint8_t x;
    uint8_t y;
} point;

typedef struct {
	uint8_t x;
	uint8_t y;
    int8_t size;
    int8_t coins;
} comet;

typedef struct {
    int8_t in_play;
    point star_ar[MAX_STARS];
} stars;

typedef struct {
    int8_t enemy;
    int8_t in_play;
    point bullet_ar[MAX_BULLETS];
}bullets;

typedef struct {
	int8_t in_play;
    comet comet_ar[MAX_COMETS];
}comets;

//functions
void initSpaceShip(void);

void spawnComet();
void spawnBullet(int);
void spawnStar();

void moveBullets(void);
void moveComets(void);
void moveStars(void);
void moveSpaceship(int);
void hitComet(int,int);

void initAmmo(bullets *, int);
void initComets(void);
void initStars(void);
void initSpaceInvaders(void);

void displayStars(uint8_t game_screen[][SCREEN_HEIGHT]);
void displayAmmo(uint8_t game_screen[][SCREEN_HEIGHT]);
void displayComets(uint8_t game_screen[][SCREEN_HEIGHT]);
void displaySpaceShip(uint8_t game_screen[][SCREEN_HEIGHT]);
void displayLives(uint8_t game_screen[][SCREEN_HEIGHT]);

void spaceShipHitColorToggle(void);
void delayBullet(void);

void beenHit(void);

//multiplayer

void moveEnemyBullets(void);
void send_Bullet(point);

#endif /* INC_SPACEINVADERS_H_ */
