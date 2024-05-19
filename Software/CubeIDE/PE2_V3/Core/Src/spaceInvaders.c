/*
 * spaceInvaders.c
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */


#include "spaceInvaders.h"
#include "displayText.h"
#include "debug.h"

#define COMET_SIZE(index) (Comet.comet_ar[index].size)
#define COMET_X(index) (Comet.comet_ar[index].x)
#define COMET_Y(index) (Comet.comet_ar[index].y)
#define COMET_COINS(index) (Comet.comet_ar[index].coins)

#define STAR_X(index) (Star.star_ar[index].x)
#define STAR_Y(index) (Star.star_ar[index].y)

#define AMMO_X(index) (ammo.bullet_ar[index].x)
#define AMMO_Y(index) (ammo.bullet_ar[index].y)

#define E_AMMO_X(index) (enemy_ammo.bullet_ar[index].x)
#define E_AMMO_Y(index) (enemy_ammo.bullet_ar[index].y)

#define SPACESHIP_X_BOUND(sign) (SpaceShip.x sign SPACESHIP_WIDTH / 2)
#define SPACESHIP_Y_BOUND (SpaceShip.y - SPACESHIP_HEIGHT / 2)

/**
 * @fn void initSpaceShip(void)
 * @brief
 *
 */
void initSpaceShip(void);

/**
 * @fn void initAmmo(bullets*, int)
 * @brief
 *
 * @param
 * @param
 */
void initAmmo(bullets *, int);

/**
 * @fn void initComets(void)
 * @brief
 *
 */
void initComets(void);

/**
 * @fn void initStars(void)
 * @brief
 *
 */
void initStars(void);

/**
 * @fn void hitComet(int, int)
 * @brief if a bullet or the spaceship hits a comet, this is called to delete the correct comet (and bullet if applicable)
 *
 * @param
 * @param
 */
void hitComet(int,int);

/**
 * @fn void beenHit(void)
 * @brief if the spaceship has been hit by a comet or an enemy bullet, this function is called
 * decrements the lives, if lives is 0, the game is restarted in the notPlay state by calling initSpaceInvaders
 *
 */
void beenHit(void);

/**
 * @fn void powerups(void)
 * @brief
 *
 */
void powerups(void);

/**
 * @fn void displayPowerup(uint16_t)
 * @brief
 *
 * @param
 */
void displayPowerup(uint16_t);

bullets ammo;
bullets enemy_ammo;
comets Comet;
stars Star;
uint8_t bullet_delay = 0;
uint8_t lives = 3;
uint32_t money = 0;
uint8_t play = 3;
uint8_t comet_delay = 0;
uint8_t star_delay = 0;
uint8_t powerUp = 0;
uint8_t powerUp_delay = 0;

static point SpaceShip = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 5};
extern uint8_t hit;
uint16_t spaceshipCurrentColor = RED;
uint8_t hit_graph_counter = 60;
uint8_t in_multiplayer = 1;
extern uint8_t connected;

/**
 * @fn void initSpaceInvaders(void)
 * @brief initialises the spaceinvaders game
 * turns the entire screen DARK_GREY
 * sets the x-cord of the spaceship to the middle of the screen
 * initialises ammo by clearing all values using initAmmo()
 * initialises comets by clearing all values using initComets()
 * initialises stars by clearing all values using initStars()
 *
 */
void initSpaceInvaders(){
    money = 0;
    lives = 3;

    Displ_CLS(DARK_GREY);
    // Seed the random number generator
    srand(HAL_GetTick());
    comet_delay = RANDOM(5,20);
    initSpaceShip();
    initAmmo(&ammo,0);
    if(in_multiplayer){
        initAmmo(&enemy_ammo,1);
    }
    initComets();
    initStars();
}

void initSpaceShip(){
    SpaceShip.x = SCREEN_WIDTH/2;
}

void initAmmo(bullets *am,int enemy){
    am->enemy = enemy;
    am->in_play=0;
    for (int i = 0;i<MAX_BULLETS;i++){
        am->bullet_ar[i].x = -1;
        am->bullet_ar[i].y = -1;
    }
}

