#include <QPropertyAnimation>
#include <QApplication>
#include <QPushButton>

#include "section.h"

Section::Section(QString title,QWidget *parent)
        : QWidget(parent) {
    toggleButton = new QToolButton(this);
    //headerLine = new QFrame(this);
    toggleAnimation = new QParallelAnimationGroup(this);
    contentArea = new QScrollArea(this);
    contentArea->setWidgetResizable(true);
    contentArea->setStyleSheet("QScrollArea{"
                               "background-color:#2b2d2f;"
                               "}");

    mainLayout = new QVBoxLayout(this);

    toggleButton->setStyleSheet("QToolButton {"
                                "border: 1px solid #3b3f43;"
                                "background-color: #4a4d51;"
                                "padding:2px;"
                                "}"
                                "QToolButton:hover{"
                                "background-color: #3b3f43;"
                                "}"
                                "QToolButton:pressed{"
                                "background-color: #2b2d2f;"
                                "}");

    toggleButton->setFont(QFont("Droid Sans",12,QFont::Bold));
    toggleButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    toggleButton->setIcon(QIcon(":/icons/right-arrow.png"));
    toggleButton->setIconSize(QSize(12,12));
    toggleButton->setText(" "+title);
    toggleButton->setCheckable(true);
    toggleButton->setChecked(false);
    toggleButton->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    contentArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    // start out collapsed
    contentArea->setMaximumHeight(0);
    contentArea->setMinimumHeight(0);

    // let the entire widget grow and shrink with its content
    toggleAnimation->addAnimation(new QPropertyAnimation(this, "minimumHeight"));
    toggleAnimation->addAnimation(new QPropertyAnimation(this, "maximumHeight"));
    toggleAnimation->addAnimation(new QPropertyAnimation(contentArea, "maximumHeight"));
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);

    int row = 0;
    mainLayout->addWidget(toggleButton);
    mainLayout->addWidget(contentArea);
    setLayout(mainLayout);

    connect(toggleButton, &QToolButton::toggled, this, &Section::toggle);
}


void Section::toggle(bool collapsed) {
    toggleButton->setIcon(QIcon(collapsed?":/icons/down-arrow.png":":/icons/right-arrow.png"));
    toggleAnimation->setDirection(collapsed ? QAbstractAnimation::Forward : QAbstractAnimation::Backward);
    toggleAnimation->start();
}


void Section::setContentLayout(QLayout &contentLayout) {
    delete contentArea->layout();
    contentArea->setLayout(&contentLayout);
    const auto collapsedHeight = sizeHint().height() - contentArea->maximumHeight();
    auto contentHeight = contentLayout.sizeHint().height();

    for (int i = 0; i < toggleAnimation->animationCount() - 1; ++i) {
        QPropertyAnimation *SectionAnimation = static_cast<QPropertyAnimation *>(toggleAnimation->animationAt(i));
        SectionAnimation->setDuration(animationDuration);
        SectionAnimation->setStartValue(collapsedHeight);
        SectionAnimation->setEndValue(collapsedHeight + contentHeight);
    }

    QPropertyAnimation *contentAnimation = static_cast<QPropertyAnimation *>(toggleAnimation->animationAt(
            toggleAnimation->animationCount() - 1));
    contentAnimation->setDuration(animationDuration);
    contentAnimation->setStartValue(0);
    contentAnimation->setEndValue(contentHeight);
}

// --------------------------------------------------------------------------------
//                          QCustomWidget methods
// --------------------------------------------------------------------------------

QString Section::name() const {
    return "Section";
}

QString Section::includeFile() const {
    return "Section.h";
}

QString Section::group() const {
    return tr("Containers");
}

QIcon Section::icon() const {
    return QIcon("icon.png");
}

QString Section::toolTip() const {
    return tr("Collapsible and expandable section");
}

QString Section::whatsThis() const
{
    return tr("Cool collapsible and expandable section widget");
}

bool Section::isContainer() const
{
    return true;
}

QWidget *Section::createWidget(QString title, QWidget *parent)
{
    return new Section(title,parent);
}
