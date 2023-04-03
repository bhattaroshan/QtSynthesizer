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

    scene = new QGraphicsScene();
    graphicsView = new QGraphicsView(scene);
    scene->setSceneRect(0,0,this->width(),this->height());

    mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *btn1 = new QPushButton("Add Track");
    connect(btn1,&QPushButton::clicked,this,&MainWindow::onOkayClicked);
    QPushButton *btn2 = new QPushButton("Cancel");
    connect(btn2,&QPushButton::clicked,this,&MainWindow::onCancelClicked);
    QPushButton *btn3 = new QPushButton("Test");
    btn1->setMaximumWidth(200);
    btn2->setMaximumWidth(200);
    buttonsLayout->addWidget(btn1);
    buttonsLayout->addWidget(btn2);
    buttonsLayout->addWidget(btn3);

    spacer1 = new QSpacerItem(0,this->height()*0.6,QSizePolicy::Expanding);

    //auto sig = signal->generateSinWave(220,1000);
    //signal->addSignalToContainer(sig,0);
    //qDebug()<<signal->scaleSignalDown(100);
    //auto res = signal->scaleSignalDown(50);
    //qDebug()<<"the size of scale down signal is ="<<res.size();
    m_graph = new Graph(signal->getSignal());

    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(m_graph);
    //mainLayout->addSpacerItem(spacer1);
    mainLayout->addWidget(graphicsView);

    window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);
}

void MainWindow::onOkayClicked(){
    Block *b = new Block();
    m_blocks.append(b); //keep track of all the tracks for future use
    scene->addItem(b);
    connect(b,&Block::onItemDrag,this,&MainWindow::resizeSlot);
    connect(b,&Block::onItemDoubleClicked,this,&MainWindow::onTrackDoubleClicked);

    //signal->clear();
    auto sig = signal->generateSinWave(220,1000);
    signal->addSignalToContainer(sig,0);
    m_graph->update(signal->getSignal());

}

void MainWindow::onCancelClicked(){
    signal->clear();
    auto sig = signal->generateSinWave(440,1000);
    signal->addSignalToContainer(sig,0);
    //auto res = signal->scaleSignalDown(100);
    //qDebug()<<"the size of scale down signal is ="<<res.size();
    m_graph->update(signal->getSignal());
}

void MainWindow::onTrackDoubleClicked(int frequency)
{
    dialogAssociatedToTrack = sender();
    if(signalDialog==nullptr){
        delete signalDialog;
    }

    signalDialog = new SignalDialog(frequency);
    signalDialog->show();

    connect(signalDialog,&SignalDialog::dialogValues,this,&MainWindow::setTrackFrequency);
}

//this is triggered when Okay is clicked on any dialog
void MainWindow::setTrackFrequency(int currentFrequency, int lastFrequency)
{
    Block *activeTrack = dynamic_cast<Block*>(dialogAssociatedToTrack);
    activeTrack->setFrequency(currentFrequency);

    if(currentFrequency!=lastFrequency){
        signal->clear();
        QList<QGraphicsItem*> items = scene->items();
        for(auto item:items){
            QGraphicsRectItem *rectItem = dynamic_cast<QGraphicsRectItem*>(item);
            if(rectItem){
                Block *b = dynamic_cast<Block*>(rectItem);
                int trackWidth = b->boundingRect().width();
                qDebug()<<"my width is ========"<<trackWidth;
                qDebug()<<b->x();

                auto sig = signal->generateSinWave(b->getFrequency(),trackWidth*2);
                int index = signal->getIndexFromTime(b->x()*10);
                if(index<0) index=0;
                signal->addSignalToContainer(sig,index);
            }
        }
        m_graph->update(signal->getSignal());
//        auto sig = signal->generateSinWave(lastFrequency,1000);
//        signal->reduceSignalFromContainer(sig,0);
//        sig = signal->generateSinWave(currentFrequency,1000);
//        signal->addSignalToContainer(sig,0);
//        m_graph->update(signal->getSignal());
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
            QRectF points = QRectF(i,0,i,new_height);
            m_graphBar.append(points);
            QGraphicsLineItem *line = new QGraphicsLineItem(points.x(),points.y(),points.width(),points.height());
            line->setZValue(-100);
            line->setPen(QPen(QColor(128,128,128,20)));
            scene->addItem(line);
        }
    }


    return {w+100,h+50};
}
