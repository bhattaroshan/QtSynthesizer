#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QSplitter>

#include "timelinewidget.h"
#include "customrect.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void resizeEvent(QResizeEvent *event) override;
    Ui::MainWindow *ui;
    QGraphicsScene *scene;
    CustomRect *itemRect;
    QGraphicsEllipseItem *itemEllipse;
    QGraphicsView *graphicsView;
    TimelineWidget *m_timelineWidget;

};
#endif // MAINWINDOW_H
