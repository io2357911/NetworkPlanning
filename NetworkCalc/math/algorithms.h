#ifndef ALGORITHMS
#define ALGORITHMS

#include <vector>
#include "networkgraph.h"

using namespace std;

namespace Math {
namespace Planning {
namespace Algorithms {


/*!
 * \brief Класс алгоритма для рассчета сетевого графа
 */
class NetworkGraphAlgoritm {
public:
    virtual ~NetworkGraphAlgoritm() {}
    virtual void compute(NetworkGraph *graph);

private:
    bool reset(NetworkGraph *graph);

    Event *nextForward(NetworkGraph *graph);
    Event *nextBackward(NetworkGraph *graph);

    double maxEarlyTime(Event *vertex);
    double minLateTime(Event *vertex);

    bool allAreCalculated(const QVector<Event*> &vertices);
};

/*!
 * \brief Класс алгоритма нахождения критического пути
 * \warning Предполагается, что параметры сетевого графа уже рассчитаны (посредством NetworkGraphAlgoritm)
 */
class CriticalPathAlgorithm {
public:
    virtual ~CriticalPathAlgorithm() {}
    virtual QVector<Work*> compute(NetworkGraph *graph);
};

/*!
 * \brief Класс алгоритма нахождения стоимости проекта
 */
class CostAlgorithm {
public:
    virtual ~CostAlgorithm() {}
    virtual void compute(NetworkGraph *graph);
};

/*!
 * \brief Класс алгоритма нахождения времени проекта
 * \warning Предполагается, что уже рассчитен крит. путь
 */
class TimeAlgorithm {
public:
    virtual ~TimeAlgorithm() {}
    virtual void compute(NetworkGraph *graph);
};

/*!
 * \brief Класс алгоритма нахождения
 */
class ModelingAlgorithm {
public:
    virtual ~ModelingAlgorithm() {}
    virtual void compute(NetworkGraph *graph) = 0;
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

