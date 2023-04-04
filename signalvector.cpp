#include "signalvector.h"
#include <QtGui>

SignalVector::SignalVector()
{

}

QVector<QPointF> SignalVector::generateSinWave(int frequency,int milliseconds){
    qreal step = 1.0/m_sampleRate;
    QVector<QPointF> res;
    int offLimit = (44100.0*milliseconds)/1000.0;

    for(int x=0;x<offLimit;++x){
        qreal y = sin(2*M_PI*x*step*frequency);
        res.push_back(QPointF(x,y));
    }
    return res;
}

void SignalVector::addSignalToContainer(QVector<QPointF> signal, int start)
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

void SignalVector::normalizeSignal()
{
    for(int i=0;i<m_signal.size();++i){
        if(m_maxSignalValue<m_signal[i].y()){
            m_maxSignalValue = m_signal[i].y();
        }
        if(m_signal[i].y()<m_minSignalValue){
            m_minSignalValue = m_signal[i].y();
        }
    }

    qreal scaleFactor = 1.0/qMax(qAbs(m_maxSignalValue),qAbs(m_minSignalValue));

    for(int i=0;i<m_signal.size();++i){
        m_signal[i].setY(m_signal[i].y()*scaleFactor);
    }
}

void SignalVector::reduceSignalFromContainer(QVector<QPointF> signal, int start)
{
    qreal new_length = start+signal.size();
    qreal length = m_signal.size();
    for(int i=start;i<new_length;++i){
        if(i>=length) break; //cannot perform calculation above this point
        m_signal[i] = QPointF(i,m_signal[i].y()-signal[i].y());
    }

    for(int i=start;i<new_length;++i){
        if(i>=length) break; //cannot perform calculation above this point
        if(m_maxSignalValue<m_signal[i].y()){
            m_maxSignalValue = m_signal[i].y();
        }
        if(m_signal[i].y()<m_minSignalValue){
            m_minSignalValue = m_signal[i].y();
        }
    }

    qreal scaleFactor = 1.0/qMax(qAbs(m_maxSignalValue),qAbs(m_minSignalValue));

    for(int i=start;i<signal.size();++i){
        m_signal[i].setY(m_signal[i].y()*scaleFactor);
    }


}

int SignalVector::getIndexFromTime(int milliseconds)
{
    int index = (milliseconds*44100)/1000;
    return index;
}

QVector<QPointF> SignalVector::scaleSignalDown(int distance){
    QVector<QPointF> res;
    for(int i=0;i<m_signal.size();i+=distance){
        res.push_back(m_signal[i]);
    }
    return res;
}

void SignalVector::clear()
{
    m_signal.clear();
}

QVector<QPointF> SignalVector::getSignal()
{
    return m_signal;
}
