#include "graph.h"
#include <QValueAxis>

Graph::Graph(QVector<QPointF> coordinates, QWidget *parent)
    :QChartView(parent)
{
    setRenderHint(QPainter::Antialiasing);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_series = new QLineSeries();
    int length = coordinates.size();

    if(length>0){
        for(int i=0;i<length;++i){
            m_series->append(coordinates[i]);
        }
    }else{
        for(int i=0;i<44100;++i){
            m_series->append(QPointF(i,i*i));
        }
    }

    m_chart = new QChart();
    m_chart->addSeries(m_series);
    m_chart->legend()->hide();
    m_chart->setBackgroundBrush(QBrush(QColor(25,25,25)));

    setChart(m_chart);
    this->setMouseTracking(true);

//    chart()->zoomReset();

//    mFactor *= 0.05;

//    QRectF rect = chart()->plotArea();
//    rect.setWidth(mFactor*rect.width());
//    chart()->zoomIn(rect);
}

Graph::~Graph(){
    //delete chartView;
}

void Graph::update(QVector<QPointF> coordinates)
{
    //m_series->clear();
    //m_series->remove(0,coordinates.length());
    //qDebug()<<"loading files";
    QValueAxis* yaxis = qobject_cast<QValueAxis*>(chart()->axisY(m_series));
    if (yaxis) {
        yaxis->setRange(-1, 1);
    }

    int length = m_series->count();
    int cs = coordinates.size();
    qDebug()<<"My length here is"<<length;
    qDebug()<<"Coordinate size is"<<coordinates.size();

    if(length>=coordinates.size()){
        m_series->replace(coordinates);
    }else{
        m_series->replace(coordinates.mid(0,length));
        qDebug()<<"I came in here";
        m_series->append(coordinates.mid(length));
    }

//    for(int i=0;i<coordinates.length();++i){
//        m_series->append(coordinates[i].x(),coordinates[i].y());
//        if(i%1000==0)
//            qDebug()<<"progress = "<<i;
//    }
    repaint();
}

void Graph::wheelEvent(QWheelEvent *event)
{
    chart()->zoomReset();

    mFactor *= event->angleDelta().y() > 0 ? 0.5 : 2;

    QRectF rect = chart()->plotArea();
    //QPointF c = chart()->plotArea().center();
    rect.setWidth(mFactor*rect.width());
    //rect.moveCenter(c);
    chart()->zoomIn(rect);

    QChartView::wheelEvent(event);
}

void Graph::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
        {
            setCursor(QCursor(Qt::SizeAllCursor));

            m_lastMousePos = event->pos();
            event->accept();
        }
}

void Graph::mouseMoveEvent(QMouseEvent *event)
{
   if (event->buttons() & Qt::MiddleButton)
   {
       auto dPos = event->pos() - m_lastMousePos;
       chart()->scroll(-dPos.x(), 0);

       m_lastMousePos = event->pos();
       event->accept();
   }

    QChartView::mouseMoveEvent(event);
}
