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
        m_lastMouseMovePos = mapToScene(event->pos());
        m_lastMousePressPos = mapToScene(event->pos());

        QGraphicsItem *clickedItem = this->itemAt(event->pos());
        Block *block = dynamic_cast<Block*>(clickedItem);
        if(block){
            m_lastPressedBlock = block;
            qreal x = block->pos().x();
            qreal width = block->boundingRect().width();

            if(x+width-m_lastMousePressPos.x()<=5){
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
    QPointF currentMousePosition = mapToScene(event->pos());

    if(m_trackMoveMode == TRACK_MOVE_MODE){
        QList<Block*> blocks = getSelectedBlocks();
        int distance = currentMousePosition.y()-m_lastMousePressPos.y();
        for(auto block:blocks){
            int x = block->pos().x();
            //x += currentMousePosition.x()-m_lastMouseMovePos.x();
            int y = block->pos().y();
            int height = block->boundingRect().height();

            //QList<QGraphicsItem*> collidingItems = block->collidingItems();
            QRectF rectTopMask = QRectF(block->x(),block->y()-1,block->boundingRect().width(),1);

            QList<QGraphicsItem*> collidingItems = scene()->items(rectTopMask);
            QList<Block*> collidingBlocks;
            for(auto cItem:collidingItems){
                Block *cBlock = dynamic_cast<Block*>(cItem);
                if(cBlock){
                    collidingBlocks.append(cBlock);
                }
            }

            qDebug()<<collidingBlocks.size();

            if(collidingBlocks.size()==0){ //there are colliding blocks here
                x += currentMousePosition.x()-m_lastMouseMovePos.x();
            }


            if(qAbs(distance)>=height and collidingBlocks.size()==0){ //stepped y movement
                if(currentMousePosition.y()>m_lastMouseMovePos.y()){
                    y+=height;
                }else{
                    y -= height;
                }
                m_lastMousePressPos = mapToScene(event->pos());
            }
            block->setPos(x,y);
        }
    }else if(m_trackMoveMode == TRACK_SCALE_MODE){
        m_lastPressedBlock->setRect(0,0,m_lastPressedBlock->boundingRect().width()+currentMousePosition.x()-m_lastMouseMovePos.x(),m_lastPressedBlock->boundingRect().height());
    }

    m_lastMouseMovePos = mapToScene(event->pos());

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
