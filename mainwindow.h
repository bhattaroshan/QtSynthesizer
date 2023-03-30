#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <signaldialog.h>
#include "block.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;
    QObject *dialogAssociatedToTrack;
    SignalDialog *signalDialog = nullptr;

public slots:
    void resizeSlot();
    void onOkayClicked();
    void onTrackDoubleClicked(int);
    void setTrackFrequency(int);

private:
    QGraphicsScene *scene;
    QGraphicsView *graphicsView;

    QSpacerItem *spacer1;
    QWidget *window;
    QVBoxLayout *mainLayout;

    QVector<Block*> m_blocks;
};
#endif // MAINWINDOW_H
