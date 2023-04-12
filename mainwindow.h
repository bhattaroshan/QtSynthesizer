#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <signaldialog.h>

#include <QAudio>
#include <QAudioSink>
#include <QBuffer>

#include "block.h"
#include "signalprocess.h"
#include "graph.h"
#include "customgraphicsscene.h"
#include "customgraphicsview.h"



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

    QColor setBrushFromFrequency(int frequency);
    QMap<int,QColor> m_colorFrequencyMap;

public slots:
    QPair<qreal,qreal> resizeSlot();
    void onAddTrackClicked();
    void addTrack(int frequency);
    void onCancelClicked();
    void onTrackDoubleClicked(int frequency,QColor color);
    void onTrackSingleClicked();
    void setTrackProperties(int,int,QColor);
    void updateGraph();
    QList<Block*> getAllTracks();
    void onGraphicsViewMousePressed();
    void onTrackSelected();

private:
    CustomGraphicsScene *scene;
    CustomGraphicsView *graphicsView;
    //QGraphicsScene *scene;
    //QGraphicsView *graphicsView;

    QWidget *window;
    QVBoxLayout *mainLayout;

    QDockWidget *m_dockWidget;

    QVector<Block*> m_blocks;
    Block *m_lastClickedTrack = nullptr;
    QVector<QRectF> m_graphBar;
    QRectF m_barRect;
    SignalProcess *signal;
    Graph *m_graph;


    QSpinBox *m_transformXSpin = nullptr;
    QSpinBox *m_timeSpin = nullptr;

    QAudioSink *m_audio = nullptr;
    QBuffer *m_buffer = nullptr;

    QList<Block*> m_selectedTracks;
};
#endif // MAINWINDOW_H
