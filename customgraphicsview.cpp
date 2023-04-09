#include "customgraphicsview.h"
#include "block.h"


CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{
    setDragMode(QGraphicsView::RubberBandDrag);
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    emit onMousePress();

    if(event->button()==Qt::LeftButton) {
        m_leftMousePressed = true;
        m_lastMousePos = event->pos();
        m_lastMousePressPos = event->pos();
    }

    QGraphicsView::mousePressEvent(event);
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        m_leftMousePressed = false;
        m_draggingZone = false;
        m_allowMovement = false;

    }
    QGraphicsView::mouseReleaseEvent(event);
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QList<QGraphicsItem*> selectedItems = scene()->selectedItems();
    QGraphicsItem *it = scene()->mouseGrabberItem();
    Block *mouseGrabbedItem = dynamic_cast<Block*>(it);

    if(mouseGrabbedItem and m_leftMousePressed) {
        QPointF trackEndPos = mouseGrabbedItem->pos()+
                              QPointF(mouseGrabbedItem->boundingRect().width(),mouseGrabbedItem->boundingRect().height());

        if(trackEndPos.x()-event->pos().x()>5 or m_allowMovement){
            m_allowMovement = true;
            for(auto item:selectedItems){
               Block *block = dynamic_cast<Block*>(item);
               if(block){
                   block->setPos(block->pos()+event->pos()-m_lastMousePos);
               }
            }

        }else{ //dragging enabled
        }
    }

//    if(mouseGrabbedItem and m_leftMousePressed){
//        QPointF trackEndPos = mouseGrabbedItem->pos()+
//                              QPointF(mouseGrabbedItem->boundingRect().width(),mouseGrabbedItem->boundingRect().height());

//        if(trackEndPos.x()-event->pos().x()<=5){
//            qDebug()<<"clicked for resize";
//        }else{
//            for(auto item:selectedItems){
//                Block *block = dynamic_cast<Block*>(item);
//                if(block){
//                    block->setPos(block->pos()+event->pos()-m_lastMousePos);
//                }
//            }
//        }
//    }


    if(m_leftMousePressed){
        m_lastMousePos = event->pos();
    }

    QGraphicsView::mouseMoveEvent(event);
}
