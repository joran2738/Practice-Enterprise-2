#include "snakefield.h"
#include "debug.h"
#include <QPainter>
extern uint32_t game_screen[SCREEN_WIDTH][SCREEN_HEIGHT];

SnakeField::SnakeField(QWidget *parent)
    : QWidget{parent}
{
    this->setMinimumSize(SCREEN_WIDTH*PXSIZE+2,SCREEN_HEIGHT*PXSIZE+2);
    this->setMaximumSize(SCREEN_WIDTH*PXSIZE+2,SCREEN_HEIGHT*PXSIZE+2);
}

void SnakeField::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.drawRect(0,0,SCREEN_WIDTH*PXSIZE+1,SCREEN_HEIGHT*PXSIZE+1);
    p.setPen(Qt::NoPen);
    for(int x = 0; x < SCREEN_WIDTH; x++){
        for(int y = 0; y < SCREEN_HEIGHT; y++){
            QBrush b((QColor(QRgb(game_screen[x][y]))));
            p.setBrush(b);
            p.drawRect(1+x*PXSIZE,1+y*PXSIZE,PXSIZE,PXSIZE);
        }
    }
}
