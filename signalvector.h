#ifndef SIGNALVECTOR_H
#define SIGNALVECTOR_H

#include<QVector>
#include<QPair>
#include <QtGui>

class SignalVector
{
public:
    SignalVector();
    QVector<QPointF> generateSinWave(int frequency,int milliseconds);
    void addSignalToContainer(QVector<QPointF> signal, int start);
    void normalizeSignal();
    int getIndexFromTime(int milliseconds);
    QVector<QPointF> scaleSignalDown(int distance);
    void clear();
    QVector<QPointF> getSignal();


private:
    int m_sampleRate = 44100;
    QVector<QPointF> m_signal;
};

#endif // SIGNALVECTOR_H
