#include "displayText.h"

void displayText(uint32_t screen[][SCREEN_HEIGHT], char* text, int x, int y, uint32_t color) {

    unsigned char buffer;
    int bit;
    uint8_t textCursor = x;

    for (int i = 0; text[i] != '\0'; i++) {
        unsigned char c = text[i];
        c = c - ' ';

        for (int j = 0; j < 5; j++) {
            buffer = ledFont[c][j];

            for (int k = 0; k < 8; k++) {
                bit = (buffer >> k) & 1;
                if (bit == 1) {
                   screen[textCursor][y + k + 1] = color; //black
                }
            }
            textCursor++;
        }
        textCursor++;
    }
}
