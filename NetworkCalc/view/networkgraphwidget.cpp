#include "networkgraphwidget.h"
#include "ui_networkgraphwidget.h"
#include <QMenu>
#include <QPainter>

NetworkGraphWidget::NetworkGraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkGraphWidget),
    firstConnectEvent(NULL),
    eventsCounter(0),
    worksCounter(0),
    workerCounter(0)
{
    ui->setupUi(this);

    aNewEvent = new QAction(tr("Новое событие"), this);
    aNewEvent->setShortcuts(QKeySequence::New);
    aNewEvent->setStatusTip(tr("Добавить новое событие"));
    connect(aNewEvent, SIGNAL(triggered()), this, SLOT(onNewEvent()));

    aTest = new QAction(tr("Тест"), this);
    connect(aTest, SIGNAL(triggered()), this, SLOT(onTest()));
}

NetworkGraphWidget::~NetworkGraphWidget()
{
    delete ui;
}

void NetworkGraphWidget::paintEvent(QPaintEvent */*evt*/)
{
    QPainter painter(this);
    painter.setPen(QPen(Qt::black, 2));
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
    menu.addAction(aTest);
    menu.exec(event->globalPos());
}

void NetworkGraphWidget::onNewEvent()
{
    EventWidget *event = createEventWidget();
    graph.addVertex(event);
    events.append(event);
}

void NetworkGraphWidget::onTest()
{
    Debug::printGraph(graph);
}

void NetworkGraphWidget::onEventClicked(EventWidget *event)
{
    if (!firstConnectEvent) return;
    if (firstConnectEvent == event) return;

    WorkWidget *work = createWorkWidget(firstConnectEvent, event);

    graph.addEdge(work);

    repaint();

    firstConnectEvent = NULL;
}

void NetworkGraphWidget::wheelEvent(QWheelEvent *event)
{
    for (int i = 0; i < events.size(); i++) {
        events[i]->wheelEvent(event);
    }
}

void NetworkGraphWidget::setAssingsWidget(AssignsWidget *wAssigns)
{
    wAssigns->setWorkers(&workers);
    wAssigns->setWorks(&works);
    connect(this, SIGNAL(showAssigns()), wAssigns, SLOT(show()));
    connect(this, SIGNAL(workersChanged()), wAssigns, SLOT(updateWorkers()));
    connect(this, SIGNAL(worksChanged()), wAssigns, SLOT(updateWorks()));

    connect(wAssigns, SIGNAL(assignChanged()), &dWork, SLOT(updateWork()));
}

void NetworkGraphWidget::setWorkersWidget(WorkersWidget *wWorkers)
{
    wWorkers->setWorkers(&workers);
    connect(wWorkers, SIGNAL(newWorker()), this, SLOT(newWorker()));
    connect(wWorkers, SIGNAL(deleteWorker(IWorker*)), this, SLOT(deleteWorker(IWorker*)));
    connect(this, SIGNAL(showWorkers()), wWorkers, SLOT(show()));
    connect(this, SIGNAL(workersChanged()), wWorkers, SLOT(updateWorkers()));
    connect(this, SIGNAL(worksChanged()), wWorkers, SLOT(updateWorks()));

    connect(wWorkers, SIGNAL(workerChanged()), &dWork, SLOT(updateWork()));
}

void NetworkGraphWidget::newGraph()
{
    eventsCounter = worksCounter = 0;

    for (int i = 0; i < events.size(); i++) {
        events[i]->close();
    }
    for (int i = 0; i < works.size(); i++) {
        works[i]->close();
    }
    events.clear();
    works.clear();
    graph.clear();

    repaint();
}

void NetworkGraphWidget::openGraph(QString fileName)
{
    newGraph();

    QStringList keys = {
        "events",
        "works"
    };

    INI::Settings setts = INI::restore(keys, fileName, "Graph");

    if (!(setts["events"].isValid() && setts["works"].isValid())) return;

    QMap<int, EventWidget*> idToEvents;
    QStringList eventsList = setts["events"].toString().split(",", QString::SkipEmptyParts);

    for (int i = 0; i < eventsList.size(); i++) {
        int id = eventsList[i].toInt();

        EventWidget *event = createEventWidget();
        event->setID(id);
        event->restore(fileName);

        graph.addVertex(event);
        events.append(event);

        idToEvents[id] = event;
    }

    QStringList worksList = setts["works"].toString().split(",", QString::SkipEmptyParts);

    for (int i = 0; i < worksList.size(); i++) {
        int id = worksList[i].toInt();
        setts = INI::restore({"firstEvent","secondEvent"}, fileName, QString("Work_%1").arg(id));

        WorkWidget *work = createWorkWidget(
                    idToEvents[setts["firstEvent"].toInt()],
                    idToEvents[setts["secondEvent"].toInt()]
        );
        work->setID(id);
        work->restore(fileName);

        graph.addEdge(work);
    }
    repaint();
}

