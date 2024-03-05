#ifndef GAME_H
#define GAME_H

//Configs
#define EVENTSIZE 100
#define SCREEN_WIDTH 30
#define SCREEN_HEIGHT 40
#define BAR_SIZE 13

//Function declaration
    void init (void);
    void loop (void);
    int readInput(void);
    void updateScreen(void);
    void playBall(void);
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
#endif // GAME_H
