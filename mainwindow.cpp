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

    m_blocks.append(new Block());
    m_blocks.append(new Block());
    m_blocks.append(new Block());

    for(int i=0;i<m_blocks.length();++i){
        scene->addItem(m_blocks[i]);
    }


    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    QPushButton *btn1 = new QPushButton("OK");
    QPushButton *btn2 = new QPushButton("Cancel");
    btn1->setMaximumWidth(200);
    btn2->setMaximumWidth(200);

    buttonsLayout->addWidget(btn1);
    buttonsLayout->addWidget(btn2);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->addWidget(graphicsView);

    QWidget *window = new QWidget();
    window->setLayout(mainLayout);
    setCentralWidget(window);

    setMouseTracking(true);
    graphicsView->setMouseTracking(true);

}

MainWindow::~MainWindow()
{
    delete ui;
}

