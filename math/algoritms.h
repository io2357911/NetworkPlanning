#ifndef ALGORITMS_H
#define ALGORITMS_H

#include <QMap>
#include "structures.h"

namespace PlanningAlgoritms {

// Алгоритм решения задачи о назначениях
class AssignmentAlgoritm {
public:
    virtual ~AssignmentAlgoritm() {}
    virtual bool compute(Matrix &a, QVector<int> &result) = 0;
};

// Венгерский алгоритм
class HungarianAlgorithm : public AssignmentAlgoritm {
public:
    bool compute(Matrix &a, QVector<int> &result);
};

// Алгоритм нахождения критического пути
class CriticalPathAlgoritm {
public:
    virtual ~CriticalPathAlgoritm() {}
    virtual bool compute(Graph &graph, double &pathLength) = 0;
};

// Нахождение критического пути методом динамического программирования
// Dynamic Programming Critical Path (DPCP)
class DPCPAlgoritm : public CriticalPathAlgoritm {
public:
    bool compute(Graph &graph, double &pathLength);
private:
    QMap<int, double> B; // Функция Беллмана динамического программирования
    Graph *graph;

    double maxLength(int vertex, QVector<int> neighbours);
};

}

#endif // ALGORITMS_H
