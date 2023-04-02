#ifndef SIGNALDIALOG_H
#define SIGNALDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>

class SignalDialog:public QDialog
{
    Q_OBJECT
public:
    SignalDialog(int defaultFrequency,QWidget *parent=nullptr);

protected:
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;

    QHBoxLayout *signalLayout;
    QLabel *signalLabel;
    QSpinBox *signalEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
    qreal m_previousValue = 0;

private slots:
    void okClicked();

signals:
    void dialogValues(int updatedFrequency, int lastFrequency);
};

#endif // SIGNALDIALOG_H
