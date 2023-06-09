#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include<QtGui>
#include<QGraphicsView>
#include<QApplication>
#include <QMimeData>
#include <QToolTip>
#include <QLabel>
#include "block.h"
#include "graphicsseek.h"

#define TRACK_IDLE_MODE      0
#define TRACK_MOVE_MODE      1
#define TRACK_SCALE_MODE     2
#define TRACK_TRANSLATE_MODE 3

class CustomGraphicsView:public QGraphicsView
{
    Q_OBJECT
public:
    CustomGraphicsView(QWidget *parent=nullptr);
    GraphicsSeek *getSeekBar(){return m_seek;}
    qreal getSeekBarCenterPos();
    void addItem(Block*);
    QList<Block *> getSelectedBlocks();
    QList<Block *> getAllBlocks();
    QList<Block*> getSelectedBlocksAtRegion(QPointF point);
    void resizeScrollBar();
    QVector<QVector<Block*>> getBlocksInOrder(QVector<Block*> blocks);
    QVector<QPoint> getBlocksDistance(QVector<Block*> blocks);
    Block* getLeftMostSelectedBlock(QVector<QVector<Block*>> blocks);
    QRect getBlockRect(Block *block);
    void setToolTip(QPoint point);

signals:
    void blockClicked();
    void offBlockClicked();

private:
    QRectF createRectToRight(Block *block, int width=1);
    QRectF createRectToLeft(Block *block, int width=1);
    QRectF createRectToTop(Block *block, int width=1);
    QRectF createRectToBottom(Block *block, int width=1);
    QList<Block*> getCollidingItems(Block *block, QRectF rect);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    bool eventFilter(QObject *watched,QEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void showEvent(QShowEvent *event) override;


    QPointF m_lastMouseMovePos;
    QPointF m_lastMousePressPos;
    int m_trackMoveMode = 0;
    Block *m_lastPressedBlock = nullptr;
    GraphicsSeek *m_seek = nullptr;

    qreal m_timelineHeight = 30;
    qreal m_seekBarHeight = 15;
    qreal m_seekBarStartPos = 25;

    bool m_isLeftButtonClicked = false;
    bool m_blockUpdateNecessary = false;

    QVector<Block*> m_updateBlockList;

    QVector<QVector<Block*>> m_blocksInOrder;
    QVector<QVector<QRect>> m_blocksTransformInOrder;
    QMap<Block*,QRect> m_blocksTransform;
    Block *m_leftMostSelectedBlock;
    QRect m_leftMostSelectedBlockRect;
    qreal answer = 0;
    QLabel *m_seekToolTip;
    //QString m_seekBarToolTip = "Hello I'm here";


    QVector<Block*> m_selectedBlocks;
    QMap<Block*,qreal> m_getBlockDistance;




signals:
    void onMousePress();
    void viewUpdated();
    void addTrack(QVector<SignalProperties>);
    void blockUpdated(QVector<Block*> blocks);
    void blockDeleted(QVector<Block*> blocks);
};

#endif // CUSTOMGRAPHICSVIEW_H
