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
    void reduceSignalFromContainer(QVector<QPointF> signal, int start);
    int getIndexFromTime(int milliseconds);
    QVector<QPointF> scaleSignalDown(int distance);
    void clear();
    QVector<QPointF> getSignal();


private:
    int m_sampleRate = 44100;
    QVector<QPointF> m_signal;
    qreal m_maxSignalValue = -std::numeric_limits<qreal>::infinity();
    qreal m_minSignalValue = std::numeric_limits<qreal>::infinity();
};

#endif // SIGNALVECTOR_H
