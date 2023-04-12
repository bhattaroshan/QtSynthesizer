#include "signalprocess.h"
#include <QtGui>

SignalProcess::SignalProcess()
{

}

QVector<QPointF> SignalProcess::generateSinWave(int frequency,int milliseconds){
    qreal step = 1.0/m_sampleRate;
    QVector<QPointF> res;
    int offLimit = (44100.0*milliseconds)/1000.0;

    for(int x=0;x<offLimit;++x){
        qreal y = sin(2*M_PI*x*step*frequency);
        res.push_back(QPointF(x,y));
    }
    return res;
}

void SignalProcess::addSignalToContainer(QVector<QPointF> &signal, int start)
{
    int signalLength = m_signal.size();
    int newSignalLength = signal.size()+start;
    if(newSignalLength>signalLength){
        m_signal.resize(newSignalLength);
        for(int i=signalLength;i<m_signal.size();++i){ //reset added buffer to zero
            m_signal[i] = QPointF(i,0);
        }
    }

    //add new signal to existing container
    for(int i=0;i<signal.size();++i){
        m_signal[start+i]=QPointF(start+i,m_signal[start+i].y()+signal[i].y());
    }

}

void SignalProcess::addADSREnvelope(qreal attackPercent, qreal decayPercent, qreal releasePercent)
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

    int start = 0;
    int i;

    for(i=start;i<attackLength;++i){
        m_signal[i] = QPointF(m_signal[i].x(),m_signal[i].y()*attackSlope*(i-start));
    }

    start = attackLength;
    for(i=start;i<attackLength+decayLength;++i) {
        m_signal[i] = QPointF(m_signal[i].x(),m_signal[i].y()*decaySlope*(attackLength-(i-start)));
    }

    qreal constantDecay = decaySlope*(attackLength-(i-start));
    start = attackLength+decayLength;
    for(i=start;i<m_signal.size();++i){
        m_signal[i] = QPointF(m_signal[i].x(),m_signal[i].y()*constantDecay);
    }

    start = m_signal.size()-releaseLength;
    for(i=start;i<m_signal.size();++i){
        m_signal[i] = QPointF(m_signal[i].x(),m_signal[i].y()*releaseSlope*(releaseLength-(i-start)));
    }
}

void SignalProcess::normalizeSignal()
{
    qreal maxSignalValue, minSignalValue;
    if(m_signal.size()){
        maxSignalValue = m_signal[0].y();
        minSignalValue = m_signal[0].y();
    }else return; //return if size is not right

    for(int i=0;i<m_signal.size();++i){
        if(maxSignalValue<m_signal[i].y()){
            maxSignalValue = m_signal[i].y();
        }
        if(m_signal[i].y()<minSignalValue){
            minSignalValue = m_signal[i].y();
        }
    }

    qreal scaleFactor = 1.0/qMax(qAbs(maxSignalValue),qAbs(minSignalValue));

    for(int i=0;i<m_signal.size();++i){
        m_signal[i] = QPointF(m_signal[i].x(),m_signal[i].y()*scaleFactor);
    }
}


int SignalProcess::getIndexFromTime(int milliseconds)
{
    int index = (milliseconds*44100)/1000;
    return index;
}

QVector<QPointF> SignalProcess::scaleSignalDown(int distance){
    QVector<QPointF> res;
    for(int i=0;i<m_signal.size();i+=distance){
        res.push_back(m_signal[i]);
    }
    return res;
}

void SignalProcess::clear()
{
    m_signal.clear();
}

QVector<QPointF> SignalProcess::getSignal()
{
    return m_signal;
}
