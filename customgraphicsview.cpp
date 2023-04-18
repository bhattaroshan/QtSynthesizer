#include "customgraphicsview.h"
#include <QMessageBox>


CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{
    setDragMode(QGraphicsView::RubberBandDrag);
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) {
        m_lastMouseMovePos = mapToScene(event->pos());
        m_lastMousePressPos = mapToScene(event->pos());

        QGraphicsItem *clickedItem = this->itemAt(event->pos());
        Block *block = dynamic_cast<Block*>(clickedItem);
        if(block){ //block is clicked
            emit trackClicked();
            m_lastPressedBlock = block;
            qreal x = block->pos().x();
            qreal width = block->boundingRect().width();

            if(x+width-m_lastMousePressPos.x()<=5){
                m_trackMoveMode = TRACK_SCALE_MODE;
            }else{
                m_trackMoveMode = TRACK_MOVE_MODE;
            }
        }else{
            //either seekbar or offtrackclicked
            if(m_lastMousePressPos.y()<=30){
                setDragMode(QGraphicsView::NoDrag);
                m_seek->setPos(m_lastMousePressPos.x(),15);
            }else{ //offtrack clicked
                setDragMode(QGraphicsView::RubberBandDrag);
                emit offTrackClicked();
            }
            m_trackMoveMode = TRACK_IDLE_MODE;
        }

    }

    QGraphicsView::mousePressEvent(event);
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        if(m_trackMoveMode != TRACK_IDLE_MODE){
            setCursor(Qt::ArrowCursor);
            emit viewUpdated(); //only emit if there are any changes
        }
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


    if(m_trackMoveMode != TRACK_IDLE_MODE){ //resizing tracks
        QList<Block*> blocks = getAllBlocks();
        qreal width = this->parentWidget()->width();
        qreal height = this->parentWidget()->height();
        for(auto block:blocks){
            if(block->sceneBoundingRect().right()>width){
                width = block->sceneBoundingRect().right();
            }
            if(block->sceneBoundingRect().bottom()>height){
                height =  block->sceneBoundingRect().bottom();
            }
        }

        this->setSceneRect(0,0,width+100,height+50);
    }

    if(m_trackMoveMode == TRACK_MOVE_MODE){
        QList<Block*> blocks = getSelectedBlocks();
        int distance = currentMousePosition.y()-m_lastMousePressPos.y();
        QPointF delta = currentMousePosition-m_lastMouseMovePos;

        for(auto block:blocks){
            int x = block->pos().x();
            int y = block->pos().y();
            int height = block->sceneBoundingRect().height();

            if(qAbs(delta.x())>qAbs(delta.y())){ //moving in x direction
                if(delta.x()<0){ //dragging to the left
                    QRectF rect = createRectToLeft(block);
                    QList<Block*> collides = getCollidingItems(block,rect);

                    if(!collides.size()){ //no collision detected
                        x += currentMousePosition.x()-m_lastMouseMovePos.x();
                        x = qMax(30,x);
                    }else{ //collision detected, reposition the block in case it has gone too far!!!
                        Block *collidedBlock = collides[0]; //assuming it can only collide with one block at at time in one direction
                        x = collidedBlock->x()+collidedBlock->sceneBoundingRect().width();
                    }
                }else if(delta.x()>0){ //dragging to the right
                    QRectF rect = createRectToRight(block);
                    QList<Block*> collides = getCollidingItems(block,rect);

                    if(!collides.size()){ //no collision detected
                        x += currentMousePosition.x()-m_lastMouseMovePos.x();
                    }else{ //collision detected, reposition the block in case it has gone too far!!!
                        Block *collidedBlock = collides[0]; //assuming it can only collide with one block at at time in one direction
                        x = collidedBlock->x()-block->sceneBoundingRect().width();
                    }
                }

            }else{
                if(delta.y()<0){ //dragging up
                    QRectF rect = createRectToTop(block);
                    QList<Block*> collides = getCollidingItems(block,rect);
                    if(qAbs(distance)>=height and !collides.size()){
                        y -= height;
                        y = qMax(30,y);
                        m_lastMousePressPos = QPointF(m_lastMousePressPos.x(),mapToScene(event->pos()).y());
                    }
                }else if(delta.y()>0){//dragging down
                    QRectF rect = createRectToBottom(block);
                    QList<Block*> collides = getCollidingItems(block,rect);
                    if(qAbs(distance)>=height and !collides.size()){
                       y += height;
                        m_lastMousePressPos = QPointF(m_lastMousePressPos.x(),mapToScene(event->pos()).y());
                    }
                }
            }

            block->setX(x);
            block->setY(y);
            block->setPos(x,y);

        }
    }else if(m_trackMoveMode == TRACK_SCALE_MODE){
        qreal width = m_lastPressedBlock->sceneBoundingRect().width()+currentMousePosition.x()
                      -m_lastMouseMovePos.x();

        m_lastPressedBlock->setRect(0,
                                    0,
                                    width,
                                    m_lastPressedBlock->sceneBoundingRect().height());
        m_lastPressedBlock->setWidth(width);
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
       emit viewUpdated();
   }

   return QGraphicsView::keyPressEvent(event);
}

void CustomGraphicsView::resizeEvent(QResizeEvent *event)
{
    //this->scene()->setSceneRect(0,0,this->width(),this->height());
    //this->scene()->setSceneRect(0,0,2000,2000);
    QList<Block*> blocks = getAllBlocks();
    qreal width = this->parentWidget()->width();
    qreal height = this->parentWidget()->height();
    for(auto block:blocks){
        if(block->sceneBoundingRect().right()>width){
            width = block->sceneBoundingRect().right();
        }
        if(block->sceneBoundingRect().bottom()>height){
            height =  block->sceneBoundingRect().bottom();
        }
    }

    this->setSceneRect(0,0,width+100,height+50);
    QGraphicsView::resizeEvent(event);
}

void CustomGraphicsView::showEvent(QShowEvent *event)
{


    QGraphicsLineItem *line = new QGraphicsLineItem();
    line->setPen(QColor(128,128,128,30));
    line->setLine(30,0,30,1000);
    scene()->addItem(line);


    for(int i=1;i<100;i++){
        GraphicsEye *eye = new GraphicsEye();
        eye->setPos(0,i*30);
        eye->setZValue(0);
        scene()->addItem(eye);

        QGraphicsLineItem *hline1 = new QGraphicsLineItem();
        hline1->setLine(0,30*i,10000,30*i);
        hline1->setPen(QColor(128,128,128,30));
        hline1->setZValue(1);
        scene()->addItem(hline1);
    }

    for(int i=0;i<1000;++i){
        QGraphicsLineItem *scale = new QGraphicsLineItem();
        if(i*5%50==0)
            scale->setLine(30+i*5,30,30+i*5,10);
        else
            scale->setLine(30+i*5,30,30+i*5,20);
        scale->setPen(QColor(128,128,128,50));
        scene()->addItem(scale);
    }

    m_seek = new GraphicsSeek();
    m_seek->setZValue(2);
    m_seek->setPos(25,15);
    scene()->addItem(m_seek);


    QGraphicsView::showEvent(event);
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

QList<Block *> CustomGraphicsView::getAllBlocks(){
    QList<QGraphicsItem *> items = scene()->items();
    QList<Block*> blocks;

    for(auto item:items){
        Block* block = dynamic_cast<Block*>(item);
        if(block){
            blocks.append(block);
        }
    }
    return blocks;
}
