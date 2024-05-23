/*
 * multiplayer.c
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */

#include "multiplayer.h"
#include "debug.h"
#include <stdint.h>
#include "string.h"
#include "z_displ_ILI9XXX.h"

extern UART_HandleTypeDef huart2;

uint8_t emulate = 1;
uint8_t connected = 0;
Multiplayer mp = disabled;
char characterArray[100];
uint8_t test = 0;
uint8_t mpGameChoice = 0;
uint8_t mpGameState;
uint8_t mpMenuState;
uint8_t x_coord = 0;


void check_availability(){
    HAL_UART_Transmit(&huart2, (uint8_t *)"play?", strlen("play?"), 300);
}

void toggle_multiplayer(){
    if(connected){
        //QD << "(mockup) stoping multiplayer";  // change to uart debug
        connected = 0;
    }else{
        //QD << "(mockup) starting multiplayer";  // change to uart debug
        check_availability();
    }

}

void addCharacterToArray(char c) {
    char filler[2];
    filler[0] = c;
    filler[1] = '\0';

    strcat(characterArray, filler);
    //printf("%c\r\n", c);

    if(c == 'o' && mp == enabled) {
        test = 1;
        //printArray();
    }else if(c == '?') {
        HAL_UART_Transmit(&huart2, (uint8_t *)"Yes!", strlen("Yes!"), 300);
        printArray();
        connected = 1;
        mp = enabled;
    }else if(c == '!') {
        connected = 1;
        mp = enabled;
        printArray();
    }else if(c == 'G') {
        test = 1;
        setMPGameChoice();
        //printArray();
    }else if(c == 'B') {
        setCoord();
    }else if(c == 'S'){
    	mpMenuState = 1;
    }
}

void printArray() {
    printf("%s\r\n", characterArray);
    memset(characterArray, '\0', sizeof(characterArray));
    test = 0;
}

int returnTest() {
    return test;
}

void turnOffTest() {
    test = 0;
}

int returnConnection() {
    return connected;
}

void setMPGameChoice() {
    mpGameChoice = characterArray[0] - '0';
    printf("%d\r\n", mpGameChoice);
}

uint8_t returnMPGameChoice() {
    return mpGameChoice;
}

void clearArray() {
    memset(characterArray, '\0', sizeof(characterArray));
}

uint8_t returnCoord() {
    return x_coord;
}

void setCoord() {
    char buffer[100];
    strncpy(buffer, characterArray, strlen(characterArray) - 2);
    buffer[strlen(characterArray) - 1] = '\0';

    x_coord = (uint8_t)atoi(buffer);
}
