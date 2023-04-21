#ifndef SIGNALPROPERTIES_H
#define SIGNALPROPERTIES_H

#include<QtCore>
#include <QColor>

#define SAMPLES(time,samplerate) ((time*samplerate)/1000)

#define SIGNAL_TYPE_SINUSOIDAL  0
#define SIGNAL_TYPE_SQUARE		1
#define SIGNAL_TYPE_TRIANGULAR	2

struct SignalProperties{  	//signal properties with default values
    qreal type				= 0;
    qreal x 				= 0;
    qreal y 				= 0;
    qreal frequency 		= 220.0;
    qreal amplitude 		= 1.0;
    qreal phase 			= 0.0;
    qreal harmonics 		= 0;
    qreal width 			= 100; //sample determines the width
    qreal time				= 1000; //in milliseconds
    qreal attackPercent 	= 10;
    qreal decayPercent 		= 20;
    qreal releasePercent	= 10;
    QColor color 			= QColor(200,50,50);
    qreal attack			= 1;
    qreal decay				= 1;
    qreal release			= 1;
    qreal sampleRate		= 44100;
};

#endif
