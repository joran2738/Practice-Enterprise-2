//This code is meant to be plain c code so it can be used on an STM32 microcontroller.

#include "debug.h"
#include "game.h"
#include <QDateTime>
#include "ledFont5x8.h"
#include "menu.h"

#define DEBUG 1

uint8_t eventlist[EVENTSIZE];
uint8_t eventindex = 0;
uint32_t game_screen[SCREEN_WIDTH][SCREEN_HEIGHT];
static point person = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 5};
bullets ammo;
comets Comet;
stars Star;

uint8_t bullet_delay = 0;
uint8_t comet_delay = 0;

uint8_t star_delay = 0;


uint8_t hit = 0;
uint8_t hit_graph_counter = 60;
uint32_t spaceshipColorNormal = 0xFFFF0000;
uint32_t spaceshipColorHit = 0xf7f7f7;
uint32_t spaceshipCurrentColor = spaceshipColorNormal;

uint8_t lives = 3;
uint8_t money = 0;

uint8_t choice = 2;
uint8_t play = 0;

char str[12];


void init (void) {
    person.x = SCREEN_WIDTH/2;
    money = 0;
    lives = 3;
    choice = 2;
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            game_screen[x][y] = 0xFF00FF00; //Green
        }
    }
    ammo.in_play=0;
    for (int i = 0;i<MAX_BULLETS;i++){
        ammo.bullet_ar[i].x = -1;
        ammo.bullet_ar[i].y = -1;
    }

    // Seed the random number generator
    srand(static_cast<quint64>(QDateTime::currentMSecsSinceEpoch()));
    comet_delay = rand() % ((20 + 1) - 5) + 5;

    Comet.in_play = 0;
    for (int i = 0;i<MAX_COMETS;i++){
        Comet.comet_ar[i].x = -1;
        Comet.comet_ar[i].y = -1;
        Comet.comet_ar[i].size = 2;
    }

    Star.in_play = 0;
    for (int i = 0;i<MAX_COMETS;i++){
        Star.star_ar[i].x = -1;
        Star.star_ar[i].y = -1;
    }
}

void loop (void) {
    int key = readInput();
    updateScreen();
    if(key == left) {
        if(play < 2){
            person.x--;
            if(person.x < SPACESHIP_WIDTH/2) {
                person.x = (SPACESHIP_WIDTH/2);
            }
        }else{
            if (choice == 1){
                choice--;
            }else if(choice == 0){
                choice++;
        }
    }
    }
    if(key == right) {
        if(play < 2){
            person.x++;
            if(person.x > SCREEN_WIDTH - (SPACESHIP_WIDTH/2) - 1) {
                person.x = SCREEN_WIDTH - (SPACESHIP_WIDTH/2) - 1;
            }
        }else{
            if (choice == 1){
                choice--;
            }else if(choice == 0){
                choice++;
            }
        }
    }
    if(key == up){
        QD << rand() % ((20 + 1) - 5) + 5;
        if (play == 0){
            play = 1;
        }else if(play == 2){
            if(choice == 0){
                QD << "return";
                play = 0;
                init();
            }
            else if(choice == 1){
                QD << "play on";
                play = 1;
            }else{
                displayPauseMenu(game_screen,none);
            }

        }
        else if(!hit){
            if(ammo.in_play < MAX_BULLETS && bullet_delay == 0){
                ammo.bullet_ar[ammo.in_play].x = person.x;
                ammo.bullet_ar[ammo.in_play].y = person.y - 3;
                ammo.in_play++;
                bullet_delay = MAX_DELAY_BULLETS;
            }
        }

    }
    if(key == down){
        play = 2;
        choice = 1;
    }

    if(play == 1){
        if(comet_delay < 1){


            if(Comet.in_play < MAX_COMETS){
                Comet.comet_ar[Comet.in_play].x = rand() % ((PLAYABLE_MAX - 1) - PLAYABLE_OFFSET) + PLAYABLE_OFFSET;
                Comet.comet_ar[Comet.in_play].y = 0;
                Comet.comet_ar[Comet.in_play].size = rand() % ((MAX_COMET_SIZE + 1) - MIN_COMET_SIZE) + MIN_COMET_SIZE;
                Comet.comet_ar[Comet.in_play].coins = 5;
                comet_delay = rand() % ((20 + 1) - 10) + 5;
                Comet.in_play++;
            }

        }else{
            comet_delay--;
        }
        if (bullet_delay > 0){
            bullet_delay--;
        }

        moveComets();
        moveBullets();
        updateScreen();
    }if(play < 2){
        if(star_delay < 1){


            if(Star.in_play < MAX_STARS){
                Star.star_ar[Star.in_play].x = rand() % ((PLAYABLE_MAX - 1) - PLAYABLE_OFFSET) + PLAYABLE_OFFSET;
                Star.star_ar[Star.in_play].y = 0;
                star_delay = rand() % ((20 + 1) - 10) + 5;
                Star.in_play++;
            }

        }else{
            star_delay--;
        }
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
    //hit
    if (hit > 0){
        if(spaceshipCurrentColor == spaceshipColorNormal && hit % 10 == 0){
            spaceshipCurrentColor = spaceshipColorHit;
            QD << "white";
        }else if(hit % 10 == 0){
            spaceshipCurrentColor = spaceshipColorNormal;
            QD << "red";
        }
        if(hit < hit_graph_counter){
            hit++;
        }else{
            hit = 0;
        }
    }
    //background
    for(int x = 0; x < SCREEN_WIDTH; x++) {
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            game_screen[x][y] = 0x2e2c2c;
        }
    }

    //stars
    for(int i = 0; i < Star.in_play; i++){
        game_screen[Star.star_ar[i].x][Star.star_ar[i].y] = 0xede609;
    }

    //bullets
    for(int i = 0; i < ammo.in_play; i++){
        game_screen[ammo.bullet_ar[i].x][ammo.bullet_ar[i].y] = 0xFFFF0000; //RED
    }

    // comets
    for (int i = 0; i < Comet.in_play; i++){
        for (int j = 0; j < Comet.comet_ar[i].size; j++){
            int ycord = Comet.comet_ar[i].y + j;
            if(ycord <= SCREEN_HEIGHT - 1){
                for (int k = 0; k < Comet.comet_ar[i].size; k++){
                    int xcord = Comet.comet_ar[i].x + k;
                    if(xcord < SCREEN_WIDTH){
                        game_screen[xcord][ycord] = 0x969696;
                    }

                }
            }
        }
    }

    //start
    if(!play){
        displayText("START", SCREEN_WIDTH - (SCREEN_WIDTH / 2)-14, SCREEN_HEIGHT - (SCREEN_HEIGHT / 2) - 4, 0xf7f7f7);
    }
    else if(play == 2){
        displayPauseMenu(game_screen,in);
    }

    //spaceship
    for (int i = person.x - SPACESHIP_WIDTH / 2; i <= person.x + SPACESHIP_WIDTH / 2; i++) {
        for (int j = person.y - SPACESHIP_HEIGHT / 2; j <= person.y + SPACESHIP_HEIGHT / 2; j++) {
            if (i == person.x && j != person.y + SPACESHIP_HEIGHT / 2){
                game_screen[i][j] = spaceshipCurrentColor;
            }else if((i == person.x - SPACESHIP_WIDTH / 2 || i == person.x + SPACESHIP_WIDTH / 2) && j != person.y - SPACESHIP_HEIGHT / 2){
                game_screen[i][j] = spaceshipCurrentColor;
            }else if((i == person.x + 1 || person.x - 1 ) && j == person.y - (SPACESHIP_HEIGHT / 2) + 2){
                game_screen[i][j] = spaceshipCurrentColor;
            }
        }
    }



    //lives
    for(int i = 0; i < lives; i++){
        game_screen[HEARTS_X+HEARTS_OFFSET*i][HEARTS_Y] = 0x02fa3c;
    }

    // score
    snprintf(str, 12, "%u", money);
    displayText(str, SCREEN_WIDTH - 18, 1, 0xf7f7f7);
}

