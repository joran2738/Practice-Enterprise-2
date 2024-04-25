//This code is meant to be plain c code so it can be used on an STM32 microcontroller.

#include "debug.h"
#include "game.h"
#include <QDateTime>
#include "displayText.h"
#include "spaceInvaders.h"
#include "menu.h"
#include "multiplayer.h"
#include "brickbreaker.h"

#define DEBUG 1

uint8_t eventlist[EVENTSIZE];
uint8_t eventindex = 0;
uint32_t game_screen[SCREEN_WIDTH][SCREEN_HEIGHT];

uint8_t hit = 0;

extern uint8_t lives;
extern uint32_t money;

uint8_t choice = 0;
uint8_t max_choice = 1;
extern uint8_t total_games;
uint8_t gamechoice = 0;
extern uint8_t play;
char str[12];

point person = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 5};
ballPoint ball = {(SCREEN_WIDTH)/2, SCREEN_HEIGHT - 6, pause};
uint8_t start = 0;
uint8_t delay = BRICK_SPEED;

void init (void) {
    choice = 0;
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            game_screen[x][y] = DARK_GRAY;
        }
    }
    QD << "gamechoice: " << gamechoice;
    if(gamechoice == 1){
        initSpaceInvaders();
    } else {
        if(gamechoice == 2) {
            initBrickbreaker();
        }
    }
}

void loop (void) {
    int key = readInput();
    updateScreen();
    if (play == menu){
        max_choice = total_games;
    }else{
        max_choice = 1;
    }
    if(key == left) {
        if(play < paused){
            if(gamechoice == 1) {
                if(play == inPlay) {
                    moveSpaceship(-1);
                }

            } else if(gamechoice == 2) {
                person.x--;
                if(person.x < BAR_SIZE/2) {
                    person.x = (BAR_SIZE/2);
                }
                if (ball.dir == pause) {
                    ball.x--;
                }
            }

        }else{
            if (choice == 0){
                choice = max_choice;
            }else{
                choice--;
            }
            QD << choice;
        }
    }
    if(key == right) {
        if(play < paused){
            if (gamechoice == 1) {
                if (play == inPlay) {
                    moveSpaceship(1);
                }
            } else if (gamechoice == 2) {
                person.x++;
                if(person.x > SCREEN_WIDTH - (BAR_SIZE/2) - 1) {
                    person.x = SCREEN_WIDTH - (BAR_SIZE/2) - 1;
                }
                if (ball.dir == pause) {
                    ball.x++;
                }
            }

        }else{
            if (choice == max_choice){
                choice = 0;
            }else{
                choice++;
            }
            QD << choice;
        }
    }
    if(key == down){
        if (play == notPlay){
            play = inPlay;
        }else if(play == inPlay) {
            if (ball.dir == pause) {
                ball.dir = NE;
                start = 1;
                playBall();
            }
            if(!hit){
                spawnBullet(0);
            }
        }else if(play == paused){
            if(choice == 0){
                QD << "return";
                play = menu;
                init();
            }
            else if(choice == 1){
                QD << "play on";
                play = inPlay;
            }else{
                displayPauseMenu(game_screen);
            }

        }else if(play == menu){
            if(choice == 0){
                toggle_multiplayer();
            }else if(choice == 1){
                gamechoice = 2;
                play = notPlay;
                init();
            }
            else if(choice == 2){
                gamechoice = 1;
                play = notPlay;
                init();
            }else{
                displayMenu(game_screen);
            }
        }
        else if(!hit){
            spawnBullet(0);
        }

    }
    if(key == up && play < 2){
        play = paused;
        choice = 1;
    }

    if(play == inPlay){
        if(gamechoice == 1) {
            spawnComet();

            delayBullet();

            moveComets();
            moveBullets();
            moveEnemyBullets();

        }else if(gamechoice == 2) {
            if (start == 1 && ball.dir != pause) {
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
    }
    if(play < paused){
        spawnStar();

        moveStars();
    }

}

int readInput()
{
    int newKey = 0;
    if(eventindex > 0) {
        newKey = eventlist[0];
        for(int i=0; i < eventindex; i++){
            eventlist[i] = eventlist[i+1];
        }
        eventindex--;
    }
    return newKey;
}

void updateScreen()
{
    if (play == menu){
        displayMenu(game_screen);
        return;
    }

    //background
    for(int x = 0; x < SCREEN_WIDTH; x++) {
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            game_screen[x][y] = DARK_GRAY;
        }
    }

    if(gamechoice == 1){
        //hit
        if (hit > 0){
            spaceShipHitColorToggle();
        }

        //stars
        displayStars(game_screen);

        //bullets
        displayAmmo(game_screen);

        //comets
        displayComets(game_screen);

        //spaceship
        displaySpaceShip(game_screen);

        //lives
        displayLives(game_screen);

        // score
        snprintf(str, 12, "%u", money);
        displayText(game_screen,str, SCREEN_WIDTH - 18, 1, WHITE);
    }else if(gamechoice == 2) {
        for(int x = 0; x < SCREEN_WIDTH; x++) {
            for(int y = 0; y < SCREEN_HEIGHT; y++) {
                game_screen[x][y] = GREEN;
            }
        }
        printScreen(game_screen);
    }
    //start
    if(!play){
        displayText(game_screen,"START", SCREEN_WIDTH - (SCREEN_WIDTH / 2)-14, SCREEN_HEIGHT - (SCREEN_HEIGHT / 2) - 4, WHITE);
    }
    else if(play == paused){
        displayPauseMenu(game_screen);
    }

}
