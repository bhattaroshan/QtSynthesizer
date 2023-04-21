#include "graph.h"
#include <QValueAxis>

Graph::Graph(QVector<QPointF> coordinates, QWidget *parent)
    :QChartView(parent)
{
    setRenderHint(QPainter::Antialiasing);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    m_series = new QLineSeries();

    m_chart = new QChart();
    m_chart->addSeries(m_series);
    m_chart->legend()->hide();
    m_chart->setBackgroundBrush(QBrush(QColor(25,25,25)));

    QValueAxis *xAxis = new QValueAxis;
    xAxis->setRange(0,44100);
    xAxis->setGridLineVisible(false);
    //xAxis->setLabelsVisible(false);
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

    chart()->zoomReset();

    mFactor *= 0.05;

    QRectF rect = chart()->plotArea();
    rect.setWidth(mFactor*rect.width());
    chart()->zoomIn(rect);

    setMouseTracking(false);
}

Graph::~Graph(){
}

void Graph::update(QVector<QPointF> coordinates)
{

    m_series->clear();
    m_series->blockSignals(true);
    m_series->append(coordinates.mid(0,coordinates.size()-1));
    m_series->blockSignals(false);
    if(coordinates.size()-1>0){
        m_series->append(coordinates[coordinates.size()-1]);
    }

 }

void Graph::wheelEvent(QWheelEvent *event)
{
    chart()->zoomReset();

    mFactor *= event->angleDelta().y() > 0 ? 0.5 : 2;

    QRectF rect = chart()->plotArea();
    rect.setWidth(mFactor*rect.width());
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
    QChartView::mousePressEvent(event);
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

