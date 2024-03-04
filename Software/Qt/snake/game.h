#ifndef GAME_H
#define GAME_H

//Configs
#define EVENTSIZE 100
#define SCREEN_WIDTH 160
#define SCREEN_HEIGHT 120

//Function declaration
    void init (void);
    void loop (void);
    int readInput(void);
    void updateScreen(void);
//Enums
    enum keys { left=1, up, right, down, action};
//Typedefs
    typedef struct {
        int x;
        int y;
    } point;
#endif // GAME_H
