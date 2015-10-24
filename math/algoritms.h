#ifndef ALGORITMS_H
#define ALGORITMS_H

#include <QMap>
#include "structures.h"

namespace PlanningAlgoritms {

static double projectCost(Matrix costs, Matrix asignments) {
    double cost;

    for (int i = 0; i < asignments.rows(); i++) {
        cost += costs[i][asignments[i][0]];
    }

    return cost;
}

// Алгоритм решения задачи о назначениях
class AsignmentAlgoritm {
public:
    virtual ~AsignmentAlgoritm() {}
    virtual Matrix compute(Matrix &a) = 0;
};

// Венгерский алгоритм
class HungarianAlgorithm : public AsignmentAlgoritm {
public:
    Matrix compute(Matrix &a);
};

// Алгоритм нахождения критического пути
class CriticalPathAlgoritm {
public:
    virtual ~CriticalPathAlgoritm() {}
    virtual double compute(Graph &graph) = 0;
};

// Нахождение критического пути методом динамического программирования
// Dynamic Programming Critical Path (DPCP)
class DPCPAlgoritm : public CriticalPathAlgoritm {
public:
    double compute(Graph &graph);
private:
    QMap<int, double> B; // Функция Беллмана динамического программирования
    Graph *graph;

    double maxLength(int vertex, QVector<int> neighbours);
};

class NetworkPlanningAlgorithm {
public:
    NetworkPlanningAlgorithm(AsignmentAlgoritm *algAssigns = new HungarianAlgorithm,
                             CriticalPathAlgoritm *algCritPath = new DPCPAlgoritm)
        : algAssigns(algAssigns),
          algCritPath(algCritPath)
    {}
    ~NetworkPlanningAlgorithm() {
        if (algAssigns) delete algAssigns;
        if (algCritPath) delete algCritPath;
    }

    Matrix compute(int workersCount,
                   double maxTime,
                   Matrix costs,
                   Matrix times,
                   Matrix netAdjecency);

private:
    AsignmentAlgoritm *algAssigns;
    CriticalPathAlgoritm *algCritPath;

    bool isCorrectInput(int workersCount,
                      double maxTime,
                      Matrix &costs,
                      Matrix &times,
                      Matrix &netAdjecency);
};

}

#endif // ALGORITMS_H
