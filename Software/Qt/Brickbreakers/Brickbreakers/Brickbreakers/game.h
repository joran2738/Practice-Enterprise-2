#ifndef GAME_H
#define GAME_H

#include <stdint.h>

//Configs
#define EVENTSIZE 100
#define SCREEN_WIDTH 50
#define SCREEN_HEIGHT 50
#define BAR_SIZE 17
#define BRICK_LINES 10
#define MAX_BRICK_LINES 38
#define BRICK_SPEED 140

//Function declaration
void init (void);
void loop (void);
int readInput(void);
void updateScreen(void);
void playBall(void);
void moveBall(void);

//Enums
enum keys { left=1, up, right, down, action};
enum directions {N, NE, E, SE, S, SW, W, NW, pause};
//Typedefs
typedef struct {
    int x;
    int y;
} point;

typedef struct {
    int x;
    int y;
    directions dir;
} ballPoint;

typedef struct {
    int x;
    int y;
    int visible;
} brick;
#endif // GAME_H
