
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
            typeCombo->addItems({"Sin Wave","Square Wave","Triangular Wave","RAW Audio"});
            typeLayout->addWidget(typeLabel);
            typeLayout->addWidget(typeCombo);
            typeCombo->setCurrentIndex(track->getType());

            auto comboBoxLambda = [=](int index){
                if(index == 0){
                    track->setType(SIGNAL_TYPE_SINUSOIDAL);
                }else{
                    track->setType(SIGNAL_TYPE_SQUARE);
                }
                updateSignal(graphicsView->getSelectedBlocks());
            };

            connect(typeCombo,&QComboBox::activated,this,comboBoxLambda);

            QHBoxLayout *transformLayout = new QHBoxLayout();
            QLabel *transformXLabel = new QLabel("X");

            if(m_transformXSpin!=nullptr) delete m_transformXSpin;
            m_transformXSpin = new QSpinBox();
            m_transformXSpin->setRange(0,100000);
            m_transformXSpin->setValue(track->x());

            auto transformXLambda = [=](){
                track->setPos(m_transformXSpin->value(),track->y());
                updateSignal(graphicsView->getSelectedBlocks());
            };
            connect(m_transformXSpin,&QSpinBox::valueChanged,this,transformXLambda);
            connect(m_transformXSpin,&QSpinBox::editingFinished,this,transformXLambda);

            transformLayout->addWidget(transformXLabel);
            transformLayout->addWidget(m_transformXSpin);

            QHBoxLayout *timeLayout = new QHBoxLayout();
            QLabel *timeLabel = new QLabel("Time (ms)");

            if(m_timeSpin!=nullptr) delete m_timeSpin;
            m_timeSpin = new QSpinBox();
            m_timeSpin->setRange(10,100000);
            m_timeSpin->setValue(time);
            auto timeLambda = [=](){
                qreal value = m_timeSpin->value();
                if(track->sceneBoundingRect().width()*10!=value){
                    track->setRect(0,0,value/10,track->sceneBoundingRect().height());
                    updateSignal(graphicsView->getSelectedBlocks());
                }
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
                    updateSignal(graphicsView->getSelectedBlocks());
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
                    updateSignal(graphicsView->getSelectedBlocks());
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
                    updateSignal(graphicsView->getSelectedBlocks());
            };

            connect(harmonicsSpin,&QSpinBox::editingFinished,this,harmonicsLambda);
            connect(harmonicsSpin,&QSpinBox::valueChanged,this,harmonicsLambda);

            harmonicsLayout->addWidget(harmonicsLabel);
            harmonicsLayout->addWidget(harmonicsSpin);

            QHBoxLayout *attackLayout = new QHBoxLayout();
            QLabel *attackLabel = new QLabel("Attack");
            QSpinBox *attackSpin = new QSpinBox();
            attackSpin->setRange(1,100);
            attackSpin->setValue(track->getAttack());


            auto attackLambda = [=](){
                qreal value = attackSpin->value();
                if(value){
                    track->setAttack(value);
                    updateGraph();
                }
            };

            connect(attackSpin,&QSpinBox::editingFinished,this,attackLambda);
            connect(attackSpin,&QSpinBox::valueChanged,this,attackLambda);

            attackLayout->addWidget(attackLabel);
            attackLayout->addWidget(attackSpin);


            QHBoxLayout *decayLayout = new QHBoxLayout();
            QLabel *decayLabel = new QLabel("Decay");
            QSpinBox *decaySpin = new QSpinBox();
            decaySpin->setRange(1,100);
            decaySpin->setValue(track->getDecay());


            auto decayLambda = [=](){
                qreal value = decaySpin->value();
                if(value){
                    track->setDecay(value);
                    updateSignal(graphicsView->getSelectedBlocks());
                }
            };

            connect(decaySpin,&QSpinBox::editingFinished,this,decayLambda);
            connect(decaySpin,&QSpinBox::valueChanged,this,decayLambda);

            decayLayout->addWidget(decayLabel);
            decayLayout->addWidget(decaySpin);


            QHBoxLayout *releaseLayout = new QHBoxLayout();
            QLabel *releaseLabel = new QLabel("Release");
            QSpinBox *releaseSpin = new QSpinBox();
            releaseSpin->setRange(1,100);
            releaseSpin->setValue(track->getRelease());


            auto releaseLambda = [=](){
                qreal value = releaseSpin->value();
                if(value){
                    track->setRelease(value);
                    updateSignal(graphicsView->getSelectedBlocks());
                }
            };

            connect(releaseSpin,&QSpinBox::editingFinished,this,releaseLambda);
            connect(releaseSpin,&QSpinBox::valueChanged,this,releaseLambda);

            releaseLayout->addWidget(releaseLabel);
            releaseLayout->addWidget(releaseSpin);

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

            QHBoxLayout *applyLayout = new QHBoxLayout();
            QPushButton *applyBtn = new QPushButton("Apply Changes");
            applyBtn->setMinimumHeight(40);
            applyBtn->setStyleSheet("QPushButton{"
                                     "border-radius:5px;"
                                     "border: 1px solid #409b40;"
                                     "border-color: #40a540;"
                                     "font-weight:700;"
                                     "}"
                                     "QPushButton:hover{"
                                     "border-color:#57d457;"
                                     "}"
                                     "QPushButton:pressed{"
                                     "border-color:#409b40;"
                                    "}");

            applyBtn->setCursor(Qt::PointingHandCursor);
            applyLayout->addWidget(applyBtn);

            QHBoxLayout *deleteLayout = new QHBoxLayout();
            QPushButton *deleteBtn = new QPushButton("Delete Track");
            deleteBtn->setMinimumHeight(40);
            deleteBtn->setStyleSheet("QPushButton {"
                                     "border-radius:5px;"
                                     "border: 1px solid #b37436;"
                                     "border-color: #ffa64d;"
                                     "font-weight:700;"
                                     "}"
                                     "QPushButton:hover{"
                                     "border-color:#eda660;"
                                     "}"
                                     "QPushButton:pressed{"
                                     "border-color:#cc8f52;"
                                     "}");
            deleteBtn->setCursor(Qt::PointingHandCursor);
            connect(deleteBtn,&QPushButton::clicked,this,[=](){
                auto response = QMessageBox::warning(this,"Warning!!!","Are you sure you want to delete this track ?",QMessageBox::Ok|QMessageBox::Cancel);
                if(response == QMessageBox::Ok){
                    scene->removeItem(track);
                    updateSignal(graphicsView->getSelectedBlocks());
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
            m_trackMainLayout->addLayout(attackLayout);
            m_trackMainLayout->addLayout(decayLayout);
            m_trackMainLayout->addLayout(releaseLayout);
            m_trackMainLayout->addLayout(colorLayout);
            m_trackMainLayout->addLayout(applyLayout);
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

    QPushButton *test = new QPushButton("test");
    connect(test,&QPushButton::clicked,this,&MainWindow::combineSignals);

    m_generalMainLayout->addWidget(addTrackButton);
    m_generalMainLayout->addWidget(test);


    QWidget *widget = new QWidget();
    widget->setLayout(m_generalMainLayout);
    m_dockWidget->setWidget(widget);
}