void moveBullets(){
    for(int i = 0; i < ammo.in_play; i++){
        for (int j = 0; j < Comet.in_play; j++){
            for (int k = 0; k < Comet.comet_ar[j].size; k++){
                if(ammo.bullet_ar[i].x == Comet.comet_ar[j].x + k){
                    if (ammo.bullet_ar[i].y <= Comet.comet_ar[j].y + Comet.comet_ar[j].size - 1){
                        QD << "hit a comet";
                        hitComet(i,j);
                    }
                }
            }
        }
        ammo.bullet_ar[i].y--;
        if (ammo.bullet_ar[i].y < 0){
            for(int j = i; j < MAX_BULLETS - 1; j++){
                ammo.bullet_ar[j].x = ammo.bullet_ar[j+1].x;
                ammo.bullet_ar[j].y = ammo.bullet_ar[j+1].y;
            }
            ammo.in_play--;
        }
    }
}

void moveComets(){
    for (int i = 0; i<Comet.in_play; i++){
        for(int j = 0; j < Comet.comet_ar[i].size; j++){
            if(Comet.comet_ar[i].x + j <= person.x + SPACESHIP_WIDTH / 2 && Comet.comet_ar[i].x + j >= person.x - SPACESHIP_WIDTH / 2){
                if (Comet.comet_ar[i].y + Comet.comet_ar[i].size - 1 >= person.y - SPACESHIP_HEIGHT / 2 && hit == 0){
                    QD << "been hit";
                    hitComet(420,i);
                    hit = 1;
                    lives--;
                    if(lives <= 0){
                        play = 0;
                        hit = 0;
                        QD << "money: " << money << " coins";
                        init();

                    }
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
}

void moveStars(){
    for (int i = 0; i<Star.in_play; i++){

        Star.star_ar[i].y++;
        if (Star.star_ar[i].y > SCREEN_HEIGHT - 1){
            for(int j = i; j < MAX_COMETS - 1; j++){
                Star.star_ar[j].x = Star.star_ar[j+1].x;
                Star.star_ar[j].y = Star.star_ar[j+1].y;
            }
            Star.in_play--;
        }
    }
}

void hitComet(int bul,int com){
    money += Comet.comet_ar[com].coins;
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

void displayText(char * text, int x, int y, uint32_t color) {

    unsigned char buffer;
    int bit;

    for (int i = 0; text[i] != '\0'; i++) {
        unsigned char c = text[i];
        c = c - ' ';


        for (int j = 0; j < 5; j++) {
            buffer = ledFont[c][j];

            for (int k = 0; k < 8; k++) {
                bit = (buffer >> k) & 1;

                if (bit == 1) {
                    game_screen[x][y + k + 1] = color; //black
                }
            }
            x++;
        }
        x++;
    }
}



