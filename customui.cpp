
#include "mainwindow.h"
#include "signalproperties.h"

void MainWindow::initializeUI(){

    m_transformLayout = new QVBoxLayout();
    m_transformSection = new Section("Transform",true);

    m_xPositionLayout = new QHBoxLayout();
    m_xPositionLabel = new QLabel("X");
    m_xPositionSpinBox = new QSpinBox(); //horizontal movement
    m_xPositionSpinBox->setRange(0,1000000);
    m_xPositionLayout->addWidget(m_xPositionLabel);
    m_xPositionLayout->addWidget(m_xPositionSpinBox);
    connect(m_xPositionSpinBox,&QSpinBox::editingFinished,
            this, &MainWindow::triggered_xPositionSpinBox);

    m_yPositionLayout = new QHBoxLayout();
    m_yPositionLabel = new QLabel("Y");
    m_yPositionSpinBox = new QSpinBox(); //layer
    m_yPositionSpinBox->setRange(0,20);
    m_yPositionLayout->addWidget(m_yPositionLabel);
    m_yPositionLayout->addWidget(m_yPositionSpinBox);
    connect(m_yPositionSpinBox,&QSpinBox::editingFinished,
            this, &MainWindow::triggered_yPositionSpinBox);

    m_timeLayout = new QHBoxLayout();
    m_timeLabel = new QLabel("Time (ms)");
    m_timeSpinBox = new QSpinBox();
    m_timeSpinBox->setRange(10,1000000);
    m_timeLayout->addWidget(m_timeLabel);
    m_timeLayout->addWidget(m_timeSpinBox);
    connect(m_timeSpinBox,&QSpinBox::editingFinished,
            this, &MainWindow::triggered_timeSpinBox);


    m_transformLayout->addLayout(m_xPositionLayout);
    m_transformLayout->addLayout(m_yPositionLayout);
    m_transformLayout->addLayout(m_timeLayout);

    /////////////////////////////////////////////////////////////////////////////////

    m_signalLayout = new QVBoxLayout();
    m_signalSection = new Section("Signal",true);

    m_signalTypeLayout = new QHBoxLayout();
    m_signalTypeLabel = new QLabel("Type");
    m_signalTypeComboBox = new QComboBox();
    m_signalTypeComboBox->addItems({"Sin Wave","Square Wave"});
    m_signalTypeLayout->addWidget(m_signalTypeLabel);
    m_signalTypeLayout->addWidget(m_signalTypeComboBox);

    connect(m_signalTypeComboBox,&QComboBox::activated,
            this,&MainWindow::clicked_signalTypeComboBox);

    m_frequencyLayout = new QHBoxLayout();
    m_frequencyLabel = new QLabel("Frequency");
    m_frequencyDoubleSpinBox = new QDoubleSpinBox(); //horizontal movement
    m_frequencyDoubleSpinBox->setRange(1,20000);
    m_frequencyDoubleSpinBox->setSingleStep(0.1);
    m_frequencyLayout->addWidget(m_frequencyLabel);
    m_frequencyLayout->addWidget(m_frequencyDoubleSpinBox);
    connect(m_frequencyDoubleSpinBox,&QDoubleSpinBox::editingFinished,
            this,&MainWindow::triggered_frequencySpinBox);


    m_amplitudeLayout = new QHBoxLayout();
    m_amplitudeLabel = new QLabel("Amplitude");
    m_amplitudeDoubleSpinBox = new QDoubleSpinBox(); //horizontal movement
    m_amplitudeDoubleSpinBox->setRange(0,1);
    m_amplitudeDoubleSpinBox->setSingleStep(0.1);
    m_amplitudeLayout->addWidget(m_amplitudeLabel);
    m_amplitudeLayout->addWidget(m_amplitudeDoubleSpinBox);
    connect(m_amplitudeDoubleSpinBox,&QDoubleSpinBox::editingFinished,
            this,&MainWindow::triggered_amplitudeSpinBox);

    m_phaseLayout = new QHBoxLayout();
    m_phaseLabel = new QLabel("Phase");
    m_phaseSpinBox = new QSpinBox(); //horizontal movement
    m_phaseSpinBox->setRange(0,359);
    m_phaseLayout->addWidget(m_phaseLabel);
    m_phaseLayout->addWidget(m_phaseSpinBox);

    m_harmonicsLayout = new QHBoxLayout();
    m_harmonicsLabel = new QLabel("Harmonics");
    m_harmonicsSpinBox = new QSpinBox();
    m_harmonicsSpinBox->setRange(0,20);
    m_harmonicsLayout->addWidget(m_harmonicsLabel);
    m_harmonicsLayout->addWidget(m_harmonicsSpinBox);

    connect(m_harmonicsSpinBox,&QSpinBox::editingFinished,
            this,&MainWindow::triggered_harmonicsSpinBox);

    m_signalTypeLabel->setMinimumSize(m_frequencyLabel->sizeHint());

    m_signalLayout->addLayout(m_signalTypeLayout);
    m_signalLayout->addLayout(m_frequencyLayout);
    m_signalLayout->addLayout(m_amplitudeLayout);
    m_signalLayout->addLayout(m_phaseLayout);
    m_signalLayout->addLayout(m_harmonicsLayout);



    m_transformSection->setContentLayout(*m_transformLayout);
    m_signalSection->setContentLayout(*m_signalLayout);

    //attribute dock
    m_blockAttributeLayout = new QVBoxLayout();
    m_blockAttributeLayout->setAlignment(Qt::AlignTop);
    m_blockAttributeLayout->setContentsMargins(0,0,0,0);
    m_blockAttributeLayout->setSpacing(2);
    m_blockAttributeDockWidget = new QDockWidget("Attributes");
    m_blockAttributeWidget = new QWidget();
    m_blockAttributeScrollArea = new QScrollArea();
    m_blockAttributeScrollArea->setWidgetResizable(true);
    m_blockAttributeDockWidget->setMinimumWidth(200);

    m_blockAttributeLayout->addWidget(m_transformSection);
    m_blockAttributeLayout->addWidget(m_signalSection);
    m_blockAttributeWidget->setLayout(m_blockAttributeLayout);
    m_blockAttributeScrollArea->setWidget(m_blockAttributeWidget);

    addDockWidget(Qt::DockWidgetArea::RightDockWidgetArea,m_blockAttributeDockWidget);

    /////////////////////////////////////////////////////////////////////////////////

    m_projectSignalSection = new Section("Track",true);
    m_projectSignalLayout = new QVBoxLayout();
    m_projectSignalBlockButton = new QPushButton("Add Signal Block");
    m_projectMediaBlockButton = new QPushButton("Add Media Block");
    m_projectSignalLayout->addWidget(m_projectSignalBlockButton);
    m_projectSignalLayout->addWidget(m_projectMediaBlockButton);

    connect(m_projectSignalBlockButton,&QPushButton::clicked,
            this,&MainWindow::clicked_projectSignalBlockButton);

    m_projectSignalSection->setContentLayout(*m_projectSignalLayout);

    //project dock
    m_projectLayout = new QVBoxLayout();
    m_projectLayout->setAlignment(Qt::AlignTop);
    m_projectLayout->setContentsMargins(0,0,0,0);
    m_projectLayout->setSpacing(2);
    m_projectLayout->addWidget(m_projectSignalSection);

    m_projectDockWidget = new QDockWidget("Project");
    m_projectDockWidget->setMinimumWidth(200);

    m_projectWidget = new QWidget();
    m_projectWidget->setLayout(m_projectLayout);

    m_projectScrollArea = new QScrollArea();
    m_projectScrollArea->setWidgetResizable(true);
    m_projectScrollArea->setWidget(m_projectWidget);
    m_projectDockWidget->setWidget(m_projectScrollArea);

    addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea,m_projectDockWidget);
}

