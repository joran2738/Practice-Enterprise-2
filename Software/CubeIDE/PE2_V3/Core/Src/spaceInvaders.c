/**
 * @file spaceInvaders.c
 *
 * @brief SpaceInvaders game source
 *  Created on: Apr 17, 2024
 *      Author: Joran
 */

// Includes
#include "spaceInvaders.h"
#include "displayText.h"
#include "debug.h"
#include "multiplayer.h"

extern UART_HandleTypeDef huart2;

// Defines
#define COMET_SIZE(index) (Comet.comet_ar[index].size)					  // Macro to get the size of the comet at a certain index of the comet array
#define COMET_X(index) (Comet.comet_ar[index].x)						  // Macro to get the x-cord of the comet at a certain index of the comet array
#define COMET_Y(index) (Comet.comet_ar[index].y)						  // Macro to get the y-cord of the comet at a certain index of the comet array
#define COMET_COINS(index) (Comet.comet_ar[index].coins)				  // Macro to get the amount of coins of the comet at a certain index of the comet array

#define STAR_X(index) (Star.star_ar[index].x)							  // Macro to get the x-cord of the star at a certain index of the stars array
#define STAR_Y(index) (Star.star_ar[index].y)							  // Macro to get the y-cord of the star at a certain index of the stars array

#define AMMO_X(index) (ammo.bullet_ar[index].x)							  // Macro to get the x-cord of the bullet at a certain index of the bullets array
#define AMMO_Y(index) (ammo.bullet_ar[index].y)							  // Macro to get the y-cord of the bullet at a certain index of the bullets array

#define E_AMMO_X(index) (enemy_ammo.bullet_ar[index].x)					  // Macro to get the x-cord of the bullet at a certain index of the enemy bullets array
#define E_AMMO_Y(index) (enemy_ammo.bullet_ar[index].y)					  // Macro to get the y-cord of the bullet at a certain index of the enemy bullets array

#define SPACESHIP_X_BOUND(sign) (SpaceShip.x sign SPACESHIP_WIDTH / 2)    // Macro to get the x-cord boundaries of the spaceship
#define SPACESHIP_Y_BOUND (SpaceShip.y - SPACESHIP_HEIGHT / 2)			  // Macro to get the y-cord boundaries of the spaceship

#define HEARTS_OFFSET 12
#define HEARTS_X (SCREEN_WIDTH * 4 - (3*HEARTS_OFFSET) - 8 )
// Vars
bullets_t ammo;				  // Spaceship's ammo
bullets_t enemy_ammo;		  // Enemy ammo
comets_t Comet;				  // Comets
stars_t Star;				  // Stars

uint8_t bullet_delay = 0;	  // Delay for stopping bullet spam
uint8_t comet_delay = 0;	  // Delay for spawning comets
uint8_t star_delay = 0;		  // Delay fpr spawning stars
uint8_t powerUp_delay = 0;    // Delay for stopping powerup usage

uint8_t lives = 3;			  // Amount of lives the player has
uint32_t money = 0;			  // amount of money the player has
uint8_t play = 3;			  // State of the game
uint8_t powerUp = 0;		  // Which power-up is active, 0 is nothing


static point SpaceShip = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 5};    // The spaceship's coordinates
extern uint8_t hit;												 // Whether the spaceship is in a "hit" state, allowing him to fly through comets and not allowing him to shoot
uint16_t spaceshipCurrentColor = RED;							 // Current color of the spaceship
uint8_t hit_graph_counter = 60;									 // Counter for toggling the spaceship's color when hit

extern uint8_t connected;										 // Connected to the other console or not

// Function defines
/**
 * @fn void initSpaceShip(void)
 * @brief initialises the spaceship
 *
 */
void initSpaceShip(void);

/**
 * @fn void initAmmo(bullets*, int)
 * @brief initialises the ammo
 *
 * @param[out] am bullets_t*: the bullets to initialise, can be your own or the enemy's
 * @param[in] enemy int: if 1, bullets pointer is for the enemy bullets
 */
