/*
 * brickbreaker.c
 *
 *  Created on: 16 mei 2024
 *      Author: vande
 */

#include "brickbreaker.h"
#include "displayText.h"
#include "debug.h"
#include "z_displ_ILI9XXX.h"

extern point person;
extern ballPoint ball;
static brick bricks[MAX_BRICK_LINES][10];
//static point score = {1, 9};
//static point highScorePoint = {1, 0};
extern uint8_t start;
uint8_t points = 0;
uint8_t lives1 = 3;
extern uint8_t delay;
uint8_t highScore = 0;
uint8_t loopTester = 0;
uint8_t guardian = 0;
uint8_t guardianTimer = 100;

void initBrickbreaker() {
    person.x = SCREEN_WIDTH/2;
    person.y = SCREEN_HEIGHT - 5;
    ball.x = SCREEN_WIDTH/2;
    ball.y = SCREEN_HEIGHT - 6;
    ball.dir = still;
    for (int i = 0; i < MAX_BRICK_LINES; i++) {
        for(int j = 0; j < 10; j++) {
            bricks[i][j].x = j * 8;
            bricks[i][j].y = i + 8;
            bricks[i][j].visible = 0;
            bricks[i][j].toughness = 1;
        }
    }

    for (int i = 0; i < BRICK_LINES; i++) {
        for(int j = 0; j < 10; j++) {
            bricks[i][j].visible = 1;
            bricks[i][j].toughness = setBrickToughness();
        }
    }

    Displ_CLS(D_GREEN);
}

void playBall() {
    if (ball.y == 0) {
        changeDirection(N);
    }
    if (ball.y == SCREEN_HEIGHT - 6) {
        if (ball.x >= person.x - (BAR_SIZE/2) && ball.x <= person.x + (BAR_SIZE/2)) {
            changeDirection(S);
        } else {
            if (ball.x == person.x - (BAR_SIZE/2) - 1) {
                if (ball.dir == SE) {
                    ball.dir = NW;
                } else {
                    lives1--;
                    gameEnd();
                }
            } else {
                if (ball.x == person.x + (BAR_SIZE/2) + 1) {
                    if (ball.dir == SW) {
                        ball.dir = NE;
                    } else {
                        lives1--;
                        gameEnd();
                    }
                } else {
                    lives1--;
                    gameEnd();
                }
            }
        }
    }
    if (ball.x == 0) {
        changeDirection(W);
    }
    if (ball.x == SCREEN_WIDTH - 1) {
        changeDirection(E);
    }

    if (guardian == 1) {
        if (ball.y == person.y - 2) {
            changeDirection(S);
        }
    }

    checkBrickHit();
    moveBall();
}

void moveBall() {
    Displ_FillArea(ball.x*4, ball.y*4, 4, 4, D_GREEN);
    switch (ball.dir) {
    case N:
        ball.y -= 1;
        break;
    case S:
        ball.y += 1;
        break;
    case NE:
        ball.y -= 1;
        ball.x += 1;
        break;
    case NW:
        ball.y -= 1;
        ball.x -= 1;
        break;
    case SE:
        ball.y += 1;
        ball.x += 1;
        break;
    case SW:
        ball.y += 1;
        ball.x -= 1;
        break;
    case still:
        break;
    default:
        //////QD << "You shouldn't be here!";
        ball.x = ball.x;
    }

}

