#include brickbreaker.cpp

static point person = {SCREEN_WIDTH/2, SCREEN_HEIGHT - 5};
static ballPoint ball = {(SCREEN_WIDTH)/2, SCREEN_HEIGHT - 6, pause};
static brick bricks[MAX_BRICK_LINES][10];
static point score = {1, 9};
static point highScorePoint = {1, 0};
uint8_t start = 0;
uint8_t points = 0;
uint8_t lives = 3;
uint8_t delay = BRICK_SPEED;
uint8_t highScore = 0;
uint8_t loopTester = 0;
uint8_t guardian = 0;
uint8_t guardianTimer = 100;