void initAmmo(bullets_t *, int);

/**
 * @fn void initComets(void)
 * @brief initialises the comets
 *
 */
void initComets(void);

/**
 * @fn void initStars(void)
 * @brief initialises the stars
 *
 */
void initStars(void);

/**
 * @fn void hitComet(int, int)
 * @brief if a bullet or the spaceship hits a comet, this is called to delete the correct comet (and bullet if applicable)
 *
 * @param[in] bul int: the index of the bullet that hit a comet, if the spaceship hit the comet 420 is given to this function
 * @param[in] com int: the index of the comet that has been hit
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
 * @brief randomly gives a power-up when a comet has been hit
 *
 */
void powerups(void);

/**
 * @fn void displayPowerup(uint16_t)
 * @brief displays the power-up on the screen
 *
 * @param[in] color uint16_t: the color to display the power-up with
 */
void displayPowerup(uint16_t);

// Functions
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
    setBgColor(DARK_GREY);
    Displ_CLS(DARK_GREY);

    srand(HAL_GetTick());		   // Seed the random number generator
    comet_delay = RANDOM(5,20);
    initSpaceShip();
    initAmmo(&ammo,0);
    if(connected){
        initAmmo(&enemy_ammo,1);
    }
    initComets();
    initStars();
}

/**
 * @fn void initSpaceShip(void)
 * @brief initialises the spaceship
 *
 */
void initSpaceShip(){
    SpaceShip.x = SCREEN_WIDTH/2;
}

/**
 * @fn void initAmmo(bullets*, int)
 * @brief initialises the ammo
 * the bullets in play are set to 0
 * x and y cords are set to -1
 *
 * @param[out] am bullets_t*: the bullets to initialise, can be your own or the enemy's
 * @param[in] enemy int: if 1, bullets pointer is for the enemy bullets
 */
void initAmmo(bullets_t *am,int enemy){
    am->enemy = enemy;
    am->in_play=0;
    for (int i = 0;i<MAX_BULLETS;i++){
        am->bullet_ar[i].x = -1;
        am->bullet_ar[i].y = -1;
    }
}

/**
 * @fn void initComets(void)
 * @brief initialises the comets
 * the comets in play are set to 0
 * x and y cords are set to -1
 *
 */
void initComets(){
    Comet.in_play = 0;
    for (int i = 0;i<MAX_COMETS;i++){
        COMET_X(i) = -1;
        COMET_Y(i) = -1;
        COMET_SIZE(i) = 2;
    }
}

/**
 * @fn void initStars(void)
 * @brief initialises the stars
 * the stars in play are set to 0
 * x and y cords are set to -1
 *
 */
void initStars(){
    Star.in_play = 0;
    for (int i = 0;i<MAX_COMETS;i++){
    	STAR_X(i) = -1;
    	STAR_Y(i) = -1;
    }
}

/**
 * @fn void spawnComet()
 * @brief spawns a comet at a random x-cord with a random size
 * generates a random delay for the spawning of the next comet
 *
 */
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

/**
 * @fn void spawnBullet()
 * @brief spawns a bullet at the x-cord of the spaceship
 * reloads the bullet_delay variable, so the user can't spam bullets
 *
 */
void spawnBullet(){
    int x_val = SpaceShip.x;
    int y_val = SpaceShip.y - 3;

    if(ammo.in_play < MAX_BULLETS && bullet_delay == 0){
        if(ammo.enemy){
            //x_val = read the input from multiplayer;
            y_val = 3;
        }
        ammo.bullet_ar[ammo.in_play].x = x_val;
        ammo.bullet_ar[ammo.in_play].y = y_val;
        ammo.in_play++;
        bullet_delay = MAX_DELAY_BULLETS;
    }
}

/**
 * @fn void spawnEnemyBullet()
 * @brief spawns an enemy bullet
 *
 */
