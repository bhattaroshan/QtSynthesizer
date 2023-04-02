#include "graph.h"
#include <QValueAxis>

Graph::Graph(QVector<QPointF> coordinates, QWidget *parent)
    :QChartView(parent)
{
    setRenderHint(QPainter::Antialiasing);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_series = new QLineSeries();
    //int length = coordinates.size();
    int length = 441000;

    //m_series->append(coordinates.mid(0,coordinates.size()));
    //QVector<
    //m_series->append()
    for(int i=coordinates.size();i<length;++i){
        m_series->append(QPointF(i,0));
    }

    m_chart = new QChart();
    m_chart->addSeries(m_series);
    m_chart->legend()->hide();
    m_chart->setBackgroundBrush(QBrush(QColor(25,25,25)));

    QValueAxis *xAxis = new QValueAxis;
    xAxis->setRange(0,44100);
    xAxis->setGridLineVisible(false);
    xAxis->setLabelsVisible(false);
    xAxis->setLineVisible(false);
    m_chart->addAxis(xAxis,Qt::AlignBottom);
    m_series->attachAxis(xAxis);

    QValueAxis *yAxis = new QValueAxis;
    yAxis->setRange(-1,1);
    yAxis->setGridLineVisible(false);
    yAxis->setLabelsVisible(false);
    yAxis->setLineVisible(false);
    m_chart->addAxis(yAxis,Qt::AlignLeft);
    m_series->attachAxis(yAxis);

    QMargins margins = m_chart->margins();
    margins.setLeft(0);
    margins.setRight(0);

    m_chart->setMargins(margins);
    setChart(m_chart);
    this->setMouseTracking(true);

//    chart()->zoomReset();

    mFactor *= 0.05;

    QRectF rect = chart()->plotArea();
    rect.setWidth(mFactor*rect.width());
    chart()->zoomIn(rect);
}

Graph::~Graph(){
    //delete chartView;
}

void Graph::update(QVector<QPointF> coordinates)
{
    //m_series->clear();
    //m_series->remove(0,coordinates.length());
    //qDebug()<<"loading files";
    //QValueAxis* yaxis = qobject_cast<QValueAxis*>(chart()->axisY(m_series));
    //if (yaxis) {
    //    yaxis->setRange(-1, 1);
    //}
    int length = m_series->count();
    qDebug()<<"my length is "<<length;
    qDebug()<<"coordinates size is = "<<coordinates.size();
    //int cs = coordinates.size();

    qDebug()<<"i started with replace";
    m_series->replace(coordinates.mid(0,length));
    qDebug()<<"I completed replace";

    if(coordinates.size()>length) {
        qDebug()<<"I entered extra space";
        QVector<QPointF> res = coordinates.mid(length);
        m_series->append(res);
        qDebug()<<"This one took me more time";
    }

//    if(length>=coordinates.size()){
//        m_series->replace(coordinates);
//    }else{
//        m_series->replace(coordinates.mid(0,length));
//        m_series->append(coordinates.mid(length));
//    }

//    for(int i=0;i<coordinates.length();++i){
//        m_series->append(coordinates[i].x(),coordinates[i].y());
//        if(i%1000==0)
//            qDebug()<<"progress = "<<i;
//    }
    //repaint();
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
    if (event->button() == Qt::LeftButton)
        {
            setCursor(QCursor(Qt::SizeAllCursor));

            m_lastMousePos = event->pos();
            event->accept();
        }
}

void Graph::mouseMoveEvent(QMouseEvent *event)
{
   if (event->buttons() & Qt::LeftButton)
   {
       auto dPos = event->pos() - m_lastMousePos;
       chart()->scroll(-dPos.x(), 0);

       m_lastMousePos = event->pos();
       event->accept();
   }

   QChartView::mouseMoveEvent(event);
}

