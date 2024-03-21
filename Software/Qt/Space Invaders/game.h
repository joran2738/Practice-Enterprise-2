#ifndef GAME_H
#define GAME_H

//Configs
#define EVENTSIZE 100
#define SCREEN_WIDTH 50
#define SCREEN_HEIGHT 40
#define PLAYABLE_OFFSET 5
#define PLAYABLE_MAX 45


#define SPACESHIP_WIDTH 5
#define SPACESHIP_HEIGHT 5

#define MAX_BULLETS 20
#define MAX_DELAY_BULLETS 10

#define MAX_COMETS 20
#define MIN_COMET_SIZE 2
#define MAX_COMET_SIZE 4

#define MAX_STARS 20

#define HEARTS_Y 2
#define HEARTS_X 2
#define HEARTS_OFFSET 3

//Function declaration
    void init (void);
    void loop (void);
    int readInput(void);
    void updateScreen(void);
    void moveBullets(void);
    void moveComets(void);
    void moveStars(void);
    void hitComet(int,int);

//Enums
    enum keys { left=1, up, right, down, action};

//Typedefs
    typedef struct {
        int x;
        int y;
    } point;

    typedef struct {
        int x;
        int y;
        int size;
        int coins;
    } comet;

    typedef struct {
        int in_play;
        point star_ar[MAX_STARS];
    } stars;

    typedef struct {
        int in_play;
        point bullet_ar[MAX_BULLETS];
    }bullets;

    typedef struct {
        int in_play;
        comet comet_ar[MAX_COMETS];
    }comets;

#endif // GAME_H
