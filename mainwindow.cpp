#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "graph.h"
#include <QGraphicsLineItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    signal = new SignalVector();

    scene = new CustomGraphicsScene();

    graphicsView = new CustomGraphicsView();
    graphicsView->setScene(scene);
    scene->setSceneRect(0,0,this->width(),this->height());

    connect(graphicsView,&CustomGraphicsView::onMousePress,this,&MainWindow::onGraphicsViewMousePressed);

    mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *btn1 = new QPushButton("Add Track");
    connect(btn1,&QPushButton::clicked,this,&MainWindow::onAddTrackClicked);
    QPushButton *btn2 = new QPushButton(">");
    btn2->setFixedSize(30,40);
    connect(btn2,&QPushButton::clicked,this,&MainWindow::onCancelClicked);
    QPushButton *btn3 = new QPushButton("Test");
    buttonsLayout->addWidget(btn1);
    buttonsLayout->addWidget(btn2);
    buttonsLayout->addWidget(btn3);


    m_graph = new Graph(signal->getSignal());

    mainLayout->addWidget(m_graph);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(graphicsView);

    m_dockWidget = new QDockWidget("Properties");
    m_dockWidget->setMinimumWidth(250);
    m_dockWidget->setFeatures(m_dockWidget->features() & ~(QDockWidget::DockWidgetClosable  |
                                                           QDockWidget::DockWidgetFloatable |
                                                           QDockWidget::DockWidgetMovable ));
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea,m_dockWidget);

    window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);
}

void MainWindow::onAddTrackClicked(){
    QList<Block*> allTracks = getAllTracks();
    //get minimum y, get minimum farthest
    qreal minY = -1;
    qreal maxFarthest = -1;

    for(int i=0;i<allTracks.size();++i){
        qreal x = allTracks[i]->x();
        qreal y = allTracks[i]->y();
        qreal farthest = allTracks[i]->boundingRect().width()+x;
        if(maxFarthest==-1 or farthest>maxFarthest){
            maxFarthest = farthest;
            minY = y;
        }
    }

    int blockX=0,blockY=20;
    if(minY>0 and maxFarthest>0){
        blockX = maxFarthest-1;
        blockY = minY;
    }

    Block *b = new Block(blockX,blockY);
    m_blocks.append(b); //keep track of all the tracks for future use
    scene->addItem(b);
    connect(b,&Block::onItemDrag,this,&MainWindow::resizeSlot);
    connect(b,&Block::onItemDoubleClicked,this,&MainWindow::onTrackDoubleClicked);
    connect(b,&Block::trackUpdated,this,&MainWindow::updateGraph);
    connect(b,&Block::onItemSingleClick,this,&MainWindow::onTrackSingleClicked);
    updateGraph();

}

void MainWindow::onCancelClicked(){
}

void MainWindow::onTrackDoubleClicked(int frequency,QColor color)
{
    dialogAssociatedToTrack = sender();
    if(signalDialog!=nullptr){
        delete signalDialog;
    }

    signalDialog = new SignalDialog(frequency,color);
    signalDialog->show();

    connect(signalDialog,&SignalDialog::updateDialogProperties,this,&MainWindow::setTrackProperties);
}

