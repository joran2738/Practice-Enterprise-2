
#ifndef GAME_H
#define GAME_H
#include <stdint.h>
//Configs
#define EVENTSIZE 100
#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 60






//Enums
enum keys { left=1, up, right, down, action};


//Function declaration
    void init (void);
    void loop (void);
    int readInput(void);
    void updateScreen(void);




#endif // GAME_H
