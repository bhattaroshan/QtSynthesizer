#include "signaldialog.h"

SignalDialog::SignalDialog(int defaultFrequency, QColor color, QWidget *parent)
    :QDialog(parent),
     m_color(color)
{
    m_previousValue = defaultFrequency;
    setFixedSize(350,250);
    setWindowModality(Qt::ApplicationModal);

    mainLayout = new QVBoxLayout();
    buttonLayout = new QHBoxLayout();

    signalLayout = new QHBoxLayout();
    signalLabel = new QLabel("Signal (Hz)");
    signalEdit = new QSpinBox();
    signalEdit->setRange(1,20000);
    signalEdit->setValue(defaultFrequency);
    signalLayout->setAlignment(Qt::AlignLeft);
    signalLayout->addWidget(signalLabel);
    signalLayout->addWidget(signalEdit);

    colorLayout = new QHBoxLayout();
    signalColor = new QLabel("Track Color");
    signalColorPickBtn = new QPushButton();
    signalColorPickBtn->setFixedSize(32,35);
    signalColorPickBtn->setIcon(QIcon(":/icons/color-picker.png"));
    signalColorPickBtn->setStyleSheet("QPushButton {"
                                      "border-radius:5px;"
                                      "border: 1px solid #aeaeae;"
                                      "}"
                                      "QPushButton:hover{"
                                      "background: #808080;"
                                      "}"
                                      "QPushButton:pressed{"
                                      "background: #101010;"
                                      "}"
                                      );
    connect(signalColorPickBtn,&QPushButton::clicked,this,&SignalDialog::onColorPickerClicked);

    colorLayout->setAlignment(Qt::AlignLeft);
    colorLayout->addWidget(signalColor);
    colorLayout->addWidget(signalColorPickBtn);




    okButton = new QPushButton("OK");
    okButton->setDefault(true);
    connect(okButton,&QPushButton::clicked,this,&SignalDialog::okClicked);

    cancelButton = new QPushButton("Cancel");
    connect(cancelButton,&QPushButton::clicked,this,&SignalDialog::cancelClicked);

    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);

    mainLayout->addLayout(signalLayout);
    mainLayout->addLayout(colorLayout);
    mainLayout->addLayout(buttonLayout);


    setLayout(mainLayout);
}

void SignalDialog::okClicked()
{
    DialogProperties dialogProperties;
    dialogProperties.currentFrequency = signalEdit->value();
    dialogProperties.lastFrequency = m_previousValue;
    dialogProperties.color = m_color;
    emit updateDialogProperties(dialogProperties.currentFrequency,
                                dialogProperties.lastFrequency,
                                dialogProperties.color
                                );

    close();
}

void SignalDialog::cancelClicked()
{
    close();
}

void SignalDialog::onColorPickerClicked()
{
    QColor color = QColorDialog::getColor(m_color);
    if(color.isValid()){
        m_color = color;
    }
}
