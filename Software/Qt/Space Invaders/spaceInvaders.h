#ifndef SPACEINVADERS_H
#define SPACEINVADERS_H

#include <stdint.h>
#include <QDateTime>

#define SPACESHIP_WIDTH 5
#define SPACESHIP_HEIGHT 5

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 60
#define PLAYABLE_OFFSET 5
#define PLAYABLE_MAX 75

#define MAX_BULLETS 20
#define MAX_DELAY_BULLETS 10

#define MAX_COMETS 20
#define MIN_COMET_SIZE 2
#define MAX_COMET_SIZE 4

#define MAX_STARS 20

#define HEARTS_Y 2
#define HEARTS_X 2
#define HEARTS_OFFSET 3


//Typedefs
typedef struct {
    int x;
    int y;
} point;

typedef struct {
    int x;
    int y;
    int size;
    int coins;
} comet;

typedef struct {
    int in_play;
    point star_ar[MAX_STARS];
} stars;

typedef struct {
    int enemy;
    int in_play;
    point bullet_ar[MAX_BULLETS];
}bullets;

typedef struct {
    int in_play;
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

void displayStars(uint32_t game_screen[][SCREEN_HEIGHT]);
void displayAmmo(uint32_t game_screen[][SCREEN_HEIGHT]);
void displayComets(uint32_t game_screen[][SCREEN_HEIGHT]);
void displaySpaceShip(uint32_t game_screen[][SCREEN_HEIGHT]);
void displayLives(uint32_t game_screen[][SCREEN_HEIGHT]);

void spaceShipHitColorToggle(void);
void delayBullet(void);

void beenHit(void);

//multiplayer

void moveEnemyBullets(void);
void send_Bullet(point);


#endif // SPACEINVADERS_H
