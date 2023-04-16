#ifndef SIGNALPROPERTIES_H
#define SIGNALPROPERTIES_H

#include<QtCore>
#include <QColor>

struct SignalProperties{  	//signal properties with default values
    qreal x 				= 0;
    qreal y 				= 0;
    qreal frequency 		= 220.0;
    qreal amplitude 		= 1.0;
    qreal phase 			= 0.0;
    qreal harmonics 		= 0;
    qreal width 			= 10; //sample determines the width
    qreal attackPercent 	= 10;
    qreal decayPercent 		= 20;
    qreal releasePercent	= 10;
    QColor color 			= QColor(200,50,50);
};

#endif
