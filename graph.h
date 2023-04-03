#ifndef GRAPH_H
#define GRAPH_H

#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>

class Graph:public QChartView
{
    Q_OBJECT
public:
    Graph(QVector<QPointF> coordinates, QWidget *parent=nullptr);
    ~Graph();
    void update(QVector<QPointF> coordinates);

protected:
    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

    qreal mFactor=1.0;
    QPointF m_lastMousePos;
    QLineSeries *m_series;
    QChart *m_chart;
    int m_length = 44100;
};

#endif // GRAPH_H
