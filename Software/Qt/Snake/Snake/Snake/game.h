#ifndef GAME_H
#define GAME_H

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
enum directions {N, E, S, W, pause};
//Typedefs
typedef struct {
    int x;
    int y;
} point;

typedef struct {
    int x;
    int y;
    directions dir;
} snake;
#endif // GAME_H
