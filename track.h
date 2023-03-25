#ifndef TRACK_H
#define TRACK_H

#include <QGraphicsItem>
#include <QGraphicsRectItem>
#include <QGraphicsWidget>
#include <QGraphicsSceneMouseEvent>
#include <QtGui>
#include <QMenu>
#include <QMessageBox>
#include <QColorDialog>

class Track: public QGraphicsRectItem
{
public:
     explicit Track(int x,int y,QGraphicsItem *parent = nullptr);
     void setHeight(qreal height);
     qreal getHeight();
protected:
    void hoverMoveEvent(QGraphicsSceneHoverEvent* event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    //QVariant itemChange(GraphicsItemChange change, const QVariant& value) override;

private:
    bool m_RightBorderClicked = false;
    QColor m_trackColor = Qt::red;
    qreal m_height = 20;
    QPointF m_lastRectPos = QPointF(0,0);
    bool m_pulledOnce = false;
};

#endif // TRACK_H
