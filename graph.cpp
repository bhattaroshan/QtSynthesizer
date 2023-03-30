#include "graph.h"
#include <QValueAxis>

Graph::Graph(QVector<QPair<qreal, qreal> > coordinates, QWidget *parent)
    :QChartView(parent)
{
    QLineSeries *series = new QLineSeries();
    int length = coordinates.size();

    for(int i=0;i<length;++i){
        series->append(coordinates[i].first,coordinates[i].second);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->legend()->hide();
    chart->setBackgroundBrush(QBrush(QColor(25,25,25)));

    setChart(chart);
    this->setMouseTracking(true);
}

Graph::~Graph(){
    //delete chartView;
}

void Graph::zoomInX() {
    // get current x-axis range
    QValueAxis *xAxis = qobject_cast<QValueAxis *>(chart()->axes(Qt::Horizontal).at(0));
    qreal xMin = xAxis->min();
    qreal xMax = xAxis->max();
    // zoom in on chart
    chart()->zoomIn();
    // set y-axis range to original range
    QValueAxis *yAxis = qobject_cast<QValueAxis *>(chart()->axes(Qt::Vertical).at(0));
    yAxis->setRange(yAxis->min(), yAxis->max());
    // set new x-axis range
    xAxis->setRange(xMin, xMax);
}

void Graph::wheelEvent(QWheelEvent *event)
{
    chart()->zoomReset();

    mFactor *= event->angleDelta().y() > 0 ? 0.5 : 2;

    QRectF rect = chart()->plotArea();
    QPointF c = chart()->plotArea().center();
    rect.setWidth(mFactor*rect.width());
    rect.moveCenter(c);
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
       chart()->scroll(-dPos.x(), dPos.y());

       m_lastMousePos = event->pos();
       event->accept();
   }

    QChartView::mouseMoveEvent(event);
}
