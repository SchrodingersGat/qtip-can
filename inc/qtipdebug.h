#ifndef QTIPDEBUG_H
#define QTIPDEBUG_H

#define DEBUG_QTIP

#ifdef DEBUG_QTIP
#include <qdebug.h>
#define QTipDebug() qDebug()
#else
// TODO
#endif

#endif // QTIPDEBUG_H