void changeDirection(directions inDir) {
    if (inDir == N) {
        switch (ball.dir) {
        case N:
            ball.dir = S;
            break;
        case NW:
            ball.dir = SW;
            break;
        case NE:
            ball.dir = SE;
            break;
        case still:
            break;
        default:
            //////QD << "You shouldn't be here!";
            ball.x = ball.x;
        }
    }

    if (inDir == E) {
        switch (ball.dir) {
        case NE:
            ball.dir = NW;
            break;
        case SE:
            ball.dir = SW;
            break;
        case still:
            break;
        default:
            //////QD << "You shouldn't be here!";
            ball.x = ball.x;
        }
    }

    if (inDir == S) {
        switch (ball.dir) {
        case S:
            ball.dir = N;
            break;
        case SE:
            ball.dir = NE;
            break;
        case SW:
            ball.dir = NW;
            break;
        case still:
            break;
        default:
            //////QD << "You shouldn't be here!";
            ball.dir = ball.dir;
        }
    }

    if (inDir == W) {
        switch (ball.dir) {
        case NW:
            ball.dir = NE;
            break;
        case SW:
            ball.dir = SE;
            break;
        case still:
            break;
        default:
            //////QD << "You shouldn't be here!";
            ball.x = ball.x;
        }
    }
}

void gameEnd() {
    Displ_FillArea(0, (person.y - 1)*4, SCREEN_WIDTH, 10, D_GREEN);
    person.x = (SCREEN_WIDTH)/2;
    ball.x = person.x;
    ball.y = SCREEN_HEIGHT - 6;
    ball.dir = still;

    if (lives1 <= 0) {
        if (points > highScore) {
            highScore = points;
            //////QD << highScore;
        }

        start = 0;
        points = 0;
        lives1 = 3;
    }
}

void lowerBricks() {
    for (int i = MAX_BRICK_LINES - 2; i >= 0; i--) {
        for (int j = 0; j < 10; j++) {
            bricks[i + 1][j].visible = bricks[i][j].visible;
            bricks[i + 1][j].toughness = bricks[i][j].toughness;
        }
    }
    for(int j = 0; j < 10; j++) {
        bricks[0][j].visible = 1;
        bricks[0][j].toughness = setBrickToughness();
    }
}

void checkGameOver() {
    for (int j = 0; j < 10; j++) {
        if (bricks[MAX_BRICK_LINES - 1][j].visible == 1) {

            lives1 = 0;
            ball.x = person.x;
            ball.y = SCREEN_HEIGHT - 6;
            ball.dir = still;
            gameEnd();
            //////QD << "Game Over, noob";
        }
    }
}