void initComets(){
    Comet.in_play = 0;
    for (int i = 0;i<MAX_COMETS;i++){
        COMET_X(i) = -1;
        COMET_Y(i) = -1;
        COMET_SIZE(i) = 2;
    }
}

void initStars(){
    Star.in_play = 0;
    for (int i = 0;i<MAX_COMETS;i++){
    	STAR_X(i) = -1;
    	STAR_Y(i) = -1;
    }
}

void spawnComet(){
    if(comet_delay < 1 && Comet.in_play < MAX_COMETS){
    	COMET_X(Comet.in_play) = RANDOM(PLAYABLE_OFFSET, PLAYABLE_MAX);
    	COMET_Y(Comet.in_play) = 0;
        COMET_SIZE(Comet.in_play) = RANDOM(MIN_COMET_SIZE,MAX_COMET_SIZE);

        COMET_COINS(Comet.in_play) = 7 - COMET_SIZE(Comet.in_play);
        comet_delay = RANDOM(5,20);
        Comet.in_play++;
    }else{
        comet_delay--;
    }

}

void spawnBullet(int enemy){
    int x_val = SpaceShip.x;
    int y_val = SpaceShip.y - 3;
    bullets *am_p;
    if (enemy){
        am_p = &enemy_ammo;
    }else{
        am_p = &ammo;
    }
    if(am_p->in_play < MAX_BULLETS && bullet_delay == 0){
        if(am_p->enemy){
            //x_val = read the input from multiplayer;
            y_val = 3;
        }
        am_p->bullet_ar[am_p->in_play].x = x_val;
        am_p->bullet_ar[am_p->in_play].y = y_val;
        am_p->in_play++;
        bullet_delay = MAX_DELAY_BULLETS;
    }
}

void spawnStar(){
    if(star_delay < 1 && Star.in_play < MAX_STARS){
    	STAR_X(Star.in_play) = RANDOM(PLAYABLE_OFFSET,PLAYABLE_MAX);
    	STAR_Y(Star.in_play) = 0;
        star_delay = RANDOM(5,20);
        Star.in_play++;
    }else{
        star_delay--;
    }
}

void moveBullets(){
	displayAmmo(DARK_GREY);
	for(int i = 0; i < ammo.in_play; i++){
		for (int j = 0; j < Comet.in_play; j++){
			if(AMMO_X(i) >= COMET_X(j) && AMMO_X(i) < COMET_X(j) + COMET_SIZE(j)){
				if (AMMO_Y(i) <= COMET_Y(j) + COMET_SIZE(j) - 1){
					hitComet(i,j);
					continue;
				}
			}

		}
		AMMO_Y(i)--;
		if (AMMO_Y(i) < 0){
			send_Bullet(ammo.bullet_ar[i]);
			for(int j = i; j < MAX_BULLETS - 1; j++){
				AMMO_X(j) = AMMO_X(j+1);
				AMMO_Y(j) = AMMO_Y(j+1);
			}
			ammo.in_play--;
		}
	}
	displayAmmo(RED);
}

void moveEnemyBullets(void){
    if (!in_multiplayer){
        //QD << "not multiplayer";  // change to uart debug
        return;
    }
    displayAmmo(DARK_GREY);
    for(int i = 0; i < enemy_ammo.in_play; i++){
        if(E_AMMO_X(i) <= SPACESHIP_X_BOUND(+) && E_AMMO_X(i) >= SPACESHIP_X_BOUND(-)){
            if (E_AMMO_Y(i) >= SPACESHIP_Y_BOUND && hit == 0){
                beenHit();
                for(int j = i; j < MAX_BULLETS - 1; j++){
                	E_AMMO_X(j) = E_AMMO_X(j+1);
                	E_AMMO_Y(j)= E_AMMO_Y(j+1);
                }
                enemy_ammo.in_play--;
            }
        }
        E_AMMO_Y(i)++;
        if (E_AMMO_Y(i) > SCREEN_HEIGHT - 1){
            for(int j = i; j < MAX_BULLETS - 1; j++){
            	E_AMMO_X(j) = E_AMMO_X(j+1);
            	E_AMMO_Y(j)= E_AMMO_Y(j+1);
            }
            enemy_ammo.in_play--;
        }
    }
    displayAmmo(LIGHT_BLUE);
}

