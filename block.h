#ifndef BLOCK_H
#define BLOCK_H
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QMenu>

class Block:public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Block(QGraphicsItem *parent=nullptr);
    void getAllBlocksInfo(QList<QRectF> blockRect);
    int getFrequency();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    QList<QGraphicsRectItem*> getAllTracks(QRectF rect);

    QColor m_brushColor;

    bool m_dragEnabled = false;

    QList<QRectF> m_rectangle;
    QPointF m_lastMouseClickPos = QPointF(0,0);

    int m_frequency = 220;
    QGraphicsTextItem *m_frequencyText;

public slots:
    void setFrequency(int);

signals:
    void onItemDrag(Block *item);
    void onItemDoubleClicked(int);
    void trackUpdated();

};

#endif // BLOCK_H
