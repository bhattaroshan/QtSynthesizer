#include "block.h"
#include <QPen>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

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

    m_frequencyText = new QGraphicsTextItem(QString::number(m_frequency),this);
    m_frequencyText->setDefaultTextColor(QColor(255,255,255));
    m_frequencyText->setTextInteractionFlags(Qt::NoTextInteraction);
    m_frequencyText->setFlag(QGraphicsItem::ItemIsSelectable,false);
    m_frequencyText->setFlag(QGraphicsItem::ItemIsFocusable,false);

    QPointF textPos = this->boundingRect().center() - m_frequencyText->boundingRect().center();
    m_frequencyText->setPos(textPos);

    //setPos(QRandomGenerator::global()->bounded(400),QRandomGenerator::global()->bounded(400));
    setAcceptHoverEvents(true);
}

void Block::getAllBlocksInfo(QList<QRectF> blockRect)
{
    m_rectangle = blockRect;
}

int Block::getFrequency()
{
    return m_frequency;
}


void Block::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit onItemDrag(this);

    if(m_dragEnabled){
        setRect(0,0,event->pos().x(),20); //intersection not checked while scaling the track
        QPointF textPos = this->boundingRect().center() - m_frequencyText->boundingRect().center();
        m_frequencyText->setPos(textPos);
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
            QList<QGraphicsRectItem*> intersectingRectItems;

            for(auto graphicsItem:intersectingItems){
                QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem*>(graphicsItem);
                if(rectItem){
                    intersectingRectItems.append(rectItem);
                }
            }

            if(intersectingRectItems.length()<1){
                y+=20;
            }
        }
        else if(m_lastMouseClickPos.y()-mouseY>20){
            rect.setTop(rect.y()-20);
            rect.setBottom(rect.bottom()-20);
            QList<QGraphicsItem*> intersectingItems = scene()->items(rect,Qt::IntersectsItemBoundingRect);

            QList<QGraphicsRectItem*> intersectingRectItems;

            for(auto graphicsItem:intersectingItems){
                QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem*>(graphicsItem);
                if(rectItem){
                    intersectingRectItems.append(rectItem);
                }
            }

            if(intersectingRectItems.length()<1){
                y-=20;
            }
        }


        rect = sceneBoundingRect();
        if(mouseX>m_lastMouseClickPos.x()){ //dragging item to the right
            rect.setRect(rect.right(),rect.top(),1,20);
            QList<QGraphicsItem*> intersectingItems = scene()->items(rect,Qt::IntersectsItemBoundingRect);
            QList<QGraphicsRectItem*> intersectingRectItems;

            for(auto graphicsItem:intersectingItems){
                QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem*>(graphicsItem);
                if(rectItem){
                    intersectingRectItems.append(rectItem);
                }
            }
            if(intersectingRectItems.length()>=1){
                //considering there can only one intersecting item
                QRectF iRect = intersectingItems[0]->sceneBoundingRect();
                x = iRect.left()-this->rect().width();
            }

        }else{ //dragging item to the left
            rect.setRect(rect.left()-1,rect.top(),1,20);
            QList<QGraphicsItem*> intersectingItems = scene()->items(rect,Qt::IntersectsItemBoundingRect);
            QList<QGraphicsRectItem*> intersectingRectItems;

            for(auto graphicsItem:intersectingItems){
                QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem*>(graphicsItem);
                if(rectItem){
                    intersectingRectItems.append(rectItem);
                }
            }
            if(intersectingRectItems.length()>=1){
                //considering there can only one intersecting item
                QRectF iRect = intersectingItems[0]->sceneBoundingRect();
                x = iRect.left()+iRect.width();
            }
        }


        //qDebug()<<x<<" , "<<y;
        setPos(x,y);
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

void Block::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    emit onItemDoubleClicked(m_frequency);
    QGraphicsRectItem::mouseDoubleClickEvent(event);
}

void Block::setFrequency(int frequency)
{
    m_frequency = frequency;
    m_frequencyText->setPlainText(QString::number(m_frequency));
    QPointF textPos = this->boundingRect().center() - m_frequencyText->boundingRect().center();
    m_frequencyText->setPos(textPos);
}
