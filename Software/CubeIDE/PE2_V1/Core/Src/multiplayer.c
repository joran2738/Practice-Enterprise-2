/*
 * multiplayer.c
 *
 *  Created on: Apr 17, 2024
 *      Author: joran
 */

#include "multiplayer.h"
#include "debug.h"
#include <stdint.h>

uint8_t emulate = 1;
uint8_t connected = 0;


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

