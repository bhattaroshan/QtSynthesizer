#ifndef CUSTOMGRAPHICS_H
#define CUSTOMGRAPHICS_H

#include <QtWidgets>

class CustomGraphics:public QGraphicsWidget
{
public:
    CustomGraphics(){
           setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
       }

       QRectF boundingRect() const override
       {
           return QRectF(0, 0, 100, 100);
       }

       void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override
       {
           Q_UNUSED(option);
           Q_UNUSED(widget);

           painter->setRenderHint(QPainter::Antialiasing);
           painter->setBrush(Qt::blue);
           painter->drawEllipse(boundingRect());
       }
};

#endif // CUSTOMGRAPHICS_H
