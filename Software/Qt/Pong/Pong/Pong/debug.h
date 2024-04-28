#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>
#define QD qDebug() << __FILE__ << __LINE__
#define QDV(x) qDebug() << __FILE__ << __LINE__ << #x":" << x

#endif // DEBUG_H
