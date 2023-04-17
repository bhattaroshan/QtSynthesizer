#ifndef GRAPHICSSEEK_H
#define GRAPHICSSEEK_H

#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QPolygon>

class GraphicsSeek:public QObject, public QGraphicsItem
{
    Q_OBJECT
protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    QPolygon polygon;
    QRectF rect;
public:
    GraphicsSeek();
};

#endif // GRAPHICSSEEK_H
