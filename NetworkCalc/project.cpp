#include "project.h"
#include "tools/ini.h"


ProjectWork::ProjectWork(Event *event1, Event *event2, QString name)
    : Work(event1, event2, name)
{}


ProjectEvent::ProjectEvent(QString name)
    : Event(name)
{}


ProjectResourse::ProjectResourse(QString name, double quantity, double cost)
    : Resourse(name, quantity, cost)
{}

void ProjectResourse::store(QString file) {
    INI::Settings setts;

    setts["name"] = name();
    setts["quantity"] = quantity();
    setts["cost"] = cost();

    INI::store(setts, file, QString("Resourse_%1").arg(name()));
}

void ProjectResourse::restore(QString file) {
    QStringList keys = {
        "name",
        "quantity",
        "cost",
    };

    INI::Settings setts = INI::restore(keys, file, QString("Resourse_%1").arg(name()));

    if (setts["name"].isValid())
        setName(setts["name"].toString());

    if (setts["quantity"].isValid())
        setQuantity(setts["quantity"].toDouble());

    if (setts["cost"].isValid())
        setCost(setts["cost"].toDouble());
}


Project::Project(QObject *parent)
    : QObject(parent),
      m_factoryEvent(0), m_cntEvents(0),
      m_factoryWork(0), m_cntWorks(0),
      m_factoryResourse(0), m_cntResourse(0) {
}

Project *Project::instance() {
    static Project proj;
    return &proj;
}

void Project::reset() {
    m_cntEvents = m_cntWorks = m_cntResourse = 0;

    for (int i = 0; i < m_events.size(); i++) {
        m_events[i]->release();
    }
    for (int i = 0; i < m_works.size(); i++) {
        m_works[i]->release();
    }
    m_resourses.clear();
    m_events.clear();
    m_works.clear();
    m_graph.clear();

    emit graphChanged();
}

void Project::store(QString fileName) {
    QString resoursesNames;
    for (int i = 0; i < m_resourses.size(); i++) {
        m_resourses[i]->store(fileName);
        resoursesNames += QString("%1,").arg(m_resourses[i]->name());
    }
    QString eventsNames;
    for (int i = 0; i < m_events.size(); i++) {
        m_events[i]->store(fileName);
        eventsNames += QString("%1,").arg(m_events[i]->name());
    }
    QString worksNames;
    for (int i = 0; i < m_works.size(); i++) {
        m_works[i]->store(fileName);
        worksNames += QString("%1,").arg(m_works[i]->name());
    }

    INI::Settings setts;
    setts["resourses"] = resoursesNames;
    setts["events"] = eventsNames;
    setts["works"] = worksNames;

    INI::store(setts, fileName, "Graph");
}

void Project::restore(QString fileName) {
    reset();

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

        ProjectResourse* res = createResourse();
        res->setName(name);
        res->restore(fileName);

        nameToResourses[name] = res;
    }

    // события
    QMap<QString, ProjectEvent*> nameToEvents;
    QStringList eventsList = setts["events"].toString().split(",", QString::SkipEmptyParts);
    for (int i = 0; i < eventsList.size(); i++) {
        QString name = eventsList[i];

        ProjectEvent *event = createEvent();
        event->setName(name);
        event->restore(fileName);

        nameToEvents[name] = event;
    }

    // работы
    QStringList worksList = setts["works"].toString().split(",", QString::SkipEmptyParts);
    for (int i = 0; i < worksList.size(); i++) {
        QString name = worksList[i];
        setts = INI::restore({"firstEvent","secondEvent","resourseName"}, fileName, QString("Work_%1").arg(name));

        ProjectWork *work = createWork(
                nameToEvents[setts["firstEvent"].toString()],
                nameToEvents[setts["secondEvent"].toString()]
        );
        work->setName(name);
        work->setResourse(nameToResourses.value(setts["resourseName"].toString(), 0));
        work->restore(fileName);
    }

    m_graph.setName(fileName);

    emit resoursesChanged();
    emit graphChanged();
}

NetworkGraph *Project::graph() {
    return &m_graph;
}

ProjectEvent* Project::createEvent() {
    if (!m_factoryEvent) return 0;

    ProjectEvent *event = m_factoryEvent->createEvent();
    if (!event) return 0;

    event->setName(QString("%1").arg(++m_cntEvents));

    m_events.append(event);
    m_graph.addVertex(event);

    emit eventsChanged();

    return event;
}

ProjectWork* Project::createWork(Event *event1, Event *event2) {
    if (!m_factoryWork) return 0;

    ProjectWork *work = m_factoryWork->createWork(event1, event2);
    if (!work) return 0;

    work->setName(QString("Работа-%1").arg(++m_cntWorks));

    m_works.append(work);
    m_graph.addEdge(work);

    emit worksChanged();

    return work;
}

ProjectResourse* Project::createResourse() {
    if (!m_factoryResourse) return 0;

    ProjectResourse *resourse = m_factoryResourse->createResourse();
    if (!resourse) return 0;

    resourse->setName(QString("Ресурс-%1").arg(++m_cntWorks));

    m_resourses.append(resourse);

    emit resoursesChanged();

    return resourse;
}

void Project::deleteEvent(ProjectEvent *event) {
    m_graph.deleteVertex(event);
    m_events.removeAll(event);
}

void Project::deleteWork(ProjectWork *work) {
    m_graph.deleteEdge(work);
    m_works.removeAll(work);

//    for (int i = 0; i < workers.size(); i++) workers[i]->deleteWork(widget);
//    if (dWork.getWork() == dynamic_cast<IWork*>(widget)) dWork.setWork(NULL);
    emit worksChanged();
}

void Project::deleteResourse(ProjectResourse *resourse) {
    m_resourses.removeAll(resourse);

    for (int i = 0; i < m_works.size(); i++)
        if (m_works[i]->resourse() == resourse)
            m_works[i]->setResourse(0);

    emit resoursesChanged();

    delete resourse;
}

QVector<ProjectEvent*>* Project::events() {
    return &m_events;
}

void Project::setEventFactory(ProjectEventFactory *factory) {
    m_factoryEvent = factory;
}

QVector<ProjectWork*>* Project::works() {
    return &m_works;
}

void Project::setWorkFactory(ProjectWorkFactory *factory) {
    m_factoryWork = factory;
}

QVector<ProjectResourse*>* Project::resourses() {
    return &m_resourses;
}

void Project::setResourseFactory(ProjectResourseFactory *factory) {
    m_factoryResourse = factory;
}
