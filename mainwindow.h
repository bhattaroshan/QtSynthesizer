#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <signaldialog.h>
#include "block.h"
#include "signalvector.h"
#include "graph.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;
    QObject *dialogAssociatedToTrack;
    SignalDialog *signalDialog = nullptr;

public slots:
    QPair<qreal,qreal> resizeSlot();
    void onOkayClicked();
    void onCancelClicked();
    void onTrackDoubleClicked(int);
    void setTrackFrequency(int currentFrequency, int lastFrequency);
    void updateGraph();
    QList<Block*> getAllTracks();

private:
    QGraphicsScene *scene;
    QGraphicsView *graphicsView;

    QWidget *window;
    QVBoxLayout *mainLayout;

    QVector<Block*> m_blocks;
    QVector<QRectF> m_graphBar;
    SignalVector *signal;
    Graph *m_graph;
};
#endif // MAINWINDOW_H
