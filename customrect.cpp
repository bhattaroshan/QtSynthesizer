#include "customrect.h"

CustomRect::CustomRect(int x,int y,QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    setRect(x,y,100,20);
    setFlags(QGraphicsItem::GraphicsItemFlag::ItemIsMovable);
    setAcceptHoverEvents(true);
}


void CustomRect::hoverMoveEvent(QGraphicsSceneHoverEvent* event){
    QRectF rect = this->rect();

    if(qAbs(event->pos().x()-(rect.x()+rect.width()))<5){
        this->setCursor(Qt::SizeHorCursor);
    }else{
        this->setCursor(Qt::ArrowCursor);
    }
    QGraphicsRectItem::hoverMoveEvent(event);
}

void CustomRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QRectF rect = this->rect();
    if(event->button()==Qt::LeftButton){
        int cursor_x = event->pos().x();
        if(qAbs(cursor_x-(rect.x()+rect.width()))<5){
            m_RightBorderClicked = true;
        }
    }
    QGraphicsRectItem::mousePressEvent(event);
}

void CustomRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        m_RightBorderClicked = false;
    }
    QGraphicsRectItem::mouseReleaseEvent(event);
}

void CustomRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(m_RightBorderClicked){
        int cursor_x = event->pos().x();
        //int cursor_y = event->pos().y();
        QRectF rect = this->rect();
        this->setRect(rect.x(),rect.y(),rect.width()+(cursor_x-rect.x()-rect.width()),rect.height());
    }else{
        QGraphicsRectItem::mouseMoveEvent(event);
    }
}


