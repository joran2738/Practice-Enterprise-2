#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include <QWidget>
#include <game.h>

#define PXSIZE 4
class GameField : public QWidget
{
    Q_OBJECT
public:
    explicit GameField(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
signals:

};

#endif // GAMEFIELD_H
