#ifndef GAME_H
#define GAME_H

#include <stdint.h>

//Configs
#define EVENTSIZE 100
#define SCREEN_WIDTH 50
#define SCREEN_HEIGHT 50

//Function declaration
void init (void);
void loop (void);
int readInput(void);
void updateScreen(void);

//Enums
enum keys { left=1, up, right, down, action};
enum directions {N, NE, E, SE, S, SW, W, NW, pause};

//Typedefs
typedef struct {
    int x;
    int y;
} point;

#endif // GAME_H
