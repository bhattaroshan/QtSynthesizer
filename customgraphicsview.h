#ifndef CUSTOMGRAPHICSVIEW_H
#define CUSTOMGRAPHICSVIEW_H

#include<QtGui>
#include<QGraphicsView>

class CustomGraphicsView:public QGraphicsView
{
    Q_OBJECT
public:
    CustomGraphicsView(QWidget *parent=nullptr);
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    QPointF m_lastMousePos;
    QPointF m_lastMousePressPos;
    bool m_leftMousePressed = false;
    bool m_draggingZone = false;
    bool m_allowMovement = false;

signals:
    void onMousePress();
};

#endif // CUSTOMGRAPHICSVIEW_H
