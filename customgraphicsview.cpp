#include "customgraphicsview.h"
#include "graphicseye.h"
#include <QMessageBox>


CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{
    setDragMode(QGraphicsView::RubberBandDrag);
    setCursor(Qt::ArrowCursor);
    installEventFilter(this);
    setMouseTracking(false);
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
        QVector<Block*> selectedBlocks = getSelectedBlocks();

        if(selectedBlocks.size()>0){ //dragged and selected the blocks
            //store the order of blocks here
            m_selectedBlocksInOrder = getBlocksInOrder(selectedBlocks);
            //m_selectedBlocksInOrder = getBlocksInOrder(selectedBlocks);
            m_selectedBlocksDistancesInOrder.clear(); //clear previous values
            for(auto layers:m_selectedBlocksInOrder){
                m_selectedBlocksDistancesInOrder.push_back(getBlocksDistance(layers));
            }
            //m_selectedBlocksDistancesInOrder = getBlocksDistance(m_selectedBlocksInOrder);

            //experimentation from this point
            m_blocksTransform.clear(); //clear all previous keys and values
            m_blocksInOrder = getBlocksInOrder(getAllBlocks()); //get the list of blocks

            for(int layers=0;layers<m_blocksInOrder.size();++layers){
                for(auto block:m_blocksInOrder[layers]){
                    //store the transformation of all the blocks when the mouse was released
                    m_blocksTransform[block] = QRect(block->x(),
                                                     block->y(),
                                                     block->sceneBoundingRect().width(),
                                                     block->sceneBoundingRect().height()
                                                    );
                }
            }
            m_leftMostSelectedBlock = getLeftMostSelectedBlock(m_blocksInOrder);
            m_leftMostSelectedBlockRect = getBlockRect(m_leftMostSelectedBlock);

            emit blockClicked();
        }

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
            block->setPos(x,y);
        }
    }else if(m_trackMoveMode == TRACK_SCALE_MODE){
//            QList<Block*> blocks = getSelectedBlocks();
//            m_updateBlockList = blocks;

//            for(int i=0;i<blocks.size();++i){
//                Block *block = blocks[i];
//                //implementation of resizing according
//                qreal deltax = currentMousePosition.x()-m_lastMouseMovePos.x();
//                qreal width = block->sceneBoundingRect().width()+deltax;

//                if(width>=10){
//                    block->setRect(0,0,width,block->sceneBoundingRect().height());
//                }
                m_updateBlockList.clear();
                qreal deltax = currentMousePosition.x()-m_lastMouseMovePos.x();
                //qDebug()<<"-------------------------";
                for(int layers=0;layers<m_blocksInOrder.size();++layers){
                    for(int block=0;block<m_blocksInOrder[layers].size();++block){
                        Block *currentBlock = m_blocksInOrder[layers][block];

                        //handle scaling of the tracks
                        if(currentBlock->isSelected()){
                            m_updateBlockList.append(currentBlock);
                            qreal distance;
                            qreal xPos;

                            if(block==0){
                                distance =	m_blocksTransform[currentBlock].x()-
                                            (
                                             m_blocksTransform[m_leftMostSelectedBlock].x()+
                                             m_blocksTransform[m_leftMostSelectedBlock].width()
                                            );

                                xPos = m_blocksTransform[m_leftMostSelectedBlock].x()+
                                        m_leftMostSelectedBlock->sceneBoundingRect().width()+
                                        distance;
                            }else{
                                Block *previousBlock = m_blocksInOrder[layers][block-1];
                                distance =	m_blocksTransform[currentBlock].x()-
                                            (
                                              m_blocksTransform[previousBlock].x()+
                                              m_blocksTransform[previousBlock].width()
                                            );

                                xPos = previousBlock->x()+
                                       previousBlock->sceneBoundingRect().width()+
                                       distance;
                            }

                            qreal width = currentBlock->sceneBoundingRect().width()+deltax;
                            if(width>=10){
                                currentBlock->setRect(0,0,width,currentBlock->sceneBoundingRect().height());
                            }

                            if(currentBlock!=m_leftMostSelectedBlock){
                                currentBlock->setPos(xPos,currentBlock->y());
                            }
                        }

                    }
                }

//                if(i!=0){
//                    block->setPos(ref-diff,block->y());
//                }

//                for(int i=1;i<m_selectedBlocksInOrder.size();++i){
//                    qreal posX = m_selectedBlocksInOrder[i-1]->x()+
//                                m_selectedBlocksInOrder[i-1]->sceneBoundingRect().width()+
//                                m_selectedBlocksDistancesInOrder[i];
//                    if(posX>=30){
//                        m_selectedBlocksInOrder[i]->setPos(posX,m_selectedBlocksInOrder[i]->y());
//                    }
//                }

//              for(int row=0;row<m_selectedBlocksInOrder.size();++row){
//                  skip re-positioning first block
//                  for(int col=0;col<m_selectedBlocksInOrder[row].size();++col){
//                      qreal newRow = row;
//                      qreal newCol = col-1;

//                      if(row>=1 and col==0){
//                          newRow -= 1;
//                          newCol = 0;
//                      }

//                      if(newCol==-1) continue;
//                      qreal posX = 	m_selectedBlocksInOrder[newRow][newCol]->x()+
//                                    m_selectedBlocksInOrder[newRow][newCol]->sceneBoundingRect().width()+
//                                    (m_selectedBlocksDistancesInOrder[row][col].x()-
//                                    m_selectedBlocksDistancesInOrder[newRow][newCol].y());
//                      qreal posY = m_selectedBlocksInOrder[row][col]->y();
//                      if(posX>=30){
//                            //getSelectedBlocksAtRegion(QPointF(posX,m_selectedBlocksInOrder[row][col]->y())).size()==0){
//                          if(m_selectedBlocksInOrder[row][col]->y()>
//                             m_selectedBlocksInOrder[row][col]->sceneBoundingRect().x()+
//                                  m_selectedBlocksInOrder[row][col]->sceneBoundingRect().width()){
//                             qDebug()<<"squeezing the bar";
//                          }else{
//                             qDebug()<<"expanding the bar";
//                          }
//                          //getSelectedBlocksAtRegion(QPointF(posX,posY)).size()==0
//                            m_selectedBlocksInOrder[row][col]->setPos(posX,posY);

//                      }
//                    }
//                }
//            }
    }

    m_lastMouseMovePos = mapToScene(event->pos());

    return QGraphicsView::mouseMoveEvent(event);
}

