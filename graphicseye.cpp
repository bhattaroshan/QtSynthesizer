#include "graphicseye.h"

GraphicsEye::GraphicsEye(QGraphicsItem *parent)
    :QGraphicsItem(parent)
{
    setAcceptHoverEvents(true);
}

QRectF GraphicsEye::boundingRect() const
{
    return QRectF(0,0,30,30);
}

void GraphicsEye::paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget)
{

    painter->setBrush(QColor(128,128,128,40));
    painter->setPen(QColor(128,128,128,50));
    //painter->drawRoundedRect(boundingRect(),2,2);
    painter->drawRect(boundingRect());
    QImage image(":/icons/eye-visible.png");
    QImage scaledImage = image.scaled(QSize(18,18),Qt::KeepAspectRatio);
    painter->drawImage(boundingRect().width()/2-scaledImage.width()/2,
                       boundingRect().height()/2-scaledImage.height()/2,
                       scaledImage);
}

void GraphicsEye::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit clicked();
    QGraphicsItem::mousePressEvent(event);
}

void GraphicsEye::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::PointingHandCursor);
    QGraphicsItem::hoverEnterEvent(event);
}

void GraphicsEye::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    setCursor(Qt::ArrowCursor);
    QGraphicsItem::hoverLeaveEvent(event);
}