void checkBrickHit() {
    uint8_t bricksHit = 0;
    switch (ball.dir) {
    case NW :
        if (bricks[ball.y - 1 - 8][ball.x/8].visible == 1) {
            bricks[ball.y - 1 - 8][ball.x/8].toughness--;
            if (bricks[ball.y - 1 - 8][ball.x/8].toughness <= 0) {
                bricks[ball.y - 1 - 8][ball.x/8].visible = 0;
                points++;
                //////QD << "score:" << points;
                Displ_FillArea(bricks[ball.y - 1 - 8][ball.x/8].x*4, bricks[ball.y - 1 - 8][ball.x/8].y*4, 32, 4, D_GREEN);
                dropPowerUp(ball.x/8, ball.y - 1 - 8);
            }
            bricksHit++;
            ball.dir = SW;
        }
        if (bricks[ball.y - 8][(ball.x - 1)/8].visible == 1) {
            bricks[ball.y - 8][(ball.x - 1)/8].toughness--;
            if (bricks[ball.y - 8][(ball.x - 1)/8].toughness <= 0) {
                bricks[ball.y - 8][(ball.x - 1)/8].visible = 0;
                points++;
                //////QD << "score:" << points;
                Displ_FillArea(bricks[ball.y - 8][(ball.x - 1)/8].x*4, bricks[ball.y - 8][(ball.x - 1)/8].y*4, 32, 4, D_GREEN);
                dropPowerUp((ball.x - 1)/8, ball.y - 8);
            }
            bricksHit++;
            if (ball.dir == NW) {
                ball.dir = NE;
            }
            if (ball.dir == SW) {
                ball.dir = SE;
            }
        }
        if (bricksHit <= 0) {
            if (bricks[ball.y - 1 - 8][(ball.x - 1)/8].visible == 1) {
                bricks[ball.y - 1 - 8][(ball.x - 1)/8].toughness--;
                if (bricks[ball.y - 1 - 8][(ball.x - 1)/8].toughness <= 0) {
                    bricks[ball.y - 1 - 8][(ball.x - 1)/8].visible = 0;
                    points++;
                    //////QD << "score:" << points;
                    Displ_FillArea(bricks[ball.y - 1 - 8][(ball.x - 1)/8].x*4, bricks[ball.y - 1 - 8][(ball.x - 1)/8].y*4, 32, 4, D_GREEN);
                    dropPowerUp((ball.x - 1)/8, ball.y - 1 - 8);
                }
                ball.dir = SE;
            }
        }
        break;
    case NE:
        if (bricks[ball.y - 1 - 8][ball.x/8].visible == 1) {
            bricks[ball.y - 1 - 8][ball.x/8].toughness--;
            if (bricks[ball.y - 1 - 8][ball.x/8].toughness <= 0) {
                bricks[ball.y - 1 - 8][ball.x/8].visible = 0;
                points++;
                //////QD << "score:" << points;
                Displ_FillArea(bricks[ball.y - 1 - 8][ball.x/8].x*4, bricks[ball.y - 1 - 8][ball.x/8].y*4, 32, 4, D_GREEN);
                dropPowerUp(ball.x/8, ball.y - 1 - 8);
            }
            bricksHit++;
            ball.dir = SE;
        }
        if (bricks[ball.y - 8][(ball.x + 1)/8].visible == 1) {
            bricks[ball.y - 8][(ball.x + 1)/8].toughness--;
            if (bricks[ball.y - 8][(ball.x + 1)/8].toughness <= 0) {
                bricks[ball.y - 8][(ball.x + 1)/8].visible = 0;
                points++;
                //////QD << "score:" << points;
                Displ_FillArea(bricks[ball.y - 8][(ball.x + 1)/8].x*4, bricks[ball.y - 8][(ball.x + 1)/8].y*4, 32, 4, D_GREEN);
                dropPowerUp((ball.x + 1)/8, ball.y - 8);
            }
            bricksHit++;
            if (ball.dir == NE) {
                ball.dir = NW;
            }
            if (ball.dir == SE) {
                ball.dir = SW;
            }
        }
        if (bricksHit <= 0) {
            if (bricks[ball.y - 1 - 8][(ball.x + 1)/8].visible == 1) {
                bricks[ball.y - 1 - 8][(ball.x + 1)/8].toughness--;
                if (bricks[ball.y - 1 - 8][(ball.x + 1)/8].toughness <= 0) {
                    bricks[ball.y - 1 - 8][(ball.x + 1)/8].visible = 0;
                    points++;
                    //////QD << "score:" << points;
                    Displ_FillArea(bricks[ball.y - 1 - 8][(ball.x + 1)/8].x*4, bricks[ball.y - 1 - 8][(ball.x + 1)/8].y*4, 32, 4, D_GREEN);
                    dropPowerUp((ball.x + 1)/8, ball.y - 1 - 8);
                }
                ball.dir = SW;
            }
        }
        break;
    case SE:
        if (bricks[ball.y + 1 - 8][ball.x/8].visible == 1) {
            bricks[ball.y + 1 - 8][ball.x/8].toughness--;
            if (bricks[ball.y + 1 - 8][ball.x/8].toughness <= 0) {
                bricks[ball.y + 1 - 8][ball.x/8].visible = 0;
                points++;
                //////QD << "score:" << points;
                Displ_FillArea(bricks[ball.y + 1 - 8][ball.x/8].x*4, bricks[ball.y + 1 - 8][ball.x/8].y*4, 32, 4, D_GREEN);
                dropPowerUp(ball.x/8, ball.y + 1 - 8);
            }
            bricksHit++;
            ball.dir = NE;
        }
        if (bricks[ball.y - 8][(ball.x + 1)/8].visible == 1) {
            bricks[ball.y - 8][(ball.x + 1)/8].toughness--;
            if (bricks[ball.y - 8][(ball.x + 1)/8].toughness <= 0) {
                bricks[ball.y - 8][(ball.x + 1)/8].visible = 0;
                points++;
                //////QD << "score:" << points;
                Displ_FillArea(bricks[ball.y - 8][(ball.x + 1)/8].x*4, bricks[ball.y - 8][(ball.x + 1)/8].y*4, 32, 4, D_GREEN);
                dropPowerUp((ball.x + 1)/8, ball.y - 8);
            }
            bricksHit++;
            if (ball.dir == SE) {
                ball.dir = SW;
            }
            if (ball.dir == NE) {
                ball.dir = NW;
            }
        }
        if (bricksHit <= 0) {
            if (bricks[ball.y - 8 + 1][(ball.x + 1)/8].visible == 1) {
                bricks[ball.y - 8 + 1][(ball.x + 1)/8].toughness--;
                if (bricks[ball.y - 8 + 1][(ball.x + 1)/8].toughness <= 0) {
                    bricks[ball.y - 8 + 1][(ball.x + 1)/8].visible = 0;
                    points++;
                    //////QD << "score:" << points;
                    Displ_FillArea(bricks[ball.y - 8 + 1][(ball.x + 1)/8].x*4, bricks[ball.y - 8 + 1][(ball.x + 1)/8].y*4, 32, 4, D_GREEN);
                    dropPowerUp((ball.x + 1)/8, ball.y - 8 + 1);
                }
                ball.dir = NW;
            }
        }
        break;
    case SW:
        if (bricks[ball.y + 1 - 8][ball.x/8].visible == 1) {
            bricks[ball.y + 1 - 8][ball.x/8].toughness--;
            if (bricks[ball.y + 1 - 8][ball.x/8].toughness <= 0) {
                bricks[ball.y + 1 - 8][ball.x/8].visible = 0;
                points++;
                ////QD << "score:" << points;
                Displ_FillArea(bricks[ball.y + 1 - 8][ball.x/8].x*4, bricks[ball.y + 1 - 8][ball.x/8].y*4, 32, 4, D_GREEN);
                dropPowerUp(ball.x/8, ball.y + 1 - 8);
            }
            bricksHit++;
            ball.dir = NW;
        }
        if (bricks[ball.y - 8][(ball.x - 1)/8].visible == 1) {
            bricks[ball.y - 8][(ball.x - 1)/8].toughness--;
            if (bricks[ball.y - 8][(ball.x - 1)/8].toughness <= 0) {
                bricks[ball.y - 8][(ball.x - 1)/8].visible = 0;
                points++;
                ////QD << "score:" << points;
                Displ_FillArea(bricks[ball.y - 8][(ball.x - 1)/8].x*4, bricks[ball.y - 8][(ball.x - 1)/8].y*4, 32, 4, D_GREEN);
                dropPowerUp((ball.x - 1)/8, ball.y - 8);
            }
            bricksHit++;
            if (ball.dir == SW) {
                ball.dir = SE;
            }
            if (ball.dir == NW) {
                ball.dir = NE;
            }
        }
        if (bricksHit <= 0) {
            if (bricks[ball.y + 1 - 8][(ball.x - 1)/8].visible == 1) {
                bricks[ball.y + 1 - 8][(ball.x - 1)/8].toughness--;
                if (bricks[ball.y + 1 - 8][(ball.x - 1)/8].toughness <= 0) {
                    bricks[ball.y + 1 - 8][(ball.x - 1)/8].visible = 0;
                    points++;
                    ////QD << "score:" << points;
                    Displ_FillArea(bricks[ball.y + 1 - 8][(ball.x - 1)/8].x*4, bricks[ball.y + 1 - 8][(ball.x - 1)/8].y*4, 32, 4, D_GREEN);
                    dropPowerUp((ball.x - 1)/8, ball.y + 1 - 8);
                }
                ball.dir = NE;
            }
        }
        break;
    case still:
        break;
    default:
        //////QD << "You shouldn't be here";
        ball.dir = ball.dir;
    }
}

