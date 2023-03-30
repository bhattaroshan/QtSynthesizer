#include "graph.h"

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
}

Graph::~Graph(){
    //delete chartView;
}

