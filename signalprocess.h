#ifndef SIGNALVECTOR_H
#define SIGNALVECTOR_H

#include<QVector>
#include<QPair>
#include <QtGui>
#include <QPointF>
#include "signalproperties.h"


class SignalProcess
{
public:
    SignalProcess();
    static void generateSignal(QVector<QPointF> &sig, SignalProperties sp);
    static void generateSinWave(QVector<QPointF> &sig, SignalProperties sp);
    static void generateSquareWave(QVector<QPointF> &sig, SignalProperties sp);
    static void normalizeSignal(QVector<QPointF> &sig);

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
    QVector<QVector<QPointF>> m_trackSignals;
};

#endif // SIGNALVECTOR_H
