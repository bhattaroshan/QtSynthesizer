#ifndef TRACKPROPERTIES_H
#define TRACKPROPERTIES_H

#include <QVector>
#include <QPointF>


class SignalProperties
{
public:
    SignalProperties();
    void generateSinWave(qreal frequency=220, qreal valueLength=0);
    void addADSREnvelope(qreal attackPercent = 10, qreal decayPercent = 20, qreal releasePercent = 10);
    qreal size();
private:
    qreal m_sampleRate = 44100.0; 	//by default sample rate is 44100
    qreal m_startPoint = 0; 		//by default signal starts at zero
    qreal m_amplitude = 1.0; 		//default amplitude is 1
    qreal m_phase = 0.0; 			//default phase is 1
    QVector<QPointF> m_signal; 		//this is where my main signal data is stored
};

#endif // TRACKPROPERTIES_H