void MainWindow::clicked_projectSignalBlockButton(){
    onAddTrackClicked();
}

void MainWindow::triggered_xPositionSpinBox(){
    QList<Block*> blocks = graphicsView->getSelectedBlocks();
    for(auto block:blocks){
        qreal newPos = m_xPositionSpinBox->value();
        block->setX(newPos);
        block->setPos(newPos+30,block->getY());
    }
    updateSignal(blocks);
}

void MainWindow::triggered_yPositionSpinBox(){
    QList<Block*> blocks = graphicsView->getSelectedBlocks();
    for(auto block:blocks){
        qreal newPos = m_yPositionSpinBox->value()*30;
        block->setY(newPos);
        block->setPos(block->getX(),newPos+30);
    }
    updateSignal(blocks);
}

void MainWindow::triggered_frequencySpinBox(){

    QList<Block*> blocks = graphicsView->getSelectedBlocks();
    for(auto block:blocks){
        block->setFrequency(m_frequencyDoubleSpinBox->value());
    }
    updateSignal(blocks);
}

void MainWindow::clicked_signalTypeComboBox(int index){
    QList<Block*> blocks = graphicsView->getSelectedBlocks();
    for(auto block:blocks){
        block->setType(index);
    }
    updateSignal(blocks);
}

void MainWindow::triggered_harmonicsSpinBox(){
    QList<Block*> blocks = graphicsView->getSelectedBlocks();
    for(auto block:blocks){
        block->setHarmonics(m_harmonicsSpinBox->value());
    }
    updateSignal(blocks);
}

