#include "algorithms.h"

#include <vector>

using namespace std;

#define INF INT_MAX

#define IS_ZERO(dbl, eps) (dbl < eps)

namespace Math {
namespace Planning {
namespace Algorithms {

bool NetworkAlgorithm::compute() {
    if (m_graph->isNull()) return false;
    if (!m_graph->firstVertex()) return false;

//    // TODO: убрать - вставил для проверки
//    QVector<Work*> works = m_graph->edges();
//    for (int i = 0; i < works.size(); i++) works[i]->time()->random();

    Event *event;

    // проход в прямом направлении
    reset(m_graph);
    event = nextForward();
    while (event != NULL) {
        event->setEarlyTime(maxEarlyTime(event));
        event->setIsCalculated(true);

        event = nextForward();
    }

    // проход в обратном направлении
    reset(m_graph);
    event = nextBackward();
    while (event != NULL) {
        event->setLateTime(minLateTime(event));
        event->setReserve(event->lateTime() - event->earlyTime());
        event->setIsCalculated(true);

        event = nextBackward();
    }

    return true;
}

bool NetworkAlgorithm::reset(NetworkGraph *graph) {
    QVector<Event*> vertices = graph->vertices();
    for (int i = 0; i < vertices.size(); i++) {
        vertices[i]->setIsCalculated(false);
    }
    return true;
}

Event* NetworkAlgorithm::nextForward() {
    QVector<Event*> events = m_graph->vertices();

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

Event* NetworkAlgorithm::nextBackward() {
    QVector<Event*> events = m_graph->vertices();

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

double NetworkAlgorithm::maxEarlyTime(Event *event) {
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

double NetworkAlgorithm::minLateTime(Event *event) {
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

bool NetworkAlgorithm::allAreCalculated(const QVector<Event *> &events) {
    for (int i = 0; i < events.size(); i++) {
        if (!events[i]->isCalculated())
            return false;
    }
    return true;
}

bool CriticalPathAlgorithm::compute() {
    if (m_graph->isNull()) return false;

    // рассчитаем полные резервы работ и определим, лежат ли они на критическом пути
    QVector<Work*> works = m_graph->edges();
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

    return true;
}

bool CostAlgorithm::compute() {
    int cost = 0;
    QVector<Work*> works = m_graph->edges();
    for (int i = 0; i < works.size(); i++) {
        cost += works[i]->cost();
    }
    m_graph->setCost(cost);

    return true;
}

bool TimeAlgorithm::compute() {
    int time = 0;
    QVector<Work*> works = m_graph->edges();
    for (int i = 0; i < works.size(); i++) {
        if (works[i]->isCritical())
            time += works[i]->time()->value();
    }
    m_graph->setTime(time);

    return true;
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
