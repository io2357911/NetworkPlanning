#include "algorithms.h"

#include <vector>

using namespace std;

#define INF INT_MAX

#define IS_ZERO(dbl, eps) (dbl < eps)

namespace Math {
namespace Planning {
namespace Algorithms {

void NetworkGraphAlgoritm::compute(NetworkGraph *graph) {
    if (graph->isNull()) return;
    if (!graph->firstVertex()) return;


    // TODO: убрать - вставил для проверки
    QVector<Work*> works = graph->edges();
    for (int i = 0; i < works.size(); i++) works[i]->time()->random();


    Event *event;

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
        event->setReserve(event->lateTime() - event->earlyTime());
        event->setIsCalculated(true);

        event = nextBackward(graph);
    }
}

bool NetworkGraphAlgoritm::reset(NetworkGraph *graph) {
    QVector<Event*> vertices = graph->vertices();
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i]->setIsCalculated(false);
    }
    return true;
}

Event *NetworkGraphAlgoritm::nextForward(NetworkGraph *graph) {
    QVector<Event*> events = graph->vertices();

    for (int i = 0; i < events.size(); i++) {
        Event *event = events[i];

        if (event->isCalculated())
            continue;

        if (event->isFirst())
            return event;

        if (allAreCalculated(event->previousVertices())) {
            return event;
        }
    }
    return NULL;
}

Event *NetworkGraphAlgoritm::nextBackward(NetworkGraph *graph) {
    QVector<Event*> events = graph->vertices();

    for (int i = 0; i < events.size(); i++) {
        Event *event = events[i];

        if (event->isCalculated())
            continue;

        if (event->isLast())
            return event;

        if (allAreCalculated(event->nextVertices())) {
            return event;
        }
    }
    return NULL;
}

double NetworkGraphAlgoritm::maxEarlyTime(Event *event) {
    if (event->isFirst())
        return 0;

    double maxTime = 0;
    QVector<Work*> works = event->previousEdges();
    for (int i = 0; i < works.size(); i++) {
        Work *prevWork = works[i];
        Event *prevEvent = works[i]->vertex1();

        double time = prevEvent->earlyTime() + prevWork->time()->value();
        if (time > maxTime)
            maxTime = time;
    }
    return maxTime;
}

double NetworkGraphAlgoritm::minLateTime(Event *event) {
    if (event->isLast())
        return event->earlyTime();

    double minTime = pow(2, 8 * sizeof(int)) / 2 - 1;
    QVector<Work*> edges = event->nextEdges();
    for (int i = 0; i < edges.size(); i++) {
        Work *nextWork = edges[i];
        Event *nextEvent = edges[i]->vertex2();

        double time = nextEvent->lateTime() - nextWork->time()->value();
        if (time < minTime)
            minTime = time;
    }
    return minTime;
}

bool NetworkGraphAlgoritm::allAreCalculated(const QVector<Event *> &events) {
    for (int i = 0; i < events.size(); i++) {
        if (!events[i]->isCalculated())
            return false;
    }
    return true;
}

QVector<Work*> CriticalPathAlgorithm::compute(NetworkGraph *graph)
{
    QVector<Work*> critPath;
    if (graph->isNull()) return critPath;

    Event *event = graph->firstVertex();
    if (!event) return critPath;

    Event *lastEvent = graph->lastVertex();
    if (!lastEvent) return critPath;

    // рассчитаем полные резервы работ и определим, лежат ли они на критическом пути
    QVector<Work*> works = graph->edges();
    for (int i = 0; i < works.size(); i++) {
        Event *firstEvent = works[i]->vertex1();
        Event *secondEvent = works[i]->vertex2();
        Work *work = works[i];

        int fullReserve = secondEvent->lateTime() - firstEvent->earlyTime() - work->time()->value();

        work->setFullReserve(fullReserve);
        work->setIsCritical(IS_ZERO(firstEvent->reserve(), 0.001) &&
                            IS_ZERO(secondEvent->reserve(), 0.001) &&
                            IS_ZERO(work->fullReserve(), 0.001));
//        work->setIsCritical(firstEvent->reserve() == 0 &&
//                            secondEvent->reserve() == 0 &&
//                            work->fullReserve() == 0);
    }

    // построим критический путь
    while (event != lastEvent) {
        QVector<Work*> nextEdges = event->nextEdges();
        for (int i = 0; i < nextEdges.size(); i++) {
            Work *work = nextEdges[i];

            if (work->isCritical()) {
                critPath.append(nextEdges[i]);
                event = nextEdges[i]->vertex2();
                break;
            }
        }
    }

    return critPath;
}

void CostAlgorithm::compute(NetworkGraph *graph)
{
    int cost = 0;
    QVector<Work*> works = graph->edges();
    for (int i = 0; i < works.size(); i++) {
        cost += works[i]->cost();
    }
    graph->setCost(cost);
}

void TimeAlgorithm::compute(NetworkGraph *graph) {
    int time = 0;
    QVector<Work*> works = graph->edges();
    for (int i = 0; i < works.size(); i++) {
        if (works[i]->isCritical())
            time += works[i]->time()->value();
    }
    graph->setTime(time);
}

void MonteCarloModelingAlgorithm::compute(NetworkGraph */*graph*/) {
    int exps = 50;
    for (int i = 0; i < exps; i++) {
//        works[i]->time()->random();

    }
}

void MonteCarloModelingAlgorithm::modelWorks(NetworkGraph *graph) {
    QVector<Work*> works = graph->edges();
    for (int i = 0; i < works.size(); i++) {
        Work* work = works[i];
        work->time()->random();
    }
}

} // namespace Algorithms
} // namespace Planning
} // namespace Math
