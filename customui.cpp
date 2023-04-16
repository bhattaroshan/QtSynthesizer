
#include "mainwindow.h"
#include "signalproperties.h"

QPushButton* MainWindow::createIconButton(QString icon){
    QPushButton *colorBtn = new QPushButton();
    colorBtn->setIcon(QIcon(icon));
    colorBtn->setFixedSize(32,35);
    colorBtn->setStyleSheet("QPushButton {"
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

    return colorBtn;
}

void MainWindow::createTrackWidget(){

    Block* currentTrack = dynamic_cast<Block*>(sender());
    m_lastClickedTrack = currentTrack;

    QList<Block*> tracks = getAllTracks();
    for(auto track:tracks){
        if(track==currentTrack){
            track->setZValue(1);
            SignalProperties sp = track->getBlockProperties();
            int frequency = sp.frequency;
            int time = track->boundingRect().width()*10;
            QColor color = sp.color;

            //add dockWidget settings for track
            m_trackMainLayout = new QVBoxLayout();

            QHBoxLayout *typeLayout = new QHBoxLayout();
            QLabel *typeLabel = new QLabel("Type");
            QComboBox *typeCombo = new QComboBox();
            typeCombo->addItems({"Sin Wave","Triangular Wave","RAW Audio"});
            typeLayout->addWidget(typeLabel);
            typeLayout->addWidget(typeCombo);

            QHBoxLayout *transformLayout = new QHBoxLayout();
            QLabel *transformXLabel = new QLabel("X");

            if(m_transformXSpin!=nullptr) delete m_transformXSpin;
            m_transformXSpin = new QSpinBox();
            m_transformXSpin->setRange(0,100000);
            m_transformXSpin->setValue(track->x());

            auto transformXLambda = [=](){
                track->setPos(m_transformXSpin->value(),track->y());
                updateGraph();
            };
            connect(m_transformXSpin,&QSpinBox::valueChanged,this,transformXLambda);
            connect(m_transformXSpin,&QSpinBox::editingFinished,this,transformXLambda);

            transformLayout->addWidget(transformXLabel);
            transformLayout->addWidget(m_transformXSpin);

            QHBoxLayout *timeLayout = new QHBoxLayout();
            QLabel *timeLabel = new QLabel("Time (ms)");

            if(m_timeSpin!=nullptr) delete m_timeSpin;
            m_timeSpin = new QSpinBox();
            m_timeSpin->setRange(300,100000);
            m_timeSpin->setValue(time);
            auto timeLambda = [=](){
                updateGraph();
            };
            connect(m_timeSpin,&QSpinBox::valueChanged,this,timeLambda);
            connect(m_timeSpin,&QSpinBox::editingFinished,this,timeLambda);

            timeLayout->addWidget(timeLabel);
            timeLayout->addWidget(m_timeSpin);

            QHBoxLayout *frequencyLayout = new QHBoxLayout();
            QLabel *frequencyLabel = new QLabel("Frequency (Hz)");
            QSpinBox *frequencySpin = new QSpinBox();
            frequencySpin->setRange(1,20000);
            frequencySpin->setValue(frequency);

            auto frequencyLambda = [=](){
                qreal value = frequencySpin->value();
                if(track->getFrequency()!=value){
                    track->setFrequency(value);
                    track->setColor(setBrushFromFrequency(value));
                    updateGraph();
                }
            };

            connect(frequencySpin,&QSpinBox::editingFinished,this,frequencyLambda);
            connect(frequencySpin,&QSpinBox::valueChanged,this,frequencyLambda);

            frequencyLayout->addWidget(frequencyLabel);
            frequencyLayout->addWidget(frequencySpin);

            QHBoxLayout *amplitudeLayout = new QHBoxLayout();
            QLabel *amplitudeLabel = new QLabel("Amplitude");
            QDoubleSpinBox *amplitudeSpin = new QDoubleSpinBox();
            amplitudeSpin->setRange(0,1);
            amplitudeSpin->setSingleStep(0.1);
            amplitudeSpin->setValue(track->getAmplitude());

            amplitudeLayout->addWidget(amplitudeLabel);
            amplitudeLayout->addWidget(amplitudeSpin);

            auto amplitudeLambda = [=](){
                qreal value = amplitudeSpin->value();

                if(value && track->getAmplitude()!=value){
                    track->setAmplitude(value);
                    updateGraph();
                }
            };

            connect(amplitudeSpin,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,amplitudeLambda);

            QHBoxLayout *phaseLayout = new QHBoxLayout();
            QLabel *phaseLabel = new QLabel("Phase");
            QDial *phaseDial = new QDial();
            phaseDial->setFixedSize(42,42);
            phaseDial->setNotchTarget(0);
            phaseLayout->addWidget(phaseLabel);
            phaseLayout->addWidget(phaseDial);

            QHBoxLayout *harmonicsLayout = new QHBoxLayout();
            QLabel *harmonicsLabel = new QLabel("Harmonics");
            QSpinBox *harmonicsSpin = new QSpinBox();
            harmonicsSpin->setRange(0,20);
            harmonicsSpin->setValue(track->getHarmonics());


            auto harmonicsLambda = [=](){
                qreal value = harmonicsSpin->value();
                    track->setHarmonics(value);
                    updateGraph();
            };

            connect(harmonicsSpin,&QSpinBox::editingFinished,this,harmonicsLambda);
            connect(harmonicsSpin,&QSpinBox::valueChanged,this,harmonicsLambda);

            harmonicsLayout->addWidget(harmonicsLabel);
            harmonicsLayout->addWidget(harmonicsSpin);

            QHBoxLayout *colorLayout = new QHBoxLayout();
            QLabel *colorLabel = new QLabel("Track Color");
            QPushButton *colorBtn = createIconButton(":/icons/color-picker.png");

            connect(colorBtn,&QPushButton::clicked,this,[=](){
                QColor newColor = QColorDialog::getColor(color);
                if(newColor.isValid()){
                    track->setColor(newColor);
                }
            });

            colorLayout->addWidget(colorLabel);
            colorLayout->addWidget(colorBtn);

            QHBoxLayout *deleteLayout = new QHBoxLayout();
            QPushButton *deleteBtn = new QPushButton("Delete Track");
            connect(deleteBtn,&QPushButton::clicked,this,[=](){
                auto response = QMessageBox::warning(this,"Warning!!!","Are you sure you want to delete this track ?",QMessageBox::Ok|QMessageBox::Cancel);
                if(response == QMessageBox::Ok){
                    scene->removeItem(track);
                    updateGraph();
                    m_dockWidget->setWidget(nullptr);
                }
            });
            deleteLayout->addWidget(deleteBtn);


            QSpacerItem *spacer = new QSpacerItem(0,200,QSizePolicy::Minimum,QSizePolicy::Expanding);

            m_trackMainLayout->addLayout(typeLayout);
            m_trackMainLayout->addLayout(transformLayout);
            m_trackMainLayout->addLayout(timeLayout);
            m_trackMainLayout->addLayout(frequencyLayout);
            m_trackMainLayout->addLayout(amplitudeLayout);
            m_trackMainLayout->addLayout(phaseLayout);
            m_trackMainLayout->addLayout(harmonicsLayout);
            m_trackMainLayout->addLayout(colorLayout);
            m_trackMainLayout->addLayout(deleteLayout);
            m_trackMainLayout->addSpacerItem(spacer);

            QWidget *widget = new QWidget();
            widget->setLayout(m_trackMainLayout);
            m_dockWidget->setWidget(widget);

        }else{
            track->setZValue(0);
        }
    }

}

void MainWindow::createGeneralWidget(){

    m_generalMainLayout = new QVBoxLayout();
    QPushButton *addTrackButton = new QPushButton("Add Track");
    addTrackButton->setFixedHeight(40);
    connect(addTrackButton,&QPushButton::clicked,this,&MainWindow::onAddTrackClicked);
    m_generalMainLayout->addWidget(addTrackButton);

    QWidget *widget = new QWidget();
    widget->setLayout(m_generalMainLayout);
    m_dockWidget->setWidget(widget);
}
