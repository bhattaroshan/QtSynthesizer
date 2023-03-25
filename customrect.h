#ifndef CUSTOMRECT_H
#define CUSTOMRECT_H

#include <QObject>
#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
#include <QGraphicsSceneMouseEvent>
#include <QtGui>

class CustomRect : public QGraphicsRectItem
{
public:
    explicit CustomRect(int x,int y,QGraphicsItem *parent = nullptr);
protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
signals:

private:
    bool m_RightBorderClicked = false;


};

#endif // CUSTOMRECT_H
