/*
 * spaceInvaders.c
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */


#include "spaceInvaders.h"
#include "displayText.h"
#include "debug.h"

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
    comet_delay = rand() % ((20 + 1) - 5) + 5;
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
        Comet.comet_ar[i].x = -1;
        Comet.comet_ar[i].y = -1;
        Comet.comet_ar[i].size = 2;
    }
}

void initStars(){
    Star.in_play = 0;
    for (int i = 0;i<MAX_COMETS;i++){
        Star.star_ar[i].x = -1;
        Star.star_ar[i].y = -1;
    }
}

void spawnComet(){
    if(comet_delay < 1 && Comet.in_play < MAX_COMETS){
        Comet.comet_ar[Comet.in_play].x = rand() % ((PLAYABLE_MAX - 1) - PLAYABLE_OFFSET) + PLAYABLE_OFFSET;
        Comet.comet_ar[Comet.in_play].y = 0;
        Comet.comet_ar[Comet.in_play].size = rand() % ((MAX_COMET_SIZE + 1) - MIN_COMET_SIZE) + MIN_COMET_SIZE;

        Comet.comet_ar[Comet.in_play].coins = 7 - Comet.comet_ar[Comet.in_play].size;
        comet_delay = rand() % ((20 + 1) - 10) + 5;
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
        Star.star_ar[Star.in_play].x = rand() % ((PLAYABLE_MAX - 1) - PLAYABLE_OFFSET) + PLAYABLE_OFFSET;
        Star.star_ar[Star.in_play].y = 0;
        star_delay = rand() % ((20 + 1) - 10) + 5;
        Star.in_play++;
    }else{
        star_delay--;
    }
}

