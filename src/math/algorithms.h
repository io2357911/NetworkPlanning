#ifndef ALGORITHMS
#define ALGORITHMS

#include <vector>
#include "networkgraph.h"
#include "interfaces.h"

using namespace std;

namespace Math {

/**
 * @brief The SumIterator class Перебирает множество векторов длины count, сумма элементов которого равна sum
 */
class SumIterator : public Iterator<QVector<uint>> {
public:
    SumIterator(uint count, uint sum);

    // Iterator interface
    virtual bool reset();
    virtual bool toNext();
    virtual QVector<uint> next();

protected:
    uint sum(const QVector<uint> &vec);
    bool nextSum();

protected:
    QVector<uint> m_vec;
    uint m_sum;
};

class MetaIterator : public IIterator {
public:
    MetaIterator(const QVector<IIterator*> iters = QVector<IIterator*>())
        : m_iters(iters) {

        reset();
    }

    // Iterator interface
    bool reset() {
        if (m_iters.empty()) return false;

        m_iters[0]->reset();
        for (int i = 1; i < m_iters.size(); i++) {
            m_iters[i]->reset();
            m_iters[i]->toNext();
        }
        return true;
    }

    bool toNext() {
        for (int i = 0; i < m_iters.size(); i++) {
            if (!m_iters[i]->toNext()) {
                m_iters[i]->reset();
                m_iters[i]->toNext();

            } else {
                return true;
            }
        }
        return false;
    }

protected:
    QVector<IIterator*> m_iters;
};

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
    INetworkAlgorithm *m_algCalc;
};

/**
 * @brief The ResourseNetworkAlgorithm class Алгоритм для оптимального распределения ресурсов
 */
class ResourseNetworkAlgorithm : public INetworkAlgorithm {
public:
    ResourseNetworkAlgorithm(NetworkGraph* graph, IAlgorithm *algCalc, double prob);
    virtual ~ResourseNetworkAlgorithm() {}

    // IAlgorithm interface
    bool compute();

private:
    bool setMaxResourseCount();

private:
    IAlgorithm *m_algCalc;
    double m_prob;

private:
    /**
     * @brief WorkGroup Группа работ с ресурсом одного типа, выполняемых одновременно
     */
    typedef QVector<Work*> WorkGroup;
    /**
     * @brief The ResourseIterator class Итератор для перебора вариантов распределения ресурсов
     */
    class ResourseIterator : public MetaIterator {
    public:
        ResourseIterator(NetworkGraph *graph = 0);
        ~ResourseIterator();

    private:
        QVector<WorkGroup> findWorkGroups(NetworkGraph* graph);
    };
    /**
     * @brief The WorkGroupIterator class Итератор для перебора вариантов распределения ресурсов одного типа для одновременно выполняемых работ
     *
     * Перебор осуществляется между работами, выполняемыми одновременно (выходят из одного события).
     * При этом количество ресурсов работ выбирается так, чтобы их сумма был равна количеству доступного ресурса данного типа
     */
    class WorkGroupIterator : public SumIterator {
    public:
        WorkGroupIterator(const WorkGroup &group);

        // IIterator interface
        bool toNext();

    private:
        WorkGroup m_wg;
    };
};

class PertNetworkAlgorithm : public INetworkAlgorithm {
public:
    PertNetworkAlgorithm(NetworkGraph* graph) : INetworkAlgorithm(graph) {}
    virtual ~PertNetworkAlgorithm() {}

    // IAlgorithm interface
    bool compute();

private:
    bool computeTime();
    bool computeCost();
};

class MonteCarloNetworkAlgorithm : public INetworkAlgorithm {
public:
    MonteCarloNetworkAlgorithm(NetworkGraph* graph,
                               IRandomFactory *workRandomFactory = new Randoms::BetaFactory,
                               uint iterations = 100, uint intervals = 10);
    virtual ~MonteCarloNetworkAlgorithm();

    // IAlgorithm interface
    bool compute();

private:
    bool computeTime();
    bool computeCost();

private:
    IRandomFactory *m_workRandomFactory;
    uint m_iterations;
    uint m_intervals;
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

} // namespace Algorithms
} // namespace Planning
} // namespace Math

#endif // ALGORITHMS

