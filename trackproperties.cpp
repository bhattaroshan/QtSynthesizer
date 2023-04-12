#include "trackproperties.h"
#include <QtMath>

SignalProperties::SignalProperties()
{
    m_signalLength = m_sampleRate;
}

void SignalProperties::generateSinWave(qreal frequency,qreal valueLength)
{
    if(valueLength==0){
        m_signalLength = m_sampleRate;
    }else{
        m_signalLength = valueLength;
    }

    qreal instant = 1.0/m_sampleRate;

    for(int x=0;x<m_signalLength;++x){
        qreal y = m_amplitude * qSin(2*M_PI*frequency*instant*x);
        m_signal.append(QPointF(x,y));
    }
}

void SignalProperties::addADSREnvelope(qreal attackPercent, qreal decayPercent, qreal releasePercent)
{
    qreal attackLength =
}
