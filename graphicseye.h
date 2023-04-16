#ifndef GRAPHICSEYE_H
#define GRAPHICSEYE_H

#include <QtGui>
#include <QGraphicsItem>

class GraphicsEye:public QObject,public QGraphicsItem
{
    Q_OBJECT
public:
    GraphicsEye(QGraphicsItem *parent=nullptr);
    void setActive(bool state){m_isActive=state;}
    bool isActive(){return m_isActive;};

signals:
    void clicked();

protected:
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *options, QWidget *widget) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;

    QColor m_greenColor = QColor(0x0d6534);
    QColor m_greyColor = QColor(0x7c7e83);
    bool m_isActive = true;

};

#endif // GRAPHICSEYE_H
