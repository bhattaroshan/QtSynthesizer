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

QRectF CustomGraphicsView::createRectToRight(Block *block, int width){
    QRectF sceneRect = block->sceneBoundingRect();
    return QRectF(sceneRect.x()+sceneRect.width(),sceneRect.y(),width,sceneRect.height());
}

QRectF CustomGraphicsView::createRectToLeft(Block *block ,int width){
    QRectF sceneRect = block->sceneBoundingRect();
    return QRectF(sceneRect.x()-width,sceneRect.y(),width,sceneRect.height());
}

QRectF CustomGraphicsView::createRectToTop(Block *block ,int width){
    QRectF sceneRect = block->sceneBoundingRect();
    return QRectF(sceneRect.x(),sceneRect.y()-width,sceneRect.width(),width);
}

QRectF CustomGraphicsView::createRectToBottom(Block *block ,int width){
    QRectF sceneRect = block->sceneBoundingRect();
    return QRectF(sceneRect.x(),sceneRect.y()+sceneRect.height(),sceneRect.width(),width);
}

QList<Block*> CustomGraphicsView::getCollidingItems(Block *block, QRectF rect){
    QList<QGraphicsItem*> collidingItems = scene()->items(rect);
    QList<Block*> blocks;

    for(auto item:collidingItems){
        Block *b = dynamic_cast<Block*>(item);
        if(b and b!=block){
            blocks.append(b);
        }
    }
    return blocks;
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF currentMousePosition = mapToScene(event->pos());

    if(m_trackMoveMode == TRACK_MOVE_MODE){
        QList<Block*> blocks = getSelectedBlocks();
        int distance = currentMousePosition.y()-m_lastMousePressPos.y();
        QPointF delta = currentMousePosition-m_lastMouseMovePos;

        for(auto block:blocks){
            int x = block->pos().x();
            //x += currentMousePosition.x()-m_lastMouseMovePos.x();
            int y = block->pos().y();
            int height = block->sceneBoundingRect().height();

            if(qAbs(delta.x())>qAbs(delta.y())){ //moving in x direction
                if(delta.x()<0){ //dragging to the left
                    QRectF rect = createRectToLeft(block);
                    QList<Block*> collides = getCollidingItems(block,rect);

                    if(!collides.size()){ //no collision detected
                        x += currentMousePosition.x()-m_lastMouseMovePos.x();
                    }else{ //collision detected, reposition the block in case it has gone too far!!!
                        Block *collidedBlock = collides[0]; //assuming it can only collide with one block at at time in one direction
                        //x = collidedBlock->x()+collidedBlock->sceneBoundingRect().width();
                    }
                    qDebug()<<"dragging to the left";
                }else if(delta.x()>0){ //dragging to the right
                    QRectF rect = createRectToRight(block);
                    QList<Block*> collides = getCollidingItems(block,rect);

                    if(!collides.size()){ //no collision detected
                        x += currentMousePosition.x()-m_lastMouseMovePos.x();
                    }else{ //collision detected, reposition the block in case it has gone too far!!!
                        qDebug()<<"I did have a collision here";
                        Block *collidedBlock = collides[0]; //assuming it can only collide with one block at at time in one direction
                        qDebug()<<(QGraphicsItem*)(collidedBlock);
                        //x = collidedBlock->x()-block->sceneBoundingRect().width();
                    }
                    qDebug()<<"dragging to the right";
                }

            }else{
                if(delta.y()<0){ //dragging up
                    QRectF rect = createRectToTop(block);
                    QList<Block*> collides = getCollidingItems(block,rect);
                    if(qAbs(distance)>=height and !collides.size()){
                        y -= 20;
                        m_lastMousePressPos = QPointF(m_lastMousePressPos.x(),mapToScene(event->pos()).y());
                    }
                    qDebug()<<"dragging top";
                }else if(delta.y()>0){//dragging down
                    qDebug()<<"distance = "<<distance;
                    QRectF rect = createRectToBottom(block);
                    QList<Block*> collides = getCollidingItems(block,rect);
                    if(qAbs(distance)>=height and !collides.size()){
                        qDebug()<<"dragging bottom";
                       y += 20;
                        m_lastMousePressPos = QPointF(m_lastMousePressPos.x(),mapToScene(event->pos()).y());
                    }
                }
            }


            block->setPos(x,y);
        }
    }else if(m_trackMoveMode == TRACK_SCALE_MODE){
        m_lastPressedBlock->setRect(0,0,m_lastPressedBlock->sceneBoundingRect().width()+currentMousePosition.x()-m_lastMouseMovePos.x(),m_lastPressedBlock->sceneBoundingRect().height());
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