void MainWindow::triggered_amplitudeSpinBox(){
    QList<Block*> blocks = graphicsView->getSelectedBlocks();
    for(auto block:blocks){
        block->setAmplitude(m_amplitudeDoubleSpinBox->value());
    }
    updateSignal(blocks);
}

void MainWindow::triggered_timeSpinBox(){
    QList<Block*> blocks = graphicsView->getSelectedBlocks();
    for(auto block:blocks){
        int width = m_timeSpinBox->value()/10;
        block->setWidth(width);
        block->setRect(0,0,width,block->sceneBoundingRect().height());
    }
    updateSignal(blocks);
}

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

    //QList<Block*> tracks = graphicsView->getAllBlocks();
    QList<Block*> tracks = graphicsView->getSelectedBlocks();

    for(auto track:tracks){
   //     if(track==currentTrack){
            track->setZValue(1);
            SignalProperties sp = track->getBlockProperties();
            m_xPositionSpinBox->setValue(sp.x-30);
            m_yPositionSpinBox->setValue(int((sp.y-30)/30));
            m_timeSpinBox->setValue(sp.time);
            m_signalTypeComboBox->setCurrentIndex(sp.type);
            m_frequencyDoubleSpinBox->setValue(sp.frequency);
            m_amplitudeDoubleSpinBox->setValue(sp.amplitude);
            m_harmonicsSpinBox->setValue(sp.harmonics);
            m_phaseSpinBox->setValue(sp.phase);
            m_blockAttributeDockWidget->setWidget(m_blockAttributeScrollArea);


//            SignalProperties sp = track->getBlockProperties();
//            int frequency = sp.frequency;
//            int time = track->boundingRect().width()*10;
//            QColor color = sp.color;

//            //add dockWidget settings for track
//            m_trackMainLayout = new QVBoxLayout();

//            QHBoxLayout *typeLayout = new QHBoxLayout();
//            QLabel *typeLabel = new QLabel("Type");
//            QComboBox *typeCombo = new QComboBox();
//            typeCombo->addItems({"Sin Wave","Square Wave","Triangular Wave","RAW Audio"});
//            typeLayout->addWidget(typeLabel);
//            typeLayout->addWidget(typeCombo);
//            typeCombo->setCurrentIndex(track->getType());

//            auto comboBoxLambda = [=](int index){
//                if(index == 0){
//                    track->setType(SIGNAL_TYPE_SINUSOIDAL);
//                }else{
//                    track->setType(SIGNAL_TYPE_SQUARE);
//                }
//                updateSignal(graphicsView->getSelectedBlocks());
//            };

//            connect(typeCombo,&QComboBox::activated,this,comboBoxLambda);

//            QHBoxLayout *transformLayout = new QHBoxLayout();
//            QLabel *transformXLabel = new QLabel("X");

//            if(m_transformXSpin!=nullptr) delete m_transformXSpin;
//            m_transformXSpin = new QSpinBox();
//            m_transformXSpin->setRange(0,100000);
//            m_transformXSpin->setValue(track->x());

//            auto transformXLambda = [=](){
//                track->setPos(m_transformXSpin->value(),track->y());
//                updateSignal(graphicsView->getSelectedBlocks());
//            };
//            connect(m_transformXSpin,&QSpinBox::valueChanged,this,transformXLambda);
//            connect(m_transformXSpin,&QSpinBox::editingFinished,this,transformXLambda);

//            transformLayout->addWidget(transformXLabel);
//            transformLayout->addWidget(m_transformXSpin);

//            QHBoxLayout *timeLayout = new QHBoxLayout();
//            QLabel *timeLabel = new QLabel("Time (ms)");

//            if(m_timeSpin!=nullptr) delete m_timeSpin;
//            m_timeSpin = new QSpinBox();
//            m_timeSpin->setRange(10,100000);
//            m_timeSpin->setValue(time);
//            auto timeLambda = [=](){
//                qreal value = m_timeSpin->value();
//                if(track->sceneBoundingRect().width()*10!=value){
//                    track->setRect(0,0,value/10,track->sceneBoundingRect().height());
//                    updateSignal(graphicsView->getSelectedBlocks());
//                }
//            };
//            connect(m_timeSpin,&QSpinBox::valueChanged,this,timeLambda);
//            connect(m_timeSpin,&QSpinBox::editingFinished,this,timeLambda);

//            timeLayout->addWidget(timeLabel);
//            timeLayout->addWidget(m_timeSpin);

//            QHBoxLayout *frequencyLayout = new QHBoxLayout();
//            QLabel *frequencyLabel = new QLabel("Frequency (Hz)");
//            QSpinBox *frequencySpin = new QSpinBox();
//            frequencySpin->setRange(1,20000);
//            frequencySpin->setValue(frequency);

//            auto frequencyLambda = [=](){
//                qreal value = frequencySpin->value();
//                if(track->getFrequency()!=value){
//                    track->setFrequency(value);
//                    track->setColor(setBrushFromFrequency(value));
//                    updateSignal(graphicsView->getSelectedBlocks());
//                }
//            };

//            connect(frequencySpin,&QSpinBox::editingFinished,this,frequencyLambda);
//            connect(frequencySpin,&QSpinBox::valueChanged,this,frequencyLambda);

//            frequencyLayout->addWidget(frequencyLabel);
//            frequencyLayout->addWidget(frequencySpin);

//            QHBoxLayout *amplitudeLayout = new QHBoxLayout();
//            QLabel *amplitudeLabel = new QLabel("Amplitude");
//            QDoubleSpinBox *amplitudeSpin = new QDoubleSpinBox();
//            amplitudeSpin->setRange(0,1);
//            amplitudeSpin->setSingleStep(0.1);
//            amplitudeSpin->setValue(track->getAmplitude());

//            amplitudeLayout->addWidget(amplitudeLabel);
//            amplitudeLayout->addWidget(amplitudeSpin);

//            auto amplitudeLambda = [=](){
//                qreal value = amplitudeSpin->value();

//                if(value && track->getAmplitude()!=value){
//                    track->setAmplitude(value);
//                    updateSignal(graphicsView->getSelectedBlocks());
//                }
//            };

//            connect(amplitudeSpin,QOverload<double>::of(&QDoubleSpinBox::valueChanged),this,amplitudeLambda);

//            QHBoxLayout *phaseLayout = new QHBoxLayout();
//            QLabel *phaseLabel = new QLabel("Phase");
//            QDial *phaseDial = new QDial();
//            phaseDial->setFixedSize(42,42);
//            phaseDial->setNotchTarget(0);
//            phaseLayout->addWidget(phaseLabel);
//            phaseLayout->addWidget(phaseDial);

//            QHBoxLayout *harmonicsLayout = new QHBoxLayout();
//            QLabel *harmonicsLabel = new QLabel("Harmonics");
//            QSpinBox *harmonicsSpin = new QSpinBox();
//            harmonicsSpin->setRange(0,20);
//            harmonicsSpin->setValue(track->getHarmonics());


//            auto harmonicsLambda = [=](){
//                qreal value = harmonicsSpin->value();
//                    track->setHarmonics(value);
//                    updateSignal(graphicsView->getSelectedBlocks());
//            };

//            connect(harmonicsSpin,&QSpinBox::editingFinished,this,harmonicsLambda);
//            connect(harmonicsSpin,&QSpinBox::valueChanged,this,harmonicsLambda);

//            harmonicsLayout->addWidget(harmonicsLabel);
//            harmonicsLayout->addWidget(harmonicsSpin);

//            QHBoxLayout *attackLayout = new QHBoxLayout();
//            QLabel *attackLabel = new QLabel("Attack");
//            QSpinBox *attackSpin = new QSpinBox();
//            attackSpin->setRange(1,100);
//            attackSpin->setValue(track->getAttack());


//            auto attackLambda = [=](){
//                qreal value = attackSpin->value();
//                if(value){
//                    track->setAttack(value);
//                    updateGraph();
//                }
//            };

//            connect(attackSpin,&QSpinBox::editingFinished,this,attackLambda);
//            connect(attackSpin,&QSpinBox::valueChanged,this,attackLambda);

//            attackLayout->addWidget(attackLabel);
//            attackLayout->addWidget(attackSpin);


//            QHBoxLayout *decayLayout = new QHBoxLayout();
//            QLabel *decayLabel = new QLabel("Decay");
//            QSpinBox *decaySpin = new QSpinBox();
//            decaySpin->setRange(1,100);
//            decaySpin->setValue(track->getDecay());


//            auto decayLambda = [=](){
//                qreal value = decaySpin->value();
//                if(value){
//                    track->setDecay(value);
//                    updateSignal(graphicsView->getSelectedBlocks());
//                }
//            };

//            connect(decaySpin,&QSpinBox::editingFinished,this,decayLambda);
//            connect(decaySpin,&QSpinBox::valueChanged,this,decayLambda);

//            decayLayout->addWidget(decayLabel);
//            decayLayout->addWidget(decaySpin);


//            QHBoxLayout *releaseLayout = new QHBoxLayout();
//            QLabel *releaseLabel = new QLabel("Release");
//            QSpinBox *releaseSpin = new QSpinBox();
//            releaseSpin->setRange(1,100);
//            releaseSpin->setValue(track->getRelease());


//            auto releaseLambda = [=](){
//                qreal value = releaseSpin->value();
//                if(value){
//                    track->setRelease(value);
//                    updateSignal(graphicsView->getSelectedBlocks());
//                }
//            };

//            connect(releaseSpin,&QSpinBox::editingFinished,this,releaseLambda);
//            connect(releaseSpin,&QSpinBox::valueChanged,this,releaseLambda);

//            releaseLayout->addWidget(releaseLabel);
//            releaseLayout->addWidget(releaseSpin);

//            QHBoxLayout *colorLayout = new QHBoxLayout();
//            QLabel *colorLabel = new QLabel("Track Color");
//            QPushButton *colorBtn = createIconButton(":/icons/color-picker.png");

//            connect(colorBtn,&QPushButton::clicked,this,[=](){
//                QColor newColor = QColorDialog::getColor(color);
//                if(newColor.isValid()){
//                    track->setColor(newColor);
//                }
//            });

//            colorLayout->addWidget(colorLabel);
//            colorLayout->addWidget(colorBtn);

//            QHBoxLayout *applyLayout = new QHBoxLayout();
//            QPushButton *applyBtn = new QPushButton("Apply Changes");
//            applyBtn->setMinimumHeight(40);
//            applyBtn->setStyleSheet("QPushButton{"
//                                     "border-radius:5px;"
//                                     "border: 1px solid #409b40;"
//                                     "border-color: #40a540;"
//                                     "font-weight:700;"
//                                     "}"
//                                     "QPushButton:hover{"
//                                     "border-color:#57d457;"
//                                     "}"
//                                     "QPushButton:pressed{"
//                                     "border-color:#409b40;"
//                                    "}");

//            applyBtn->setCursor(Qt::PointingHandCursor);
//            applyLayout->addWidget(applyBtn);

//            QHBoxLayout *deleteLayout = new QHBoxLayout();
//            QPushButton *deleteBtn = new QPushButton("Delete Track");
//            deleteBtn->setMinimumHeight(40);
//            deleteBtn->setStyleSheet("QPushButton {"
//                                     "border-radius:5px;"
//                                     "border: 1px solid #b37436;"
//                                     "border-color: #ffa64d;"
//                                     "font-weight:700;"
//                                     "}"
//                                     "QPushButton:hover{"
//                                     "border-color:#eda660;"
//                                     "}"
//                                     "QPushButton:pressed{"
//                                     "border-color:#cc8f52;"
//                                     "}");
//            deleteBtn->setCursor(Qt::PointingHandCursor);
//            connect(deleteBtn,&QPushButton::clicked,this,[=](){
//                auto response = QMessageBox::warning(this,"Warning!!!","Are you sure you want to delete this track ?",QMessageBox::Ok|QMessageBox::Cancel);
//                if(response == QMessageBox::Ok){
//                    scene->removeItem(track);
//                    updateSignal(graphicsView->getSelectedBlocks());
//                    m_dockWidget->setWidget(nullptr);
//                }
//            });
//            deleteLayout->addWidget(deleteBtn);


//            QSpacerItem *spacer = new QSpacerItem(0,200,QSizePolicy::Minimum,QSizePolicy::Expanding);

//            m_trackMainLayout->addLayout(typeLayout);
//            m_trackMainLayout->addLayout(transformLayout);
//            m_trackMainLayout->addLayout(timeLayout);
//            m_trackMainLayout->addLayout(frequencyLayout);
//            m_trackMainLayout->addLayout(amplitudeLayout);
//            m_trackMainLayout->addLayout(phaseLayout);
//            m_trackMainLayout->addLayout(harmonicsLayout);
//            m_trackMainLayout->addLayout(attackLayout);
//            m_trackMainLayout->addLayout(decayLayout);
//            m_trackMainLayout->addLayout(releaseLayout);
//            m_trackMainLayout->addLayout(colorLayout);
//            m_trackMainLayout->addLayout(applyLayout);
//            m_trackMainLayout->addLayout(deleteLayout);
//            m_trackMainLayout->addSpacerItem(spacer);

//            QWidget *widget = new QWidget();
//            widget->setLayout(m_trackMainLayout);
//            m_dockWidget->setWidget(widget);

//        }else{
//            track->setZValue(0);
//        }
    }

}

void MainWindow::createGeneralWidget(){

//    m_generalMainLayout = new QVBoxLayout();
//    QPushButton *addTrackButton = new QPushButton("Add Track");
//    addTrackButton->setFixedHeight(40);
//    connect(addTrackButton,&QPushButton::clicked,this,&MainWindow::onAddTrackClicked);

//    m_generalMainLayout->addWidget(addTrackButton);

//    QWidget *widget = new QWidget();
//    widget->setLayout(m_generalMainLayout);
    m_blockAttributeDockWidget->setWidget(nullptr);
    //m_blockAttributeDockWidget->setWidget(nullptr);
}