void moveComets(){
	displayComets(DARK_GREY);
	for (int i = 0; i<Comet.in_play; i++){
		for(int j = 0; j < COMET_SIZE(i); j++){
			if(COMET_X(i) + j <= SPACESHIP_X_BOUND(+) && COMET_X(i) + j >= SPACESHIP_X_BOUND(-)){
				if(powerUp == 2 && COMET_Y(i) + COMET_SIZE(i) - 1 >= SPACESHIP_Y_BOUND){
					hitComet(420,i);
					displayPowerup(DARK_GREY);
					powerUp = 0;
				}
                if (COMET_Y(i) + COMET_SIZE(i) - 1 >= SPACESHIP_Y_BOUND && hit == 0){
                    hitComet(420,i);
                    beenHit();
                }
            }
        }
        if(powerUp == 1 && SpaceShip.x >= COMET_X(i) && SpaceShip.x <= COMET_X(i) + COMET_SIZE(i)){
        	hitComet(420,i);
        	if(powerUp_delay == 0){
        		displayPowerup(DARK_GREY);
        		powerUp = 0;

        	}
        	powerUp_delay--;
        }
        COMET_Y(i)++;
        if (COMET_Y(i) > SCREEN_HEIGHT - 1){
            for(int j = i; j < MAX_COMETS - 1; j++){
            	COMET_X(j) = COMET_X(j+1);
            	COMET_Y(j) = COMET_Y(j+1);
                COMET_SIZE(j) = COMET_SIZE(j+1);
            }
            Comet.in_play--;
        }
    }
    displayComets(GREY);
}

void moveStars(){
	displayStars(DARK_GREY);
    for (int i = 0; i<Star.in_play; i++){
    	STAR_Y(i)++;
        if (STAR_Y(i) > SCREEN_HEIGHT - 4){
            for(int j = i; j < MAX_COMETS - 1; j++){
            	STAR_X(j) = STAR_X(j+1);
            	STAR_Y(j) = STAR_Y(j+1);
            }
            Star.in_play--;
        }
    }
    displayStars(YELLOW);
}

void moveSpaceship(int direction){
	displaySpaceShip(DARK_GREY);
	if(powerUp > 0){
		displayPowerup(DARK_GREY);
	}
    if(direction < 0){
        SpaceShip.x--;
        if(SpaceShip.x < SPACESHIP_WIDTH/2) {
        	SpaceShip.x = (SPACESHIP_WIDTH/2);
        }
        displaySpaceShip(spaceshipCurrentColor);
        if(powerUp == 1){
        	displayPowerup(RED);
        }
        if(powerUp == 2){
        	displayPowerup(LIGHT_BLUE);
        }
        return;
    }
    SpaceShip.x++;
    if(SpaceShip.x > SCREEN_WIDTH - (SPACESHIP_WIDTH/2) - 1) {
    	SpaceShip.x = SCREEN_WIDTH - (SPACESHIP_WIDTH/2) - 1;
    }
    displaySpaceShip(spaceshipCurrentColor);
    if(powerUp == 1){
    	displayPowerup(RED);
    }
	if(powerUp == 2){
		displayPowerup(LIGHT_BLUE);
	}
}

void hitComet(int bul,int com){
    money += COMET_COINS(com);
    Displ_FillArea(COMET_X(com) * 4, COMET_Y(com) * 4, COMET_SIZE(com) * 4, COMET_SIZE(com) * 4, DARK_GREY);
    if(bul != 420){
        for(int j = bul; j < MAX_BULLETS - 1; j++){
        	AMMO_X(j)= AMMO_X(j+1);
        	AMMO_Y(j) = AMMO_Y(j+1);
        }
        powerups();
    }

    for(int j = com; j < MAX_COMETS - 1; j++){
    	COMET_X(j) = COMET_X(j+1);
    	COMET_Y(j) = COMET_Y(j+1);
        COMET_SIZE(j) = COMET_SIZE(j+1);
    }
    Comet.in_play--;
}




