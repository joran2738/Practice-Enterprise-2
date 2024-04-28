#ifndef DISPLAYTEXT_H
#define DISPLAYTEXT_H

#include <stdint.h>
#include "ledFont5x8.h"
#include "game.h"

#define RED 0xFFFF0000
#define GREEN 0xFF00FF00
#define BLUE 0xFF0000FF
#define LIGHT_BLUE 0xFF02f7db
#define BLACK 0xFF000000
#define DARK_GRAY 0xFF2E2C2C
#define YELLOW 0xFFEDE609
#define GRAY 0xFF969696
#define LIGHT_GREEN 0xFF02FA3C
#define WHITE 0xFFFFFFFF

void displayText(uint32_t[][SCREEN_HEIGHT], char*, int, int, uint32_t);

#endif // DISPLAYTEXT_H
