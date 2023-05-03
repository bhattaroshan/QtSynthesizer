#ifndef EFFECTSDIALOG_H
#define EFFECTSDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QListWidget>

class EffectsDialog:public QDialog
{
    Q_OBJECT
public:
    EffectsDialog(QWidget *parent=nullptr);

private:
    QVBoxLayout *mainLayout;
    QPushButton *addBtn;
    QListWidget *listWidget;

public slots:
    void addBtnClicked();

signals:
    void clicked(int);
};

#endif // EFFECTSDIALOG_H
