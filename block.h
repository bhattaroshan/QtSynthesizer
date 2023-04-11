#ifndef BLOCK_H
#define BLOCK_H
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QMenu>

class Block:public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Block(int x,int y,int frequency=220,QGraphicsItem *parent=nullptr);
    void getAllBlocksInfo(QList<QRectF> blockRect);
    int getFrequency();
    QColor getColor();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    QColor m_brushColor;

    bool m_dragEnabled = false;

    QList<QRectF> m_rectangle;
    QPointF m_lastMouseClickPos = QPointF(0,0);

    int m_frequency = 220;
    int m_height = 20;
    int m_width = 50;
    QGraphicsTextItem *m_frequencyText;

public slots:
    void setFrequency(int);
    void setColor(QColor);

signals:
    void onItemDrag(Block *item);
    void onItemDoubleClicked(int frequency,QColor color);
    void onItemSingleClick();
    void trackUpdated();

};

#endif // BLOCK_H
