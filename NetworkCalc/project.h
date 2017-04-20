#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>

#include "math/networkgraph.h"

using namespace Math::Planning;


class IPersistent {
public:
    virtual void store(QString file) = 0;
    virtual void restore(QString file) = 0;
};


class IRelease {
public:
    virtual void release() = 0;
};


class ProjectWork :
        public Work,
        public IPersistent,
        public IRelease
{
public:
    ProjectWork(Event *event1 = 0, Event *event2 = 0, QString name = "");
};
class ProjectWorkFactory {
public:
    virtual ProjectWork* createWork(Event *event1 = 0, Event *event2 = 0) = 0;
};


class ProjectEvent :
        public Event,
        public IPersistent,
        public IRelease
{
public:
    ProjectEvent(QString name = "");
};
class ProjectEventFactory {
public:
    virtual ProjectEvent* createEvent() = 0;
};


class ProjectResourse :
        public Resourse,
        public IPersistent,
        public IRelease
{
public:
    ProjectResourse(QString name = "", double quantity = 0, double cost = 0);
    virtual ~ProjectResourse() {}

    // IPersistent interface
    virtual void store(QString file);
    virtual void restore(QString file);

    // IRelease interface
    virtual void release() {}
};
class ProjectResourseFactory {
public:
    virtual ProjectResourse* createResourse() = 0;
};


class Project :
        public QObject,
        public IPersistent,
        public ProjectEventFactory,
        public ProjectWorkFactory,
        public ProjectResourseFactory
{
    Q_OBJECT
public:
    static Project* instance();

    // IPersistent interface
    void store(QString fileName);
    void restore(QString fileName);

    NetworkGraph* graph();

    ProjectEvent* createEvent();
    void deleteEvent(ProjectEvent* event);
    QVector<ProjectEvent*>* events();
    void setEventFactory(ProjectEventFactory *factory);

    ProjectWork* createWork(Event *event1 = 0, Event *event2 = 0);
    void deleteWork(ProjectWork* work);
    QVector<ProjectWork*>* works();
    void setWorkFactory(ProjectWorkFactory *factory);

    ProjectResourse* createResourse();
    void deleteResourse(ProjectResourse* resourse);
    QVector<ProjectResourse*>* resourses();
    void setResourseFactory(ProjectResourseFactory *factory);

signals:
    void graphChanged();

    void eventsChanged();
    void worksChanged();
    void resoursesChanged();

public slots:
    void reset();

private:
    Project(QObject *parent = 0);

private:
    NetworkGraph m_graph;

    QVector<ProjectEvent*> m_events;
    ProjectEventFactory* m_factoryEvent;
    int m_cntEvents;

    QVector<ProjectWork*> m_works;
    ProjectWorkFactory* m_factoryWork;
    int m_cntWorks;

    QVector<ProjectResourse*> m_resourses;
    ProjectResourseFactory* m_factoryResourse;
    int m_cntResourse;
};

#endif // PROJECT_H
