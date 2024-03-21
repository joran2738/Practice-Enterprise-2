#include "debug.h"
#include "game.h"
#include "displayText.h"

uint8_t eventlist[EVENTSIZE];
uint8_t eventindex = 0;
uint32_t game_screen[SCREEN_WIDTH][SCREEN_HEIGHT];
static point person = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 5};
static ballPoint ball = {(SCREEN_WIDTH)/2, SCREEN_HEIGHT - 6, pause};
static brick bricks[MAX_BRICK_LINES][10];
uint8_t start = 0;
uint8_t points = 0;
uint8_t lives = 3;
uint8_t delay = 5;
uint8_t highScore = 0;
uint8_t loopTester = 0;

void changeDirection(directions);
void gameEnd(void);
void lowerBricks(void);
void checkGameOver(void);

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
        lives = 0;
        gameEnd();
    }
    playBall();

    if (start == 1) {
        if (delay <= 0) {
            lowerBricks();
            delay = 5;
        } else {
            delay--;
        }
        checkGameOver();
    }

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
        for (int i = 0; i < MAX_BRICK_LINES; i++) {
            for(int j = 0; j < 10; j++) {
                bricks[i][j].x = j * 5;
                bricks[i][j].y = i + 8;
                bricks[i][j].visible = 0;
            }
        }

        for (int i = 0; i < BRICK_LINES; i++) {
            for(int j = 0; j < 10; j++) {
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
        game_screen[48 - (i*2)][0] = 0xFF0000FF; //blue
    }

    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            if(x == person.x && y == person.y){
                for(int i = x - BAR_SIZE/2; i <= x + BAR_SIZE/2; i++) {
                    game_screen[i][y] = 0xFFFF0000; //red
                }
            }
        }
    }

    game_screen[ball.x][ball.y] = 0xFF0000FF; //blue

    for (int i = 0; i < MAX_BRICK_LINES; i++) {
        for(int j = 0; j < 10; j++) {
            if(bricks[i][j].visible == 1) {
                for(int l = 0; l < 5; l++) {
                    game_screen[bricks[i][j].x + l][bricks[i][j].y] = 0xFF000000; //black
                }
            }
        }
    }

    char str[12];
    snprintf(str, 12, "%u", points);
    displayText(str, 1, 9, 0xFFFFFFFF);
    snprintf(str, 12, "%u", highScore);
    displayText(str, 1, 0, 0xFFFFFFFF);
}

void playBall() {
    if (ball.y == 0) {
        changeDirection(N);
    }
    if (ball.y == SCREEN_HEIGHT - 6) {
        if (ball.x >= person.x - (BAR_SIZE/2) && ball.x <= person.x + (BAR_SIZE/2)) {
            changeDirection(S);
        } else {
            lives--;
            gameEnd();
        }
    }
    if (ball.x == 0) {
        changeDirection(W);
    }
    if (ball.x == SCREEN_WIDTH - 1) {
        changeDirection(E);
    }

    for (int i = 0; i < MAX_BRICK_LINES; i++) {
        for (int j = 0; j < 10; j++) {
            if (ball.y - 1 == bricks[i][j].y && ball.x >= bricks[i][j].x && ball.x < bricks[i][j].x + 4) {
                if (bricks[i][ball.x/5].visible == 1) {
                    bricks[i][ball.x/5].visible = 0;
                    points += 1;
                    QD << "score:" << points;
                    changeDirection(N);
                }
                break;
            }
            if (ball.y + 1 == bricks[i][j].y && ball.x >= bricks[i][j].x && ball.x < bricks[i][j].x + 4) {
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
    if (points > highScore) {
        highScore = points;
        QD << highScore;
    }

    if (lives <= 0) {
        start = 0;
        points = 0;
        lives = 3;
    }
}

void lowerBricks() {
    for (int i = MAX_BRICK_LINES - 2; i >= 0; i--) {
        for (int j = 0; j < 10; j++) {
            bricks[i + 1][j].visible = bricks[i][j].visible;
        }
    }
}

void checkGameOver() {
    for (int j = 0; j < 10; j++) {
        if (bricks[MAX_BRICK_LINES - 1][j].visible == 1) {
            lives = 0;
            ball = {(person.x), SCREEN_HEIGHT - 6, pause};
            gameEnd();
            QD << "Game Over, noob";
        }
    }
}

void displayText(char* text, int x, int y, uint32_t color) {

    unsigned char buffer;
    int bit;
    uint8_t textCursor = x;

    for (int i = 0; text[i] != '\0'; i++) {
        unsigned char c = text[i];
        c = c - ' ';

        for (int j = 0; j < 5; j++) {
            buffer = ledFont[c][j];

            for (int k = 0; k < 8; k++) {
                bit = (buffer >> k) & 1;
                if (bit == 1) {
                    game_screen[textCursor][y + k + 1] = color; //black
                }
            }
            textCursor++;
        }
        textCursor++;
    }
}
