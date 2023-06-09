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
#include <QDoubleSpinBox>

#include <QAudio>
#include <QAudioSink>
#include <QBuffer>

#include "block.h"
#include "signalprocess.h"
#include "graph.h"
#include "customgraphicsscene.h"
#include "customgraphicsview.h"
#include "section.h"
#include "effectsdialog.h"


class DelayEffectUI:public QHBoxLayout{
    Q_OBJECT
public:
    DelayEffectUI(){
        label = new QLabel("Delay (ms)");
        spinBox = new QSpinBox();
        spinBox->setRange(1,1000000);
        spinBox->setFocusPolicy(Qt::StrongFocus);
        spinBox->setObjectName("delayValue");
        connect(spinBox,&QSpinBox::editingFinished,this,[=](){
            emit value(spinBox->value());
        });

        addWidget(label);
        addWidget(spinBox);
    }
    int getValue(){
        return spinBox->value();
    }

signals:
    void value(int v);
private:
    QLabel *label;
    QSpinBox *spinBox;
};

struct Effects{
    int effectsIndex;
    Section *section;
};

struct BlockAttributes{
    QVector<QPointF> signal;
    QVector<Effects*> effects;
};


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void onAddTrackClicked();
    void addTrack(QVector<SignalProperties> sp);
    void onTrackSingleClicked();
    void updateSignal(QVector<Block*> blocks);
    void deleteSignal(QVector<Block*> blocks);
    void combineSignals();

    void showEffectsDialog();

    //audio functions
    void playSignal();


    //menu functions
    void loadMenuBar();
    void onMenuAction_Save();
    void onMenuAction_Open();

    //ui functions
    void createTrackWidget();
    void createGeneralWidget();
    QPushButton *createIconButton(QString icon);


    //new slots
    void triggered_xPositionSpinBox();
    void triggered_yPositionSpinBox();
    void clicked_projectSignalBlockButton();
    void triggered_frequencySpinBox();
    void clicked_signalTypeComboBox(int);
    void triggered_harmonicsSpinBox();
    void triggered_amplitudeSpinBox();
    void triggered_timeSpinBox();
    void triggered_phaseSpinBox();
    void dialogAddClicked(int);

private:
    QVBoxLayout *m_trackMainLayout = nullptr;
    QVBoxLayout *m_generalMainLayout = nullptr;

    CustomGraphicsScene *scene;
    CustomGraphicsView *graphicsView;

    QWidget *window;
    QVBoxLayout *mainLayout;

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
    //QMap<Block*,QVector<QPointF>> m_blockList;
    QMap<Block*, BlockAttributes> m_blockList;

    //PROJECT DOCK WIDGET
    QVBoxLayout *m_projectLayout;
    QDockWidget *m_projectDockWidget;
    QWidget *m_projectWidget;
    QScrollArea *m_projectScrollArea;

    Section *m_projectSignalSection;
    QVBoxLayout *m_projectSignalLayout;
    QPushButton *m_projectSignalBlockButton;
    QPushButton *m_projectMediaBlockButton;

    //BLOCK ATTRIBUTE DOCK WIDGET
    QVBoxLayout *m_blockAttributeLayout;
    QDockWidget *m_blockAttributeDockWidget;
    QWidget *m_blockAttributeWidget;
    QScrollArea *m_blockAttributeScrollArea;

    // TRANSFORM ATTRIBUTE
    QVBoxLayout *m_transformLayout;
    Section *m_transformSection;

    QHBoxLayout *m_xPositionLayout;
    QLabel *m_xPositionLabel;
    QSpinBox *m_xPositionSpinBox;

    QHBoxLayout *m_yPositionLayout;
    QLabel *m_yPositionLabel;
    QSpinBox *m_yPositionSpinBox;

    QHBoxLayout *m_timeLayout;
    QLabel *m_timeLabel;
    QSpinBox *m_timeSpinBox;

    //SIGNAL ATTRIBUTE
    QVBoxLayout *m_signalLayout;
    Section *m_signalSection;

    QHBoxLayout *m_signalTypeLayout;
    QLabel *m_signalTypeLabel;
    QComboBox *m_signalTypeComboBox;

    QHBoxLayout *m_frequencyLayout;
    QLabel *m_frequencyLabel;
    QDoubleSpinBox *m_frequencyDoubleSpinBox;


    QHBoxLayout *m_amplitudeLayout;
    QLabel *m_amplitudeLabel;
    QDoubleSpinBox *m_amplitudeDoubleSpinBox;

    QHBoxLayout *m_phaseLayout;
    QLabel *m_phaseLabel;
    QSpinBox *m_phaseSpinBox;


    QHBoxLayout *m_harmonicsLayout;
    QLabel *m_harmonicsLabel;
    QSpinBox *m_harmonicsSpinBox;

    void initializeUI();
};
#endif // MAINWINDOW_H
