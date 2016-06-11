#include "workwidget.h"
#include "ui_workwidget.h"
#include <QMenu>

WorkWidget::WorkWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkWidget),
    firstEvent(NULL),
    secondEvent(NULL),
    id (0),
    cost(0),
    fullReserve(0),
    m_isCritical(false),
    m_isVirtual(false)
{
    ui->setupUi(this);

    aProperties = new QAction(tr("Свойства"), this);
    aProperties->setShortcuts(QKeySequence::New);
    aProperties->setStatusTip(tr("Настраиваемые свойства работы"));
    connect(aProperties, SIGNAL(triggered()), this, SLOT(onProperties()));

    aDelete = new QAction(tr("Удалить"), this);
    aDelete->setShortcuts(QKeySequence::Delete);
    aDelete->setStatusTip(tr("Удалить работу"));
    connect(aDelete, SIGNAL(triggered()), this, SLOT(onDelete()));
}

WorkWidget::~WorkWidget()
{
    delete ui;
}

void WorkWidget::draw(QPainter *painter)
{
    if (!(firstEvent && secondEvent)) return;

    QPoint c1 = firstEvent->center();
    QPoint c2 = secondEvent->center();

    painter->pen();

    QPoint loc((c1.x()+c2.x())/2, (c1.y()+c2.y())/2);

    move(loc - QPoint(width()/2, height()));

    QPen origPen = painter->pen();

    if (isCritical()) {
        if (isVirtual()) {
            painter->setPen(QPen(Qt::red, origPen.width()+1, Qt::DashLine));
        } else {
            painter->setPen(QPen(Qt::red, origPen.width()+1));
        }
    } else if (isVirtual()){
        painter->setPen(QPen(origPen.color(), origPen.width(), Qt::DashLine));
    }
    painter->drawLine(c1, c2);
    painter->setPen(origPen);
}

void WorkWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(aProperties);
    menu.addAction(aDelete);
    menu.exec(event->globalPos());
}

EventWidget *WorkWidget::getFirstEvent() const
{
    return firstEvent;
}

void WorkWidget::setFirstEvent(EventWidget *widget)
{
    firstEvent = widget;
    setVertex1(widget);
    connect(widget, SIGNAL(deleteMe(EventWidget*)), this, SLOT(onDeleteEvent(EventWidget*)));
}

EventWidget *WorkWidget::getSecondEvent() const
{
    return secondEvent;
}

void WorkWidget::setSecondEvent(EventWidget *widget)
{
    secondEvent = widget;
    setVertex2(widget);
    connect(widget, SIGNAL(deleteMe(EventWidget*)), this, SLOT(onDeleteEvent(EventWidget*)));
}

void WorkWidget::store(QString fileName)
{
    INI::Settings setts;

    setts["firstEvent"] = getFirstEvent()->getID();
    setts["secondEvent"] = getSecondEvent()->getID();
    setts["cost"] = getCost();
    setts["time"] = getTime();
    setts["virtual"] = isVirtual();

    INI::store(setts, fileName, QString("Work_%1").arg(getID()));
}

void WorkWidget::restore(QString fileName)
{
    QStringList keys = {
        "cost",
        "time",
        "virtual"
    };

    INI::Settings setts = INI::restore(keys, fileName, QString("Work_%1").arg(getID()));

    if (setts["cost"].isValid())
        setCost(setts["cost"].toInt());

    if (setts["time"].isValid())
        setTime(setts["time"].toInt());

    if (setts["virtual"].isValid())
        setIsVirtual(setts["virtual"].toBool());
}

int WorkWidget::getID() const
{
    return id;
}

void WorkWidget::setID(int value)
{
    id = value;
}

int WorkWidget::getCost() const
{
    return worker ? worker->getCost((IWork*)this)
                  : cost;
}

void WorkWidget::setCost(int value)
{
    cost = value;
}

int WorkWidget::getTime() const
{
    return worker ? worker->getTime((IWork*)this)
                  : ui->lTime->text().toInt();
}

void WorkWidget::setTime(int value)
{
    ui->lTime->setText(QString::number(value));
}

bool WorkWidget::isCritical() const
{
    return m_isCritical;
}

void WorkWidget::setIsCritical(bool value)
{
    m_isCritical = value;
}

bool WorkWidget::isVirtual() const { return m_isVirtual; }

void WorkWidget::setIsVirtual(bool value) {
    m_isVirtual = value;
    ui->lTime->setVisible(!m_isVirtual);
}
