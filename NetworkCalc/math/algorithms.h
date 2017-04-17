#ifndef ALGORITHMS
#define ALGORITHMS

#include <vector>
#include "networkgraph.h"
#include "interfaces.h"

using namespace std;

namespace Math {
namespace Planning {
namespace Algorithms {

/*!
 * \brief Класс алгоритмов на сетевом графе
 */
class INetworkAlgorithm : public IAlgorithm {
public:
    INetworkAlgorithm(NetworkGraph* graph) : m_graph(graph) {}
    virtual ~INetworkAlgorithm() {}

    // IAlgorithm interface
    virtual bool compute() = 0;

protected:
    NetworkGraph* m_graph;
};

/*!
 * \brief Класс алгоритма для рассчета сетевого графа
 *
 * Расчитывает ранние и поздние сроки наступления событий
 */
class NetworkAlgorithm : public INetworkAlgorithm {
public:
    NetworkAlgorithm(NetworkGraph* graph) : INetworkAlgorithm(graph) {}
    virtual ~NetworkAlgorithm() {}

    // IAlgorithm interface
    virtual bool compute();

private:
    bool reset(NetworkGraph *graph);

    Event* nextForward();
    Event* nextBackward();

    double maxEarlyTime(Event *vertex);
    double minLateTime(Event *vertex);

    bool allAreCalculated(const QVector<Event*> &vertices);
};

/*!
 * \brief Класс алгоритма нахождения критического пути
 * \warning Предполагается, что параметры сетевого графа уже рассчитаны (посредством NetworkGraphAlgoritm)
 *
 * Определяет принадлежность работ критическому пути (свободные резервы слобытий и полный резерв работы равны нулю)
 */
class CriticalPathAlgorithm : public INetworkAlgorithm {
public:
    CriticalPathAlgorithm(NetworkGraph* graph) : INetworkAlgorithm(graph) {}
    virtual ~CriticalPathAlgorithm() {}

    // IAlgorithm interface
    bool compute();
};

/*!
 * \brief Класс алгоритма нахождения стоимости проекта
 */
class CostAlgorithm : public INetworkAlgorithm {
public:
    CostAlgorithm(NetworkGraph* graph) : INetworkAlgorithm(graph) {}
    virtual ~CostAlgorithm() {}

    // IAlgorithm interface
    bool compute();
};

/*!
 * \brief Класс алгоритма нахождения времени проекта
 * \warning Предполагается, что уже рассчитен крит. путь
 */
class TimeAlgorithm : public INetworkAlgorithm {
public:
    TimeAlgorithm(NetworkGraph* graph) : INetworkAlgorithm(graph) {}
    virtual ~TimeAlgorithm() {}

    // IAlgorithm interface
    bool compute();
};


/*!
 * \brief Класс алгоритма нахождения
 */
class MonteCarloModelingAlgorithm {
public:
    virtual ~MonteCarloModelingAlgorithm() {}
    virtual void compute(NetworkGraph *graph);

private:
    void modelWorks(NetworkGraph *graph);
};

} // namespace Algorithms
} // namespace Planning
} // namespace Math

#endif // ALGORITHMS

