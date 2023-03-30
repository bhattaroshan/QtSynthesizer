#include "signalvector.h"
#include <QtGui>

SignalVector::SignalVector()
{

}

QVector<QPair<qreal,qreal>> SignalVector::generateSinWave(int frequency,int milliseconds){
    qreal step = 1.0/m_sampleRate;
    QVector<QPair<qreal,qreal>> res;
    int offLimit = (44100.0*milliseconds)/1000.0;

    for(int x=0;x<offLimit;++x){
        qreal y = sin(2*M_PI*x*step*frequency);
        res.push_back({x,y});
    }

    return res;
}

