#include "multiplayer.h"
#include "debug.h"

uint8_t emulate = 1;
uint8_t connected = 0;


void check_availability(){
    QD << "(mockup) is player 2 there?";
    if(emulate /*or answer was received */){
        connected = 1;
        QD << "(mockup) player 2 connected";
    }
}

void toggle_multiplayer(){
    if(connected){
        QD << "(mockup) stoping multiplayer";
        connected = 0;
    }else{
        QD << "(mockup) starting multiplayer";
        check_availability();
    }

}
