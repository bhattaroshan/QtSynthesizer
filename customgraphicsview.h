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

signals:
    void onMousePress();
};

#endif // CUSTOMGRAPHICSVIEW_H
