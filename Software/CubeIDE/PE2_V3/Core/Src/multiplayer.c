/*
 * multiplayer.c
 *
 *      Author: joran & Robin
 */

#include "multiplayer.h"
#include "debug.h"
#include <stdint.h>
#include "string.h"
#include "z_displ_ILI9XXX.h"

extern UART_HandleTypeDef huart2;

uint8_t connected = 0;  //0 if there is no connection, 1 if there is a connection
Multiplayer mp = disabled;  //whether multiplayer is disabled or enabled
char characterArray[100]; //buffer to hold the incoming data
uint8_t inSignal = 0;   //0 if no new data has been received, 1 if there is new unused data
uint8_t mpGameChoice = 0;   //holds the number of the game chosen in multiplayer
GameState mpGameState = 0;  //holds the state of the game when multiplayer is enabled
uint8_t mpMenuState;    //holds the state of the game
uint8_t x_coord = -1;    //holds the received x coordinate for multiplayer games

/**
 * @fn void check_availability()
 * @brief Asks the other console to engage multiplayer
 *
 */
void check_availability(){
    HAL_UART_Transmit(&huart2, (uint8_t *)"?", strlen("?"), 300);
}

/**
 * @fn void addCharacterToArray(char)
 * @brief adds the incoming character to the input buffer array and checks if it is a command character
 *
 * @param c incoming character
 */
void addCharacterToArray(char c) {
    char filler[2];
    filler[0] = c;
    filler[1] = '\0';

    strcat(characterArray, filler);
    //printf("%c\r\n", c);

    if(c == 'O' && mp == enabled) {
        inSignal = 1;
        printArray();
        clearArray();
    }else if(c == '?') {
        HAL_UART_Transmit(&huart2, (uint8_t *)"!", strlen("!"), 300);
        printArray();
        clearArray();
        connected = 1;
        mp = enabled;
    }else if(c == '!') {
        connected = 1;
        mp = enabled;
        printArray();
        clearArray();
    }else if(c == 'G') {
        inSignal = 1;
        setMPGameChoice();
        printArray();
        clearArray();
    }else if(c == 'B') {
    	inSignal = 1;
        setCoord();
        printArray();
    }else if(c == 'S'){
        inSignal = 1;
    	mpMenuState = 1;
    	printArray();
    	clearArray();
    }else if(c == 'X') {
       connected = 0;
       mp = disabled;
       printArray();
       clearArray();
    }else if(c == 'P') {
        inSignal = 1;
        mpMenuState = 2;
        printArray();
        clearArray();
    }else if(c == 'C') {
        inSignal = 1;
        mpMenuState = 1;
        printArray();
        clearArray();
    }else if(c == 'M') {
        inSignal = 1;
        mpMenuState = 3;
        printArray();
        clearArray();
    }else if(c == 'L') {
        mpGameState = win;
    }
}

/**
 * @fn void printArray()
 * @brief prints the buffer array
 *
 */
void printArray() {
    printf("%s\r\n", characterArray);
}

/**
 * @fn void clearArray()
 * @brief clears the buffer array
 *
 */
void clearArray() {
    memset(characterArray, '\0', sizeof(characterArray));
}

/**7
 * @fn uint8_t getSignal()
 * @brief returns the inSignal variable
 *
 * @return 1 there has been incoming data, 0 if not
 */
uint8_t getSignal() {
    return inSignal;
}

/**
 * @fn void turnOffSignal()
 * @brief sets the inSignal variable to 0, meaning the incoming data has been handled
 *
 */
void turnOffSignal() {
    inSignal = 0;
}

/**
 * @fn uint8_t getConnection()
 * @brief returns the connection state of multiplayer
 *
 * @return 0 if no connection has been made, 1 if there is a connection established
 */
uint8_t getConnection() {
    return connected;
}

/**
 * @fn void setMPGameChoice()
 * @brief saves the number of the game chosen by the opposing console
 *
 */
void setMPGameChoice() {
    mpGameChoice = characterArray[0] - '0';
    printf("%d\r\n", mpGameChoice);
}

/**
 * @fn uint8_t getMPGameChoice()
 * @brief returns the number of the game chosen by the opposing console
 *
 * @return the number of the game chosen by the opposing console
 */
uint8_t getMPGameChoice() {
    return mpGameChoice;
}

/**
 * @fn uint8_t getCoord()
 * @brief returns the x coordinate of the bullet shot by the opposing player
 *
 * @return x coordinate between 0 <= x < 80
 */
uint8_t getCoord() {
    return x_coord;
}

/**
 * @fn void setCoord()
 * @brief converts the incomming coordinate to a uint8_t and saves it
 *
 */
void setCoord() {
    char buffer[100];
    strncpy(buffer, characterArray, strlen(characterArray) - 1);
    buffer[strlen(characterArray) - 1] = '\0';

    x_coord = (uint8_t)atoi(buffer);
}

void clearCoord(){
	x_coord = -1;
}
/**
 * @fn void closeConnection()
 * @brief Closes the connection for multiplayer
 *
 */
void closeConnection() {
    connected = 0;
    mp = disabled;
}

/**
 * @fn uint8_t getMPMenuState()
 * @brief returns the Menu State in multiplayer when the opposing player has made a change
 *
 * @return the Menu State in multiplayer when the opposing player has made a change
 */
uint8_t getMPMenuState() {
    return mpMenuState;
}

/**
 * @fn void setGameState(GameState)
 * @brief when the player loses or wins, the state of the game is saved
 *
 * @param state holds the state of the game: ongoing, won or lost
 */
void setGameState(GameState state) {
    mpGameState = state;
}

/**
 * @fn GameState getGameState()
 * @brief returns the state of the game, whether it is still going, the player has won or lost
 *
 * @return the state of the game
 */
GameState getGameState() {
    return mpGameState;
}
