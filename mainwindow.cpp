#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QMouseEvent>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene = new QGraphicsScene();
    graphicsView = new QGraphicsView(scene);
    scene->setSceneRect(0,0,this->width(),this->height());



    mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *btn1 = new QPushButton("OK");
    connect(btn1,&QPushButton::clicked,this,&MainWindow::onOkayClicked);
    QPushButton *btn2 = new QPushButton("Cancel");
    btn1->setMaximumWidth(200);
    btn2->setMaximumWidth(200);
    buttonsLayout->addWidget(btn1);
    buttonsLayout->addWidget(btn2);

    spacer1 = new QSpacerItem(0,this->height()*0.6,QSizePolicy::Expanding);


    mainLayout->addLayout(buttonsLayout);
    mainLayout->addSpacerItem(spacer1);
    mainLayout->addWidget(graphicsView);

    window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);

    setMouseTracking(true);
    graphicsView->setMouseTracking(true);

}

void MainWindow::onOkayClicked(){
    Block *b = new Block();
    scene->addItem(b);
    connect(b,&Block::onItemDrag,this,&MainWindow::resizeSlot);
}


MainWindow::~MainWindow()
{
    delete ui;
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
    qreal height = this->height()*0.7;
    if(this->height()>800) {
        height=this->height()*0.8;
    }
    spacer1->changeSize(0,height,QSizePolicy::Minimum,QSizePolicy::Expanding);
    mainLayout->invalidate();
}
