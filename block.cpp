#include "block.h"
#include "QtWidgets/qstyleoption.h"
#include <QPen>
#include <QtGui>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QStyle>
#include <QApplication>
#include <QPushButton>

Block::Block(SignalProperties sp, QGraphicsItem *parent)
    :QGraphicsRectItem(parent)
{
    m_sp = sp;

    setFlags(ItemIsSelectable);

    setPen(Qt::NoPen);
    setBrush(m_sp.color);
    setRect(0,0,m_sp.width,30);
    setPos(m_sp.x,m_sp.y);
    setAcceptHoverEvents(true);
}

bool Block::operator<(const Block &other) const
{
    return this<&other;
}

SignalProperties Block::getBlockProperties(){
    m_sp.x = this->x();
    m_sp.y = this->y();
    m_sp.width = this->sceneBoundingRect().width();
    m_sp.time  = m_sp.width*10;
    return m_sp;
}

void Block::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    //emit clicked();
    QGraphicsRectItem::mousePressEvent(event);
}

void Block::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    int ax = this->boundingRect().width()+this->boundingRect().x();
    int start = this->boundingRect().x();

    if(qAbs(event->pos().x()-ax)<=5){
        setCursor(Qt::SizeHorCursor);
    }else if(qAbs(event->pos().x()-start)<=5){
        //setCursor(Qt::SizeAllCursor);
    }else{
        setCursor(Qt::ArrowCursor);
    }
    QGraphicsItem::hoverMoveEvent(event);
}


void Block::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
//    QMenu menu;
//    QMenu noteMenu("Note");
//    noteMenu.setMinimumWidth(100);
//    QAction *c4Action = noteMenu.addAction("C4");
//    QAction *d4Action = noteMenu.addAction("D4");
//    QAction *e4Action = noteMenu.addAction("E4");
//    QAction *f4Action = noteMenu.addAction("F4");
//    QAction *g4Action = noteMenu.addAction("G4");
//    QAction *a4Action = noteMenu.addAction("A4");
//    QAction *b4Action = noteMenu.addAction("B4");
//    QAction *c5Action = noteMenu.addAction("C5");
//    qreal freq;

//    connect(c4Action,&QAction::triggered,this,[=]()mutable{
//        freq = 261;
//        setFrequency(freq);
//    });
//    connect(d4Action,&QAction::triggered,this,[=]()mutable{
//        freq = 293;
//        setFrequency(freq);
//    });
//    connect(e4Action,&QAction::triggered,this,[=]()mutable{
//        freq = 329;
//        setFrequency(freq);
//    });
//    connect(f4Action,&QAction::triggered,this,[=](){
//        setFrequency(349);
//    });
//    connect(g4Action,&QAction::triggered,this,[=](){
//        setFrequency(392);
//    });
//    connect(a4Action,&QAction::triggered,this,[=](){
//        setFrequency(440);
//    });
//    connect(b4Action,&QAction::triggered,this,[=](){
//        setFrequency(493);
//    });
//    connect(c5Action,&QAction::triggered,this,[=](){
//        setFrequency(523);
//    });

//    menu.addMenu(&noteMenu);
//    menu.addAction("Copy");
//    menu.addAction("Paste");
//    menu.addAction("Delete");
//    menu.addAction("Properties");
//    menu.exec(event->screenPos());
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

    if(this->rect().width()>=10){
        QPolygon polygon;
        qreal x = rect().width()-2;
        qreal y = rect().height()-2;
        painter->setBrush(QColor(200,200,200));
        polygon<<QPoint(x,y)<<QPoint(x,y-6)<<QPoint(x-6,y)<<QPoint(x,y);
        painter->drawPolygon(polygon);
    }

    QFont font = QApplication::font();
    QFontMetrics metrics(font);
    int width = metrics.horizontalAdvance(QString::number(m_sp.frequency));
    int height = metrics.height();
    painter->setPen(QPen(Qt::white,1,Qt::SolidLine));
    painter->setFont(QFont("",10));
    if(this->rect().width()>=40){
        painter->drawText(QPointF(this->rect().width()/2-width/2,this->rect().height()-height/1.5),QString::number(m_sp.frequency));
    }

}
