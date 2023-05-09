
#include "mainwindow.h"
#include "signalproperties.h"
#include <QScrollBar>

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
    connect(m_phaseSpinBox,&QSpinBox::editingFinished,
            this,&MainWindow::triggered_phaseSpinBox);

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

    //update rendering issue when scroll bar moves in dockwidget
    QScrollBar *verticalScroll = m_blockAttributeScrollArea->verticalScrollBar();
    connect(verticalScroll,&QScrollBar::valueChanged,this,[=](){
        m_blockAttributeScrollArea->update();
    });

    QPushButton *addEffect = new QPushButton("Add Effect");
    connect(addEffect,&QPushButton::clicked,this,&MainWindow::showEffectsDialog);
    //addEffect->setFixedHeight(40);
    //addEffect->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);

    m_blockAttributeLayout->addWidget(m_transformSection);
    m_blockAttributeLayout->addWidget(m_signalSection);
    m_blockAttributeLayout->addWidget(addEffect);
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


void MainWindow::showEffectsDialog(){
    EffectsDialog *effectsDialog = new EffectsDialog();
    connect(effectsDialog,&EffectsDialog::clicked,this,&MainWindow::dialogAddClicked);
    effectsDialog->exec();
    delete effectsDialog;
}

void MainWindow:: dialogAddClicked(int index){
    if(index==0){ //echo clicked
        DelayEffectUI *delayEffect = new DelayEffectUI();
        Section *section = new Section("Echo",true,true);
        section->setContentLayout(*delayEffect);

        Effects *effects = new Effects;
        effects->effectsIndex = index;
        effects->section = section;

        effectsVec.push_back(effects);

        connect(section,&Section::closed,this,[=]() mutable {
            for(int i=0;i<effectsVec.size();++i){
                if(effectsVec[i]==effects){
                    QWidget *widget = effectsVec[i]->section;
                    m_blockAttributeLayout->removeWidget(widget);
                    effectsVec.remove(i);
                    delete widget;
                }
            }
        });
        m_blockAttributeLayout->insertWidget(m_blockAttributeLayout->count()-1,section);
    }
}

void MainWindow::sectionClosed(){
    m_blockAttributeLayout->removeWidget(static_cast<Section*>(sender()));
    m_blockAttributeLayout->invalidate();
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
    QVector<QVector<Block*>> blocks = graphicsView->getBlocksInOrder(graphicsView->getSelectedBlocks());
    QVector<Block*> updateBlocks;
    int changedValue = m_timeSpinBox->value();
    int newWidth = changedValue/10.0;

    for(int layers=0;layers<blocks.size();++layers){
        qreal startPos = blocks[layers][0]->x();
        for(int block=0;block<blocks[layers].size();++block){
            Block *currBlock = blocks[layers][block];
            qreal dist = 0;

            if(block<blocks[layers].size()-1){
                dist = blocks[layers][block+1]->x()-(currBlock->x()+currBlock->sceneBoundingRect().width());
            }

            currBlock->setPos(startPos,currBlock->y());
            currBlock->setRect(0,0,newWidth,currBlock->sceneBoundingRect().height());
            startPos = currBlock->x()+currBlock->sceneBoundingRect().width()+dist;
            updateBlocks.push_back(currBlock);
        }
    }

    updateSignal(updateBlocks);
}

void MainWindow::triggered_phaseSpinBox(){
    QList<Block*> blocks = graphicsView->getSelectedBlocks();
    for(auto block:blocks){
        block->setPhase(m_phaseSpinBox->value());
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
