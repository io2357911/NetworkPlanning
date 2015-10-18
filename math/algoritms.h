#ifndef ALGORITMS_H
#define ALGORITMS_H

#include <QMap>
#include "structures.h"

namespace PlanningAlgoritms {

static double calcCost(Matrix costs, Matrix asignments) {
    double cost;

    for (int i = 0; i < asignments.rows(); i++) {
        cost += costs[i][asignments[i][0]];
    }

    return cost;

}

// Алгоритм решения задачи о назначениях
class AssignmentAlgoritm {
public:
    virtual ~AssignmentAlgoritm() {}
    virtual Matrix compute(Matrix &a) = 0;
};

// Венгерский алгоритм
class HungarianAlgorithm : public AssignmentAlgoritm {
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

}

#endif // ALGORITMS_H
