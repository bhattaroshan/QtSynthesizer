#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include<QtGui>
#include<QGraphicsView>
#include<QApplication>
#include <QMimeData>
#include "block.h"
#include "graphicseye.h"
#include "graphicsseek.h"

#define TRACK_IDLE_MODE  0
#define TRACK_MOVE_MODE  1
#define TRACK_SCALE_MODE 2

class CustomGraphicsView:public QGraphicsView
{
    Q_OBJECT
public:
    CustomGraphicsView(QWidget *parent=nullptr);
    GraphicsSeek *getSeekBar(){return m_seek;}
    qreal getSeekBarCenterPos();

signals:
    void trackClicked();
    void offTrackClicked();

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

    QList<Block *> getSelectedBlocks();
    QList<Block *> getAllBlocks();

    QPointF m_lastMouseMovePos;
    QPointF m_lastMousePressPos;
    int m_trackMoveMode = 0;
    Block *m_lastPressedBlock = nullptr;
    GraphicsSeek *m_seek = nullptr;

    qreal m_timelineHeight = 30;
    qreal m_seekBarHeight = 15;
    qreal m_seekBarStartPos = 25;

    bool m_isLeftButtonClicked = false;

signals:
    void onMousePress();
    void viewUpdated();
    void addTrack(SignalProperties);
};

#endif // CUSTOMGRAPHICSVIEW_H
