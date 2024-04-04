
#include "spaceInvaders.h"
#include "displayText.h"

bullets ammo;
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
uint32_t spaceshipCurrentColor = RED;
uint8_t hit_graph_counter = 60;

void initSpaceInvaders(){
    money = 0;
    lives = 3;

    // Seed the random number generator
    srand(static_cast<quint64>(QDateTime::currentMSecsSinceEpoch()));
    comet_delay = rand() % ((20 + 1) - 5) + 5;

    initSpaceShip();
    initAmmo();

    initComets();
    initStars();
}

void initSpaceShip(){
    SpaceShip.x = SCREEN_WIDTH/2;
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

void spawnBullet(){
    if(ammo.in_play < MAX_BULLETS && bullet_delay == 0){
        ammo.bullet_ar[ammo.in_play].x = SpaceShip.x;
        ammo.bullet_ar[ammo.in_play].y = SpaceShip.y - 3;
        ammo.in_play++;
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
    for(int i = 0; i < ammo.in_play; i++){
        for (int j = 0; j < Comet.in_play; j++){
            for (int k = 0; k < Comet.comet_ar[j].size; k++){
                if(ammo.bullet_ar[i].x == Comet.comet_ar[j].x + k){
                    if (ammo.bullet_ar[i].y <= Comet.comet_ar[j].y + Comet.comet_ar[j].size - 1){
                        //QD << "hit a comet";
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
            if(Comet.comet_ar[i].x + j <= SpaceShip.x + SPACESHIP_WIDTH / 2 && Comet.comet_ar[i].x + j >= SpaceShip.x - SPACESHIP_WIDTH / 2){
                if (Comet.comet_ar[i].y + Comet.comet_ar[i].size - 1 >= SpaceShip.y - SPACESHIP_HEIGHT / 2 && hit == 0){
                    hitComet(420,i);
                    hit = 1;
                    lives--;
                    if(lives <= 0){
                        play = 0;
                        hit = 0;
                        initSpaceInvaders();

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

void moveSpaceship(int direction){
    if(direction < 0){
        SpaceShip.x--;
        if(SpaceShip.x < SPACESHIP_WIDTH/2) {
            SpaceShip.x = (SPACESHIP_WIDTH/2);
        }
        return;
    }
    SpaceShip.x++;
    if(SpaceShip.x > SCREEN_WIDTH - (SPACESHIP_WIDTH/2) - 1) {
        SpaceShip.x = SCREEN_WIDTH - (SPACESHIP_WIDTH/2) - 1;
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

void initAmmo(){
    ammo.in_play=0;
    for (int i = 0;i<MAX_BULLETS;i++){
        ammo.bullet_ar[i].x = -1;
        ammo.bullet_ar[i].y = -1;
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


void displayStars(uint32_t game_screen[][SCREEN_HEIGHT]){
    for(int i = 0; i < Star.in_play; i++){
        game_screen[Star.star_ar[i].x][Star.star_ar[i].y] = YELLOW;
    }
}

void displayAmmo(uint32_t game_screen[][SCREEN_HEIGHT]){
    for(int i = 0; i < ammo.in_play; i++){
        game_screen[ammo.bullet_ar[i].x][ammo.bullet_ar[i].y] = RED;
    }
}

void displayComets(uint32_t game_screen[][SCREEN_HEIGHT]){
    for (int i = 0; i < Comet.in_play; i++){
        for (int j = 0; j < Comet.comet_ar[i].size; j++){
            int ycord = Comet.comet_ar[i].y + j;
            if(ycord <= SCREEN_HEIGHT - 1){
                for (int k = 0; k < Comet.comet_ar[i].size; k++){
                    int xcord = Comet.comet_ar[i].x + k;
                    if(xcord < SCREEN_WIDTH){
                        game_screen[xcord][ycord] = GRAY;
                    }

                }
            }
        }
    }
}

void displaySpaceShip(uint32_t game_screen[][SCREEN_HEIGHT]){
    for (int i = SpaceShip.x - SPACESHIP_WIDTH / 2; i <= SpaceShip.x + SPACESHIP_WIDTH / 2; i++) {
        for (int j = SpaceShip.y - SPACESHIP_HEIGHT / 2; j <= SpaceShip.y + SPACESHIP_HEIGHT / 2; j++) {
            if (i == SpaceShip.x && j != SpaceShip.y + SPACESHIP_HEIGHT / 2){
                game_screen[i][j] = spaceshipCurrentColor;
            }else if((i == SpaceShip.x - SPACESHIP_WIDTH / 2 || i == SpaceShip.x + SPACESHIP_WIDTH / 2) && j != SpaceShip.y - SPACESHIP_HEIGHT / 2){
                game_screen[i][j] = spaceshipCurrentColor;
            }else if((i == SpaceShip.x + 1 || SpaceShip.x - 1 ) && j == SpaceShip.y - (SPACESHIP_HEIGHT / 2) + 2){
                game_screen[i][j] = spaceshipCurrentColor;
            }
        }
    }
}

void displayLives(uint32_t game_screen[][SCREEN_HEIGHT]){
    for(int i = 0; i < lives; i++){
        game_screen[HEARTS_X+HEARTS_OFFSET*i][HEARTS_Y] = LIGHT_GREEN;
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
