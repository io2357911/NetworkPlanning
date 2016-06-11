#include "algorithms.h"

void PlanningAlgoritms::NetworkGraphAlgoritm::compute(NetworkGraph *graph) {
    if (graph->isNull()) return;
    if (!graph->firstVertex()) return;

    IEvent *event;

    // проход в прямом направлении
    reset(graph);
    event = nextForward(graph);
    while (event != NULL) {
        event->setEarlyTime(maxEarlyTime(event));
        event->setIsCalculated(true);

        event = nextForward(graph);
    }

    // проход в обратном направлении
    reset(graph);
    event = nextBackward(graph);
    while (event != NULL) {
        event->setLateTime(minLateTime(event));
        event->setReserve(event->getLateTime() - event->getEarlyTime());
        event->setIsCalculated(true);

        event = nextBackward(graph);
    }
}

bool PlanningAlgoritms::NetworkGraphAlgoritm::reset(NetworkGraph *graph) {
    QVector<IEvent*> vertices = graph->vertices();
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i]->setIsCalculated(false);
    }
    return true;
}

IEvent *PlanningAlgoritms::NetworkGraphAlgoritm::nextForward(NetworkGraph *graph) {
    QVector<IEvent*> events = graph->vertices();

    for (int i = 0; i < events.size(); i++) {
        IEvent *event = events[i];

        if (event->getIsCalculated())
            continue;

        if (event->isFirst())
            return event;

        if (allAreCalculated(event->previousVertices())) {
            return event;
        }
    }
    return NULL;
}

IEvent *PlanningAlgoritms::NetworkGraphAlgoritm::nextBackward(NetworkGraph *graph) {
    QVector<IEvent*> events = graph->vertices();

    for (int i = 0; i < events.size(); i++) {
        IEvent *event = events[i];

        if (event->getIsCalculated())
            continue;

        if (event->isLast())
            return event;

        if (allAreCalculated(event->nextVertices())) {
            return event;
        }
    }
    return NULL;
}

int PlanningAlgoritms::NetworkGraphAlgoritm::maxEarlyTime(IEvent *event) {
    if (event->isFirst())
        return 0;

    int maxTime = 0;
    QVector<IWork*> works = event->previousEdges();
    for (int i = 0; i < works.size(); i++) {
        IWork *prevWork = works[i];
        IEvent *prevEvent = works[i]->vertex1();

        int time = prevEvent->getEarlyTime() + prevWork->getTime();
        if (time > maxTime)
            maxTime = time;
    }
    return maxTime;
}

int PlanningAlgoritms::NetworkGraphAlgoritm::minLateTime(IEvent *event) {
    if (event->isLast())
        return event->getEarlyTime();

    int minTime = pow(2, 8 * sizeof(int)) / 2 - 1;
    QVector<IWork*> edges = event->nextEdges();
    for (int i = 0; i < edges.size(); i++) {
        IWork *nextWork = edges[i];
        IEvent *nextEvent = edges[i]->vertex2();

        int time = nextEvent->getLateTime() - nextWork->getTime();
        if (time < minTime)
            minTime = time;
    }
    return minTime;
}

bool PlanningAlgoritms::NetworkGraphAlgoritm::allAreCalculated(const QVector<IEvent *> &events) {
    for (int i = 0; i < events.size(); i++) {
        if (!events[i]->getIsCalculated())
            return false;
    }
    return true;
}

QVector<IWork*> PlanningAlgoritms::CriticalPathAlgorithm::compute(NetworkGraph *graph)
{
    QVector<IWork*> critPath;
    if (graph->isNull()) return critPath;

    IEvent *event = graph->firstVertex();
    if (!event) return critPath;

    IEvent *lastEvent = graph->lastVertex();
    if (!lastEvent) return critPath;

    // рассчитаем полные резервы работ и определим, лежат ли они на критическом пути
    QVector<IWork*> works = graph->edges();
    for (int i = 0; i < works.size(); i++) {
        IEvent *firstEvent = works[i]->vertex1();
        IEvent *secondEvent = works[i]->vertex2();
        IWork *work = works[i];

        int fullReserve = secondEvent->getLateTime() - firstEvent->getEarlyTime() - work->getTime();

        work->setFullReserve(fullReserve);
        work->setIsCritical(firstEvent->getReserve() == 0 &&
                            secondEvent->getReserve() == 0 &&
                            work->getFullReserve() == 0);
    }

    // построим критический путь
    while (event != lastEvent) {
        QVector<IWork*> nextEdges = event->nextEdges();
        for (int i = 0; i < nextEdges.size(); i++) {
            IWork *work = nextEdges[i];

            if (work->isCritical()) {
                critPath.append(nextEdges[i]);
                event = nextEdges[i]->vertex2();
                break;
            }
        }
    }

    return critPath;
}