void MainWindow::onTrackSingleClicked()
{
    Block* currentTrack = dynamic_cast<Block*>(sender());
    QList<Block*> tracks = getAllTracks();
    for(auto track:tracks){
        if(track==currentTrack){
            track->setZValue(1);
            track->setOutline(true);

            //add dockWidget settings for track
            QVBoxLayout *mainLayout = new QVBoxLayout();

            QHBoxLayout *timeLayout = new QHBoxLayout();
            QLabel *timeLabel = new QLabel("Time (ms)");
            QSpinBox *timeSpin = new QSpinBox();
            timeSpin->setRange(1,100000);
            timeLayout->addWidget(timeLabel);
            timeLayout->addWidget(timeSpin);

            QHBoxLayout *frequencyLayout = new QHBoxLayout();
            QLabel *frequencyLabel = new QLabel("Frequency (Hz)");
            QSpinBox *frequencySpin = new QSpinBox();
            frequencySpin->setRange(1,20000);
            frequencySpin->setValue(track->getFrequency());
            frequencyLayout->addWidget(frequencyLabel);
            frequencyLayout->addWidget(frequencySpin);

            QHBoxLayout *phaseLayout = new QHBoxLayout();
            QLabel *phaseLabel = new QLabel("Phase");
            QDial *phaseDial = new QDial();
            phaseDial->setFixedSize(42,42);
            phaseDial->setNotchTarget(0);
            phaseLayout->addWidget(phaseLabel);
            phaseLayout->addWidget(phaseDial);

            QHBoxLayout *colorLayout = new QHBoxLayout();
            QLabel *colorLabel = new QLabel("Track Color");
            QPushButton *colorBtn = new QPushButton();
            colorBtn->setIcon(QIcon(":/icons/color-picker.png"));
            colorBtn->setFixedSize(32,35);
            colorBtn->setStyleSheet("QPushButton {"
                                              "border-radius:5px;"
                                              "border: 1px solid #aeaeae;"
                                              "}"
                                              "QPushButton:hover{"
                                              "background: #808080;"
                                              "}"
                                              "QPushButton:pressed{"
                                              "background: #101010;"
                                              "}"
                                              );
            colorLayout->addWidget(colorLabel);
            colorLayout->addWidget(colorBtn);

            QSpacerItem *spacer = new QSpacerItem(0,200,QSizePolicy::Minimum,QSizePolicy::Expanding);

            mainLayout->addLayout(timeLayout);
            mainLayout->addLayout(frequencyLayout);
            mainLayout->addLayout(phaseLayout);
            mainLayout->addLayout(colorLayout);
            mainLayout->addSpacerItem(spacer);

            QWidget *widget = new QWidget();
            widget->setLayout(mainLayout);
            m_dockWidget->setWidget(widget);

        }else{
            track->setZValue(0);
            track->setOutline(false);
        }
    }

}

void MainWindow::onGraphicsViewMousePressed()
{
    QList<Block*> tracks = getAllTracks();
    for(auto track:tracks){
        track->setOutline(false);
    }
    QWidget *widget = new QWidget();
    m_dockWidget->setWidget(widget);
    //disable all other settings and enable general settings
}

QList<Block*> MainWindow::getAllTracks(){
    QList<QGraphicsItem*> items = scene->items();
    QList<Block*> blocks;
    for(auto item:items){
        QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem*>(item);
        if(rectItem){
            blocks.append(dynamic_cast<Block*>(rectItem));
        }
    }
    return blocks;
}


void MainWindow::updateGraph(){
    signal->clear();
    for(auto b:getAllTracks()){
        int trackWidth = b->boundingRect().width();
        auto sig = signal->generateSinWave(b->getFrequency(),trackWidth*10);
        int index = signal->getIndexFromTime(b->x()*10);
        if(index<0) index=0;
        signal->addSignalToContainer(sig,index);
    }
    signal->normalizeSignal();
    m_graph->update(signal->getSignal());
}

void MainWindow::setTrackProperties(int currentFrequency, int lastFrequency, QColor color)
{
    Block *activeTrack = dynamic_cast<Block*>(dialogAssociatedToTrack);

    activeTrack->setFrequency(currentFrequency);
    activeTrack->setColor(color);
    if(currentFrequency!=lastFrequency){
        updateGraph();
    }
}


MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resizeSlot();

    QMainWindow::resizeEvent(event);
}

QPair<qreal,qreal> MainWindow::resizeSlot(){
    qreal w = this->width();
    qreal h = this->height();

    QList<QGraphicsItem*> items = scene->items();
    for(auto item:items){
        qreal new_width = item->x()+item->boundingRect().width();
        qreal new_height = item->y()+item->boundingRect().height();
        if(new_width>w) {
            w=new_width;
        }
        if(new_height>h){
            h = new_height;
        }
    }

    int new_width = w+100;
    int new_height = h+50;
    scene->setSceneRect(0,0,new_width,new_height);

    int barCount =	(new_width/50)-1;
    int graphBarLength = m_graphBar.size();

    if(barCount>graphBarLength){  //new line plotting is needed here
        for(int i=50+(graphBarLength*50);i<new_width;i+=50){
            QRectF points = QRectF(i,20,i,new_height);
            m_graphBar.append(points);
            QGraphicsLineItem *line = new QGraphicsLineItem(points.x(),points.y(),points.width(),points.height());
            line->setZValue(-100);
            line->setPen(QPen(QColor(128,128,128,20)));
            scene->addItem(line);
        }

        //Need to fix this later on
        QRectF barRect = QRectF(0,20,1000,20);
        if(barRect!=m_barRect){
            m_barRect = barRect;
            QGraphicsLineItem *bar = new QGraphicsLineItem(0,20,new_width,20);
            bar->setPen(QPen(QColor(128,128,128,20)));
            bar->setZValue(-100);
            scene->addItem(bar);
        }
    }



    return {w+100,h+50};
}
