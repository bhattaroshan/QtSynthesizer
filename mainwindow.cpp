#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QPushButton>
#include <QLabel>
#include <QSizeGrip>
#include <QHBoxLayout>
#include <QDockWidget>
#include <QTreeView>
#include <QtGui>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    m_timelineWidget = new TimelineWidget(0,0,this->width(),this->height(),this);
//    m_timelineWidget->addTrack();
//    m_timelineWidget->addTrack();
//    m_timelineWidget->addTrack();

    QWidget *window = new QWidget();
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QHBoxLayout *upperLayout = new QHBoxLayout();
    QHBoxLayout *lowerLayout = new QHBoxLayout();

    QHBoxLayout *trackLayout = new QHBoxLayout();

    QSpacerItem *spacer = new QSpacerItem(0,this->height()*0.5,QSizePolicy::Expanding);


    QPushButton *btn1 = new QPushButton("OK");
    QPushButton *btn2 = new QPushButton("OK");
    m_timelineWidget = new TimelineWidget(0,0,this->width(),this->height(),this);
    m_timelineWidget->addTrack();
    m_timelineWidget->addTrack();
    m_timelineWidget->addTrack();

    mainLayout->addLayout(upperLayout);
    mainLayout->addSpacerItem(spacer);
    mainLayout->addLayout(lowerLayout);

    upperLayout->addWidget(btn1);
    upperLayout->addWidget(btn2);

    QVBoxLayout *buttonLayout = new QVBoxLayout();
    QHBoxLayout *timelineLayout = new QHBoxLayout();

    QPushButton *addButton1 = new QPushButton("+");
    addButton1->setFixedSize(25,25);
    addButton1->setStyleSheet("background-color:#bcbcbc;border-radius:5px;color:#252525;");
    QPushButton *addButton2 = new QPushButton("+");
    addButton2->setFixedSize(25,25);
    addButton2->setStyleSheet("background-color:#bcbcbc;border-radius:5px;color:#252525;");
    QPushButton *addButton3 = new QPushButton("+");
    addButton3->setFixedSize(25,25);
    addButton3->setStyleSheet("background-color:#bcbcbc;border-radius:5px;color:#252525;");
    QPushButton *addButton4 = new QPushButton("+");
    addButton4->setFixedSize(25,25);
    addButton4->setStyleSheet("background-color:#bcbcbc;border-radius:5px;color:#252525;");
    buttonLayout->addWidget(addButton1);
    buttonLayout->addWidget(addButton2);
    buttonLayout->addWidget(addButton3);
    buttonLayout->addWidget(addButton4);

    timelineLayout->addWidget(m_timelineWidget);
    trackLayout->addLayout(buttonLayout);
    trackLayout->addLayout(timelineLayout);

    lowerLayout->addLayout(trackLayout);

    window->setLayout(mainLayout);
    setCentralWidget(window);





}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent *event){
    m_timelineWidget->onWindowSizeChange();
}
