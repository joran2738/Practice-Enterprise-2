#ifndef EVENTFILTER_H
#define EVENTFILTER_H

#include <QObject>
#include <QKeyEvent>
#include <QtWidgets>
#include "debug.h"

class EventFilter : public QObject
{
    Q_OBJECT
public:
    explicit EventFilter(QObject *parent = nullptr);

signals:
protected:
    bool eventFilter(QObject *obj, QEvent *event) override;
};

#endif // EVENTFILTER_H
