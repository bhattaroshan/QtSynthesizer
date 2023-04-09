#include "customgraphicsview.h"
#include <QMessageBox>


CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{
    setDragMode(QGraphicsView::RubberBandDrag);
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    emit onMousePress();

    if(event->button()==Qt::LeftButton) {
        m_lastMouseMovePos = event->pos();
        m_lastMousePressPos = event->pos();

        QGraphicsItem *clickedItem = scene()->itemAt(event->pos(),QTransform());
        Block *block = dynamic_cast<Block*>(clickedItem);
        if(block){
            m_lastPressedBlock = block;
            qreal x = block->pos().x();
            qreal width = block->boundingRect().width();

            if(x+width-event->pos().x()<=5){
                m_trackMoveMode = TRACK_SCALE_MODE;
            }else{
                m_trackMoveMode = TRACK_MOVE_MODE;
            }
        }else{
            m_trackMoveMode = TRACK_IDLE_MODE;
        }

    }

    QGraphicsView::mousePressEvent(event);
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        m_trackMoveMode = TRACK_IDLE_MODE;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{

    if(m_trackMoveMode == TRACK_MOVE_MODE){
        QList<Block*> blocks = getSelectedBlocks();
        for(auto block:blocks){
            int height = block->boundingRect().height();
            int clickY = m_lastMousePressPos.y();
            int y = block->y();
            int dist = event->pos().y()-clickY;
            if(qAbs(dist)>=20){
                m_lastMousePressPos = event->pos();
                y += (dist<0)?-20:20;
            }


            QPointF itemLocation = QPointF(block->x()+event->pos().x()-m_lastMouseMovePos.x(),y);
            block->setPos(itemLocation);
        }
    }else if(m_trackMoveMode == TRACK_SCALE_MODE){
        Block* sBlock = m_lastPressedBlock; //only grab one selected block even if multiple tracks are selected
        sBlock->setRect(0,0,sBlock->boundingRect().width()+event->pos().x()-m_lastMouseMovePos.x(),sBlock->boundingRect().height());
    }

    m_lastMouseMovePos = event->pos();

    return QGraphicsView::mouseMoveEvent(event);
}

void CustomGraphicsView::keyPressEvent(QKeyEvent *event)
{

   if(event->key() == Qt::Key_Backspace){
       QList<Block*> blocks = getSelectedBlocks();
       if(blocks.size()<=0) return; //do nothing if non of the items are selected
       QString msg = QString::number(blocks.size())+ " item";
       if(blocks.size()>1){
           msg +="s";
       }

       QMessageBox::StandardButton ret = QMessageBox::warning(nullptr,"Delete",QString("Are you sure you want to delete %1").arg(msg),QMessageBox::Yes|QMessageBox::No);
       if(ret == QMessageBox::No) return; //deleting item not confirmed

       for(auto block:blocks){
           scene()->removeItem(block);
       }
   }

   return QGraphicsView::keyPressEvent(event);
}

QList<Block *> CustomGraphicsView::getSelectedBlocks()
{
    QList<QGraphicsItem *> items = scene()->selectedItems();
    QList<Block*> blocks;

    for(auto item:items){
        Block* block = dynamic_cast<Block*>(item);
        if(block){
            blocks.append(block);
        }
    }
    return blocks;
}
