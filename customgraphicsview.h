#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include<QtGui>
#include<QGraphicsView>
#include "block.h"

#define TRACK_IDLE_MODE  0
#define TRACK_MOVE_MODE  1
#define TRACK_SCALE_MODE 2

class CustomGraphicsView:public QGraphicsView
{
    Q_OBJECT
public:
    CustomGraphicsView(QWidget *parent=nullptr);
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

    QList<Block *> getSelectedBlocks();

    QPointF m_lastMouseMovePos;
    QPointF m_lastMousePressPos;
    int m_trackMoveMode = 0;
    Block *m_lastPressedBlock = nullptr;

signals:
    void onMousePress();
};

#endif // CUSTOMGRAPHICSVIEW_H
