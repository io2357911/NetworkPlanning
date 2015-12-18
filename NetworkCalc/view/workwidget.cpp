#include "workwidget.h"
#include "ui_workwidget.h"
#include <QMenu>

WorkWidget::WorkWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WorkWidget),
    firstEvent(NULL),
    secondEvent(NULL)
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

    QPoint loc((c1.x()+c2.x())/2, (c1.y()+c2.y())/2);

    move(loc - QPoint(width()/2, height()));

    painter->drawLine(c1, c2);
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
    connect(widget, SIGNAL(deleteMe(EventWidget*)), this, SLOT(onDeleteEvent(EventWidget*)));
}

EventWidget *WorkWidget::getSecondEvent() const
{
    return secondEvent;
}

void WorkWidget::setSecondEvent(EventWidget *widget)
{
    secondEvent = widget;
    connect(widget, SIGNAL(deleteMe(EventWidget*)), this, SLOT(onDeleteEvent(EventWidget*)));
}


