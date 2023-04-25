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

    initializeUI(); //create UIs related to transform section

    signal = new SignalProcess();

    scene = new CustomGraphicsScene();

    graphicsView = new CustomGraphicsView();
    connect(graphicsView,&CustomGraphicsView::blockClicked,this,&MainWindow::createTrackWidget);
    connect(graphicsView,&CustomGraphicsView::offBlockClicked,this,&MainWindow::createGeneralWidget);
    connect(graphicsView,&CustomGraphicsView::blockUpdated,this,&MainWindow::updateSignal);
    connect(graphicsView,&CustomGraphicsView::blockDeleted,this,&MainWindow::deleteSignal);
    connect(graphicsView,&CustomGraphicsView::addTrack,this,&MainWindow::addTrack);


    graphicsView->setScene(scene);
    scene->setSceneRect(0,0,this->width(),this->height());

    mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *rewindButton = createIconButton(":/icons/rewind-backward.png");
    m_playButton = createIconButton(":/icons/play.png");
    QPushButton *forwardButton = createIconButton(":/icons/rewind-forward.png");
    QPushButton *graphVisibleButton = createIconButton(":/icons/eye-visible.png");
    buttonsLayout->addWidget(rewindButton);
    buttonsLayout->addWidget(m_playButton);
    buttonsLayout->addWidget(forwardButton);
    buttonsLayout->addWidget(graphVisibleButton);

    QHBoxLayout *mainButtonLayout = new QHBoxLayout();
    mainButtonLayout->addLayout(buttonsLayout);

    connect(m_playButton,&QPushButton::clicked,this,&MainWindow::playSignal);
    connect(rewindButton,&QPushButton::clicked,this,[=](){
        graphicsView->getSeekBar()->setPos(25,15);
    });


    m_graph = new Graph(signal->getSignal());

    mainLayout->addWidget(m_graph);
    mainLayout->addLayout(mainButtonLayout);
    mainLayout->addWidget(graphicsView);

    loadMenuBar();

    window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);
    createGeneralWidget();
}

void MainWindow::clicked_projectSignalBlockButton(){
    onAddTrackClicked();
}

void MainWindow::playSignal(){
    QAudioFormat format;
    format.setChannelCount(1);
    format.setSampleRate(44100);
    format.setSampleFormat(QAudioFormat::Float);
    if(m_audio!=nullptr) delete m_audio;
    m_audio = new QAudioSink(format);

    //qreal currentSeekPosition = graphicsView->getSeekBar()->x();

    if(m_buffer!=nullptr) delete m_buffer;
    m_buffer = new QBuffer();

    QVector<float> soundData;
    QVector<QPointF> signalData = m_signal;

    for(int i=0;i<signalData.size();++i){
        soundData.append(signalData[i].y());
    }

    m_buffer->setData(reinterpret_cast<const char*>(soundData.constData()),soundData.size()*sizeof(float));
    m_buffer->open(QIODevice::ReadOnly);
    m_audio->start(m_buffer);
    if(signalData.size()){
         m_playButton->setIcon(QIcon(":/icons/pause.png"));
        QTimer *timer = new QTimer(this);
        connect(timer,&QTimer::timeout,this,[=](){
        qreal totalTime = signalData.size()/44.1;
        qreal remBytes = m_audio->elapsedUSecs()/1000;
        qreal realWidth = (signalData.size()/44100.0)*100;
        qreal currentWidth = (remBytes/1000)*100;
        qreal x_pos = 30+currentWidth-5;
        graphicsView->getSeekBar()->setPos(x_pos,15);
        if(m_lastProcessed!=(remBytes*100)/totalTime){
            m_lastProcessed = (remBytes*100)/totalTime;

            if(m_lastProcessed >= 100){
                m_playButton->setIcon(QIcon(":/icons/play.png"));
                timer->stop();
                delete timer;
            }
        }
        });
        timer->start(10);
    }
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

    QList<Block*> tracks = graphicsView->getAllBlocks();
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
    QVector<SignalProperties> spList;

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

        spList.append(sp);
    }
    addTrack(spList);

}

void MainWindow::onMenuAction_Save()
{
    QList<Block*> tracks = graphicsView->getAllBlocks();
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
    SignalProperties sp;

    QList<Block*> allTracks = graphicsView->getAllBlocks();
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

    int blockX=30,blockY=m_timelineHeight;
    if(minY>0 and maxFarthest>0){
        blockX = maxFarthest;
        blockY = minY;
    }


    sp.x = blockX;
    sp.y = blockY;
    sp.frequency = 220;
    sp.attack = 1;
    sp.decay = 1;
    sp.release = 1;



    addTrack({sp});
}

