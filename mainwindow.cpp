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
#include <QAudioBuffer>

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
    QPushButton *playButton = createIconButton(":/icons/play.png");
    QPushButton *graphVisibleButton = createIconButton(":/icons/eye-visible.png");
    buttonsLayout->addWidget(playButton);
    buttonsLayout->addWidget(graphVisibleButton);


    QHBoxLayout *mainButtonLayout = new QHBoxLayout();
    mainButtonLayout->addLayout(buttonsLayout);

    connect(playButton,&QPushButton::clicked,this,&MainWindow::playSignal);


    m_graph = new Graph(signal->getSignal());

    mainLayout->addWidget(m_graph);
    mainLayout->addLayout(mainButtonLayout);
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

    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,this,[=](){
        qreal totalTime = signalData.size()/44.1;
        qreal remBytes = m_audio->processedUSecs()/1000;
        qreal realWidth = (signalData.size()/44100.0)*100;
        qreal currentWidth = (remBytes/1000)*100;
        qreal x_pos = 30+currentWidth;
        graphicsView->getSeekBar()->setPos(x_pos,15);
        if(m_lastProcessed!=(remBytes*100)/totalTime){
            m_lastProcessed = (remBytes*100)/totalTime;

            qDebug()<<"i am called";
            if(m_lastProcessed >= 100){
                timer->stop();
                delete timer;
                qDebug()<<"this should be my last time";
            }
        }
    });
    timer->start(10);

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

    QList<Block*> tracks = getAllTracks();
    for(auto track:tracks){
        scene->removeItem(track);
    }


    QFile file("test.json");
    if(!file.open(QIODevice::ReadOnly)){
        qDebug()<<"couldn't load the file";
        return;
    }
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();
    QJsonArray array = obj.value("props").toArray();

    for(auto f:array){
        qreal amplitude = f.toObject().value("amplitude").toDouble();
        qreal frequency = f.toObject().value("frequency").toDouble();
        QColor color = QColor(f.toObject().value("color").toString());
        qreal x = f.toObject().value("x").toDouble();
        qreal y = f.toObject().value("y").toDouble();
        qreal width = f.toObject().value("width").toDouble();
        qreal harmonics = f.toObject().value("harmonics").toDouble();
        qreal attack = f.toObject().value("attack").toDouble();
        qreal decay = f.toObject().value("decay").toDouble();
        qreal release = f.toObject().value("release").toDouble();

        SignalProperties sp;
        sp.amplitude = amplitude;
        sp.frequency = frequency;
        sp.color = color;
        sp.x = x;
        sp.y = y;
        sp.width = width;
        sp.harmonics = harmonics;
        sp.attack = attack;
        sp.decay = decay;
        sp.release = release;


        Block *b = new Block(sp);
        scene->addItem(b);
        connect(b,&Block::onItemSingleClick,this,&MainWindow::onTrackSingleClicked);
        connect(b,&Block::trackUpdated,this,&MainWindow::updateGraph);
    }
    updateGraph();

}

void MainWindow::onMenuAction_Save()
{
    QList<Block*> tracks = getAllTracks();
    QJsonArray arrayObject;

    for(auto track:tracks){
        SignalProperties sp = track->getBlockProperties();
        qDebug()<<"amplitude : "<<sp.amplitude;
        qDebug()<<"frequency : "<<sp.frequency;
        qDebug()<<"phase : "<<sp.phase;
        qDebug()<<"color : "<<sp.color;
        qDebug()<<"x : "<<sp.x;
        qDebug()<<"width : "<<sp.width;

        QJsonObject trackInfo;
        trackInfo.insert("amplitude",sp.amplitude);
        trackInfo.insert("frequency",sp.frequency);
        trackInfo.insert("color",sp.color.name());
        trackInfo.insert("x",sp.x);
        trackInfo.insert("y",sp.y);
        trackInfo.insert("width",sp.width);
        trackInfo.insert("harmonics",sp.harmonics);
        trackInfo.insert("attack",sp.attack);
        trackInfo.insert("decay",sp.decay);
        trackInfo.insert("release",sp.release);

        arrayObject.append(trackInfo);
    }

    QJsonObject mainObject;
    mainObject.insert("props",arrayObject);

    QJsonDocument jsonDoc(mainObject);
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

    int blockX=30,blockY=30;
    if(minY>0 and maxFarthest>0){
        blockX = maxFarthest;
        blockY = minY;
    }

    SignalProperties sp;
    sp.x = blockX;
    sp.y = blockY;
    sp.frequency = frequency;
    sp.attack = 1;
    sp.decay = 1;
    sp.release = 1;

    Block *b = new Block(sp);
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
        sp.width = trackWidth;
        auto sig = signal->generateSinWave(sp);
        signal->addADSREnvelope(sig,sp.attack,sp.decay,sp.release);
        signal->addSignalToContainer(sig,sp.x*441-30*441);
    }
    signal->normalizeSignal();
    m_graph->update(signal->getSignal());
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent *event)
{

    //scene->setSceneRect(0,0,this->width(),this->height());

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
