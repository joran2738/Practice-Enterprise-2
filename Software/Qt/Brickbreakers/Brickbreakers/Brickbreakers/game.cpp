//This code is meant to be plain c code so it can be used on an STM32 microcontroller.

#include <stdint.h>
#include "debug.h"
#include "game.h"

uint8_t eventlist[EVENTSIZE];
uint8_t eventindex = 0;
uint32_t game_screen[SCREEN_WIDTH][SCREEN_HEIGHT];
static point person = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 5};
static ballPoint ball = {(SCREEN_WIDTH)/2, SCREEN_HEIGHT - 6, pause};
static brick bricks[BRICK_LINES][10];
uint8_t start = 0;
uint8_t points = 0;
uint8_t lives = 3;

void changeDirection(directions);
void gameEnd(void);

void init (void) {
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            game_screen[x][y] = 0xFF00FF00; //Green
        }
    }
}

void loop (void) {
    int key = readInput();
    if(key == left) {
        person.x--;
        if(person.x < BAR_SIZE/2) {
            person.x = (BAR_SIZE/2);
        }
        if (ball.dir == pause) {
            ball.x--;
        }
    }
    if(key == right) {
        person.x++;
        if(person.x > SCREEN_WIDTH - (BAR_SIZE/2) - 1) {
            person.x = SCREEN_WIDTH - (BAR_SIZE/2) - 1;
        }
        if (ball.dir == pause) {
            ball.x++;
        }
    }
    if(key == action) {
        if (ball.dir == pause) {
            ball.dir = NW;
            start = 1;

        }
    }
    if (points == BRICK_LINES * 10) {
        gameEnd();
    }
    playBall();

    updateScreen();
}

int readInput()
{
    int newKey = 0;
    if(eventindex > 0) {
        newKey = eventlist[0];
        //QD << newKey << eventindex;
        for(int i=0; i < eventindex; i++){
            eventlist[i] = eventlist[i+1];
        }
        eventindex--;
    }
    return newKey;
}

void updateScreen()
{
    if (start == 0) {
        for (int i = 0; i < BRICK_LINES; i++) {
            for(int j = 0; j < 10; j++) {
                bricks[i][j].x = j * 5;
                bricks[i][j].y = i + 2;
                bricks[i][j].visible = 1;
            }
        }
    }

    for(int x = 0; x < SCREEN_WIDTH; x++) {
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            game_screen[x][y] = 0xFF00FF00; //Green
        }
    }

    for (int i = 0; i < lives; i++) {
        game_screen[2 + (i*2)][0] = 0xFF0000FF; //blue
    }

    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            if(x == person.x && y == person.y){
                for(int i = x - BAR_SIZE/2; i <= x + BAR_SIZE/2; i++) {
                    game_screen[i][y] = 0xFFFF0000; //red
                }
            }
            if(x == ball.x && y == ball.y) {
                game_screen[x][y] = 0xFF0000FF; //blue
            }
        }
    }

    for (int i = 0; i < BRICK_LINES; i++) {
        for(int j = 0; j < 10; j++) {
            if(bricks[i][j].visible == 1) {
                for(int l = 0; l < 5; l++) {
                    game_screen[bricks[i][j].x + l][bricks[i][j].y] = 0xFF000000; //black
                }
            }
        }
    }

}

void playBall() {
    if (ball.y == 0) {
        changeDirection(N);
    }
    if (ball.y == SCREEN_HEIGHT - 6) {
        if (ball.x >= person.x - (BAR_SIZE/2) && ball.x <= person.x + (BAR_SIZE/2)) {
            changeDirection(S);
        } else {
            gameEnd();
        }
    }
    if (ball.x == 0) {
        changeDirection(W);
    }
    if (ball.x == SCREEN_WIDTH - 1) {
        changeDirection(E);
    }

    for (int i = 0; i < BRICK_LINES; i++) {
        for (int j = 0; j < 10; j++) {
            if (ball.y - 1 == bricks[i][j].y && ball.x >= bricks[i][j].x && ball.x < bricks[i][j].x + 5) {
                if (bricks[i][ball.x/5].visible == 1) {
                    bricks[i][ball.x/5].visible = 0;
                    points += 1;
                    QD << "score:" << points;
                    changeDirection(N);
                }
                break;
            }
            if (ball.y + 1 == bricks[i][j].y && ball.x >= bricks[i][j].x && ball.x < bricks[i][j].x + 5) {
                if (bricks[i][ball.x/5].visible == 1) {
                    bricks[i][ball.x/5].visible = 0;
                    points += 1;
                    QD << "score:" << points;
                    changeDirection(S);
                }
                break;
            }

            if (ball.x == bricks[i][j].x - 1 && ball.y == bricks[i][j].y) {
                if (bricks[i][ball.x/5].visible == 1) {
                    bricks[i][ball.x/5].visible = 0;
                    points += 1;
                    QD << "score:" << points;
                    changeDirection(E);
                }
                break;
            }

            if (ball.x == bricks[i][j].x + 5 && ball.y == bricks[i][j].y) {
                if (bricks[i][ball.x/5].visible == 1) {
                    bricks[i][ball.x/5].visible = 0;
                    points += 1;
                    QD << "score:" << points;
                    changeDirection(W);
                }
                break;
            }
        }
    }

    moveBall();
}

void moveBall() {
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
    case pause:
        break;
    default:
        QD << "You shouldn't be here!";
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
        case pause:
            break;
        default:
            QD << "You shouldn't be here!";
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
        case pause:
            break;
        default:
            QD << "You shouldn't be here!";
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
        case pause:
            break;
        default:
            QD << "You shouldn't be here!";
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
        case pause:
            break;
        default:
            QD << "You shouldn't be here!";
        }
    }
}

void gameEnd() {
    person.x = (SCREEN_WIDTH)/2;
    ball = {(person.x), SCREEN_HEIGHT - 6, pause};
    lives--;

    if (lives <= 0) {
        start = 0;
        points = 0;
        lives = 3;
    }
}
