#include "customgraphicsview.h"
#include "graphicseye.h"
#include <QMessageBox>


CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{
    setDragMode(QGraphicsView::RubberBandDrag);
    setCursor(Qt::ArrowCursor);
    installEventFilter(this);
}

qreal CustomGraphicsView::getSeekBarCenterPos()
{
    return m_seek->x()+5;
}

void CustomGraphicsView::addItem(Block *block)
{
    scene()->addItem(block);
    //emit blockUpdated({block}); //added only one track, but need to pass in vector
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

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton) {
        m_updateBlockList.clear();
        m_isLeftButtonClicked = true;
        m_lastMouseMovePos = mapToScene(event->pos());
        m_lastMousePressPos = mapToScene(event->pos());

        QList<QGraphicsItem*> clickedItems = this->items(event->pos());
        QList<Block*> blocks;
        for(auto item:clickedItems) {
            Block *b = dynamic_cast<Block*>(item);
            if(b){
                blocks.append(b);
            }
        }

        if(blocks.size()>0){ //block is clicked
            Block *block = blocks[0];
            emit blockClicked();
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
            if(m_lastMousePressPos.y()<=m_timelineHeight){
                setDragMode(QGraphicsView::NoDrag);
                qreal newPos = m_lastMousePressPos.x()-5;
                newPos = qMax(newPos,m_seekBarStartPos);
                m_seek->setPos(newPos,m_seekBarHeight);
            }else{ //offtrack clicked
                setDragMode(QGraphicsView::RubberBandDrag);
                emit offBlockClicked();
            }
            m_trackMoveMode = TRACK_IDLE_MODE;
        }

    }

    QGraphicsView::mousePressEvent(event);
}

void CustomGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button()==Qt::LeftButton){
        m_isLeftButtonClicked = false;
        if(m_updateBlockList.size()){
            emit blockUpdated(m_updateBlockList);
            m_updateBlockList.clear();
        }
        if(m_trackMoveMode != TRACK_IDLE_MODE){
            setCursor(Qt::ArrowCursor);
        }
        m_trackMoveMode = TRACK_IDLE_MODE;
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void CustomGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF currentMousePosition = mapToScene(event->pos());


    if(m_isLeftButtonClicked and m_lastMousePressPos.y()<=m_timelineHeight) { //drag the seek according to mouse
        qreal newPos = m_seek->x()+(currentMousePosition.x()-m_lastMouseMovePos.x());
        newPos = qMax(newPos,m_seekBarStartPos);
        m_seek->setPos(newPos,m_seekBarHeight);
    }

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
        m_updateBlockList = blocks;
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
                        y = qMax(int(m_timelineHeight),y);
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
//        m_updateBlockList={m_lastPressedBlock};
//        qreal width = m_lastPressedBlock->sceneBoundingRect().width()+currentMousePosition.x()
//                      -m_lastMouseMovePos.x();

//        m_lastPressedBlock->setRect(0,
//                                    0,
//                                    width,
//                                    m_lastPressedBlock->sceneBoundingRect().height());
//        m_lastPressedBlock->setWidth(width);
          QList<Block*> blocks = getSelectedBlocks();
          for(auto block:blocks){
              qreal width = block->sceneBoundingRect().width()+currentMousePosition.x()
                            -m_lastMouseMovePos.x();
              block->setRect(0,0,width,block->sceneBoundingRect().height());
              block->setWidth(width);
          }
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

       emit blockDeleted(blocks);
       for(auto block:blocks){
           scene()->removeItem(block);
       }
   }

   return QGraphicsView::keyPressEvent(event);
}

bool CustomGraphicsView::eventFilter(QObject *watched, QEvent *event)
{
    if(event->type()==QEvent::ShortcutOverride){
        QKeyEvent *e = static_cast<QKeyEvent*>(event);
        if(e->modifiers()==Qt::ControlModifier and e->key()==Qt::Key_C){
            QList<Block*> selectedBlocks = getSelectedBlocks();
            if(selectedBlocks.size()>0){ //there are items to copy
                QByteArray serializedData;
                QDataStream stream(&serializedData,QIODevice::WriteOnly);
                for(auto item:selectedBlocks){
                    stream<<quintptr(item);
                }
                QMimeData *mimeData = new QMimeData();
                mimeData->setData("application/x-qgraphicsitem",serializedData);
                QApplication::clipboard()->setMimeData(mimeData);
            }
        }else  if(e->modifiers()==Qt::ControlModifier and e->key()==Qt::Key_V){
            const QMimeData* mimeData = QApplication::clipboard()->mimeData();
            if (mimeData->hasFormat("application/x-qgraphicsitem")) {
                QByteArray encodedData = mimeData->data("application/x-qgraphicsitem");
                QDataStream stream(&encodedData, QIODevice::ReadOnly);
                QVector<SignalProperties> spVector;
                while (!stream.atEnd()) {
                    quintptr itemPtr;
                    stream >> itemPtr;
                    Block* item = reinterpret_cast<Block*>(itemPtr);
                    SignalProperties sp = item->getBlockProperties();
                    sp.x += m_seek->x()-m_seekBarStartPos;
                    spVector.append(sp);
                }
                emit addTrack(spVector);
            }
        }
    }
    return QGraphicsView::eventFilter(watched,event);
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
        int x1 = 30+i*5;
        int y1 = 30;
        int x2 = x1;
        int y2 = 10;
        int y3 = 20;

        if(i*5%50==0)
            scale->setLine(x1,y1,x2,y2);
        else
            scale->setLine(x1,y1,x2,y3);
        scale->setPen(QColor(128,128,128,50));
        scene()->addItem(scale);
    }

    m_seek = new GraphicsSeek();
    m_seek->setZValue(2);
    m_seek->setPos(m_seekBarStartPos,m_seekBarHeight);
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