//QVector<qreal> CustomGraphicsView::getBlocksDistance(QVector<Block*> blocks){
//    QVector<qreal> distance;
//    distance.push_back(0); //first block has nothing to compare with

//    for(int i=1;i<blocks.size();++i){
//        distance.push_back(blocks[i]->x()-blocks[i-1]->x()-blocks[i-1]->sceneBoundingRect().width());
//    }
//    return distance;
//}

//QVector<Block*> CustomGraphicsView::getBlocksInOrder(QVector<Block*> blocks){

//    //adding all blocks according to layers
//    QVector<Block*> b = blocks;
//    std::sort(b.begin(),b.end(),[](Block *b1,Block *b2){
//      return b1->x()<b2->x();
//    });

//    return b;
//}

QVector<QPoint> CustomGraphicsView::getBlocksDistance(QVector<Block*> blocks){
    //QVector<qreal> distance;
    QVector<QPoint> distance;

    //distance.push_back(QPoint(0,0)); //first block has nothing to compare with

    for(int i=0;i<blocks.size();++i){
        //distance.push_back(blocks[i]->x()-blocks[i-1]->x()-blocks[i-1]->sceneBoundingRect().width());
        distance.push_back(QPoint(blocks[i]->x(),blocks[i]->x()+blocks[i]->sceneBoundingRect().width()));
    }
    return distance;
}

QRect CustomGraphicsView::getBlockRect(Block *block){
    return QRect(
                    block->x(),
                    block->y(),
                    block->sceneBoundingRect().width(),
                    block->sceneBoundingRect().height()
                );
}

Block* CustomGraphicsView::getLeftMostSelectedBlock(QVector<QVector<Block*>> blocks){
    Block *block = nullptr;
    int res = std::numeric_limits<int>::max();

    for(auto layer:blocks){
        qreal x = layer[0]->x();
//        qreal y = layer[0]->y();
//        qreal width = layer[0]->sceneBoundingRect().width();
//        qreal height = layer[0]->sceneBoundingRect().height();
        if(x<res){
            res = x;
            block = layer[0];
        }
    }

    return block;
}

QVector<QVector<Block*>> CustomGraphicsView::getBlocksInOrder(QVector<Block*> blocks){
    QMap<qreal,QVector<Block*>> res;
    QVector<QVector<Block*>> fin;

    //adding all blocks according to layers
    for(auto block:blocks){
        res[block->y()].push_back(block);
    }

    //sorting all blocks according to their order in x axis
    for(auto it=res.begin();it!=res.end();++it){
        QVector<Block*> bList = it.value();
        std::sort(bList.begin(),bList.end(),[](Block *b1,Block *b2){
            return b1->x()<b2->x();
        });

        //re-equate value according x-axis sort
        //res[it.key()]=bList;
        fin.push_back(bList);

    }

    return fin;
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
                resizeScrollBar();
            }
        }
    }
    return QGraphicsView::eventFilter(watched,event);
}

void CustomGraphicsView::resizeScrollBar(){
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

void CustomGraphicsView::resizeEvent(QResizeEvent *event)
{
    //this->scene()->setSceneRect(0,0,this->width(),this->height());
    //this->scene()->setSceneRect(0,0,2000,2000);
    resizeScrollBar();
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

QList<Block*> CustomGraphicsView::getSelectedBlocksAtRegion(QPointF point){
    QList<QGraphicsItem *> items = scene()->items(point);
    QList<Block*> blocks;

    for(auto item:items){
        Block* block = dynamic_cast<Block*>(item);
        if(block){
            blocks.append(block);
        }
    }
    return blocks;

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



