#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QComboBox>
#include <QDial>
#include <QMessageBox>
#include <QDockWidget>
#include <signaldialog.h>

#include <QAudio>
#include <QAudioSink>
#include <QBuffer>

#include "block.h"
#include "signalprocess.h"
#include "graph.h"
#include "customgraphicsscene.h"
#include "customgraphicsview.h"
#include "section.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    QObject *dialogAssociatedToTrack;

    QColor setBrushFromFrequency(int frequency);
    QMap<int,QColor> m_colorFrequencyMap;

public slots:
    QPair<qreal,qreal> resizeSlot();
    void onAddTrackClicked();
    void addTrack(QVector<SignalProperties> sp);
    void onTrackSingleClicked();
    void updateGraph();
    void updateSignal(QVector<Block*> blocks);
    void deleteSignal(QVector<Block*> blocks);
    void combineSignals();
    QList<Block*> getAllTracks();

    //audio functions
    void playSignal();

    //dock functions
    void createDockView();

    //menu functions
    void loadMenuBar();
    void onMenuAction_Save();
    void onMenuAction_Open();

    //ui functions
    void createTrackWidget();
    void createGeneralWidget();
    QPushButton *createIconButton(QString icon);

private:
    QVBoxLayout *m_trackMainLayout = nullptr;
    QVBoxLayout *m_generalMainLayout = nullptr;

    CustomGraphicsScene *scene;
    CustomGraphicsView *graphicsView;
    //QGraphicsScene *scene;
    //QGraphicsView *graphicsView;

    QWidget *window;
    QVBoxLayout *mainLayout;

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

    qreal m_lastProcessed = 0;
    QPushButton *m_playButton = nullptr;
    qreal m_playingStatus = 0; //zero means stopped, 1 means playing
    qreal m_timelineHeight = 30;

    QVector<QPointF> m_signal;
    QMap<Block*,QVector<QPointF>> m_blockList;

    //DOCK WIDGET
    QVBoxLayout *m_mainBlockLayout;
    QDockWidget *m_dockWidget;
    QWidget *m_mainBlockWidget;
    QScrollArea *m_dockScrollArea;

    // TRANSFORM ATTRIBUTE
    QVBoxLayout *m_transformLayout;
    Section *m_transformSection;

    QHBoxLayout *m_xPositionLayout;
    QLabel *m_xPositionLabel;
    QSpinBox *m_xPositionSpinBox;

    QHBoxLayout *m_yPositionLayout;
    QLabel *m_yPositionLabel;
    QSpinBox *m_yPositionSpinBox;

    QHBoxLayout *m_lengthLayout;
    QLabel *m_lengthLabel;
    QSpinBox *m_lengthSpinBox;

    //SIGNAL ATTRIBUTE
    QVBoxLayout *m_signalLayout;
    Section *m_signalSection;

    QHBoxLayout *m_signalTypeLayout;
    QLabel *m_signalTypeLabel;
    QComboBox *m_signalTypeComboBox;

    QHBoxLayout *m_frequencyLayout;
    QLabel *m_frequencyLabel;
    QSpinBox *m_frequencySpinBox;


    QHBoxLayout *m_amplitudeLayout;
    QLabel *m_amplitudeLabel;
    QDoubleSpinBox *m_amplitudeDoubleSpinBox;

    QHBoxLayout *m_phaseLayout;
    QLabel *m_phaseLabel;
    QSpinBox *m_phaseSpinBox;

    void initializeUI();


};
#endif // MAINWINDOW_H
