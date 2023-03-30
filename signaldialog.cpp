#include "signaldialog.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QShortcut>

SignalDialog::SignalDialog(QWidget *parent)
    :QDialog(parent)
{
    setFixedSize(400,300);
    setWindowModality(Qt::ApplicationModal);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QHBoxLayout *signalLayout = new QHBoxLayout();
    QLabel *signalLabel = new QLabel("Signal (Hz)");
    QSpinBox *signalEdit = new QSpinBox();
    signalEdit->setRange(1,20000);
    signalLayout->addWidget(signalLabel);
    signalLayout->addWidget(signalEdit);

    QPushButton *okButton = new QPushButton("OK");
    QShortcut *okShortcut = new QShortcut(QKeySequence(Qt::Key_Enter),this);
    connect(okButton,&QPushButton::clicked,this,&SignalDialog::close);

    QPushButton *cancelButton = new QPushButton("Cancel");
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(signalLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void SignalDialog::okClicked()
{

}
