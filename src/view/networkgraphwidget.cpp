#include "networkgraphwidget.h"
#include "ui_networkgraphwidget.h"
#include <QMenu>
#include <QPainter>
#include <QMessageBox>

using namespace Math::Planning::Algorithms;

NetworkGraphWidget::NetworkGraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkGraphWidget),
    firstConnectEvent(NULL) {

    ui->setupUi(this);

    aNewEvent = new QAction(tr("Новое событие"), this);
    aNewEvent->setShortcuts(QKeySequence::New);
    aNewEvent->setStatusTip(tr("Добавить новое событие"));
    connect(aNewEvent, SIGNAL(triggered()), this, SLOT(onNewEvent()));

    aTest = new QAction(tr("Тест"), this);
    connect(aTest, SIGNAL(triggered()), this, SLOT(onTest()));
}

NetworkGraphWidget::~NetworkGraphWidget() {
    delete ui;
}

void NetworkGraphWidget::paintEvent(QPaintEvent */*evt*/) {
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 2));

    QVector<ProjectWork*>* works = Project::instance()->works();
    for (int i = 0; i < works->size(); i++) {
        WorkWidget* work = dynamic_cast<WorkWidget*>(works->at(i));
        work->draw(&painter);
    }
}

void NetworkGraphWidget::keyPressEvent(QKeyEvent *event) {
    if (event->key() & Qt::Key_Escape) {
        firstConnectEvent = NULL;
    }
}

void NetworkGraphWidget::contextMenuEvent(QContextMenuEvent *event) {
    QMenu menu(this);
    menu.addAction(aNewEvent);
    menu.addAction(aTest);
    menu.exec(event->globalPos());
}

void NetworkGraphWidget::onNewEvent() {
    Project::instance()->createEvent();
}

void NetworkGraphWidget::onTest() {
//    Debug::printGraph(graph);
}

void NetworkGraphWidget::onEventClicked(EventWidget *event) {
    if (!firstConnectEvent) return;
    if (firstConnectEvent == event) return;

    Project::instance()->createWork(firstConnectEvent, event);

    repaint();

    firstConnectEvent = NULL;
}

void NetworkGraphWidget::wheelEvent(QWheelEvent *event) {
    QVector<ProjectEvent*>* events = Project::instance()->events();
    for (int i = 0; i < events->size(); i++) {
        EventWidget* pevent = dynamic_cast<EventWidget*>(events->at(i));
        pevent->wheelEvent(event);
    }
}

ProjectWork *NetworkGraphWidget::createWork(Event *firstEvent, Event *secondEvent) {
    WorkWidget *work = new WorkWidget(this);
    work->setFirstEvent(dynamic_cast<EventWidget*>(firstEvent));
    work->setSecondEvent(dynamic_cast<EventWidget*>(secondEvent));
    work->show();

    connect(work, SIGNAL(deleteMe(WorkWidget*)), this, SLOT(onWorkDelete(WorkWidget*)));
    connect(work, SIGNAL(properties(WorkWidget*)), this, SLOT(onWorkProperties(WorkWidget*)));

    return work;
}

ProjectEvent *NetworkGraphWidget::createEvent() {
    EventWidget *event = new EventWidget(this);
    event->move(mapFromGlobal(QCursor::pos()) - QPoint(event->width()/2, event->height()/2));
    event->show();

    connect(event, SIGNAL(clicked(EventWidget*)), this, SLOT(onEventClicked(EventWidget*)));
    connect(event, SIGNAL(moved(EventWidget*)), this, SLOT(onEventMoved(EventWidget*)));
    connect(event, SIGNAL(connectMe(EventWidget*)), this, SLOT(onEventStartConnect(EventWidget*)));
    connect(event, SIGNAL(properties(EventWidget*)), this, SLOT(onEventProperties(EventWidget*)));
    connect(event, SIGNAL(deleteMe(EventWidget*)), this, SLOT(onEventDelete(EventWidget*)));

    return event;
}

void NetworkGraphWidget::onGraphChanged() {
    repaint();
}

void NetworkGraphWidget::onEventMoved(EventWidget *) {
    repaint();
}

void NetworkGraphWidget::onEventStartConnect(EventWidget *widget) {
    firstConnectEvent = widget;
}

void NetworkGraphWidget::onEventProperties(EventWidget *widget) {
    dEvent.setEvent(widget);
    dEvent.show();
}

void NetworkGraphWidget::onEventDelete(EventWidget *widget) {
    Project::instance()->deleteEvent(widget);
    widget->close();
}

void NetworkGraphWidget::onWorkProperties(WorkWidget *widget) {
    dWork.setWork(widget);
    dWork.show();
}

void NetworkGraphWidget::onWorkDelete(WorkWidget *widget) {
    Project::instance()->deleteWork(widget);

    widget->close();
    repaint();
}
