#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QtCharts/QtCharts>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include "graph.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{

    scene = new QGraphicsScene();
    graphicsView = new QGraphicsView(scene);
    scene->setSceneRect(0,0,this->width(),this->height());



    mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *btn1 = new QPushButton("Add Track");
    connect(btn1,&QPushButton::clicked,this,&MainWindow::onOkayClicked);
    QPushButton *btn2 = new QPushButton("Cancel");
    QPushButton *btn3 = new QPushButton("Test");
    btn1->setMaximumWidth(200);
    btn2->setMaximumWidth(200);
    buttonsLayout->addWidget(btn1);
    buttonsLayout->addWidget(btn2);
    buttonsLayout->addWidget(btn3);

    spacer1 = new QSpacerItem(0,this->height()*0.6,QSizePolicy::Expanding);


    QVector<QPair<qreal,qreal>> coordinates;
    qreal step = 1.0/44100.0;
    for(int x=0;x<44100;++x){
        qreal y = sin(2*M_PI*x*step);
        coordinates.append({x,y});
    }
    Graph *graph = new Graph(coordinates);

    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(graph);
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

void MainWindow::setTrackFrequency(int frequency)
{
    dynamic_cast<Block*>(dialogAssociatedToTrack)->setFrequency(frequency);
}


MainWindow::~MainWindow()
{
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    resizeSlot();
    QMainWindow::resizeEvent(event);
}

void MainWindow::resizeSlot(){
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

    scene->setSceneRect(0,0,w+100,h+50);

    //qreal height = this->height()*0.7;
    //if(this->height()>800) {
    //    height=this->height()*0.8;
    //}
    //spacer1->changeSize(0,height,QSizePolicy::Minimum,QSizePolicy::Expanding);
    //mainLayout->invalidate();
}