void dropPowerUp(int x, int y) {
    uint8_t dropChance = 0;
    if (guardian == 0) {
        srand(HAL_GetTick());
        dropChance = rand() % 100;
    }

    if (dropChance <= 10 && dropChance > 0) {
        guardian = 1;
        //////QD << "A power up just dropped from brick at x =" << x << "y =" << y;
    }

    if (dropChance <= 15 && dropChance > 10) {
        //////QD << "test";
        explodeBricks(x, y);
        //////QD << "An explosion just happened at x =" << x << "y =" << y;
    }
}

void printScreen() {
    for (int i = 0; i < lives1; i++) {
        Displ_FillArea(SCREEN_WIDTH*4 - 8 - (i*8), 4, 4, 4, D_BLUE);
    }

    if (guardian == 1) {
        Displ_FillArea(0, (person.y - 1)*4, SCREEN_WIDTH*4, 4, GREY);
    }
//
//    for(int x = 0; x < SCREEN_WIDTH; x++){
//        for(int y = 0; y < SCREEN_HEIGHT; y++){
//            if(x == person.x && y == person.y){
//                for(int i = x - BAR_SIZE/2; i <= x + BAR_SIZE/2; i++) {
//                    game_screen[i][y] = ENC_RED;
//                }
//            }
//        }
//    }
    Displ_FillArea(person.x*4 - BAR_SIZE*2, person.y*4, BAR_SIZE*4, 4, RED);
//
//    game_screen[ball.x][ball.y] = ENC_BLUE;
    Displ_FillArea(ball.x*4, ball.y*4, 4, 4, D_BLUE);
//
    for (int i = 0; i < MAX_BRICK_LINES; i++) {
        for(int j = 0; j < 10; j++) {
            if(bricks[i][j].visible == 1) {
                if (bricks[i][j].toughness == 3) {
                    Displ_FillArea(bricks[i][j].x*4, bricks[i][j].y*4, 32, 4, BLACK);
                }
                if (bricks[i][j].toughness == 2) {
                    Displ_FillArea(bricks[i][j].x*4, bricks[i][j].y*4, 32, 4, DARK_GREY);
                }
                if (bricks[i][j].toughness == 1) {
                    Displ_FillArea(bricks[i][j].x*4, bricks[i][j].y*4, 32, 4, GREY);
                }
            }
        }
    }

    //char str[12];
    //snprintf(str, 12, "%u", points);
    //displayText(game_screen, str, score.x, score.y, WHITE);
    //snprintf(str, 12, "%u", highScore);
    //displayText(game_screen, str, highScorePoint.x, highScorePoint.y, WHITE);
}

