#include "networkgraphwidget.h"
#include "ui_networkgraphwidget.h"
#include <QMenu>
#include <QPainter>
#include <QMessageBox>

using namespace Math::Planning::Algorithms;

NetworkGraphWidget::NetworkGraphWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NetworkGraphWidget),
    firstConnectEvent(NULL),
    eventsCounter(0),
    worksCounter(0),
    resourseCounter(0) {

    ui->setupUi(this);

    aNewEvent = new QAction(tr("Новое событие"), this);
    aNewEvent->setShortcuts(QKeySequence::New);
    aNewEvent->setStatusTip(tr("Добавить новое событие"));
    connect(aNewEvent, SIGNAL(triggered()), this, SLOT(onNewEvent()));

    aTest = new QAction(tr("Тест"), this);
    connect(aTest, SIGNAL(triggered()), this, SLOT(onTest()));

    dWork.setResourses(&resourses);
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

//void NetworkGraphWidget::setAssingsWidget(AssignsWidget *wAssigns)
//{
//    wAssigns->setWorkers(&workers);
//    wAssigns->setWorks(&works);
//    connect(this, SIGNAL(showAssigns()), wAssigns, SLOT(show()));
//    connect(this, SIGNAL(workersChanged()), wAssigns, SLOT(updateWorkers()));
//    connect(this, SIGNAL(worksChanged()), wAssigns, SLOT(updateWorks()));

//    connect(wAssigns, SIGNAL(assignChanged()), &dWork, SLOT(updateWork()));
//}

void NetworkGraphWidget::setResoursesWidget(ResoursesWidget *wResourses) {
    wResourses->setResourses(&resourses);
    connect(wResourses, SIGNAL(newResourse()), this, SLOT(onNewResourse()));
    connect(wResourses, SIGNAL(deleteResourse(WorkResourse*)), this, SLOT(onDeleteResourse(WorkResourse*)));
//    connect(this, SIGNAL(showWorkers()), wResourses, SLOT(show()));
    connect(this, SIGNAL(resoursesChanged()), wResourses, SLOT(onResoursesChanged()));
//    connect(this, SIGNAL(worksChanged()), wResourses, SLOT(updateWorks()));

    connect(wResourses, SIGNAL(resourseChanged()), &dWork, SLOT(updateWork()));
}

void NetworkGraphWidget::newGraph() {
    eventsCounter = worksCounter = 0;

    for (int i = 0; i < events.size(); i++) {
        events[i]->close();
    }
    for (int i = 0; i < works.size(); i++) {
        works[i]->close();
    }
    resourses.clear();
    events.clear();
    works.clear();
    graph.clear();

    repaint();
}

void NetworkGraphWidget::openGraph(QString fileName) {
    newGraph();

    QStringList keys = {
        "resourses",
        "events",
        "works"
    };

    INI::Settings setts = INI::restore(keys, fileName, "Graph");

    if (!(setts["resourses"].isValid() &&
          setts["events"].isValid() &&
          setts["works"].isValid())) return;

    // ресурсы
    QMap<QString, Resourse*> nameToResourses;
    QStringList resoursesList = setts["resourses"].toString().split(",", QString::SkipEmptyParts);
    for (int i = 0; i < resoursesList.size(); i++) {
        QString name = resoursesList[i];

        WorkResourse* res = new WorkResourse();
        res->setName(name);
        res->restore(fileName);

        resourses.append(res);

        nameToResourses[name] = res;
    }

    // события
    QMap<QString, EventWidget*> nameToEvents;
    QStringList eventsList = setts["events"].toString().split(",", QString::SkipEmptyParts);
    for (int i = 0; i < eventsList.size(); i++) {
        QString name = eventsList[i];

        EventWidget *event = createEventWidget();
        event->setName(name);
        event->restore(fileName);

        graph.addVertex(event);
        events.append(event);

        nameToEvents[name] = event;
    }

    // работы
    QStringList worksList = setts["works"].toString().split(",", QString::SkipEmptyParts);
    for (int i = 0; i < worksList.size(); i++) {
        QString name = worksList[i];
        setts = INI::restore({"firstEvent","secondEvent","resourseName"}, fileName, QString("Work_%1").arg(name));

        WorkWidget *work = createWorkWidget(
                    nameToEvents[setts["firstEvent"].toString()],
                    nameToEvents[setts["secondEvent"].toString()]
        );
        work->setName(name);
        work->setResourse(nameToResourses.value(setts["resourseName"].toString(), 0));
        work->restore(fileName);

        graph.addEdge(work);
    }

    repaint();

    emit resoursesChanged();
}

void NetworkGraphWidget::saveGraph(QString fileName) {
    QString resoursesNames;
    for (int i = 0; i < resourses.size(); i++) {
        resourses[i]->store(fileName);
        resoursesNames += QString("%1,").arg(resourses[i]->name());
    }
    QString eventsNames;
    for (int i = 0; i < events.size(); i++) {
        events[i]->store(fileName);
        eventsNames += QString("%1,").arg(events[i]->name());
    }
    QString worksNames;
    for (int i = 0; i < works.size(); i++) {
        works[i]->store(fileName);
        worksNames += QString("%1,").arg(works[i]->name());
    }

    INI::Settings setts;
    setts["resourses"] = resoursesNames;
    setts["events"] = eventsNames;
    setts["works"] = worksNames;

    INI::store(setts, fileName, "Graph");
}

void NetworkGraphWidget::computePert() {

    if (!PertNetworkAlgorithm(&graph).compute()) {
        qDebug("computePert - FAILED");
        return;
    }

    emit graphComputed(&graph);

    repaint();
}

void NetworkGraphWidget::computeMonteCarlo() {
    if (!MonteCarloNetworkAlgorithm(&graph, 100).compute()) {
        qDebug("computeMoteCarlo - FAILED");
        return;
    }

    emit graphComputed(&graph);

    repaint();
}

//void NetworkGraphWidget::computeAssigns() {
//    QVector<IWork *> works;
//    for (int i = 0; i < this->works.size(); i++)
//        works.append(this->works[i]);

//    PlanningAlgoritms::HungarianAlgorithm().compute(workers, works);

//    emit worksChanged();
//    computeNetworkGraph();
//}

//void NetworkGraphWidget::computeAssigns(int maxTime)
//{
//    QVector<IWork *> works;
//    for (int i = 0; i < this->works.size(); i++)
//        works.append(this->works[i]);

//    if (!NetworkPlanningAlgorithm(graph, workers, works, maxTime).compute()) {
//        QMessageBox::warning(NULL, "Ошибка рассчет назначений", "Задача не имеет решения");
//    }

//    emit worksChanged();
//    computeNetworkGraph();
//}

void NetworkGraphWidget::onNewResourse() {
    WorkResourse* resourse = new WorkResourse(QString("Ресурс-%1").arg(++resourseCounter));
    resourses.append(resourse);

    emit resoursesChanged();
}

void NetworkGraphWidget::onDeleteResourse(WorkResourse *resourse) {
    resourses.removeAll(resourse);

    for (int i = 0; i < works.size(); i++)
        if (works[i]->resourse() == resourse)
            works[i]->setResourse(0);

    emit resoursesChanged();

    delete resourse;
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

void NetworkGraphWidget::onWorkDelete(WorkWidget *widget) {
    graph.deleteEdge(widget);
    works.removeAll(widget);

//    for (int i = 0; i < workers.size(); i++) workers[i]->deleteWork(widget);
//    if (dWork.getWork() == dynamic_cast<IWork*>(widget)) dWork.setWork(NULL);
    emit worksChanged();

    widget->close();
    repaint();
}

EventWidget *NetworkGraphWidget::createEventWidget()
{
    eventsCounter++;

    EventWidget *event = new EventWidget(this);
    event->setName(QString::number(eventsCounter));
    event->move(mapFromGlobal(QCursor::pos()) - QPoint(event->width()/2, event->height()/2));
    event->show();

    connect(event, SIGNAL(clicked(EventWidget*)), this, SLOT(onEventClicked(EventWidget*)));
    connect(event, SIGNAL(moved(EventWidget*)), this, SLOT(onEventMoved(EventWidget*)));
    connect(event, SIGNAL(connectMe(EventWidget*)), this, SLOT(onEventStartConnect(EventWidget*)));
    connect(event, SIGNAL(properties(EventWidget*)), this, SLOT(onEventProperties(EventWidget*)));
    connect(event, SIGNAL(deleteMe(EventWidget*)), this, SLOT(onEventDelete(EventWidget*)));

    return event;
}

WorkWidget *NetworkGraphWidget::createWorkWidget(EventWidget *firstEvent, EventWidget *secondEvent) {
    worksCounter++;

    WorkWidget *work = new WorkWidget(this);
    work->setName(QString("Работа-%1").arg(worksCounter));
    work->setFirstEvent(firstEvent);
    work->setSecondEvent(secondEvent);
    work->show();

    works.append(work);

//    for (int i = 0; i < workers.size(); i++) workers[i]->addWork(work);
    emit worksChanged();

    connect(work, SIGNAL(deleteMe(WorkWidget*)), this, SLOT(onWorkDelete(WorkWidget*)));
    connect(work, SIGNAL(properties(WorkWidget*)), this, SLOT(onWorkProperties(WorkWidget*)));

    return work;
}
