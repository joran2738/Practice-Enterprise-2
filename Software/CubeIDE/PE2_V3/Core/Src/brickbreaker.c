/*
 * brickbreaker.c
 *
 *      Author: Robin
 */

#include "brickbreaker.h"
#include "displayText.h"
#include "debug.h"
#include "z_displ_ILI9XXX.h"

extern UART_HandleTypeDef huart2;

extern point person;
extern ballPoint ball;
static brick bricks[MAX_BRICK_LINES][10];
static point score = {1, 9};
//static point highScorePoint = {1, 0};
extern uint8_t start;
uint8_t points = 0;
uint8_t lives1 = 3;
extern uint8_t delay;
uint8_t highScore = 0;
uint8_t loopTester = 0;
uint8_t guardian = 0;
uint8_t guardianTimer = 100;
int toughnessColor[4] = {D_GREEN, GREY, DARK_GREY, BLACK};

/**
 * @fn void initBrickbreaker()
 * @brief initialises the brickbreaker game:
 *      -puts the player bar and ball to the starting location and state
 *      -creates a new set of bricks
 *      -sets score to 0
 *      -sets the first lines of bricks to visible
 *      -draws the bricks
 *
 */
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
    points = 0;
    Displ_CLS(D_GREEN);
    drawBricks();
}

/**
 * @fn void playBall()
 * @brief handles the movement of the ball, changes its direction when needed
 * also checks if the ball hit a brick or the user bar
 *
 */
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

/**
 * @fn void moveBall()
 * @brief moves the ball along it's direction
 *
 */
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
        printf("You shouldn't be here\r\n");
    }

}

/**
 * @fn void changeDirection(directions)
 * @brief changes the direction of the ball based on the direction of the obstacle the ball hit
 *
 * @param dirHit the direction where the ball hit an obstacle
 */
void changeDirection(directions dirHit) {
    if (dirHit == N) {
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
            printf("You shouldn't be here\r\n");
        }
    }

    if (dirHit == E) {
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
            printf("You shouldn't be here\r\n");
        }
    }

    if (dirHit == S) {
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
            printf("You shouldn't be here\r\n");
        }
    }

    if (dirHit == W) {
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
            printf("You shouldn't be here\r\n");
        }
    }
}

/**
 * @fn void gameEnd()
 * @brief called when the player has lost a life, resets the player bar and ball to the initial position
 * if the player has lost all his lives, also resets score
 *
 */
void gameEnd() {
    Displ_FillArea(0, (person.y - 1)*4, SCREEN_WIDTH*4, 10, D_GREEN);
    person.x = (SCREEN_WIDTH)/2;
    ball.x = person.x;
    ball.y = SCREEN_HEIGHT - 6;
    ball.dir = still;

    if (lives1 <= 0) {
        if (points > highScore) {
            highScore = points;
        }

        start = 0;
        points = 0;
        lives1 = 3;
        initBrickbreaker();
        HAL_UART_Transmit(&huart2, (uint8_t *)"L", strlen("L"), 300);
        setGameState(loss);
    }
}

/**
 * @fn void lowerBricks()
 * @brief lowers the bricks to the next level
 *
 */
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

    drawBricks();
}
/**
 * @fn void checkGameOver()
 * @brief checks if the final line of active bricks has reaches the player bar
 *
 */
void checkGameOver() {
    for (int j = 0; j < 10; j++) {
        if (bricks[MAX_BRICK_LINES - 1][j].visible == 1) {

            lives1 = 0;
            ball.x = person.x;
            ball.y = SCREEN_HEIGHT - 6;
            ball.dir = still;
            gameEnd();
        }
    }
}

/**
 * @fn void checkBrickHit()
 * @brief checks if the ball hit one or more bricks
 *
 */