void display(SignalProperties sp){

    /*
        struct SignalProperties{
        qreal type				= 0;
        qreal x 				= 0;
        qreal y 				= 0;
        qreal frequency 		= 220.0;
        qreal amplitude 		= 1.0;
        qreal phase 			= 0.0;
        qreal harmonics 		= 0;
        qreal width 			= 100;
        qreal time				= 1000;
        qreal attackPercent 	= 10;
        qreal decayPercent 		= 20;
        qreal releasePercent	= 10;
        QColor color 			= QColor(200,50,50);
        qreal attack			= 1;
        qreal decay				= 1;
        qreal release			= 1;
        qreal sampleRate		= 44100;
        };
    */

    qDebug()<<"-----------------------------------------------";
    qDebug()<<"type = "<<sp.type;
    qDebug()<<"x = "<<sp.x;
    qDebug()<<"y = "<<sp.y;
    qDebug()<<"frequency = "<<sp.frequency;
    qDebug()<<"amplitude = "<<sp.amplitude;
    qDebug()<<"phase = "<<sp.phase;
    qDebug()<<"harmonics = "<<sp.harmonics;
    qDebug()<<"withd = "<<sp.width;
    qDebug()<<"time = "<<sp.time;
    qDebug()<<"attackPercent = "<<sp.attackPercent;
    qDebug()<<"decayPercent = "<<sp.decayPercent;
    qDebug()<<"releasePercent = "<<sp.releasePercent;
    qDebug()<<"color = "<<sp.color;
    qDebug()<<"attack = "<<sp.attack;
    qDebug()<<"decay = "<<sp.decay;
    qDebug()<<"release = "<<sp.release;
    qDebug()<<"sampleRate = "<<sp.sampleRate;
    qDebug()<<"-----------------------------------------------";
}

void MainWindow::addTrack(QVector<SignalProperties> sp)
{

    //optimize here
    QVector<Block*> tempBlock;
    for(int i=0;i<sp.size();++i){
        Block *block = new Block(sp[i]);
        display(sp[i]);
        QVector<QPointF> sig;
        SignalProcess::generateSignal(sig,sp[i]); //this signal is generated for default values
        m_blockList[block] = sig;
        graphicsView->addItem(block);
        connect(block,&Block::clicked,this,&MainWindow::onTrackSingleClicked);
        tempBlock.append(block);
    }
    updateSignal(tempBlock);

//    //bp.signal = signal->generateSinWave(sp);
//    m_blocks.append(bp);

//    Block *b = new Block(sp);
//    b->setSignal(m_blocks.last());
//    b->setColor(setBrushFromFrequency(sp.frequency));
//    graphicsView->addItem(b);
//    connect(b,&Block::clicked,this,&MainWindow::onTrackSingleClicked);
}


void MainWindow::onTrackSingleClicked()
{
    createTrackWidget();
}

void MainWindow::updateSignal(QVector<Block *> blocks)
{
    for(auto block:blocks){
        QVector<QPointF> sig;
        SignalProperties sp = block->getBlockProperties();
        SignalProcess::generateSignal(sig,sp);
        m_blockList[block] = sig;
    }
    combineSignals();

}

void MainWindow::deleteSignal(QVector<Block *> blocks)
{
    //delete all the blocks listed in the QVector
    for(auto block:blocks){
        if(m_blockList.contains(block)){
            m_blockList.remove(block);
        }
    }

    combineSignals();
}

void MainWindow::combineSignals()
{
    m_signal.clear();
    if(m_blockList.size()){
       for(auto it=m_blockList.begin();it!=m_blockList.end();++it){
            Block *block = it.key();
            QVector<QPointF> sig = it.value();
            int startPos = block->getX();
            int startIndex = (startPos-30)*441;
            int signalSize = startIndex+sig.size();
            if(m_signal.size()<=signalSize){
                int signalLength = m_signal.size();
                m_signal.resize(signalSize); //ensure that size is atleast the signal container+start point
                for(int i=signalLength;i<m_signal.size();++i){
                m_signal[i] = QPointF(i,0.0);
                }
            }
            for(int i=0;i<sig.size();++i){
                m_signal[startIndex+i] = QPointF(startIndex+i,m_signal[startIndex+i].y()+sig[i].y());
            }
        }

        SignalProcess::normalizeSignal(m_signal);
    }

    m_graph->update(m_signal);
}

MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent *event)
{

    QMainWindow::resizeEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Space){
        playSignal();
    }
    QMainWindow::keyPressEvent(event);
}
