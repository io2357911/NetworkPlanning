#ifndef ALGORITHMS
#define ALGORITHMS

#include "networkgraph.h"

namespace PlanningAlgoritms {

/*!
 * \brief Класс алгоритма для рассчета сетевого графа
 */
class NetworkGraphAlgoritm {
public:
    virtual ~NetworkGraphAlgoritm() {}
    virtual void compute(NetworkGraph *graph);

private:
    bool reset(NetworkGraph *graph);

    IEvent *nextForward(NetworkGraph *graph);
    IEvent *nextBackward(NetworkGraph *graph);

    int maxEarlyTime(IEvent *vertex);
    int minLateTime(IEvent *vertex);

    bool allAreCalculated(const QVector<IEvent*> &vertices);
};

/*!
 * \brief Класс алгоритма нахождения критического пути
 * \warning Предполагается, что параметры сетевого графа уже рассчитаны (посредством NetworkGraphAlgoritm)
 */
class CriticalPathAlgorithm {
public:
    virtual ~CriticalPathAlgorithm() {}
    virtual QVector<IWork*> compute(NetworkGraph *graph);
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

}

#endif // ALGORITHMS