void checkBrickHit() {
    uint8_t bricksHit = 0;
    switch (ball.dir) {
    case NW :
        if (bricks[ball.y - 1 - 8][ball.x/8].visible == 1) {
            bricks[ball.y - 1 - 8][ball.x/8].toughness--;
            if (bricks[ball.y - 1 - 8][ball.x/8].toughness <= 0) {
                bricks[ball.y - 1 - 8][ball.x/8].visible = 0;
                points++;
                dropPowerUp(ball.x/8, ball.y - 1 - 8);
                checkEmptyRow(ball.y - 1 - 8);
            }
            Displ_FillArea(bricks[ball.y - 1 - 8][ball.x/8].x*4, bricks[ball.y - 1 - 8][ball.x/8].y*4, 32, 4, toughnessColor[bricks[ball.y - 1 - 8][ball.x/8].toughness]);
            bricksHit++;
            ball.dir = SW;
        }
        if (bricks[ball.y - 8][(ball.x - 1)/8].visible == 1) {
            bricks[ball.y - 8][(ball.x - 1)/8].toughness--;
            if (bricks[ball.y - 8][(ball.x - 1)/8].toughness <= 0) {
                bricks[ball.y - 8][(ball.x - 1)/8].visible = 0;
                points++;
                dropPowerUp((ball.x - 1)/8, ball.y - 8);
                checkEmptyRow(ball.y - 8);
            }
            Displ_FillArea(bricks[ball.y - 8][(ball.x - 1)/8].x*4, bricks[ball.y - 8][(ball.x - 1)/8].y*4, 32, 4, toughnessColor[bricks[ball.y - 8][(ball.x - 1)/8].toughness]);
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
                    dropPowerUp((ball.x - 1)/8, ball.y - 1 - 8);
                    checkEmptyRow(ball.y - 1 - 8);
                }
                Displ_FillArea(bricks[ball.y - 1 - 8][(ball.x - 1)/8].x*4, bricks[ball.y - 1 - 8][(ball.x - 1)/8].y*4, 32, 4, toughnessColor[bricks[ball.y - 1 - 8][(ball.x - 1)/8].toughness]);
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
                dropPowerUp(ball.x/8, ball.y - 1 - 8);
                checkEmptyRow(ball.y - 1 - 8);
            }
            Displ_FillArea(bricks[ball.y - 1 - 8][ball.x/8].x*4, bricks[ball.y - 1 - 8][ball.x/8].y*4, 32, 4, toughnessColor[bricks[ball.y - 1 - 8][ball.x/8].toughness]);
            bricksHit++;
            ball.dir = SE;
        }
        if (bricks[ball.y - 8][(ball.x + 1)/8].visible == 1) {
            bricks[ball.y - 8][(ball.x + 1)/8].toughness--;
            if (bricks[ball.y - 8][(ball.x + 1)/8].toughness <= 0) {
                bricks[ball.y - 8][(ball.x + 1)/8].visible = 0;
                points++;
                dropPowerUp((ball.x + 1)/8, ball.y - 8);
                checkEmptyRow(ball.y - 8);
            }
            Displ_FillArea(bricks[ball.y - 8][(ball.x + 1)/8].x*4, bricks[ball.y - 8][(ball.x + 1)/8].y*4, 32, 4, toughnessColor[bricks[ball.y - 8][(ball.x + 1)/8].toughness]);
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
                    dropPowerUp((ball.x + 1)/8, ball.y - 1 - 8);
                    checkEmptyRow(ball.y - 1 - 8);
                }
                Displ_FillArea(bricks[ball.y - 1 - 8][(ball.x + 1)/8].x*4, bricks[ball.y - 1 - 8][(ball.x + 1)/8].y*4, 32, 4, toughnessColor[bricks[ball.y - 1 - 8][(ball.x + 1)/8].toughness]);
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
                dropPowerUp(ball.x/8, ball.y + 1 - 8);
                checkEmptyRow(ball.y + 1 - 8);
            }
            Displ_FillArea(bricks[ball.y + 1 - 8][ball.x/8].x*4, bricks[ball.y + 1 - 8][ball.x/8].y*4, 32, 4, toughnessColor[bricks[ball.y + 1 - 8][ball.x/8].toughness]);
            bricksHit++;
            ball.dir = NE;
        }
        if (bricks[ball.y - 8][(ball.x + 1)/8].visible == 1) {
            bricks[ball.y - 8][(ball.x + 1)/8].toughness--;
            if (bricks[ball.y - 8][(ball.x + 1)/8].toughness <= 0) {
                bricks[ball.y - 8][(ball.x + 1)/8].visible = 0;
                points++;
                dropPowerUp((ball.x + 1)/8, ball.y - 8);
                checkEmptyRow(ball.y - 8);
            }
            Displ_FillArea(bricks[ball.y - 8][(ball.x + 1)/8].x*4, bricks[ball.y - 8][(ball.x + 1)/8].y*4, 32, 4, toughnessColor[bricks[ball.y - 8][(ball.x + 1)/8].toughness]);
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

                    dropPowerUp((ball.x + 1)/8, ball.y - 8 + 1);
                    checkEmptyRow(ball.y + 1 - 8);
                }
                Displ_FillArea(bricks[ball.y - 8 + 1][(ball.x + 1)/8].x*4, bricks[ball.y - 8 + 1][(ball.x + 1)/8].y*4, 32, 4, toughnessColor[bricks[ball.y - 8 + 1][(ball.x + 1)/8].toughness]);
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
                dropPowerUp(ball.x/8, ball.y + 1 - 8);
                checkEmptyRow(ball.y + 1 - 8);
            }
            Displ_FillArea(bricks[ball.y + 1 - 8][ball.x/8].x*4, bricks[ball.y + 1 - 8][ball.x/8].y*4, 32, 4, toughnessColor[bricks[ball.y + 1 - 8][ball.x/8].toughness]);
            bricksHit++;
            ball.dir = NW;
        }
        if (bricks[ball.y - 8][(ball.x - 1)/8].visible == 1) {
            bricks[ball.y - 8][(ball.x - 1)/8].toughness--;
            if (bricks[ball.y - 8][(ball.x - 1)/8].toughness <= 0) {
                bricks[ball.y - 8][(ball.x - 1)/8].visible = 0;
                points++;
                dropPowerUp((ball.x - 1)/8, ball.y - 8);
                checkEmptyRow(ball.y - 8);
            }
            Displ_FillArea(bricks[ball.y - 8][(ball.x - 1)/8].x*4, bricks[ball.y - 8][(ball.x - 1)/8].y*4, 32, 4, toughnessColor[bricks[ball.y - 8][(ball.x - 1)/8].toughness]);
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
                    dropPowerUp((ball.x - 1)/8, ball.y + 1 - 8);
                    checkEmptyRow(ball.y + 1 - 8);
                }
                ball.dir = NE;
                Displ_FillArea(bricks[ball.y + 1 - 8][(ball.x - 1)/8].x*4, bricks[ball.y + 1 - 8][(ball.x - 1)/8].y*4, 32, 4, toughnessColor[bricks[ball.y + 1 - 8][(ball.x - 1)/8].toughness]);
            }
        }
        break;
    case still:
        break;
    default:
        ball.dir = ball.dir;
    }
}