void moveBullets(){
	displayAmmo(DARK_GREY);
	for(int i = 0; i < ammo.in_play; i++){
		for (int j = 0; j < Comet.in_play; j++){
			if(ammo.bullet_ar[i].x >= Comet.comet_ar[j].x && ammo.bullet_ar[i].x < Comet.comet_ar[j].x + Comet.comet_ar[j].size){
				if (ammo.bullet_ar[i].y <= Comet.comet_ar[j].y + Comet.comet_ar[j].size - 1){
					//QD << "hit a comet";
					hitComet(i,j);
					continue;
				}
			}

		}
		ammo.bullet_ar[i].y--;
		if (ammo.bullet_ar[i].y < 0){
			send_Bullet(ammo.bullet_ar[i]);
			for(int j = i; j < MAX_BULLETS - 1; j++){
				ammo.bullet_ar[j].x = ammo.bullet_ar[j+1].x;
				ammo.bullet_ar[j].y = ammo.bullet_ar[j+1].y;
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
        if(enemy_ammo.bullet_ar[i].x <= SpaceShip.x + SPACESHIP_WIDTH / 2 && enemy_ammo.bullet_ar[i].x >= SpaceShip.x - SPACESHIP_WIDTH / 2){
            if (enemy_ammo.bullet_ar[i].y >= SpaceShip.y - SPACESHIP_HEIGHT / 2 && hit == 0){
                beenHit();
                for(int j = i; j < MAX_BULLETS - 1; j++){
                    enemy_ammo.bullet_ar[j].x = enemy_ammo.bullet_ar[j+1].x;
                    enemy_ammo.bullet_ar[j].y = enemy_ammo.bullet_ar[j+1].y;
                }
                enemy_ammo.in_play--;
            }
        }
        enemy_ammo.bullet_ar[i].y++;
        if (enemy_ammo.bullet_ar[i].y > SCREEN_HEIGHT - 1){
            for(int j = i; j < MAX_BULLETS - 1; j++){
                enemy_ammo.bullet_ar[j].x = enemy_ammo.bullet_ar[j+1].x;
                enemy_ammo.bullet_ar[j].y = enemy_ammo.bullet_ar[j+1].y;
            }
            enemy_ammo.in_play--;
        }
    }
    displayAmmo(LIGHT_BLUE);
}

void moveComets(){
	displayComets(DARK_GREY);
    for (int i = 0; i<Comet.in_play; i++){
        for(int j = 0; j < Comet.comet_ar[i].size; j++){
            if(Comet.comet_ar[i].x + j <= SpaceShip.x + SPACESHIP_WIDTH / 2 && Comet.comet_ar[i].x + j >= SpaceShip.x - SPACESHIP_WIDTH / 2){
                if (Comet.comet_ar[i].y + Comet.comet_ar[i].size - 1 >= SpaceShip.y - SPACESHIP_HEIGHT / 2 && hit == 0){
                    hitComet(420,i);
                    beenHit();
                }
            }
        }

        Comet.comet_ar[i].y++;
        if (Comet.comet_ar[i].y > SCREEN_HEIGHT - 1){
            for(int j = i; j < MAX_COMETS - 1; j++){
                Comet.comet_ar[j].x = Comet.comet_ar[j+1].x;
                Comet.comet_ar[j].y = Comet.comet_ar[j+1].y;
                Comet.comet_ar[j].size = Comet.comet_ar[j+1].size;
            }
            Comet.in_play--;
        }
    }
    displayComets(GREY);
}

void moveStars(){
	displayStars(DARK_GREY);
    for (int i = 0; i<Star.in_play; i++){
        Star.star_ar[i].y++;
        if (Star.star_ar[i].y > SCREEN_HEIGHT - 4){
            for(int j = i; j < MAX_COMETS - 1; j++){
                Star.star_ar[j].x = Star.star_ar[j+1].x;
                Star.star_ar[j].y = Star.star_ar[j+1].y;
            }
            Star.in_play--;
        }
    }
    displayStars(YELLOW);
}

void moveSpaceship(int direction){
	displaySpaceShip(DARK_GREY);
    if(direction < 0){
        SpaceShip.x--;
        if(SpaceShip.x < SPACESHIP_WIDTH/2) {
            SpaceShip.x = (SPACESHIP_WIDTH/2);
        }
        displaySpaceShip(RED);
        return;
    }
    SpaceShip.x++;
    if(SpaceShip.x > SCREEN_WIDTH - (SPACESHIP_WIDTH/2) - 1) {
        SpaceShip.x = SCREEN_WIDTH - (SPACESHIP_WIDTH/2) - 1;
    }
    displaySpaceShip(spaceshipCurrentColor);
}

void hitComet(int bul,int com){
    money += Comet.comet_ar[com].coins;
    Displ_FillArea(Comet.comet_ar[com].x * 4, Comet.comet_ar[com].y * 4, Comet.comet_ar[com].size * 4, Comet.comet_ar[com].size * 4, DARK_GREY);
    if(bul != 420){
        for(int j = bul; j < MAX_BULLETS - 1; j++){
            ammo.bullet_ar[j].x = ammo.bullet_ar[j+1].x;
            ammo.bullet_ar[j].y = ammo.bullet_ar[j+1].y;
        }
    }

    for(int j = com; j < MAX_COMETS - 1; j++){
        Comet.comet_ar[j].x = Comet.comet_ar[j+1].x;
        Comet.comet_ar[j].y = Comet.comet_ar[j+1].y;
        Comet.comet_ar[j].size = Comet.comet_ar[j+1].size;
    }
    Comet.in_play--;
}




void displayStars(uint16_t color){
    for(int i = 0; i < Star.in_play; i++){
    	Displ_FillArea(Star.star_ar[i].x * 4, Star.star_ar[i].y * 4, 4, 4, color);
    }
}

void displayAmmo(uint16_t color){
    for(int i = 0; i < ammo.in_play; i++){
        Displ_FillArea(ammo.bullet_ar[i].x * 4, ammo.bullet_ar[i].y * 4, 4, 4, color);
    }
    if(in_multiplayer){
        for(int i = 0; i < enemy_ammo.in_play; i++){
            Displ_FillArea(enemy_ammo.bullet_ar[i].x * 4, enemy_ammo.bullet_ar[i].y * 4, 4, 4, LIGHT_BLUE);
        }
    }
}

void displayComets(uint16_t color){
    for (int i = 0; i < Comet.in_play; i++){
        Displ_FillArea(Comet.comet_ar[i].x * 4, Comet.comet_ar[i].y  * 4, Comet.comet_ar[i].size * 4, Comet.comet_ar[i].size * 4, color);
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
            enemy_ammo.bullet_ar[enemy_ammo.in_play].x = rand() % ((PLAYABLE_MAX - 1) - PLAYABLE_OFFSET) + PLAYABLE_OFFSET;
            enemy_ammo.bullet_ar[enemy_ammo.in_play].y = 3;
            enemy_ammo.in_play++;
        }else{
            star_delay--;
        }
    }
}




