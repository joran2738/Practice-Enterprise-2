#ifndef SNAKEFIELD_H
#define SNAKEFIELD_H

#include <QWidget>
#include <game.h>

#define PXSIZE 4
class SnakeField : public QWidget
{
    Q_OBJECT
public:
    explicit SnakeField(QWidget *parent = nullptr);
    void paintEvent(QPaintEvent *event);
signals:

};

#endif // SNAKEFIELD_H
