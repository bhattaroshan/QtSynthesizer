#include "signaldialog.h"

SignalDialog::SignalDialog(int defaultFrequency, QWidget *parent)
    :QDialog(parent)
{
    setFixedSize(400,300);
    setWindowModality(Qt::ApplicationModal);

    mainLayout = new QVBoxLayout();
    buttonLayout = new QHBoxLayout();

    signalLayout = new QHBoxLayout();
    signalLabel = new QLabel("Signal (Hz)");
    signalEdit = new QSpinBox();
    signalEdit->setRange(1,20000);
    signalEdit->setValue(defaultFrequency);
    signalLayout->addWidget(signalLabel);
    signalLayout->addWidget(signalEdit);

    okButton = new QPushButton("OK");
    connect(okButton,&QPushButton::clicked,this,&SignalDialog::okClicked);

    cancelButton = new QPushButton("Cancel");
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(signalLayout);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void SignalDialog::okClicked()
{
    emit dialogValues(signalEdit->value());
    close();
}
