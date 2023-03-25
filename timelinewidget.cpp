#include "timelinewidget.h"

TimelineWidget::TimelineWidget(int x, int y, int width, int height, QWidget *parent)
    :QWidget(parent)
{
    setMouseTracking(true);

    m_parent = parent;
    m_scene = new QGraphicsScene(0,0,width,height,this);
    m_graphicsView = new QGraphicsView(m_scene,this);
    m_graphicsView->setMouseTracking(true);
    //m_graphicsView->setScene(m_scene);
    m_graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //connect(m_graphicsView,SIGNAL(),this,SLOT(mouseMove()));
}

void TimelineWidget::addTrack()
{
    int len = m_track.size();
    int x = 0;
    int y = 0;
    if(len>0)
        x = m_track.last()->rect().x()+m_track.last()->rect().width();

    Track *t = new Track(x,2);
    m_track.push_back(t);
    m_scene->addItem(m_track.last());
    //t->setBrush(QColor(255,0,0));
}

void TimelineWidget::onWindowSizeChange()
{
    m_graphicsView->resize(m_parent->width(),m_parent->height());
}

void TimelineWidget::mouseMoveEvent(QMouseEvent *event)
{
    qDebug()<<"I am always called here";
}


