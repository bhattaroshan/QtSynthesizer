#include "block.h"
#include <QPen>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsTextItem>

Block::Block(int x,int y,QGraphicsItem *parent)
    :QGraphicsRectItem(parent){

    setFlags(ItemIsMovable);

    m_brushColor = QColor(QRandomGenerator::global()->bounded(256),
                                        QRandomGenerator::global()->bounded(256),
                                        QRandomGenerator::global()->bounded(256));

    setPen(Qt::NoPen);
    setBrush(m_brushColor);
    setRect(0,0,m_width,m_height);
    setPos(x,y);

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

QColor Block::getColor()
{
    return m_brushColor;
}

void Block::handleDraggable(qreal x){
    m_width = x;
    setRect(0,0,m_width,m_height); //intersection not checked while scaling the track
    QPointF textPos = this->boundingRect().center() - m_frequencyText->boundingRect().center();
    m_frequencyText->setPos(textPos);
}

void Block::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->buttons() & Qt::LeftButton){
         emit onItemDrag(this);

        if(m_dragEnabled){
            handleDraggable(event->pos().x());
        }else{
            int mouseY = event->pos().y();
            int mouseX = event->pos().x();
            int y = this->y();
            int x = this->x()+event->pos().x()-m_lastMouseClickPos.x();
            QRectF rect = sceneBoundingRect()	;

            if(mouseY-m_lastMouseClickPos.y()> m_height){
                rect.setTop(rect.y()+m_height);
                rect.setBottom(rect.bottom()+m_height);
                rect.setRect(rect.right(),rect.top(),1,m_height);
                QList<QGraphicsRectItem*> intersectingRectItems = getAllTracks(rect);

                if(intersectingRectItems.length()<1){
                    y+=m_height;
                }
            }
            else if(m_lastMouseClickPos.y()-mouseY>m_height){
                rect.setTop(rect.y()-m_height);
                rect.setBottom(rect.bottom()-m_height);
                rect.setRect(rect.right(),rect.top(),1,m_height);
                QList<QGraphicsRectItem*> intersectingRectItems = getAllTracks(rect);

                if(intersectingRectItems.length()<1 and y>=m_height*2){
                    y-=m_height;
                }
        }


        rect = sceneBoundingRect();
        if(mouseX>m_lastMouseClickPos.x()){ //dragging item to the right
            rect.setRect(rect.right(),rect.top(),1,20);
            QList<QGraphicsRectItem*> intersectingRectItems = getAllTracks(rect);

            if(intersectingRectItems.length()>=1){
                //considering there can only one intersecting item
                QRectF iRect = intersectingRectItems[0]->sceneBoundingRect();
                x = iRect.left()-this->rect().width();
            }

        }else{ //dragging item to the left
            if(rect.left()-1>0){
                rect.setRect(rect.left()-1,rect.top(),1,20);
                QList<QGraphicsRectItem*> intersectingRectItems = getAllTracks(rect);

                if(intersectingRectItems.length()>=1){
                //considering there can only one intersecting item
                    QRectF iRect = intersectingRectItems[0]->sceneBoundingRect();
                    x = iRect.left()+iRect.width();
                }

            }else{
                x = 0;
            }
        }
            setPos(x,y);
        }
    }

}

QList<QGraphicsRectItem*> Block::getAllTracks(QRectF rect)
{
    QList<QGraphicsItem*> intersectingItems = scene()->items(rect,Qt::IntersectsItemBoundingRect);
    QList<QGraphicsRectItem*> intersectingRectItems;

    for(auto graphicsItem:intersectingItems){
        QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem*>(graphicsItem);
        if(rectItem){
            intersectingRectItems.append(rectItem);
        }
    }
    return intersectingRectItems;
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
        emit onItemSingleClick();
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
       emit trackUpdated();
   }
   QGraphicsItem::mouseReleaseEvent(event);
}

void Block::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        emit onItemDoubleClicked(m_frequency,m_brushColor);
    }
    QGraphicsRectItem::mouseDoubleClickEvent(event);
}

void Block::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    menu.addAction("Copy");
    menu.addAction("Paste");
    menu.addAction("Delete");
    menu.addAction("Properties");
    menu.exec(event->screenPos());
}

void Block::setFrequency(int frequency)
{
    m_frequency = frequency;
    m_frequencyText->setPlainText(QString::number(m_frequency));
    QPointF textPos = this->boundingRect().center() - m_frequencyText->boundingRect().center();
    m_frequencyText->setPos(textPos);
}

void Block::setColor(QColor color)
{
    m_brushColor = color;
    setBrush(QBrush(m_brushColor));
}

void Block::setOutline(bool flag)
{
    if(flag){
        setRect(0,1,m_width-2,m_height-1);
        setPen(QPen(QColor(255,255,255),2));
    }else{
        setRect(0,0,m_width,m_height);
        setPen(Qt::NoPen);
    }
}