void spawnEnemyBullet(){
	uint8_t x_val;
	int8_t y_val;

	if(enemy_ammo.in_play < MAX_BULLETS){
		if(enemy_ammo.enemy){
			x_val = getCoord();
			printf("%d\r\n",x_val);
			if(x_val > 80 && x_val < 0){
				printf("return\r\n");
				return;
			}
			y_val = 3;
		}
		enemy_ammo.bullet_ar[enemy_ammo.in_play].x = x_val;
		enemy_ammo.bullet_ar[enemy_ammo.in_play].y = y_val;
		enemy_ammo.in_play++;
		clearCoord();
	}
}

/**
 * @fn void spawnStar()
 * @brief spawns a star at a random x-cord
 * generates a random delay for the spawning of the next star
 *
 */
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

/**
 * @fn void moveBullets(void)
 * @brief moves the bullets and displays them on the screen aswell by calling "displayAmmo()"
 *
 */
void moveBullets(){
	displayAmmo(DARK_GREY);
	for(int i = 0; i < ammo.in_play; i++){											  // For every bullet in play
		for (int j = 0; j < Comet.in_play; j++){									  	// For every comet in play
			if(AMMO_X(i) >= COMET_X(j) && AMMO_X(i) < COMET_X(j) + COMET_SIZE(j)){    		// check whether there's a comet at the same x-cord
				if (AMMO_Y(i) <= COMET_Y(j) + COMET_SIZE(j) - 1){					  		// if the y-cord is also smaller, a comet has been hit by this bullet
					hitComet(i,j);
					continue;
				}
			}

		}
		AMMO_Y(i)--;
		if (AMMO_Y(i) < 0){																// If the y-cord is negative, the bullet left the screen and should be deleted.
			send_Bullet(ammo.bullet_ar[i]);													// send the bullet to the other console (if connected)
			for(int j = i; j < MAX_BULLETS - 1; j++){										// move every bullet that's behind the one that should be deleted
				AMMO_X(j) = AMMO_X(j+1);
				AMMO_Y(j) = AMMO_Y(j+1);
			}
			ammo.in_play--;																	// decrement ammo in play
		}
	}
	displayAmmo(RED);
}

/**
 * @fn void moveComets(void)
 * @brief moves the comets and displays them on the screen aswell by calling "displayComets()"
 *
 */
