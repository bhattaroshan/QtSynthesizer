#include "block.h"
#include "QtWidgets/qstyleoption.h"
#include <QPen>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QStyle>
#include <QApplication>

Block::Block(int x,int y,int frequency, QGraphicsItem *parent)
    :QGraphicsRectItem(parent)
{
    m_sp.frequency = frequency;
    m_sp.x = x;
    m_sp.y = y;
    m_sp.harmonics = 0;
    m_sp.attackPercent = 0;
    m_sp.decayPercent = 0;
    m_sp.releasePercent = 0;
    m_sp.phase = 0;
    m_sp.samples = 44100;
    m_sp.amplitude = 1.0;

    setFlags(ItemIsSelectable);

    setPen(Qt::NoPen);
    setBrush(m_sp.color);
    setRect(0,0,100,30);
    setPos(x,y);
    setAcceptHoverEvents(true);
}

void Block::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit onItemSingleClick();
    QGraphicsRectItem::mousePressEvent(event);
}

void Block::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    int ax = this->boundingRect().width()+this->boundingRect().x();
    if(qAbs(event->pos().x()-ax)<=5){
        setCursor(Qt::SizeHorCursor);
    }else{
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsItem::hoverMoveEvent(event);
}


void Block::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QMenu menu;
    QMenu noteMenu("Note");
    noteMenu.setMinimumWidth(100);
    QAction *c4Action = noteMenu.addAction("C4");
    QAction *d4Action = noteMenu.addAction("D4");
    QAction *e4Action = noteMenu.addAction("E4");
    QAction *f4Action = noteMenu.addAction("F4");
    QAction *g4Action = noteMenu.addAction("G4");
    QAction *a4Action = noteMenu.addAction("A4");
    QAction *b4Action = noteMenu.addAction("B4");
    QAction *c5Action = noteMenu.addAction("C5");

    connect(c4Action,&QAction::triggered,this,[=](){
        setFrequency(261);
       emit trackUpdated();
    });
    connect(d4Action,&QAction::triggered,this,[=](){
        setFrequency(293);
       emit trackUpdated();
    });
    connect(e4Action,&QAction::triggered,this,[=](){
        setFrequency(329);
       emit trackUpdated();
    });
    connect(f4Action,&QAction::triggered,this,[=](){
        setFrequency(349);
       emit trackUpdated();
    });
    connect(g4Action,&QAction::triggered,this,[=](){
        setFrequency(392);
       emit trackUpdated();
    });
    connect(a4Action,&QAction::triggered,this,[=](){
        setFrequency(440);
       emit trackUpdated();
    });
    connect(b4Action,&QAction::triggered,this,[=](){
        setFrequency(493);
       emit trackUpdated();
    });
    connect(c5Action,&QAction::triggered,this,[=](){
        setFrequency(523);
       emit trackUpdated();
    });

    menu.addMenu(&noteMenu);
    menu.addAction("Copy");
    menu.addAction("Paste");
    menu.addAction("Delete");
    menu.addAction("Properties");
    menu.exec(event->screenPos());
}

void Block::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->setBrush(m_sp.color);
    if(option->state & QStyle::State_Selected){
        QColor t = QColor(0,100,150);
        painter->setBrush(t);
        painter->setPen(QPen(QColor(128,128,128),1,Qt::SolidLine));
    }else{
        painter->setPen(QPen(Qt::white,1,Qt::SolidLine));
    }
    painter->drawRect(this->rect());

    QFont font = QApplication::font();
    QFontMetrics metrics(font);
    int width = metrics.horizontalAdvance(QString::number(m_sp.frequency));
    int height = metrics.height();
    painter->setPen(QPen(Qt::white,1,Qt::SolidLine));
    painter->drawText(QPointF(this->rect().width()/2-width/2,this->rect().height()-height/2),QString::number(m_sp.frequency));

}
