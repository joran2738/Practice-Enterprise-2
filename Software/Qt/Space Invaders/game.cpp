//This code is meant to be plain c code so it can be used on an STM32 microcontroller.

#include "debug.h"
#include "game.h"
#include <QDateTime>
#include "displayText.h"
#include "spaceInvaders.h"
#include "menu.h"
#include "multiplayer.h"

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


void init (void) {
    choice = 0;
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            game_screen[x][y] = DARK_GRAY;
        }
    }
    if(gamechoice == 1){
        initSpaceInvaders();
    }else{
        //nothing yet, your init
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
            moveSpaceship(-1);
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
            moveSpaceship(1);
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
                gamechoice = 0;
                play = notPlay;
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
        spawnComet();

        delayBullet();

        moveComets();
        moveBullets();
        moveEnemyBullets();
        updateScreen();
    }if(play < paused){
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

    //hit
    if (gamechoice == 1 && hit > 0){
        spaceShipHitColorToggle();
    }
    //background
    for(int x = 0; x < SCREEN_WIDTH; x++) {
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            game_screen[x][y] = DARK_GRAY;
        }
    }

    if(gamechoice == 1){
        //stars
        displayStars(game_screen);

        //bullets
        displayAmmo(game_screen);

        // comets
        displayComets(game_screen);
    }else{
        //nothin yet, here comes your things
    }

    //start
    if(!play){
        displayText(game_screen,"START", SCREEN_WIDTH - (SCREEN_WIDTH / 2)-14, SCREEN_HEIGHT - (SCREEN_HEIGHT / 2) - 4, WHITE);
    }
    else if(play == paused){
        displayPauseMenu(game_screen);
    }

    if(gamechoice == 1){
        //spaceship
        displaySpaceShip(game_screen);

        //lives
        displayLives(game_screen);

        // score
        snprintf(str, 12, "%u", money);
        displayText(game_screen,str, SCREEN_WIDTH - 18, 1, WHITE);
    }else{
        displayText(game_screen,"bricks", SCREEN_WIDTH - (SCREEN_WIDTH / 2) - 18, SCREEN_HEIGHT - (SCREEN_HEIGHT / 2) + 5, WHITE);
        //nothin yet, here comes your things
    }


}






