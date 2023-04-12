#include "signalprocess.h"

SignalProcess::SignalProcess()
{

}

void SignalProcess::addSignals(QVector<SignalProperties> &signalTracks, QVector<QPointF> &result)
{
    result.clear(); //clear the signal
    qreal max = 0;
    for(auto track:signalTracks){
        if(track.size()>max){
            max = track.size();
        }
    }
}
