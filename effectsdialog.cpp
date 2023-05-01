#include "effectsdialog.h"
#include <QListWidget>
#include <QListWidgetItem>
#include <QStyledItemDelegate>


EffectsDialog::EffectsDialog(QWidget *parent)
    :QDialog(parent)
{
    setFixedSize(300,200);

    QListWidget *listWidget = new QListWidget(this);
    listWidget->setGeometry(0,0,300,200);
    QListWidgetItem *item1 = new QListWidgetItem("Echo",listWidget);
    QListWidgetItem *item2 = new QListWidgetItem("Reverb",listWidget);
    QListWidgetItem *item3 = new QListWidgetItem("Item 3",listWidget);
    listWidget->setGridSize(QSize(listWidget->width(),30));
    listWidget->setViewMode(QListView::IconMode);
    listWidget->setSpacing(0);
    listWidget->setFont(QFont("Droid Sans",14,QFont::Normal));
    listWidget->setAlternatingRowColors(true);
    listWidget->setStyleSheet("QListWidget::item{"
                              //"border: 1px solid #a4b0be;"
                              "background-color:#95a5a6;"
                              "}"
                              "QListWidget::item:alternate{"
                              "background-color:#7f8c8d;"
                              "}"
                              "QListWidget::item::hover{"
                              "background-color:#2c3e50;"
                              "}"
                              "QListWidget::item::selected{"
                              "background-color:#3498db;"
                              "}");

    listWidget->addItem(item1);
    listWidget->addItem(item2);
    listWidget->addItem(item3);
    listWidget->show();
}
