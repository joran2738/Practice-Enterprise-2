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
#include "z_displ_ILI9XXX.h"
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

#define HEARTS_Y 8
#define HEARTS_X 8
#define HEARTS_OFFSET 12


//Typedefs
typedef struct {
    int8_t x;
    int8_t y;
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

/**
 * @fn void initSpaceInvaders(void)
 * @brief initialises the spaceinvaders game
 *
 */
void initSpaceInvaders(void);

/**
 * @fn void spawnComet()
 * @brief spawns a comet at a random x-cord with a random size
 * generates a random delay for the spawning of the next comet
 *
 */
void spawnComet();

/**
 * @fn void spawnBullet(int)
 * @brief spawns a bullet at the x-cord of the spaceship
 * reloads the bullet_delay variable, so the user can't spam bullets
 *
 * @param[in] enemy uint8_t: if 1, it spawns an enemy bullet instead of your own bullet
 */
void spawnBullet(int);

/**
 * @fn void spawnStar()
 * @brief spawns a star at a random x-cord
 * generates a random delay for the spawning of the next star
 *
 */
void spawnStar();

/**
 * @fn void moveBullets(void)
 * @brief moves the bullets and displays them on the screen aswell by calling "displayAmmo()"
 *
 */
void moveBullets(void);

/**
 * @fn void moveComets(void)
 * @brief moves the comets and displays them on the screen aswell by calling "displayComets()"
 *
 */
void moveComets(void);

/**
 * @fn void moveStars(void)
 * @brief moves the stars and displays them on the screen aswell by calling "displayStars()"
 *
 */
void moveStars(void);

/**
 * @fn void moveSpaceship(int)
 * @brief moves the spaceship and displays it on the screen aswell by calling "displaySpaceShip()"
 *
 * @param
 */
void moveSpaceship(int);

/**
 * @fn void displayStars(uint16_t)
 * @brief 	displays the stars on the screen
 *
 * @param[in] color uint16_t: the color the star should be displayed with
 */
void displayStars(uint16_t);

/**
 * @fn void displayAmmo(uint16_t)
 * @brief displays the Bullets on the screen
 *
 * @param[in] color uint16_t: the color the ammo should be displayed with
 */
void displayAmmo(uint16_t);

/**
 * @fn void displayComets(uint16_t)
 * @brief displays the comets on the screen
 *
 * @param[in] color uint16_t: the color the comets should be displayed with
 */
void displayComets(uint16_t);

/**
 * @fn void displaySpaceShip(uint16_t)
 * @brief displays the spaceship on the screen
 *
 * @param[in] color uint16_t: the color the spaceship should be displayed with
 */
void displaySpaceShip(uint16_t);

/**
 * @fn void displayLives(void)
 * @brief displays the amount of lives left on the screen
 *
 */
void displayLives(void);

/**
 * @fn void spaceShipHitColorToggle(void)
 * @brief if the spaceship was hit by an enemy bullet or a comet, the spaceship has to blink red/white for a while
 *
 */
void spaceShipHitColorToggle(void);

/**
 * @fn void delayBullet(void)
 * @brief decrements the bullet_delay variable, if 0 the spaceship can shoot a bullet aswell
 *
 */
void delayBullet(void);

//multiplayer
/**
 * @fn void moveEnemyBullets(void)
 * @brief move the enemy bullets
 *
 */
void moveEnemyBullets(void);

/**
 * @fn void send_Bullet(point)
 * @brief if a bullet leaves the screen this function is called
 * if multiplayer is enabled, it sends this bullet's x_cord to the other console
 *
 * @param[in] bullet so the x cord can be send to the other console
 */
void send_Bullet(point);

#endif /* INC_SPACEINVADERS_H_ */
