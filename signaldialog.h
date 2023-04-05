#ifndef SIGNALDIALOG_H
#define SIGNALDIALOG_H

#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSpinBox>
#include <QPushButton>
#include <QColorDialog>

struct DialogProperties{
    int currentFrequency;
    int lastFrequency;
    QColor color;
};

class SignalDialog:public QDialog
{
    Q_OBJECT
public:
    SignalDialog(int defaultFrequency,QColor color, QWidget *parent=nullptr);


protected:
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;

    QHBoxLayout *signalLayout;
    QLabel *signalLabel;
    QSpinBox *signalEdit;

    QHBoxLayout *colorLayout;
    QLabel *signalColor;
    QPushButton *signalColorPickBtn;

    QPushButton *okButton;
    QPushButton *cancelButton;
    qreal m_previousValue = 0;
    QColor m_color;

private slots:
    void okClicked();
    void cancelClicked();
    void onColorPickerClicked();

signals:
    void updateDialogProperties(int,int,QColor);
};

#endif // SIGNALDIALOG_H
