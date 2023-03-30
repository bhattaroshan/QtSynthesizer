#ifndef GRAPH_H
#define GRAPH_H

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

class Graph:public QChartView
{
    Q_OBJECT
public:
    Graph(QVector<QPair<qreal,qreal>> coordinates, QWidget *parent=nullptr);
    void zoomInX();
    ~Graph();

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    qreal mFactor=1.0;
    QPointF m_lastMousePos;
};

#endif // GRAPH_H
