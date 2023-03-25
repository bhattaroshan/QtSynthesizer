#include "track.h"

Track::Track(int x, int y, QGraphicsItem *parent)
    : QGraphicsRectItem(parent)
{
    m_trackColor = QColor(QRandomGenerator::global()->bounded(256),QRandomGenerator::global()->bounded(256),QRandomGenerator::global()->bounded(256));
    setBrush(m_trackColor);
    setRect(x,y,100,20);
    setFlags(QGraphicsItem::GraphicsItemFlag::ItemIsMovable);
    setAcceptHoverEvents(true);
}

void Track::setHeight(qreal height)
{
    m_height = height;
}

qreal Track::getHeight(){
    return m_height;
}

void Track::hoverMoveEvent(QGraphicsSceneHoverEvent* event){
    QRectF rect = this->rect();

    if(qAbs(event->pos().x()-(rect.x()+rect.width()))<5){
        this->setCursor(Qt::SizeHorCursor);
    }else{
        this->setCursor(Qt::ArrowCursor);
    }
    QGraphicsRectItem::hoverMoveEvent(event);
}

void Track::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QRectF rect = this->rect();
    if(event->button()==Qt::LeftButton){
        int cursor_x = event->pos().x();
        if(qAbs(cursor_x-(rect.x()+rect.width()))<5){
            m_RightBorderClicked = true;
        }else{
            m_lastRectPos = event->pos();
            m_pulledOnce = true;
        }
    }
    QGraphicsRectItem::mousePressEvent(event);
}

void Track::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        m_RightBorderClicked = false;
    }
    QGraphicsRectItem::mouseReleaseEvent(event);
}

void Track::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    int cursor_x = event->pos().x();
    int cursor_y = event->pos().y();
    QRectF rect = this->rect();

    if(m_RightBorderClicked){
        this->setRect(rect.x(),rect.y(),rect.width()+(cursor_x-rect.x()-rect.width()),rect.height());
    }else{
        qreal xpos = pos().x()+cursor_x-m_lastRectPos.x();
        if(xpos<0) xpos=0;

        qreal ypos = pos().y();
        qreal diff = pos().y()-m_lastRectPos.y();
//        qDebug()<<diff;
//        if(diff<=-10 and m_pulledOnce){
//            ypos+=20;
//            m_pulledOnce = false;
//        }else if(diff>=10 and m_pulledOnce){
//            ypos -= 20;
//            m_pulledOnce = false;
//        }
        setPos(xpos,ypos);

        //QGraphicsRectItem::mouseMoveEvent(event);
    }
}

void Track::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QAction *action1 = menu.addAction("Pick Color");
    QGraphicsRectItem::contextMenuEvent(event);
    QAction *selectedAction = menu.exec(event->screenPos());
    if(selectedAction == action1){
        QColor color = QColorDialog::getColor(Qt::red,nullptr,"Select a color");
        if(color.isValid()){
            qDebug()<<color;
            m_trackColor = color;
            setBrush(m_trackColor);
        }
    }
}

//QVariant Track::itemChange(GraphicsItemChange change, const QVariant &value)
//{
////    qDebug()<<"help me here "<<change;
////    if(change==QGraphicsRectItem::ItemCursorHasChanged){
////        qDebug()<<"I was dragged here";
////        QPointF newPos = value.toPointF();
////        newPos.setY(pos().y());
////        return newPos;
////    }
//    return QGraphicsRectItem::itemChange(change,value);
//}



