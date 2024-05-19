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

uint8_t emulate = 1;
uint8_t connected = 0;
char characterArray[100];


void check_availability(){
    //QD << "(mockup) is player 2 there?";  // change to uart debug
    if(emulate /*or answer was received */){
        connected = 1;
        // QD << "(mockup) player 2 connected";  // change to uart debug
    }
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
    Displ_WString(20, 150, characterArray, Font16, 1, WHITE, BLACK);
}

