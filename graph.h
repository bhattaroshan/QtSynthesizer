#ifndef GRAPH_H
#define GRAPH_H

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

class Graph:public QChartView
{
public:
    Graph(QVector<QPair<qreal,qreal>> coordinates, QWidget *parent=nullptr);
    ~Graph();

protected:
};

#endif // GRAPH_H
