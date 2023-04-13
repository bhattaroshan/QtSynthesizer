#ifndef BLOCK_H
#define BLOCK_H
#include <QGraphicsItem>
#include <QGraphicsSceneHoverEvent>
#include <QMenu>
#include "signalproperties.h"

class Block:public QObject, public QGraphicsRectItem
{
    Q_OBJECT
public:
    Block(int x,int y,int frequency=220,QGraphicsItem *parent=nullptr);
    void getAllBlocksInfo(QList<QRectF> blockRect);


    SignalProperties getBlockProperties(){return m_sp;};
    void setBlockProperties(SignalProperties sp){m_sp=sp;};

    int getFrequency(){return m_sp.frequency;};
    QColor getColor(){return m_sp.color;};
    qreal getHarmonics(){return m_sp.harmonics;}
    qreal getAmplitude(){return m_sp.amplitude;};

    void setFrequency(int frequency){m_sp.frequency=frequency;};
    void setColor(QColor color){m_sp.color=color;};
    void setHarmonics(qreal harmonics){ m_sp.harmonics = harmonics;}
    void setAmplitude(qreal amplitude){ m_sp.amplitude = amplitude;};

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setBrushFromFrequency(int frequency);

    SignalProperties m_sp;

    bool m_dragEnabled = false;

    //QList<QRectF> m_rectangle;
    QPointF m_lastMouseClickPos = QPointF(0,0);

public slots:

signals:
    void onItemDrag(Block *item);
    void onItemDoubleClicked(int frequency,QColor color);
    void onItemSingleClick();
    void trackUpdated();

};

#endif // BLOCK_H
