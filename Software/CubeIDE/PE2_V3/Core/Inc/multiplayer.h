/*
 * multiplayer.h
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */

#ifndef INC_MULTIPLAYER_H_
#define INC_MULTIPLAYER_H_

#include "common.h"
#include "stm32f3xx_hal.h"

typedef enum  {disabled = 0, enabled} Multiplayer;
typedef enum {ongoing = 0, win, loss} GameState;

void check_availability(void);
void toggle_multiplayer(void);
void addCharacterToArray(char);

void printArray(void);
int getSignal(void);
void turnOffSignal(void);
int getConnection(void);
void setMPGameChoice(void);
uint8_t getMPGameChoice(void);
void clearArray(void);
uint8_t getCoord(void);
void setCoord(void);
void closeConnection(void);
uint8_t getMPMenuState(void);
void setGameState(GameState);
GameState getGameState(void);

#endif /* INC_MULTIPLAYER_H_ */
