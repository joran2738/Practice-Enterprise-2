#include "eventfilter.h"
#include "game.h"

extern uint8_t eventlist[100];
extern uint8_t eventindex;

EventFilter::EventFilter(QObject *parent)
    : QObject{parent}
{

}

bool EventFilter::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //QDV(keyEvent);
        if(keyEvent->isAutoRepeat()){
            QD << "*** Key Repeat is ignored!!";
            // standard event processing
            return QObject::eventFilter(obj, event);
        }
        if(keyEvent->key() == Qt::Key_Left){
            eventlist[eventindex++] = left;
            return true;
        }
        if(keyEvent->key() == Qt::Key_Up){
            eventlist[eventindex++] = up;
            return true;
        }
        if(keyEvent->key() == Qt::Key_Right){
            eventlist[eventindex++] = right;
            return true;
        }
        if(keyEvent->key() == Qt::Key_Down){
            eventlist[eventindex++] = down;
            return true;
        }
        if(keyEvent->key() == Qt::Key_Space){
            eventlist[eventindex++] = action;
            return true;
        }
    }
    // standard event processing
    return QObject::eventFilter(obj, event);
}