void NetworkGraphWidget::saveGraph(QString fileName)
{
    QString eventsId;
    for (int i = 0; i < events.size(); i++) {
        events[i]->store(fileName);
        eventsId += QString("%1,").arg(events[i]->getID());
    }
    QString worksId;
    for (int i = 0; i < works.size(); i++) {
        works[i]->store(fileName);
        worksId += QString("%1,").arg(works[i]->getID());
    }

    INI::Settings setts;
    setts["events"] = eventsId;
    setts["works"] = worksId;

    INI::store(setts, fileName, "Graph");
}

void NetworkGraphWidget::computeNetworkGraph()
{
    PlanningAlgoritms::NetworkGraphAlgoritm().compute(&graph);
    PlanningAlgoritms::CriticalPathAlgorithm().compute(&graph);
    repaint();
}

void NetworkGraphWidget::newWorker()
{
    IWorker *worker = new IWorker(++workerCounter);
    workers.append(worker);
    for (int i = 0; i < works.size(); i++) worker->addWork(works[i]);

    emit workersChanged();
}

void NetworkGraphWidget::deleteWorker(IWorker *worker)
{
    workers.removeAll(worker);

    for (int i = 0; i < works.size(); i++)
        if (works[i]->getWorker() == worker)
            works[i]->setWorker(0);

    emit workersChanged();

    delete worker;
}

void NetworkGraphWidget::onEventMoved(EventWidget *)
{
    repaint();
}

void NetworkGraphWidget::onEventStartConnect(EventWidget *widget)
{
    firstConnectEvent = widget;
}

void NetworkGraphWidget::onEventProperties(EventWidget *widget)
{
    dEvent.setEvent(widget);
    dEvent.show();
}

void NetworkGraphWidget::onEventDelete(EventWidget *widget)
{
    graph.deleteVertex(widget);
    events.removeAll(widget);
    widget->close();
}

void NetworkGraphWidget::onWorkProperties(WorkWidget *widget)
{
    dWork.setWork(widget);
    dWork.show();
}

void NetworkGraphWidget::onWorkDelete(WorkWidget *widget)
{
    graph.deleteEdge(widget);
    works.removeAll(widget);

    for (int i = 0; i < workers.size(); i++) workers[i]->deleteWork(widget);
    if (dWork.getWork() == dynamic_cast<IWork*>(widget)) dWork.setWork(NULL);
    emit worksChanged();

    widget->close();
    repaint();
}

EventWidget *NetworkGraphWidget::createEventWidget()
{
    eventsCounter++;

    EventWidget *event = new EventWidget(this);
    event->setID(eventsCounter);
    event->move(mapFromGlobal(QCursor::pos()) - QPoint(event->width()/2, event->height()/2));
    event->show();

    connect(event, SIGNAL(clicked(EventWidget*)), this, SLOT(onEventClicked(EventWidget*)));
    connect(event, SIGNAL(moved(EventWidget*)), this, SLOT(onEventMoved(EventWidget*)));
    connect(event, SIGNAL(connectMe(EventWidget*)), this, SLOT(onEventStartConnect(EventWidget*)));
    connect(event, SIGNAL(properties(EventWidget*)), this, SLOT(onEventProperties(EventWidget*)));
    connect(event, SIGNAL(deleteMe(EventWidget*)), this, SLOT(onEventDelete(EventWidget*)));

    return event;
}

WorkWidget *NetworkGraphWidget::createWorkWidget(EventWidget *firstEvent, EventWidget *secondEvent)
{
    worksCounter++;

    WorkWidget *work = new WorkWidget(this);
    work->setID(worksCounter);
    work->setFirstEvent(firstEvent);
    work->setSecondEvent(secondEvent);
    work->show();

    works.append(work);

    for (int i = 0; i < workers.size(); i++) workers[i]->addWork(work);
    emit worksChanged();

    connect(work, SIGNAL(deleteMe(WorkWidget*)), this, SLOT(onWorkDelete(WorkWidget*)));
    connect(work, SIGNAL(properties(WorkWidget*)), this, SLOT(onWorkProperties(WorkWidget*)));

    return work;
}

