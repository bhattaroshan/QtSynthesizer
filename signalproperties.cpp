#include "signalproperties.h"
#include <QtMath>

SignalProperties::SignalProperties()
{
}

void SignalProperties::generateSinWave(qreal frequency,qreal valueLength)
{
    qreal signalLength = m_sampleRate;

    if(valueLength){
        signalLength = valueLength;
    }

    qreal instant = 1.0/m_sampleRate;

    for(int x=0;x<signalLength;++x){
        qreal y = m_amplitude * qSin(2*M_PI*frequency*instant*x+m_phase);
        m_signal.append(QPointF(x,y));
    }
}

void SignalProperties::addADSREnvelope(qreal attackPercent, qreal decayPercent, qreal releasePercent)
{
    if(attackPercent>100) attackPercent = 100;
    if(decayPercent>100) decayPercent = 100;
    if(releasePercent>100) releasePercent = 100;

    qreal attackLength = int((attackPercent*m_signal.size())/100);
    qreal decayLength = int((attackLength*decayPercent)/100);
    qreal releaseLength = int((releasePercent*m_signal.size())/100);

    qreal attackSlope = 1.0/attackLength;
    qreal decaySlope = attackSlope; //assume decaySlope same as attackSlope
    qreal releaseSlope = 1.0/releaseLength;


    for(int i=0;i<attackLength;++i){
        m_signal[i] *= (attackSlope*i);
    }

    for(int i=attackLength;i<attackLength+decayLength;++i) {
        m_signal[i] *= (decaySlope*i);
    }

    for(int i=m_signal.size()-releaseLength;i<m_signal.size();++i){
        m_signal[i] *= (releaseSlope*i);
    }
}

qreal SignalProperties::size()
{
    return m_signal.size();
}
