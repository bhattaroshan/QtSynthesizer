#include "customgraphicsview.h"


CustomGraphicsView::CustomGraphicsView(QWidget *parent)
    :QGraphicsView(parent)
{

}

void CustomGraphicsView::mousePressEvent(QMouseEvent *event)
{
    emit onMousePress();
    QGraphicsView::mousePressEvent(event);
}
