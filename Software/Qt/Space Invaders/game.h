#ifndef GAME_H
#define GAME_H

//Configs
#define EVENTSIZE 100
#define SCREEN_WIDTH 50
#define SCREEN_HEIGHT 40
#define SPACESHIP_WIDTH 5
#define SPACESHIP_HEIGHT 5
#define MAX_BULLETS 20
#define MAX_DELAY 10

//Function declaration
    void init (void);
    void loop (void);
    int readInput(void);
    void updateScreen(void);
    void moveBullets(void);

//Enums
    enum keys { left=1, up, right, down, action};

//Typedefs
    typedef struct {
        int x;
        int y;
    } point;

    typedef struct {
        int in_play;
        point bullet[MAX_BULLETS];
    }bullets;

#endif // GAME_H
