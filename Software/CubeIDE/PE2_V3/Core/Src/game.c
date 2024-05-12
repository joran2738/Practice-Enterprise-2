/*
 * game.c
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */


//This code is meant to be plain c code so it can be used on an STM32 microcontroller.

#include "debug.h"
#include "game.h"
//#include <QDateTime>	// should be the tic thingy
#include "displayText.h"
#include "menu.h"
#include "multiplayer.h"

#define DEBUG 1

uint8_t eventlist[EVENTSIZE];
uint8_t eventindex = 0;

uint8_t game_screen[SCREEN_WIDTH][SCREEN_HEIGHT];

uint8_t hit = 0;

extern uint8_t lives;
extern uint32_t money;

uint8_t choice = 0;
uint8_t last_choice_pause = 40;
uint8_t max_choice = 1;
extern uint8_t total_games;
uint8_t gamechoice = 0;
extern uint8_t play;
char str[12];


void init (void) {
    choice = 0;
    if(gamechoice == 1){
        initSpaceInvaders();
    }else{
    	Displ_CLS(BLACK);
        //nothing yet, your init
    }


}

int loop (int key) {
    updateScreen();
    if (play == menu){
        max_choice = total_games;
    }else{
        max_choice = 1;
    }
    if(key == left) {
        if(play < paused){
        	if(gamechoice == 1){
        		moveSpaceship(-1);
        	}
        }else{
            if (choice == 0){
                choice = max_choice;
            }else{
                choice--;
            }
            //QD << choice; // change to uart debug
        }
        key = 0;
    }
    if(key == right) {
        if(play < paused){
        	if(gamechoice == 1){
        		moveSpaceship(1);
        	}
        }else{
            if (choice == max_choice){
                choice = 0;
            }else{
                choice++;
            }
        }
        key = 0;
    }
    if(key == down){
        if (play == notPlay){
        	Displ_FillArea(SCREEN_WIDTH + 10, SCREEN_HEIGHT * 2, 140, 20, DARK_GREY);
            play = inPlay;
        }else if(play == paused){
            if(choice == 0){
                play = menu;
                init();
            }
            else if(choice == 1){
            	displayPauseMenu(1);
                play = inPlay;
            }else{
                displayPauseMenu(0);
            }

        }else if(play == menu){
            if(choice == 0){
                toggle_multiplayer();
            }else if(choice == 1){
                gamechoice = 0;
                play = notPlay;

                init();
            }
            else if(choice == 2){
                gamechoice = 1;
                play = notPlay;

                init();
            }else{
                displayMenu();
            }
        }
        else if(!hit){
            spawnBullet(0);
        }
        key = 0;
    }
    if(key == up && play < 2){
        play = paused;
        choice = 1;
        last_choice_pause = 40;
        key = 0;
    }

    if(play == inPlay){
    	if(gamechoice == 1){
    		spawnComet();
    		delayBullet();
    		moveComets();
    		moveBullets();
    		moveEnemyBullets();
    	}

    	updateScreen();
    }if(play < paused){
    	if(gamechoice == 1){
    		spawnStar();
    		moveStars();
    	}

    }
    return key;
}


void updateScreen()
{
    if (play == menu){
    	displayMenu();
        return;
    }

    //hit
    if (gamechoice == 1 && hit > 0){
        spaceShipHitColorToggle();
    }
    //background
    for(int x = 0; x < SCREEN_WIDTH; x++) {
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            game_screen[x][y] = ENC_DARK_GRAY;
        }
    }

    //start
    if(!play){
    	Displ_WString(SCREEN_WIDTH + 10, SCREEN_HEIGHT * 2, "START", Font20, 2, WHITE, DARK_GREY);
    }
    else if(play == paused){
    	displayPauseMenu(0);
    }

    if(gamechoice == 1){
    	//spaceship
    	displaySpaceShip(RED);

    	//lives
    	displayLives();

    }else{
        //displayText(game_screen,"bricks", SCREEN_WIDTH - (SCREEN_WIDTH / 2) - 18, SCREEN_HEIGHT - (SCREEN_HEIGHT / 2) + 5, WHITE);
        //nothin yet, here comes your things
    }


}






