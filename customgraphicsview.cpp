#include "customgraphicsview.h"


CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{
    setDragMode(QGraphicsView::RubberBandDrag);
}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    emit onMousePress();
    QGraphicsView::mousePressEvent(event);
}
