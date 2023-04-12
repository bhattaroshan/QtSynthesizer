#ifndef SIGNALPROCESS_H
#define SIGNALPROCESS_H

#include "signalproperties.h"

class SignalProcess
{
public:
    SignalProcess();
    void addSignals(QVector<SignalProperties> &signalTracks,QVector<QPointF> &result);
};

#endif // SIGNALPROCESS_H
