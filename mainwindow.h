#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QObject>
#include <QSpacerItem>
#include <QVBoxLayout>


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void resizeSlot();
    void onOkayClicked();
    void onTrackDoubleClicked();

private:
    QGraphicsScene *scene;
    QGraphicsView *graphicsView;

    QSpacerItem *spacer1;
    QWidget *window;
    QVBoxLayout *mainLayout;
};
#endif // MAINWINDOW_H
