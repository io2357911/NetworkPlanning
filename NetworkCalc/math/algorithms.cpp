#include "algorithms.h"

#include <vector>

using namespace std;

#define INF INT_MAX

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

void PlanningAlgoritms::CostAlgorithm::compute(NetworkGraph *graph)
{
    int cost = 0;
    QVector<IWork*> works = graph->edges();
    for (int i = 0; i < works.size(); i++) {
        cost += works[i]->getCost();
    }
    graph->setCost(cost);
}

void PlanningAlgoritms::TimeAlgorithm::compute(NetworkGraph *graph)
{
    int time = 0;
    QVector<IWork*> works = graph->edges();
    for (int i = 0; i < works.size(); i++) {
        if (works[i]->isCritical())
            time += works[i]->getTime();
    }
    graph->setTime(time);
}

void PlanningAlgoritms::HungarianAlgorithm::compute(QVector<IWorker *> workers, QVector<IWork *> works)
{
//    Matrix result(works.size(), 1);

    int n = workers.size(); // кол-во рабочих
    if (!n) return;

    int m = works.size(); // кол-во заданий
    if (!m) return;

    if (!(n <= m)) return;

    Matrix<int> ma(n, m);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            ma[i][j] = workers[i]->getCost(works[j]);
        }
    }

    Matrix<int> a = ma.extend();

    vector<int> u (n+1), v (m+1), p (m+1), way (m+1);
    for (int i=1; i<=n; ++i) {
        p[0] = i;
        int j0 = 0;
        vector<int> minv (m+1, INF);
        vector<char> used (m+1, false);
        do {
            used[j0] = true;
            int i0 = p[j0],  delta = INF,  j1;
            for (int j=1; j<=m; ++j)
                if (!used[j]) {
                    int cur = a[i0][j]-u[i0]-v[j];
                    if (cur < minv[j])
                        minv[j] = cur,  way[j] = j0;
                    if (minv[j] < delta)
                        delta = minv[j],  j1 = j;
                }
            for (int j=0; j<=m; ++j)
                if (used[j])
                    u[p[j]] += delta,  v[j] -= delta;
                else
                    minv[j] -= delta;
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }

//    for (int j=1; j <= m; j++)
//        result[j-1][0] = p[j]-1;

    for (int i = 1; i <= m; i++) {
        works[i-1]->setWorker(workers[p[i]-1]);
    }
}
