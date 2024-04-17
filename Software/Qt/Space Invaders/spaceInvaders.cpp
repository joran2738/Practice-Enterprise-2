
#include "spaceInvaders.h"
#include "displayText.h"
#include "debug.h"

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
uint32_t spaceshipCurrentColor = RED;
uint8_t hit_graph_counter = 60;
uint8_t in_multiplayer = 1;
extern uint8_t connected;

void initSpaceInvaders(){
    money = 0;
    lives = 3;

    // Seed the random number generator
    srand(static_cast<quint64>(QDateTime::currentMSecsSinceEpoch()));
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
}

void moveEnemyBullets(void){
    if (!in_multiplayer){
        QD << "not multiplayer";
        return;
    }
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
}
void moveComets(){
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


void displayStars(uint32_t game_screen[][SCREEN_HEIGHT]){
    for(int i = 0; i < Star.in_play; i++){
        game_screen[Star.star_ar[i].x][Star.star_ar[i].y] = YELLOW;
    }
}

void displayAmmo(uint32_t game_screen[][SCREEN_HEIGHT]){
    for(int i = 0; i < ammo.in_play; i++){
        game_screen[ammo.bullet_ar[i].x][ammo.bullet_ar[i].y] = RED;
    }
    if(in_multiplayer){
        for(int i = 0; i < enemy_ammo.in_play; i++){
            game_screen[enemy_ammo.bullet_ar[i].x][enemy_ammo.bullet_ar[i].y] = LIGHT_BLUE;
        }
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
        QD << "a bullet has been send to the other player at x:" << bullet.x;
        if(enemy_ammo.in_play < MAX_BULLETS){
            enemy_ammo.bullet_ar[enemy_ammo.in_play].x = rand() % ((PLAYABLE_MAX - 1) - PLAYABLE_OFFSET) + PLAYABLE_OFFSET;
            enemy_ammo.bullet_ar[enemy_ammo.in_play].y = 3;
            enemy_ammo.in_play++;
        }else{
            star_delay--;
        }
    }
}

