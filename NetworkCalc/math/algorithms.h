#ifndef ALGORITHMS
#define ALGORITHMS

#include "networkgraph.h"

namespace PlanningAlgoritms {

/*template <typename T>
class IIterator {
public:
    T next() = 0;
};*/

/*static double projectCost(Matrix costs, Matrix assignments) {
    double cost;

    for (int i = 0; i < assignments.rows(); i++) {
        cost += costs[i][assignments[i][0]];
    }

    return cost;
}*/

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

}

#endif // ALGORITHMS

