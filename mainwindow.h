#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include "block.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onItemDragged(Block *item);

protected:

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    QGraphicsView *graphicsView;
    QList<Block *> m_blocks;
};
#endif // MAINWINDOW_H
