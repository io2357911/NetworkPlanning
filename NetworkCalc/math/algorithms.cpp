#include "algorithms.h"

#include <vector>
#include <assert.h>

using namespace std;

#define INF INT_MAX

namespace Math {

SumIterator::SumIterator(uint count, uint sum)
    : m_vec(count), m_sum(sum) {

    assert(count);
    assert(sum >= count);

    reset();
}

bool SumIterator::reset() {
    m_vec = QVector<uint>(m_vec.size(), 1);
    m_vec[0] = 0;
    return true;
}

bool SumIterator::toNext() {
    while (1) {
        if (!nextSum()) {
            return false;

        } else if (m_sum == sum(m_vec)) {
            return true;
        }
    }
}

QVector<uint> SumIterator::next() {
    return m_vec;
}

uint SumIterator::sum(const QVector<uint> &vec) {
    int sum = 0;
    for (int i = 0; i < vec.size(); i++)
        sum += m_vec[i];
    return sum;
}

bool SumIterator::nextSum() {
    for (int i = 0; i < m_vec.size(); i++) {
        m_vec[i] = m_vec[i] + 1;

        if (m_vec[i] == m_sum) {
            m_vec[i] = 1;

        } else {
            return true;
        }
    }
    return false;
}

namespace Planning {
namespace Algorithms {

void printRD(NetworkGraph *graph) {
    QString str;

    QVector<Work*> works = graph->edges();
    for (int i = 0; i < works.size(); i++) {
        Work *work = works[i];
        str += QString("%1(%2), ").arg(work->name()).arg(work->resourseCount());
    }

    qDebug(str.toStdString().c_str());
}

ResourseNetworkAlgorithm::ResourseNetworkAlgorithm(NetworkGraph *graph, IAlgorithm *algCalc, double prob)
    : INetworkAlgorithm(graph), m_algCalc(algCalc), m_prob(prob)
{}

bool ResourseNetworkAlgorithm::compute() {
    bool res = false;

    if (!m_algCalc) return res;
    if (!setMaxResourseCount()) return res;

    res = true;

    ResourseDistribution minDist;
    double minTime = std::numeric_limits<double>::max();

    uint iters = 0; // debug

    ResourseIterator iter(m_graph);
    while (iter.toNext()) {
        if (!m_algCalc->compute()) continue;

        printRD(m_graph); // debug

        double time = m_graph->time()->invF(m_prob);
        if (time < minTime) {
            minTime = time;
            minDist = m_graph->resourseDistribution();
        }
        iters++;
    }

    qDebug("ResourseNetworkAlgorithm::compute() iters: %d", iters); // debug

    m_graph->setResourseDistribution(minDist);

    return res;
}

bool ResourseNetworkAlgorithm::setMaxResourseCount() {
    QVector<Work*> works = m_graph->edges();
    for (int i = 0; i < works.size(); i++) {
        Work *work = works[i];
        if (work->isVirtual()) continue;
        if (!work->resourse()) return false;

        work->setResourseCount(work->resourse()->quantity());
    }
    return true;
}

ResourseNetworkAlgorithm::ResourseIterator::ResourseIterator(NetworkGraph *graph) {
    QVector<WorkGroup> rgs = findWorkGroups(graph);
    for (int i = 0; i < rgs.size(); i++) {
        m_iters.append(new WorkGroupIterator(rgs[i]));
    }
    reset();
}

ResourseNetworkAlgorithm::ResourseIterator::~ResourseIterator() {
    for (int i = 0; i < m_iters.size(); i++) {
        delete m_iters[i];
    }
    m_iters.clear();
}

QVector<ResourseNetworkAlgorithm::WorkGroup> ResourseNetworkAlgorithm::ResourseIterator::findWorkGroups(NetworkGraph* graph) {
    QVector<WorkGroup> rgs;

    QVector<Event*> events = graph->vertices();
    for (int i = 0; i < events.size(); i++) {

        QMap<Resourse*, WorkGroup> map;

        QVector<Work*> works = events[i]->nextEdges();
        for (int j = 0; j < works.size(); j++) {
            Work *work = works[j];
            Resourse *res = work->resourse();
            if (!res) continue;
            map[res].append(work);
        }

        QList<WorkGroup> _rgs = map.values();
        for (int j = 0; j < _rgs.size(); j++) {
            if (_rgs[j].size() > 1)
                rgs.append(_rgs[j]);
        }
    }

    return rgs;
}

ResourseNetworkAlgorithm::WorkGroupIterator::WorkGroupIterator(const ResourseNetworkAlgorithm::WorkGroup &group)
    : SumIterator(group.size(), (int)group[0]->resourse()->quantity()), m_wg(group) {

    reset();
}

bool ResourseNetworkAlgorithm::WorkGroupIterator::toNext() {
    bool res = SumIterator::toNext();
    if (res) {
        for (int i = 0; i < m_wg.size(); i++) {
            m_wg[i]->setResourseCount(m_vec[i]);
        }
    }
    return res;
}


bool PertNetworkAlgorithm::compute() {
    // зададим вероятностные хар-ки
    Randoms::PertBetaFactory    workTime;
    Randoms::PertBetaFactory    workCost;
    Random*                     graphTime = new Math::Randoms::PertNormal;
    Random*                     graphCost = new Math::Randoms::PertNormal;

    QVector<Work*> works = m_graph->edges();
    for (int i = 0; i < works.size(); i++) {
        works[i]->setTime(&workTime);
        works[i]->time()->random();

        works[i]->setCost(&workCost);
        works[i]->cost()->random();
    }
    m_graph->setTime(graphTime);
    m_graph->setCost(graphCost);

    // расчет сетевого графа
    if (!NetworkAlgorithm(m_graph).compute()) return false;

    // расчет крит. пути
    if (!CriticalPathAlgorithm(m_graph).compute()) return false;

    // расчет стоимости
    if (!computeCost()) return false;

    // расчет времени
    if (!computeTime()) return false;

    return true;
}

bool PertNetworkAlgorithm::computeTime() {
    double value = 0;
    double mathExpected = 0;
    double dispersion = 0;
    QVector<Work*> critPath = m_graph->criticalPath();
    for (int i = 0; i < critPath.size(); i++) {
        value += critPath[i]->time()->value();
        mathExpected += critPath[i]->time()->mathExpected();
        dispersion += critPath[i]->time()->dispersion();
    }
    m_graph->time()->setValue(value);
    m_graph->time()->setMathExpected(mathExpected);
    m_graph->time()->setDispersion(dispersion);

    return true;
}

bool PertNetworkAlgorithm::computeCost() {
    double value = 0;
    double mathExpected = 0;
    double dispersion = 0;

    QVector<Work*> works = m_graph->edges();
    for (int i = 0; i < works.size(); i++) {
        if (works[i]->isVirtual()) continue;

        value += works[i]->cost()->value();
        mathExpected += works[i]->cost()->mathExpected();
        dispersion += works[i]->cost()->dispersion();
    }
    m_graph->cost()->setValue(value);
    m_graph->cost()->setMathExpected(mathExpected);
    m_graph->cost()->setDispersion(dispersion);

    return true;
}


MonteCarloNetworkAlgorithm::MonteCarloNetworkAlgorithm(NetworkGraph *graph,
                                                       IRandomFactory *workRandomFactory,
                                                       uint iterations, uint intervals)
    : INetworkAlgorithm(graph), m_workRandomFactory(workRandomFactory),
      m_iterations(iterations), m_intervals(intervals)
{}

MonteCarloNetworkAlgorithm::~MonteCarloNetworkAlgorithm() {
    if (m_workRandomFactory) delete m_workRandomFactory;
}

bool MonteCarloNetworkAlgorithm::compute() {
    if (!m_workRandomFactory) return false;

    // зададим вероятностные хар-ки
    Random *graphTime = new Math::Randoms::Empirical(m_intervals);
    Random *graphCost = new Math::Randoms::Empirical(m_intervals);

    QVector<Work*> works = m_graph->edges();
    for (int i = 0; i < works.size(); i++) {
        works[i]->setTime(m_workRandomFactory);
        works[i]->setCost(m_workRandomFactory);
    }
    m_graph->setTime(graphTime);
    m_graph->setCost(graphCost);

    for (uint i = 0; i < m_iterations; i++) {
        // имитация случайных длительностей работ
        for (int i = 0; i < works.size(); i++) {
            works[i]->time()->random();
            works[i]->cost()->random();
        }

        // расчет сетевого графа
        if (!NetworkAlgorithm(m_graph).compute()) return false;

        // расчет крит. пути
        if (!CriticalPathAlgorithm(m_graph).compute()) return false;

        // расчет стоимости
        if (!computeCost()) return false;

        // расчет времени
        if (!computeTime()) return false;
    }

    return true;
}

bool MonteCarloNetworkAlgorithm::computeTime() {
    double value = 0;
    QVector<Work*> critPath = m_graph->criticalPath();
    for (int i = 0; i < critPath.size(); i++) {
        value += critPath[i]->time()->value();
    }
    m_graph->time()->setValue(value);

    return true;
}

bool MonteCarloNetworkAlgorithm::computeCost() {
    double value = 0;
    QVector<Work*> works = m_graph->edges();
    for (int i = 0; i < works.size(); i++) {
        if (works[i]->isVirtual()) continue;

        value += works[i]->cost()->value();
    }
    m_graph->cost()->setValue(value);

    return true;
}

bool NetworkAlgorithm::compute() {
    if (m_graph->isNull()) return false;
    if (!m_graph->firstVertex()) return false;

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

        if (work->isVirtual()) continue;

        int fullReserve = secondEvent->lateTime() - firstEvent->earlyTime() - work->time()->value();

        work->setFullReserve(fullReserve);
        work->setIsCritical(IS_ZERO(firstEvent->reserve(), 0.001) &&
                            IS_ZERO(secondEvent->reserve(), 0.001) &&
                            IS_ZERO(work->fullReserve(), 0.001));
    }

    return true;
}

} // namespace Algorithms
} // namespace Planning
} // namespace Math
