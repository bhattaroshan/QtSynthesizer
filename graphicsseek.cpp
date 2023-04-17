#include "graphicsseek.h"


GraphicsSeek::GraphicsSeek()
{
    polygon<<QPoint(0,0)<<QPoint(10,0)<<QPoint(10,10)<<QPoint(5,15)<<QPoint(5,1000)<<QPoint(5,15)
           <<QPoint(0,10)<<QPoint(0,0);
    rect = polygon.boundingRect();
}

QRectF GraphicsSeek::boundingRect() const{
    return rect;
}

void GraphicsSeek::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(QColor(0,100,220));
    painter->setPen(QColor(0,100,220));
    painter->drawPolygon(polygon);
}
