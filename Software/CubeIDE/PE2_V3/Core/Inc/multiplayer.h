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

void check_availability(void);
void toggle_multiplayer(void);
void addCharacterToArray(char);

void printArray(void);
int returnTest(void);
void turnOffTest(void);
int returnConnection(void);
void setMPGameChoice(void);
uint8_t returnMPGameChoice(void);
void clearArray(void);
uint8_t returnCoord(void);

#endif /* INC_MULTIPLAYER_H_ */