void displayStars(uint16_t color){
    for(int i = 0; i < Star.in_play; i++){
    	Displ_FillArea(STAR_X(i)* 4, STAR_Y(i) * 4, 4, 4, color);
    }
}

void displayAmmo(uint16_t color){
    for(int i = 0; i < ammo.in_play; i++){
    	Displ_FillArea(AMMO_X(i) * 4, AMMO_Y(i) * 4, 4, 4, color);
    }
    if(in_multiplayer){
        for(int i = 0; i < enemy_ammo.in_play; i++){
            Displ_FillArea(E_AMMO_X(i) * 4, E_AMMO_Y(i) * 4, 4, 4, LIGHT_BLUE);
        }
    }
}

void displayComets(uint16_t color){
    for (int i = 0; i < Comet.in_play; i++){
        Displ_FillArea(COMET_X(i) * 4, COMET_Y(i)  * 4, COMET_SIZE(i) * 4, COMET_SIZE(i) * 4, color);
    }
}

void displaySpaceShip(uint16_t color){
    Displ_FillArea(SpaceShip.x*4 - 8, SCREEN_HEIGHT*4 - 24, 4, 16, color);
    Displ_FillArea(SpaceShip.x*4 + 8, SCREEN_HEIGHT*4 - 24, 4, 16, color);
    Displ_FillArea(SpaceShip.x*4 - 4, SCREEN_HEIGHT*4 -20, 12, 4, color);
    Displ_FillArea(SpaceShip.x*4, SCREEN_HEIGHT*4 -28, 4, 16, color);

}

void displayLives(){
    for(int i = 0; i < lives; i++){
    	Displ_FillArea(HEARTS_X+HEARTS_OFFSET*i, HEARTS_Y, 4, 4, LIGHT_GREEN);
    }
    if(lives < 3){
    	for(int i = 0; i < 3 - lives; i++){
    		Displ_FillArea(HEARTS_X+HEARTS_OFFSET*(3-i), HEARTS_Y, 4, 4, DARK_GREY);
    	}
    }
}


void spaceShipHitColorToggle(){
    if(spaceshipCurrentColor == RED && hit % 10 == 0){
        spaceshipCurrentColor = WHITE;
    }else if(hit % 10 == 0){
        spaceshipCurrentColor = RED;
    }
    if(hit < hit_graph_counter){
        hit++;
    }else{
        hit = 0;
    }
}

void delayBullet(){
    if (bullet_delay > 0){
        bullet_delay--;
    }
}

void beenHit(){
    hit = 1;
    lives--;
    if(lives <= 0){
        play = 0;
        hit = 0;
        initSpaceInvaders();

    }
}

void send_Bullet(point bullet){
    if(connected && bullet.y != -2){
        //QD << "a bullet has been send to the other player at x:" << bullet.x;  // change to uart debug
        if(enemy_ammo.in_play < MAX_BULLETS){
        	E_AMMO_X(enemy_ammo.in_play) = RANDOM(PLAYABLE_OFFSET, PLAYABLE_MAX);
        	E_AMMO_Y(enemy_ammo.in_play) = 3;
            enemy_ammo.in_play++;
        }else{
            star_delay--;
        }
    }
}

void powerups(){
    uint8_t powerupornot = RANDOM(0,3);
    if(powerUp == 0 && powerupornot == 0){
        powerUp = RANDOM(1,3);
        if(powerUp == 1){
        	displayPowerup(RED);
        }
        else if(powerUp == 2){
        	displayPowerup(LIGHT_BLUE);
        }
        powerUp_delay = MAX_DELAY_POWERUP;
    }
}

void displayPowerup(uint16_t color){

    if(powerUp == 1){
		Displ_FillArea(SpaceShip.x * 4, 0, 4, 180, color);
		Displ_FillArea(SpaceShip.x * 4 + 1, 180, 2, 30, color);
    }
    else if(powerUp == 2){
    	Displ_drawCircle(SpaceShip.x * 4 + 1, SpaceShip.y * 4 + 2, 14, color);
    }
}



