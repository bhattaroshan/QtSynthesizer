#include "effectsdialog.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QStyledItemDelegate>
#include <QVBoxLayout>
#include <QPushButton>


EffectsDialog::EffectsDialog(QWidget *parent)
    :QDialog(parent)
{
    setWindowTitle("Effects");

    int dialogWidth = 200;
    int dialogHeight = 400;
    setFixedSize(dialogWidth,dialogHeight);

    QVBoxLayout *mainLayout = new QVBoxLayout();

    QListWidget *listWidget = new QListWidget();
    //listWidget->setGeometry(0,0,dialogWidth,dialogHeight);
    QListWidgetItem *item1 = new QListWidgetItem("Echo",listWidget);
    QListWidgetItem *item2 = new QListWidgetItem("Reverb",listWidget);
    QListWidgetItem *item3 = new QListWidgetItem("Chorus",listWidget);
    QListWidgetItem *item4 = new QListWidgetItem("Distortion",listWidget);
    QListWidgetItem *item5 = new QListWidgetItem("Tremolo",listWidget);
    QListWidgetItem *item6 = new QListWidgetItem("Frequency Shift",listWidget);
    QListWidgetItem *item7 = new QListWidgetItem("Ring Modulation",listWidget);
    //listWidget->setGridSize(QSize(listWidget->width(),30));
    //listWidget->setViewMode(QListView::IconMode);
    //listWidget->setSpacing(0);
    item1->setIcon(QIcon(":/icons/echo-effect.png"));
    item2->setIcon(QIcon(":/icons/reverb-effect.png"));
    item3->setIcon(QIcon(":/icons/reverb-effect.png"));
    item4->setIcon(QIcon(":/icons/reverb-effect.png"));
    item5->setIcon(QIcon(":/icons/reverb-effect.png"));
    item6->setIcon(QIcon(":/icons/reverb-effect.png"));
    item7->setIcon(QIcon(":/icons/reverb-effect.png"));
    listWidget->setFont(QFont("Droid Sans",14,QFont::Normal));
    listWidget->setAlternatingRowColors(true);
    listWidget->setStyleSheet("QListWidget::item{"
                              //"border: 1px solid #a4b0be;"
                              "border-bottom: 1px solid #a4b0be;"
                              "height:40px;"
                              "background-color:#34495e;"
                              "}"
                              "QListWidget::item:alternate{"
                              "background-color:#2c3e50;"
                              "}"
                              "QListWidget::item::hover{"
                              "background-color:#3498db;"
                              "}"
                              "QListWidget::item::selected{"
                              "background-color:#2980b9;"
                              "}");
    //listWidget->setMinimumWidth(dialogWidth);

    QPushButton *addBtn = new QPushButton("Add");
    addBtn->setFixedWidth(80);
    addBtn->setFixedHeight(30);
    addBtn->setStyleSheet("QPushButton{"
                          "border:1px solid #fefefe;"
                          "border-radius:10px;"
                          "}"
                          "QPushButton::hover{"
                          "background-color:#afafaf;"
                          "}"
                          "QPushButton::pressed{"
                          "background-color:#7f7f7f;"
                          "}");


    listWidget->addItem(item1);
    listWidget->addItem(item2);
    listWidget->addItem(item3);
    listWidget->addItem(item4);
    listWidget->addItem(item5);
    listWidget->addItem(item6);
    listWidget->addItem(item7);
    listWidget->setMinimumHeight(dialogHeight*0.7f);
    //listWidget->show();

    mainLayout->addWidget(listWidget,0,Qt::AlignTop);
    mainLayout->addWidget(addBtn,0,Qt::AlignBottom|Qt::AlignCenter);
    mainLayout->setContentsMargins(0,0,0,20);
    setLayout(mainLayout);

}