/**
 * @fn void dropPowerUp(int, int)
 * @brief drops a potential powerup when a brick has been destroyed
 *
 * @param x x-coordinate of the destroyed brick
 * @param y y-coordinate of the destroyed brick
 */
void dropPowerUp(int x, int y) {
    uint8_t dropChance = 0;
    if (guardian == 0) {
        srand(HAL_GetTick());
        dropChance = RANDOM(0, 100);
    }

    if (dropChance <= 10 && dropChance > 0) {
        guardian = 1;
    }

    if (dropChance <= 15 && dropChance > 10) {
        explodeBricks(x, y);
    }
}

/**
 * @fn void printScreen()
 * @brief prints the game screen
 *
 */
void printScreen() {
    Displ_FillArea(SCREEN_WIDTH*4 - 8 - (3*8), 4, 30, 4, D_GREEN);

    for (int i = 0; i < lives1; i++) {
        Displ_FillArea(SCREEN_WIDTH*4 - 8 - (i*8), 4, 4, 4, D_BLUE);
    }

    if (guardian == 1) {
        Displ_FillArea(0, (person.y - 1)*4, SCREEN_WIDTH*4, 4, GREY);
    }

    Displ_FillArea(person.x*4 - BAR_SIZE*2, person.y*4, BAR_SIZE*4, 4, RED);

    Displ_FillArea(ball.x*4, ball.y*4, 4, 4, D_BLUE);

    char str[12];
    snprintf(str, 12, "%u", points);
    Displ_WString(score.x*4, score.y, str, Font16, 1, WHITE, D_GREEN);
}

/**
 * @fn void checkGuardianTimer()
 * @brief checks the timer for the guardian power up
 *
 */
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

/**
 * @fn void explodeBricks(int, int)
 * @brief explodes bricks in a radius of 1 (power up)
 *
 * @param x x-coordinate of the central brick of the explosion
 * @param y y-coordinate of the central brick of the explosion
 */
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

                Displ_FillArea(bricks[i][j].x*4, bricks[i][j].y*4, 32, 4, D_GREEN);
                checkEmptyRow(i);
            }
        }
    }
}

/**
 * @fn int setBrickToughness(void)
 * @brief generates a random toughness level for a brick
 *
 * @return a toughness level for a brick or the amount of times a brick needs to be hit before being destroyed
 */
int setBrickToughness(void) {
    int toughnessPercent;
    int toughness = 1;

    toughnessPercent = RANDOM(0, 100);

    if(toughnessPercent > 0 && toughnessPercent <= 60) {
        toughness = 1;
    } else if(toughnessPercent > 60 && toughnessPercent < 90) {
        toughness = 2;
    } else if(toughnessPercent > 90 && toughnessPercent < 100) {
        toughness = 3;
    }

    return toughness;
}

/**
 * @fn void drawBricks()
 * @brief draws the visible bricks in their respective color based on their toughness
 *
 */
void drawBricks() {
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
}

/**
 * @fn void checkEmptyRow(uint8_t)
 * @brief checks if a row is empty
 *
 * @param row index for the row needed to be checked
 */
void checkEmptyRow(uint8_t row) {
    uint8_t empty = 0;

    for(int i = 0; i < 10; i++) {
        if(bricks[row][i].visible > 0) {
            empty = 1;
        }
    }
    if(empty == 0) {
        HAL_UART_Transmit(&huart2, (uint8_t *)"O", strlen("O"), 300);
    }

}
