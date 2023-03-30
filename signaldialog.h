#ifndef SIGNALDIALOG_H
#define SIGNALDIALOG_H

#include <QDialog>

class SignalDialog:public QDialog
{
public:
    SignalDialog(QWidget *parent=nullptr);

private slots:
    void okClicked();
};

#endif // SIGNALDIALOG_H
