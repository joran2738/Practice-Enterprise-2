/*
 * game.c
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */


//This code is meant to be plain c code so it can be used on an STM32 microcontroller.

#include "debug.h"
#include "game.h"
#include "displayText.h"
#include "menu.h"
#include "multiplayer.h"
#include "string.h"

#define DEBUG 1

extern UART_HandleTypeDef huart2;

uint8_t eventlist[EVENTSIZE];
uint8_t eventindex = 0;

uint8_t hit = 0;

extern uint8_t lives;
extern uint32_t money;

uint8_t choice = 0;
uint8_t last_choice_pause = 40;
uint8_t max_choice = 1;
extern uint8_t total_games;
uint8_t gamechoice = 0;
extern uint8_t play;
uint8_t last_state;
char str[12];

point person = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 5};
ballPoint ball = {(SCREEN_WIDTH)/2, SCREEN_HEIGHT - 6, still};
uint8_t start = 0;
uint8_t delay = BRICK_SPEED;
directions ballDirection = NE;

void init (void) {
    choice = 0;
    if(gamechoice == 1){
        initSpaceInvaders();
    }else if(gamechoice == 2){
        initBrickbreaker();
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
        	}else if(gamechoice == 2) {
        	    Displ_FillArea(person.x*4 - BAR_SIZE*2, person.y*4, BAR_SIZE*4, 4, D_GREEN);
                person.x--;
                if(person.x < BAR_SIZE/2) {
                    person.x = (BAR_SIZE/2);
                } else {
                    if (ball.dir == still) {
                        Displ_FillArea(ball.x*4, ball.y*4, 4, 4, D_GREEN);
                        ball.x--;
                        ballDirection = NW;
                    }
                }
        	}
        }
        if (choice != 0){
        	choice--;
        }

        key = 0;
    }
    else if(key == right) {
    	if(play < paused){
    		if(gamechoice == 1){
    			moveSpaceship(1);
    		}else if (gamechoice == 2) {
    			Displ_FillArea(person.x*4 - BAR_SIZE*2, person.y*4, BAR_SIZE*4, 4, D_GREEN);
    			person.x++;
    			if(person.x > SCREEN_WIDTH - (BAR_SIZE/2) - 1) {
    				person.x = SCREEN_WIDTH - (BAR_SIZE/2) - 1;
    			}
    			if (ball.dir == still) {
    				Displ_FillArea(ball.x*4, ball.y*4, 4, 4, D_GREEN);
    				ball.x++;
    				ballDirection = NE;
    			}
    		}
    	}
    	if (choice != max_choice){
    		choice++;
    	}
    	key = 0;
    }
    else if(key == down){
        if (play == notPlay){
        	Displ_FillArea(SCREEN_WIDTH + 10, SCREEN_HEIGHT * 2, 140, 20, getBgColor());
            play = inPlay;
            if(getConnection() == 1) {
                HAL_UART_Transmit(&huart2, (uint8_t *)"S", strlen("S"), 300);
            }
        }else if(play == inPlay) {
            if (ball.dir == still) {
                ball.dir = ballDirection;
                start = 1;
                playBall();
            }
            if(!hit){
                spawnBullet();
            }
        }else if(play == paused){
            if(choice == 0){
                if(getConnection() == 1) {
                    HAL_UART_Transmit(&huart2, (uint8_t *)"M", strlen("M"), 300);
                }
                play = menu;
                gamechoice = 0;
                Displ_CLS(BLACK);
            }
            else if(choice == 1){
            	displayPauseMenu(1);
                play = last_state;
                if(getConnection() == 1) {
                    HAL_UART_Transmit(&huart2, (uint8_t *)"C", strlen("C"), 300);
                }
            }else{
                displayPauseMenu(0);
            }

        }else if(play == menu){
            if(choice == 0){
                if(getConnection() == 0) {
                    HAL_UART_Transmit(&huart2, (uint8_t *)"?", strlen("?"), 300);
                } else if(getConnection() == 1) {
                    HAL_UART_Transmit(&huart2, (uint8_t *)"X", strlen("X"), 300);
                    closeConnection();
                }

            }else if(choice == 1){
                gamechoice = 2;
                play = notPlay;
                if(getConnection() == 1) {
                    HAL_UART_Transmit(&huart2, (uint8_t *)"2G", strlen("2G"), 300);
                }
                init();
            }
            else if(choice == 2){
                gamechoice = 1;
                play = notPlay;
                if(getConnection() == 1) {
                    HAL_UART_Transmit(&huart2, (uint8_t *)"1G", strlen("1G"), 300);
                }
                init();
            }else{
                displayMenu();
            }

        }
        else if(!hit){
            spawnBullet();
        }
        key = 0;
    }
    else if(key == up && play < 2){
    	last_state = play;
        play = paused;
        choice = 1;
        last_choice_pause = 40;
        key = 0;
        if(getConnection() == 1) {
            HAL_UART_Transmit(&huart2, (uint8_t *)"P", strlen("P"), 300);
        }
    }

    if(play == inPlay){
    	if(gamechoice == 1){
    		spawnComet();
    		delayBullet();
    		moveComets();
    		moveBullets();
    		moveEnemyBullets();
    	}else if(gamechoice == 2) {
            if (start == 1 && ball.dir != still) {
                if (delay <= 0) {
                    lowerBricks();
                    delay = BRICK_SPEED;
                } else {
                    delay--;
                }
                checkGameOver();
            }
            checkGuardianTimer();
            playBall();
        }

    	updateScreen();
    }if(play < paused){
    	if(gamechoice == 1){
    		spawnStar();
    		moveStars();
    	}

    }


    if (getConnection() == 1 && getSignal() == 1) {
        if(gamechoice == 0) {
            gamechoice = getMPGameChoice();
            printArray();
            clearArray();
            turnOffSignal();
            play = notPlay;
            init();
        }else if(play == notPlay && getMPMenuState() == inPlay){
        	play = inPlay;
        	turnOffSignal();
        	Displ_FillArea(SCREEN_WIDTH + 10, SCREEN_HEIGHT * 2, 140, 20, getBgColor());
        }else if (play < 2 && returnMPMenuState() == paused) {
            last_state = play;
            play = paused;
            choice = 1;
            last_choice_pause = 40;
            turnOffSignal();
        }else if (play == paused && getMPMenuState() == inPlay) {
            printf("play = %d, mpMenu = %d\r\n", play, getMPMenuState());
            turnOffSignal();
            displayPauseMenu(1);
            play = last_state;
        }else if (play == paused && getMPMenuState() == menu) {
            turnOffSignal();
            play = menu;
            gamechoice = 0;
            Displ_CLS(BLACK);
            resetLastChoiceMenu();
            init();
        }else if(gamechoice == 1) {
        	spawnEnemyBullet();
        	turnOffSignal();
        }else if (gamechoice == 2) {
            lowerBricks();
            printArray();
            clearArray();
            turnOffSignal();
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

    if (gamechoice == 1){
        //hit
        if (hit > 0) {
            spaceShipHitColorToggle();
        }

        //spaceship
        displaySpaceShip(RED);

        //lives
        displayLives();
    }else if(gamechoice == 2) {
        printScreen();
    }

    //start
    if(!play){
    	Displ_WString(SCREEN_WIDTH + 10, SCREEN_HEIGHT * 2, "START", Font20, 2, WHITE, getBgColor());
    }
    else if(play == paused){
    	displayPauseMenu(0);
    }
}
