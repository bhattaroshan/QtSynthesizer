#ifndef BLOCK_H
#define BLOCK_H
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>

class Block:public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Block(QGraphicsItem *parent=nullptr);
    void getAllBlocksInfo(QList<QRectF> blockRect);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    QColor m_brushColor;

    bool m_dragEnabled = false;

    QList<QRectF> m_rectangle;
    QPointF m_lastMouseClickPos = QPointF(0,0);

signals:
    void onItemDrag(Block *item);

};

#endif // BLOCK_H
