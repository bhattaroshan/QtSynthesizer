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
    Block(SignalProperties sp, QGraphicsItem *parent=nullptr);
    void updateBlockProperties();
    void getAllBlocksInfo(QList<QRectF> blockRect);

    //function necessary to put it in map
    bool operator<(const Block &other)const;

    SignalProperties getBlockProperties();
    void setBlockProperties(SignalProperties sp){m_sp=sp;}

    qreal getX(){return m_sp.x;}
    qreal getY(){return m_sp.y;}
    qreal getWidth(){return m_sp.width;}
    qreal getFrequency(){return m_sp.frequency;}
    QColor getColor(){return m_sp.color;}
    qreal getHarmonics(){return m_sp.harmonics;}
    qreal getAmplitude(){return m_sp.amplitude;}
    qreal getAttack(){return m_sp.attack;}
    qreal getDecay(){return m_sp.decay;}
    qreal getRelease(){return m_sp.release;}
    qreal getType(){return m_sp.type;}

    void setX(qreal x){m_sp.x = x;}
    void setY(qreal y){m_sp.y = y;}
    void setWidth(qreal width){m_sp.width=width;}
    void setFrequency(qreal frequency){m_sp.frequency=frequency;update();}
    void setColor(QColor color){m_sp.color=color;};
    void setHarmonics(qreal harmonics){ m_sp.harmonics = harmonics;}
    void setAmplitude(qreal amplitude){ m_sp.amplitude = amplitude;}
    void setAttack(qreal attack){m_sp.attack=attack;}
    void setDecay(qreal decay){m_sp.decay=decay;}
    void setRelease(qreal release){m_sp.release=release;}
    void setType(qreal type){m_sp.type=type;}

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;
    void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
    void paint(QPainter *painter,const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setBrushFromFrequency(int frequency);

    SignalProperties m_sp;

    bool m_dragEnabled = false;

    QPointF m_lastMouseClickPos = QPointF(0,0);

public slots:

signals:
    void clicked();

};

#endif // BLOCK_H
