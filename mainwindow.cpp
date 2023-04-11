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
    //resize(300,200);
    signal = new SignalVector();

    scene = new CustomGraphicsScene();

    graphicsView = new CustomGraphicsView();
    connect(graphicsView,&CustomGraphicsView::viewUpdated,this,&MainWindow::updateGraph);
    graphicsView->setScene(scene);
    scene->setSceneRect(0,0,this->width(),this->height());

    connect(graphicsView,&CustomGraphicsView::onMousePress,this,&MainWindow::onGraphicsViewMousePressed);
    connect(scene,&CustomGraphicsScene::selectionChanged,this,&MainWindow::onTrackSelected);
//    connect(scene,&CustomGraphicsScene::selectionChanged,this,[=](){
//        qDebug()<<"selection activated";
//        QList<QGraphicsItem*> selectedItems = scene->selectedItems();
//        qDebug()<<"total items = "<<selectedItems.size();
//        QVector<Block*> blocks;
//        for(auto item:selectedItems){
//            Block *b = dynamic_cast<Block*>(item);
//            if(b){
//                blocks.append(b);
//            }
//        }
//        qDebug()<<"rect items = "<<blocks.size();
//    });

    mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *btn1 = new QPushButton("Add Track");
    connect(btn1,&QPushButton::clicked,this,&MainWindow::onAddTrackClicked);
    QPushButton *btn2 = new QPushButton(">");
    btn2->setFixedSize(30,40);

    connect(btn2,&QPushButton::clicked,this,[=](){
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

    });

    QPushButton *btn3 = new QPushButton("Test");
    buttonsLayout->addWidget(btn1);
    buttonsLayout->addSpacing(100);
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

void MainWindow::onTrackSelected(){
    QList<QGraphicsItem*> selectedItems = scene->selectedItems();

    for(auto item:selectedItems){
        Block *block = dynamic_cast<Block*>(item);
        if(block){
            m_selectedTracks.append(block);
        }
    }


    //blocks list contains all the selected tracks, movement can be triggered from mousemove event
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

    int blockX=0,blockY=20;
    if(minY>0 and maxFarthest>0){
        blockX = maxFarthest;
        blockY = minY;
    }

    Block *b = new Block(blockX,blockY,frequency);
    m_blocks.append(b); //keep track of all the tracks for future use
    scene->addItem(b);
    connect(b,&Block::onItemDrag,this,&MainWindow::resizeSlot);
    connect(b,&Block::onItemDoubleClicked,this,&MainWindow::onTrackDoubleClicked);
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
    m_lastClickedTrack = currentTrack;

    QList<Block*> tracks = getAllTracks();
    for(auto track:tracks){
        if(track==currentTrack){
            track->setZValue(1);
            //track->setOutline(true);

            int frequency = track->getFrequency();
            int time = track->boundingRect().width()*10;
            QColor color = track->getColor();

            //add dockWidget settings for track
            QVBoxLayout *mainLayout = new QVBoxLayout();

            QHBoxLayout *typeLayout = new QHBoxLayout();
            QLabel *typeLabel = new QLabel("Type");
            QComboBox *typeCombo = new QComboBox();
            typeCombo->addItem("Sin Wave");
            typeCombo->addItem("Triangluar Wave");
            typeCombo->addItem("RAW Audio");
            typeCombo->addItem("WAV Audio");
            typeLayout->addWidget(typeLabel);
            typeLayout->addWidget(typeCombo);


            QHBoxLayout *transformLayout = new QHBoxLayout();
            QLabel *transformXLabel = new QLabel("X");

            if(m_transformXSpin!=nullptr) delete m_transformXSpin;
            m_transformXSpin = new QSpinBox();
            m_transformXSpin->setRange(0,100000);
            m_transformXSpin->setValue(track->x());

            auto transformXLambda = [=](){
                track->setPos(m_transformXSpin->value(),track->y());
                updateGraph();
            };
            connect(m_transformXSpin,&QSpinBox::valueChanged,this,transformXLambda);
            connect(m_transformXSpin,&QSpinBox::editingFinished,this,transformXLambda);

            transformLayout->addWidget(transformXLabel);
            transformLayout->addWidget(m_transformXSpin);

            QHBoxLayout *timeLayout = new QHBoxLayout();
            QLabel *timeLabel = new QLabel("Time (ms)");

            if(m_timeSpin!=nullptr) delete m_timeSpin;
            m_timeSpin = new QSpinBox();
            m_timeSpin->setRange(300,100000);
            m_timeSpin->setValue(time);
            auto timeLambda = [=](){
                //track->handleDraggable(m_timeSpin->value()/10);
                updateGraph();
            };
            connect(m_timeSpin,&QSpinBox::valueChanged,this,timeLambda);
            connect(m_timeSpin,&QSpinBox::editingFinished,this,timeLambda);

            timeLayout->addWidget(timeLabel);
            timeLayout->addWidget(m_timeSpin);

            QHBoxLayout *frequencyLayout = new QHBoxLayout();
            QLabel *frequencyLabel = new QLabel("Frequency (Hz)");
            QSpinBox *frequencySpin = new QSpinBox();
            frequencySpin->setRange(1,20000);
            frequencySpin->setValue(frequency);

            auto frequencyLambda = [=](){
                qreal value = frequencySpin->value();
                if(track->getFrequency()!=value){
                    track->setFrequency(value);
                    updateGraph();
                }
            };

            connect(frequencySpin,&QSpinBox::editingFinished,this,frequencyLambda);
            connect(frequencySpin,&QSpinBox::valueChanged,this,frequencyLambda);

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
            connect(colorBtn,&QPushButton::clicked,this,[=](){
                QColor newColor = QColorDialog::getColor(color);
                if(newColor.isValid()){
                    track->setColor(newColor);
                }
            });

            colorLayout->addWidget(colorLabel);
            colorLayout->addWidget(colorBtn);

            QHBoxLayout *deleteLayout = new QHBoxLayout();
            QPushButton *deleteBtn = new QPushButton("Delete Track");
            connect(deleteBtn,&QPushButton::clicked,this,[=](){
                auto response = QMessageBox::warning(this,"Warning!!!","Are you sure you want to delete this track ?",QMessageBox::Ok|QMessageBox::Cancel);
                if(response == QMessageBox::Ok){
                    scene->removeItem(track);
                    updateGraph();
                    m_dockWidget->setWidget(nullptr);
                }
            });
            deleteLayout->addWidget(deleteBtn);


            QSpacerItem *spacer = new QSpacerItem(0,200,QSizePolicy::Minimum,QSizePolicy::Expanding);

            mainLayout->addLayout(typeLayout);
            mainLayout->addLayout(transformLayout);
            mainLayout->addLayout(timeLayout);
            mainLayout->addLayout(frequencyLayout);
            mainLayout->addLayout(phaseLayout);
            mainLayout->addLayout(colorLayout);
            mainLayout->addLayout(deleteLayout);
            mainLayout->addSpacerItem(spacer);

            QWidget *widget = new QWidget();
            widget->setLayout(mainLayout);
            m_dockWidget->setWidget(widget);

        }else{
            track->setZValue(0);
        }
    }

}

void MainWindow::onGraphicsViewMousePressed()
{
    m_dockWidget->setWidget(nullptr);
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

    if(m_transformXSpin) {
        m_transformXSpin->setValue(m_lastClickedTrack->x());
    }

    if(m_timeSpin){
        m_timeSpin->setValue(m_lastClickedTrack->boundingRect().width()*10);
    }

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
