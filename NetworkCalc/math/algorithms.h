#ifndef ALGORITHMS
#define ALGORITHMS

#include <vector>
#include "structures.h"
#include "networkgraph.h"

using namespace std;

namespace PlanningAlgoritms {

struct JohnsonTrotterState
{
    vector<int> values_;
    vector<int> positions_;	// size is n+1, first element is not used
    vector<bool> directions_;
    int sign_;

    JohnsonTrotterState(int n) : values_(UpTo(n, 1)), positions_(UpTo(n + 1, -1)), directions_(n + 1, false), sign_(1) {}

    int LargestMobile() const;
    bool IsComplete() const;

    void operator++();

    vector<int> UpTo(int n, int offset = 0);
};

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

/**
 * @brief The AsignmentAlgoritm class Класс алгоритма решения задачи о назначениях
 */
class AsignmentAlgoritm {
public:
    virtual ~AsignmentAlgoritm() {}
    virtual void compute(QVector<IWorker*> workers, QVector<IWork*> works) = 0;
};

/**
 * @brief The AsignmentAlgoritm class Класс алгоритма решения задачи о назначениях Венгерским методом
 */
class HungarianAlgorithm : public AsignmentAlgoritm {
public:
    virtual void compute(QVector<IWorker*> workers, QVector<IWork*> works);
};

class AssignsIterator {
public:
    AssignsIterator(Assigns &assign);

    bool next();

private:
    Assigns baseAssign;
    Assigns &assign;
    JohnsonTrotterState state;
};


class NetworkPlanningAlgorithm {
public:
    NetworkPlanningAlgorithm(NetworkGraph &graph,
                             QVector<IWorker*> &workers,
                             QVector<IWork*> &works,
                             int maxTime);

    bool compute();

    AssingsEstimation estimate(Assigns &assigns);

private:
    Assigns toAssingsMatrix(QVector<IWork*> &works);
    void fromAssingsMatrix(Assigns &assigns);

private:
    NetworkGraph graph;
    QVector<IWorker*> workers;
    QVector<IWork*> works;
    int maxTime;
};

}

#endif // ALGORITHMS

