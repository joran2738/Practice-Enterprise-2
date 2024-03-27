#include "debug.h"
#include "game.h"
#include "displayText.h"

uint8_t eventlist[EVENTSIZE];
uint8_t eventindex = 0;
uint32_t game_screen[SCREEN_WIDTH][SCREEN_HEIGHT];
static point person = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 5};
static point bot = {SCREEN_WIDTH/2, 5};
static ballPoint ball = {(SCREEN_WIDTH)/2, SCREEN_HEIGHT - 6, pause};
static point score = {1, 9};
static point highScorePoint = {1, 0};
uint8_t start = 0;
uint8_t points = 0;
uint8_t lives = 3;
uint8_t highScore = 0;
uint8_t loopTester = 0;

void changeDirection(directions);
void gameEnd(void);
void checkGameOver(void);

void init (void) {
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            game_screen[x][y] = GREEN;
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
    playBall();

    if (start == 1 && ball.dir != pause) {
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
    for(int x = 0; x < SCREEN_WIDTH; x++) {
        for(int y = 0; y < SCREEN_HEIGHT; y++) {
            game_screen[x][y] = GREEN;
        }
    }

    for (int i = 0; i < lives; i++) {
        game_screen[48 - (i*2)][0] = BLUE;
    }

    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            if(x == person.x && y == person.y){
                for(int i = x - BAR_SIZE/2; i <= x + BAR_SIZE/2; i++) {
                    game_screen[i][y] = RED;
                }
            }
            if(x == bot.x && y == bot.y){
                for(int i = x - BAR_SIZE/2; i <= x + BAR_SIZE/2; i++) {
                    game_screen[i][y] = RED;
                }
            }
        }
    }

    game_screen[ball.x][ball.y] = BLUE;

    char str[12];
    snprintf(str, 12, "%u", points);
    displayText(game_screen, str, score.x, score.y, WHITE);
    snprintf(str, 12, "%u", highScore);
    displayText(game_screen, str, highScorePoint.x, highScorePoint.y, WHITE);
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

    if (lives <= 0) {
        if (points > highScore) {
            highScore = points;
            QD << highScore;
        }

        start = 0;
        points = 0;
        lives = 3;
    }
}

void checkGameOver() {

}

