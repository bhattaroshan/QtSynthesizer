#ifndef SIGNALVECTOR_H
#define SIGNALVECTOR_H

#include<QVector>
#include<QPair>

class SignalVector
{
public:
    SignalVector();
    QVector<QPair<qreal,qreal>> generateSinWave(int frequency,int milliseconds);

private:
    int m_sampleRate = 44100;
    QVector<QPair<qreal,qreal>> m_signal;
};

#endif // SIGNALVECTOR_H
