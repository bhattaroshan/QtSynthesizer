#include "signalprocess.h"
#include <QtGui>

SignalProcess::SignalProcess()
{

}

void addVector(){

}

void SignalProcess::generateSignal(QVector<QPointF> &sig, SignalProperties sp){
    qreal samples = SAMPLES(sp.time,sp.sampleRate);
    sig.resize(samples,QPointF(0,0)); //confirm that container size is exact the sample size
    qreal originalFrequency = sp.frequency;
    qreal originalAmplitude = sp.amplitude;
    int step = 1;
    if(sp.type==SIGNAL_TYPE_SQUARE) step = 2;

    for(int harmonics=1;harmonics<=(sp.harmonics+1)*step;harmonics+=step){ //if harmonics is zero, just run this once
        if(sp.type == SIGNAL_TYPE_SINUSOIDAL){ //perform operation for sinusoidal wave generation
            sp.amplitude = originalAmplitude*(1.0/harmonics);
            sp.frequency = originalFrequency*harmonics;
            generateSinWave(sig, sp);
        }else if(sp.type == SIGNAL_TYPE_SQUARE){ //perform operation for square wave generation
            sp.amplitude = originalAmplitude*(1.0/harmonics);
            sp.frequency = originalFrequency*harmonics;
            generateSquareWave(sig,sp);
        }
    }

}

void SignalProcess::generateSinWave(QVector<QPointF> &sig, SignalProperties sp){
    qreal samples = SAMPLES(sp.time,sp.sampleRate);
    qreal inc = (sp.frequency*2*M_PI)/sp.sampleRate;
    qreal currPhase = sp.phase;

    for(int x=0;x<samples;++x){
        qreal y = sp.amplitude*qSin(currPhase);
        currPhase += inc;

        if(currPhase >= 2*M_PI){
            currPhase -= 2*M_PI;
        }

        if(currPhase < 0){
            currPhase += 2*M_PI;
        }

        sig[x] = QPointF(x,y+sig[x].y());
    }
}

void SignalProcess::generateSquareWave(QVector<QPointF> &sig, SignalProperties sp)
{
    qreal samples = SAMPLES(sp.time,sp.sampleRate);
    qreal inc = (sp.frequency*2*M_PI)/sp.sampleRate;
    qreal currPhase = sp.phase;

    for(int x=0;x<samples;++x){
        qreal y = sp.amplitude*qSin(currPhase); //take signal slightly below highest level
        currPhase += inc;

        if(currPhase >= 2*M_PI){
            currPhase -= 2*M_PI;
        }

        if(currPhase < 0){
            currPhase += 2*M_PI;
        }

        sig[x] = QPointF(x,y+sig[x].y());
    }
}

void SignalProcess::normalizeSignal(QVector<QPointF> &sig)
{
    qreal max = sig[0].y();

    //get the signal peak
    for(int i=0;i<sig.size();++i){
        max = qMax(qFabs(sig[i].y()),max);
    }

    max = 1.0/max;

    //normalize with reference to peak
    for(int i=0;i<sig.size();++i){
        sig[i] = QPointF(i,sig[i].y()*max);
    }
}

QVector<QPointF> SignalProcess::generateSinWave(SignalProperties sp){

    qreal samples = sp.width*m_sampleRate/100;
    qreal frequency = sp.frequency;
    qreal amplitude = sp.amplitude;
    qreal harmonics = sp.harmonics;
    qreal phase = sp.phase;
    qreal currentPhase = 0.0;
    QVector<QPointF> res(samples,QPointF(0.0,0.0));

    for(int h=0;h<=harmonics;++h){
        qreal realHarmonics = h+1;
        qreal realAmp = amplitude*qreal(1.0/realHarmonics);
        qreal currentPhase = 0.0;
        qreal inc = (qreal)((2*M_PI*((realHarmonics)*frequency))/(1.0*m_sampleRate)+phase);
        for(int x=0;x<samples;++x){
            qreal y = realAmp * qSin(currentPhase);
            res[x] = QPointF(x,res[x].y()+y);
            currentPhase += inc;
            if(currentPhase>=2*M_PI){
                currentPhase -= 2*M_PI;
            }
            if(currentPhase<0){
                currentPhase += 2*M_PI;
            }
        }
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


void SignalProcess::addADSREnvelope(QVector<QPointF> &signal, qreal attackPercent, qreal decayPercent, qreal releasePercent)
{
    if(attackPercent>=100) attackPercent = 99;
    if(decayPercent>=100) decayPercent = 99;
    if(releasePercent>=100) releasePercent = 99;

    qreal attackLength = int((attackPercent*signal.size())/100);
    qreal decayLength = int((attackLength*decayPercent)/100);
    qreal releaseLength = int((releasePercent*signal.size())/100);

    if(attackLength<=0) attackLength = 1;
    if(decayLength<=0) decayLength = 1;
    if(releaseLength<=0) releaseLength = 1;

    qreal attackSlope = 1.0/attackLength;
    qreal decaySlope = attackSlope; //assume decaySlope same as attackSlope
    qreal releaseSlope = 1.0/releaseLength;

    int start = 0;
    int i;

    for(i=start;i<attackLength;++i){
        signal[i] = QPointF(signal[i].x(),signal[i].y()*attackSlope*(i-start));
    }

    start = attackLength;
    for(i=start;i<attackLength+decayLength;++i) {
        signal[i] = QPointF(signal[i].x(),signal[i].y()*decaySlope*(attackLength-(i-start)));
    }

    qreal constantDecay = decaySlope*(attackLength-(i-start));
    start = attackLength+decayLength;
    for(i=start;i<signal.size();++i){
        signal[i] = QPointF(signal[i].x(),signal[i].y()*constantDecay);
    }

    start = signal.size()-releaseLength;
    for(i=start;i<signal.size();++i){
        signal[i] = QPointF(signal[i].x(),signal[i].y()*releaseSlope*(releaseLength-(i-start)));
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
