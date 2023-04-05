#ifndef CUSTOMGRAPHICSSCENE_H
#define CUSTOMGRAPHICSSCENE_H

#include<QGraphicsScene>
#include<QtGui>

class CustomGraphicsScene:public QGraphicsScene
{
    Q_OBJECT
public:
    CustomGraphicsScene(QObject *parent=nullptr);

protected:
    //void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // CUSTOMGRAPHICSSCENE_H
