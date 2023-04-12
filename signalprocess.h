#ifndef SIGNALVECTOR_H
#define SIGNALVECTOR_H

#include<QVector>
#include<QPair>
#include <QtGui>
#include <QPointF>

struct SignalProperties{
    qreal frequency = 220.0;
    qreal amplitude = 1.0;
    qreal phase = 0.0;
    qreal harmonics = 0;
    qreal samples = 44100;
    qreal attackPercent = 10;
    qreal decayPercent = 20;
    qreal releasePercent = 20;
};

class SignalProcess
{
public:
    SignalProcess();
    QVector<QPointF> generateSinWave(SignalProperties sp);
    void addSignalToContainer(QVector<QPointF> &signal, int start);
    void addADSREnvelope(QVector<QPointF> &signal, qreal attackPercent, qreal decayPercent, qreal releasePercent);
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
