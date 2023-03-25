
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>
#include "track.h"

class TimelineWidget:public QWidget{
public:
    TimelineWidget(int x,int y,int width,int height,QWidget *parent=nullptr);
    void addTrack();
    void onWindowSizeChange();
protected:
    void mouseMoveEvent(QMouseEvent *event) override;
private:
    QVector<Track*> m_track;
    QGraphicsScene *m_scene;
    QGraphicsView *m_graphicsView;
    QWidget *m_parent;
};
