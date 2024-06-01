/*
 * multiplayer.h
 *
 *      Author: Joran & Robin
 */

#ifndef INC_MULTIPLAYER_H_
#define INC_MULTIPLAYER_H_

#include "common.h"
#include "stm32f3xx_hal.h"

typedef enum  {disabled = 0, enabled} Multiplayer;
typedef enum {ongoing = 0, win, loss} GameState;

void check_availability(void);
void addCharacterToArray(char);
void printArray(void);
uint8_t getSignal(void);
void turnOffSignal(void);
uint8_t getConnection(void);
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