void checkGuardianTimer() {
    if (guardian == 1) {
        if (guardianTimer <= 0) {
            guardian = 0;
            guardianTimer = 100;
            Displ_FillArea(0, (person.y - 1)*4, SCREEN_WIDTH*4, 4, D_GREEN);
        } else {
            guardianTimer--;
        }
    }
}

void explodeBricks(int x, int y) {
    int ymin = y - 1;
    int xmin = x - 1;
    int ymax = y + 1;
    int xmax = x + 1;
    if(y == 0) {
        ymin = y;
    }
    if(y == MAX_BRICK_LINES) {
        ymax = y;
    }
    if(x == 0) {
        xmin = x;
    }
    if(x == 10) {
        xmax = x;
    }


    for(int i = ymin; i <= ymax; i++) {
        for(int j = xmin; j <= xmax; j++) {
            if(bricks[i][j].visible == 1) {
                bricks[i][j].toughness = 0;
                bricks[i][j].visible = 0;
                points++;
            }
        }
    }
}

int setBrickToughness(void) {
    int toughnessPercent;
    int toughness = 1;

    toughnessPercent = rand() % (100) + 1;

    if(toughnessPercent > 0 && toughnessPercent <= 60) {
        toughness = 1;
    } else if(toughnessPercent > 60 && toughnessPercent < 90) {
        toughness = 2;
    } else if(toughnessPercent > 90 && toughnessPercent < 100) {
        toughness = 3;
    }

    return toughness;
}