void moveComets(){
	displayComets(DARK_GREY);
	for (int i = 0; i<Comet.in_play; i++){															   // For every comet in play
		for(int j = 0; j < COMET_SIZE(i); j++){														   // For the size of the current comet
			if(COMET_X(i) + j <= SPACESHIP_X_BOUND(+) && COMET_X(i) + j >= SPACESHIP_X_BOUND(-)){      // if the comet's x_cords are in between the bounds of the spaceship
				if(powerUp == 2 && COMET_Y(i) + COMET_SIZE(i) - 1 >= SPACESHIP_Y_BOUND){			   // if shield is enabled and the comet hit it, delete comet and get rid of shield
					hitComet(420,i);
					displayPowerup(DARK_GREY);
					powerUp = 0;
				}
				else if (COMET_Y(i) + COMET_SIZE(i) - 1 >= SPACESHIP_Y_BOUND && hit == 0){			   // else the spaceship has been hit
                    hitComet(420,i);
                    beenHit();
                }
            }
        }
        if(powerUp == 1 && SpaceShip.x >= COMET_X(i) && SpaceShip.x <= COMET_X(i) + COMET_SIZE(i)){    // If death-ray is enabled and the comet is above the spaceship, obliterate it
        	hitComet(420,i);
        	if(powerUp_delay == 0){																	   // If death ray has hit MAX_DELAY_POWERUP comets, disable powerup
        		displayPowerup(DARK_GREY);
        		powerUp = 0;

        	}
        	powerUp_delay--;
        }
        COMET_Y(i)++;
        if (COMET_Y(i) > SCREEN_HEIGHT - 1){														   // if the comet went out of the screen, delete it
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

/**
 * @fn void moveStars(void)
 * @brief moves the stars and displays them on the screen aswell by calling "displayStars()"
 *
 */
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

/**
 * @fn void moveSpaceship(int)
 * @brief moves the spaceship and displays it on the screen aswell by calling "displaySpaceShip()"
 *
 * @param
 */
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

/**
 * @fn void displayStars(uint16_t)
 * @brief 	displays the stars on the screen
 *
 * @param[in] color uint16_t: the color the star should be displayed with
 */
void displayStars(uint16_t color){
    for(int i = 0; i < Star.in_play; i++){
    	Displ_FillArea(STAR_X(i)* 4, STAR_Y(i) * 4, 4, 4, color);
    }
}

/**
 * @fn void displayAmmo(uint16_t)
 * @brief displays the Bullets on the screen
 *
 * @param[in] color uint16_t: the color the ammo should be displayed with
 */
void displayAmmo(uint16_t color){
    for(int i = 0; i < ammo.in_play; i++){
    	Displ_FillArea(AMMO_X(i) * 4, AMMO_Y(i) * 4, 4, 4, color);
    }

}

/**
 * @fn void displayEnemyAmmo(uint16_t)
 * @brief displays the enemy Bullets on the screen
 *
 * @param[in] color uint16_t: the color the ammo should be displayed with
 */
void displayEnemyAmmo(uint16_t color){
	if(connected){
		for(int i = 0; i < enemy_ammo.in_play; i++){
			Displ_FillArea(E_AMMO_X(i) * 4, E_AMMO_Y(i) * 4, 4, 4, color);
		}
	}
}

/**
 * @fn void displayComets(uint16_t)
 * @brief displays the comets on the screen
 *
 * @param[in] color uint16_t: the color the comets should be displayed with
 */
void displayComets(uint16_t color){
    for (int i = 0; i < Comet.in_play; i++){
        Displ_FillArea(COMET_X(i) * 4, COMET_Y(i)  * 4, COMET_SIZE(i) * 4, COMET_SIZE(i) * 4, color);
    }
}

/**
 * @fn void displaySpaceShip(uint16_t)
 * @brief displays the spaceship on the screen
 *
 * @param[in] color uint16_t: the color the spaceship should be displayed with
 */
void displaySpaceShip(uint16_t color){
    Displ_FillArea(SpaceShip.x*4 - 8, SCREEN_HEIGHT*4 - 24, 4, 16, color);
    Displ_FillArea(SpaceShip.x*4 + 8, SCREEN_HEIGHT*4 - 24, 4, 16, color);
    Displ_FillArea(SpaceShip.x*4 - 4, SCREEN_HEIGHT*4 -20, 12, 4, color);
    Displ_FillArea(SpaceShip.x*4, SCREEN_HEIGHT*4 -28, 4, 16, color);

}

/**
 * @fn void displayLives(void)
 * @brief displays the amount of lives left on the screen
 *
 */
void displayLives(){
    for(int i = 0; i < lives; i++){
    	Displ_FillArea(HEARTS_X+HEARTS_OFFSET*i, HEARTS_Y, 4, 4, LIGHT_GREEN);
    }
    if(lives < 3){
    	for(int i = 0; i < 3 - lives; i++){
    		Displ_FillArea(HEARTS_X+HEARTS_OFFSET*(2-i), HEARTS_Y, 4, 4, DARK_GREY);
    	}
    }
}

/**
 * @fn void spaceShipHitColorToggle(void)
 * @brief if the spaceship was hit by an enemy bullet or a comet, the spaceship has to blink red/white for a while
 *
 */
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

/**
 * @fn void delayBullet(void)
 * @brief decrements the bullet_delay variable, if 0 the spaceship can shoot a bullet again
 *
 */
void delayBullet(){
    if (bullet_delay > 0){
        bullet_delay--;
    }
}

/**
 * @fn void moveEnemyBullets(void)
 * @brief move the enemy bullets
 *
 */
void moveEnemyBullets(void){
    if (!connected){
        //QD << "not multiplayer";  // change to uart debug
        return;
    }
    displayEnemyAmmo(DARK_GREY);
    for(int i = 0; i < enemy_ammo.in_play; i++){										   // For every enemy bullet in play
        if(E_AMMO_X(i) <= SPACESHIP_X_BOUND(+) && E_AMMO_X(i) >= SPACESHIP_X_BOUND(-)){    // if it's x-cord is in the path of the spaceship
            if (E_AMMO_Y(i) >= SPACESHIP_Y_BOUND && hit == 0){							   // and it's y value is higher than the spaceship's, the spaceship has been hit
                beenHit();
                for(int j = i; j < MAX_BULLETS - 1; j++){								   // Delete this enemy bullet
                	E_AMMO_X(j) = E_AMMO_X(j+1);
                	E_AMMO_Y(j)= E_AMMO_Y(j+1);
                }
                enemy_ammo.in_play--;
            }
        }
        E_AMMO_Y(i)++;
        if (E_AMMO_Y(i) > SCREEN_HEIGHT - 1){											   // If the bullet left the screen, delete it
            for(int j = i; j < MAX_BULLETS - 1; j++){
            	E_AMMO_X(j) = E_AMMO_X(j+1);
            	E_AMMO_Y(j)= E_AMMO_Y(j+1);
            }
            enemy_ammo.in_play--;
        }
    }
    displayEnemyAmmo(LIGHT_BLUE);
}

/**
 * @fn void send_Bullet(point)
 * @brief if a bullet leaves the screen this function is called
 * if multiplayer is enabled, it sends this bullet's x_cord to the other console
 *
 * @param[in] bullet so the x cord can be send to the other console
 */
void send_Bullet(point bullet){
	char *coord = "45B";
    if(connected && bullet.y != -2){
    	HAL_UART_Transmit(&huart2, (uint8_t *)coord, strlen(coord), 300);
    	/*
        if(enemy_ammo.in_play < MAX_BULLETS){
        	E_AMMO_X(enemy_ammo.in_play) = RANDOM(PLAYABLE_OFFSET, PLAYABLE_MAX);
        	E_AMMO_Y(enemy_ammo.in_play) = 3;
            enemy_ammo.in_play++;
        }else{
            star_delay--;
        }
        */
    }
}

/**
 * @fn void hitComet(int, int)
 * @brief if a bullet or the spaceship hits a comet, this is called to delete the correct comet (and bullet if applicable)
 *
 * @param[in] bul int: the index of the bullet that hit a comet, if the spaceship hit the comet 420 is given to this function
 * @param[in] com int: the index of the comet that has been hit
 */
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

/**
 * @fn void beenHit(void)
 * @brief if the spaceship has been hit by a comet or an enemy bullet, this function is called
 * decrements the lives, if lives is 0, the game is restarted in the notPlay state by calling initSpaceInvaders
 *
 */
void beenHit(){
    hit = 1;
    lives--;
    if(lives <= 0){
        play = 0;
        hit = 0;
        HAL_UART_Transmit(&huart2, (uint8_t *)"L", strlen("L"), 300);
        setGameState(loss);
        initSpaceInvaders();
    }
}

/**
 * @fn void powerups(void)
 * @brief randomly gives a power-up when a comet has been hit
 *
 */
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

/**
 * @fn void displayPowerup(uint16_t)
 * @brief displays the power-up on the screen
 *
 * @param[in] color uint16_t: the color to display the power-up with
 */
void displayPowerup(uint16_t color){

    if(powerUp == 1){
		Displ_FillArea(SpaceShip.x * 4, 0, 4, 180, color);
		Displ_FillArea(SpaceShip.x * 4 + 1, 180, 2, 30, color);
    }
    else if(powerUp == 2){
    	Displ_drawCircle(SpaceShip.x * 4 + 1, SpaceShip.y * 4 + 2, 14, color);
    }
}






