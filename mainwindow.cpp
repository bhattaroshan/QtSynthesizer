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
#include <QDoubleSpinBox>
#include <QJsonDocument>
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    signal = new SignalProcess();

    scene = new CustomGraphicsScene();

    graphicsView = new CustomGraphicsView();
    connect(graphicsView,&CustomGraphicsView::trackClicked,this,&MainWindow::createTrackWidget);
    connect(graphicsView,&CustomGraphicsView::offTrackClicked,this,&MainWindow::createGeneralWidget);
    connect(graphicsView,&CustomGraphicsView::viewUpdated,this,&MainWindow::updateGraph);


    graphicsView->setScene(scene);
    scene->setSceneRect(0,0,this->width(),this->height());

    mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *btn2 = createIconButton(":/icons/play.png");
    buttonsLayout->addWidget(btn2);

    connect(btn2,&QPushButton::clicked,this,&MainWindow::playSignal);


    m_graph = new Graph(signal->getSignal());

    mainLayout->addWidget(m_graph);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(graphicsView);

    createDockView();
    loadMenuBar();

    window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);
    createGeneralWidget();
}

void MainWindow::createDockView(){
    m_dockWidget = new QDockWidget("Properties");
    m_dockWidget->setMinimumWidth(250);
    m_dockWidget->setFeatures(m_dockWidget->features() & ~(QDockWidget::DockWidgetClosable  |
                                                           QDockWidget::DockWidgetFloatable |
                                                           QDockWidget::DockWidgetMovable ));
    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea,m_dockWidget);
}

void MainWindow::playSignal(){
    QAudioFormat format;
    format.setChannelCount(1);
    format.setSampleRate(44100);
    format.setSampleFormat(QAudioFormat::Float);
    if(m_audio!=nullptr) delete m_audio;
    m_audio = new QAudioSink(format);

    if(m_buffer!=nullptr) delete m_buffer;
    m_buffer = new QBuffer();

    QVector<float> soundData;
    QVector<QPointF> signalData = signal->getSignal();

    for(int i=0;i<signalData.size();++i){
        soundData.append(signalData[i].y());
    }

    m_buffer->setData(reinterpret_cast<const char*>(soundData.constData()),soundData.size()*sizeof(float));
    m_buffer->open(QIODevice::ReadOnly);
    m_audio->start(m_buffer);
}

QColor MainWindow::setBrushFromFrequency(int frequency)
{
    if(!m_colorFrequencyMap.contains(frequency)){
        QRandomGenerator red = QRandomGenerator::securelySeeded();
        int redInt = red.bounded(50,201);
        QRandomGenerator green = QRandomGenerator::securelySeeded();
        int greenInt = green.bounded(50,201);
        m_colorFrequencyMap[frequency] = QColor(redInt,greenInt,50);
    }

    return m_colorFrequencyMap[frequency];
}


void MainWindow::loadMenuBar()
{
    QMenuBar *menuBar = new QMenuBar();

    QMenu *fileMenu = menuBar->addMenu("File");
    fileMenu->setFixedSize(400,100);

    QAction *newAction = new QAction("&New\tCtrl+N");
    QAction *openAction = new QAction("&Open\tCtrl+O");
    QAction *saveAction = new QAction("&Save\tCtrl+S");
    QAction *saveAsAction = new QAction("S&ave As\tCtrl+A");
    QAction *exitAction = new QAction("&Close\tCtrl+C");

    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    connect(openAction,&QAction::triggered,this,&MainWindow::onMenuAction_Open);
    connect(saveAction,&QAction::triggered,this,&MainWindow::onMenuAction_Save);

}

void MainWindow::onMenuAction_Open(){
    QFile file("test.json");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"couldn't load the file";
        return;
    }
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    for(auto o:obj){
        qDebug()<<o;
    }
}

void MainWindow::onMenuAction_Save()
{
    QList<Block*> tracks = getAllTracks();

    QJsonObject jsonObject;
    QJsonObject detailsObject;
    QJsonArray arrayObject;

    QJsonObject track1;
    QColor color(128,128,128);
    track1.insert("x",0);
    track1.insert("color",color.name());
    track1.insert("width",500);

    QJsonObject track2;
    QColor color2(255,0,0);
    track2.insert("x",45);
    track2.insert("color",color.name());
    track2.insert("width",1500);

    arrayObject.append(track1);
    arrayObject.append(track2);


    jsonObject.insert("props",arrayObject);

    QJsonDocument jsonDoc(jsonObject);
    QFile file("test.json");
    if(!file.open(QIODevice::WriteOnly)){
        qDebug()<<"error writing file";
        return;
    }
    file.write(jsonDoc.toJson());
    file.close();
}

void MainWindow::onAddTrackClicked(){
    addTrack(220);
}

void MainWindow::addTrack(int frequency)
{
    QList<Block*> allTracks = getAllTracks();
    qDebug()<<allTracks.size();
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
        blockX = maxFarthest;
        blockY = minY;
    }

    Block *b = new Block(blockX,blockY,frequency);
    b->setColor(setBrushFromFrequency(frequency));
    scene->addItem(b);
    connect(b,&Block::onItemSingleClick,this,&MainWindow::onTrackSingleClicked);
    connect(b,&Block::trackUpdated,this,&MainWindow::updateGraph);
    updateGraph();

}


void MainWindow::onTrackSingleClicked()
{
    createTrackWidget();
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
        SignalProperties sp;
        sp = b->getBlockProperties();
        sp.samples = trackWidth*441;
        auto sig = signal->generateSinWave(sp);
        signal->addADSREnvelope(sig,10,10,10);
        signal->addSignalToContainer(sig,b->x()*441);
    }
    signal->normalizeSignal();
    m_graph->update(signal->getSignal());
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
//    qreal w = this->width();
//    qreal h = this->height();

//    if(m_transformXSpin) {
//        m_transformXSpin->setValue(m_lastClickedTrack->x());
//    }

//    if(m_timeSpin){
//        m_timeSpin->setValue(m_lastClickedTrack->boundingRect().width()*10);
//    }

//    QList<QGraphicsItem*> items = scene->items();
//    for(auto item:items){
//        qreal new_width = item->x()+item->boundingRect().width();
//        qreal new_height = item->y()+item->boundingRect().height();
//        if(new_width>w) {
//            w=new_width;
//        }
//        if(new_height>h){
//            h = new_height;
//        }
//    }

//    int new_width = w+100;
//    int new_height = h+50;
//    scene->setSceneRect(0,0,new_width,new_height);

//    int barCount =	(new_width/50)-1;
//    int graphBarLength = m_graphBar.size();

//    if(barCount>graphBarLength){  //new line plotting is needed here
//        for(int i=50+(graphBarLength*50);i<new_width;i+=50){
//            QRectF points = QRectF(i,20,i,new_height);
//            m_graphBar.append(points);
//            QGraphicsLineItem *line = new QGraphicsLineItem(points.x(),points.y(),points.width(),points.height());
//            line->setZValue(-100);
//            line->setPen(QPen(QColor(128,128,128,20)));
//            scene->addItem(line);
//        }

//        //Need to fix this later on
//        QRectF barRect = QRectF(0,20,1000,20);
//        if(barRect!=m_barRect){
//            m_barRect = barRect;
//            QGraphicsLineItem *bar = new QGraphicsLineItem(0,20,new_width,20);
//            bar->setPen(QPen(QColor(128,128,128,20)));
//            bar->setZValue(-100);
//            scene->addItem(bar);
//        }
//    }

//    return {w+100,h+50};
    return {0,0};
}
