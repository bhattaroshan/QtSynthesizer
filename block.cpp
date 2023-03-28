#include "block.h"
#include <QPen>
#include <QtGui>
#include <QGraphicsScene>

Block::Block(QGraphicsItem *parent)
    :QGraphicsRectItem(parent){

    setFlags(ItemIsMovable);

    m_brushColor = QColor(QRandomGenerator::global()->bounded(256),
                                        QRandomGenerator::global()->bounded(256),
                                        QRandomGenerator::global()->bounded(256));

    setPen(Qt::NoPen);
    setBrush(m_brushColor);
    setRect(0,0,100,20);
    setPos(0,0);
    //setPos(QRandomGenerator::global()->bounded(400),QRandomGenerator::global()->bounded(400));
    setAcceptHoverEvents(true);
}

void Block::getAllBlocksInfo(QList<QRectF> blockRect)
{
    m_rectangle = blockRect;
}


void Block::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit onItemDrag(this);

    if(m_dragEnabled){
        setRect(0,0,event->scenePos().x()-this->x(),20);
    }else{
        int mouseY = event->pos().y();
        int mouseX = event->pos().x();
        int y = this->y();
        int x = this->x()+event->pos().x()-m_lastMouseClickPos.x();
        QRectF rect = sceneBoundingRect();

        if(mouseY-m_lastMouseClickPos.y()> 20){
            rect.setTop(rect.y()+20);
            rect.setBottom(rect.bottom()+20);
            QList<QGraphicsItem*> intersectingItems = scene()->items(rect,Qt::IntersectsItemBoundingRect);
            qDebug()<<intersectingItems;
            if(intersectingItems.length()<1){
                y+=20;
            }
        }
        else if(m_lastMouseClickPos.y()-mouseY>20){
            rect.setTop(rect.y()-20);
            rect.setBottom(rect.bottom()-20);
            QList<QGraphicsItem*> intersectingItems = scene()->items(rect,Qt::IntersectsItemBoundingRect);
            qDebug()<<"up = "<<intersectingItems;
            if(intersectingItems.length()<1){
                y-=20;
            }
        }

        rect = sceneBoundingRect();

        if(mouseX>m_lastMouseClickPos.x()){ //dragging item to the right

        }else{ //dragging item to the left
        }


        setPos(x,y);
        m_lastMouseMovePos = event->pos();
        //QGraphicsItem::mouseMoveEvent(event);
    }
}

void Block::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    int ax = this->boundingRect().width()+this->boundingRect().x();
    if(qAbs(event->pos().x()-ax)<=5){
        setCursor(Qt::SizeHorCursor);
    }else{
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsItem::hoverMoveEvent(event);
}

void Block::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        int ax = this->rect().width()+this->rect().x();
        if(qAbs(event->pos().x()-ax)<=5) {
            m_dragEnabled = true;
        }else{
            m_lastMouseClickPos = event->pos();
            m_lastMouseClickPosScene = event->scenePos();
        }
    }
    QGraphicsItem::mousePressEvent(event);
}

void Block::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
   if(event->button()==Qt::LeftButton){
       m_dragEnabled = false;
   }
    QGraphicsItem::mouseReleaseEvent(event);
}
