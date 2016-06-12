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



PlanningAlgoritms::AssignsIterator::AssignsIterator(Assigns &assign)
    : baseAssign(assign),
      assign(assign),
      state(assign.rows())
{}

bool PlanningAlgoritms::AssignsIterator::next()
{
    if (!state.IsComplete()) {
        ++state;

        for (uint i = 0; i < state.values_.size(); i++) {
            int index = state.values_[i] - 1;
            assign[i][0] = baseAssign[index][0];
        }

        return true;

    } else {
        return false;
    }
}

// returns 0 if no mobile integer exists
int PlanningAlgoritms::JohnsonTrotterState::LargestMobile() const {
    for (int r = values_.size(); r > 0; --r)
    {
        const int loc = positions_[r] + (directions_[r] ? 1 : -1);
        if (loc >= 0 && loc < (int)values_.size() && (int)values_[loc] < r)
            return r;
    }
    return 0;
}

bool PlanningAlgoritms::JohnsonTrotterState::IsComplete() const {
    return LargestMobile() == 0;
}

// implement Johnson-Trotter algorithm
void PlanningAlgoritms::JohnsonTrotterState::operator++() {
    const int r = LargestMobile();
    const int rLoc = positions_[r];
    const int lLoc = rLoc + (directions_[r] ? 1 : -1);
    const int l = values_[lLoc];
    // do the swap
    swap(values_[lLoc], values_[rLoc]);
    swap(positions_[l], positions_[r]);
    sign_ = -sign_;
    // change directions
    for (auto pd = directions_.begin() + r + 1; pd != directions_.end(); ++pd)
        *pd = !*pd;
}

vector<int> PlanningAlgoritms::JohnsonTrotterState::UpTo(int n, int offset)
{
    vector<int> retval(n);
    for (int ii = 0; ii < n; ++ii)
        retval[ii] = ii + offset;
    return retval;
}

PlanningAlgoritms::NetworkPlanningAlgorithm::NetworkPlanningAlgorithm(NetworkGraph &graph,
                                                                      QVector<IWorker *> &workers,
                                                                      QVector<IWork *> &works,
                                                                      int maxTime)
    : graph(graph),
      workers(workers),
      works(works),
      maxTime(maxTime)
{}

bool PlanningAlgoritms::NetworkPlanningAlgorithm::compute()
{
    // решение классической задачи о назначениях
    HungarianAlgorithm().compute(workers, works);

    Assigns assigns = toAssingsMatrix(works);

    // базовая оценка назначений
    AssingsEstimation estBase = estimate(assigns);
    if (estBase.time <= maxTime)
        return true;

    // сравнительная оценка назначений
    RelativeAssingsEstimation relEstBest;

    // выполняем последовательные переназначения
    AssignsIterator assingsIter(assigns);
    while (assingsIter.next()) {
        AssingsEstimation estNew = estimate(assigns);

        RelativeAssingsEstimation relEstNew(estBase, estNew);
        if (relEstNew.isBetter(relEstBest)) {
            relEstBest = relEstNew;
        }
    }

    fromAssingsMatrix(relEstBest.newEstimation.assigns);

    return relEstBest.newEstimation.time <= maxTime;
}

AssingsEstimation PlanningAlgoritms::NetworkPlanningAlgorithm::estimate(Assigns &assigns)
{
    fromAssingsMatrix(assigns);

    NetworkGraphAlgoritm().compute(&graph);
    CriticalPathAlgorithm().compute(&graph);
    CostAlgorithm().compute(&graph);
    TimeAlgorithm().compute(&graph);

    return AssingsEstimation(assigns, graph.getCost(), graph.getTime());
}

Assigns PlanningAlgoritms::NetworkPlanningAlgorithm::toAssingsMatrix(QVector<IWork *> &works)
{
    Assigns m(works.size(), 1);
    for (int i = 0; i < works.size(); i++) {
        for (int j = 0; j < workers.size(); j++) {
            IWorker *worker = works[i]->getWorker();
            if (worker->getID() != workers[j]->getID()) continue;

            m[i][0] = j;
        }
    }
    return m;
}

void PlanningAlgoritms::NetworkPlanningAlgorithm::fromAssingsMatrix(Assigns &assigns)
{
    for (int i = 0; i < assigns.rows(); i++) {
        works[i]->setWorker(workers.at(assigns[i][0]));
    }
}
