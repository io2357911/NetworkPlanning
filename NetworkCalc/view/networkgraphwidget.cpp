#include "networkgraphwidget.h"
#include "ui_networkgraphwidget.h"
#include <QMenu>
#include <QPainter>

NetworkGraphWidget::NetworkGraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkGraphWidget),
    firstConnectEvent(NULL)
{
    ui->setupUi(this);

    aNewEvent = new QAction(tr("Новое событие"), this);
    aNewEvent->setShortcuts(QKeySequence::New);
    aNewEvent->setStatusTip(tr("Добавить новое событие"));
    connect(aNewEvent, SIGNAL(triggered()), this, SLOT(onNewEvent()));
}

NetworkGraphWidget::~NetworkGraphWidget()
{
    delete ui;
}

void NetworkGraphWidget::paintEvent(QPaintEvent *evt)
{
    QPainter painter(this);

    for (int i = 0; i < works.size(); i++) {
        works[i]->draw(&painter);
    }
}

void NetworkGraphWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() & Qt::Key_Escape) {
        firstConnectEvent = NULL;
    }
}

void NetworkGraphWidget::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(aNewEvent);
    menu.exec(event->globalPos());
}

void NetworkGraphWidget::onNewEvent()
{
    EventWidget *widget = new EventWidget(this);
    widget->move(mapFromGlobal(QCursor::pos()) - QPoint(widget->width()/2, widget->height()/2));
    widget->show();

    connect(widget, SIGNAL(clicked(EventWidget*)), this, SLOT(onEventClicked(EventWidget*)));
    connect(widget, SIGNAL(moved(EventWidget*)), this, SLOT(onEventMoved(EventWidget*)));
    connect(widget, SIGNAL(connectMe(EventWidget*)), this, SLOT(onEventStartConnect(EventWidget*)));
    connect(widget, SIGNAL(properties(EventWidget*)), this, SLOT(onEventProperties(EventWidget*)));
    connect(widget, SIGNAL(deleteMe(EventWidget*)), this, SLOT(onEventDelete(EventWidget*)));

    events.append(widget);
}

void NetworkGraphWidget::onEventClicked(EventWidget *event)
{
    if (!firstConnectEvent) return;

    WorkWidget *work = new WorkWidget(this);
    work->setFirstEvent(firstConnectEvent);
    work->setSecondEvent(event);
    work->show();

    connect(work, SIGNAL(deleteMe(WorkWidget*)), this, SLOT(onWorkDelete(WorkWidget*)));
    connect(work, SIGNAL(properties(WorkWidget*)), this, SLOT(onWorkProperties(WorkWidget*)));

    works.append(work);

    repaint();

    firstConnectEvent = NULL;
}

void NetworkGraphWidget::wheelEvent(QWheelEvent *event)
{
    for (int i = 0; i < events.size(); i++) {
        events[i]->wheelEvent(event);
    }
}

void NetworkGraphWidget::onEventMoved(EventWidget */*widget*/)
{
    repaint();
}

void NetworkGraphWidget::onEventStartConnect(EventWidget *widget)
{
    firstConnectEvent = widget;
}

void NetworkGraphWidget::onEventProperties(EventWidget *widget)
{

}

void NetworkGraphWidget::onEventDelete(EventWidget *widget)
{
    events.removeAll(widget);
    widget->close();
}

void NetworkGraphWidget::onWorkProperties(WorkWidget *widget)
{

}

void NetworkGraphWidget::onWorkDelete(WorkWidget *widget)
{
    works.removeAll(widget);
    widget->close();
    repaint();
}
