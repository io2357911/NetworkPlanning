#ifndef ALGORITMS_H
#define ALGORITMS_H

#include <QMap>
#include "structures.h"

#include <iostream>
#include <vector>

using namespace std;

struct JohnsonTrotterState
{
    vector<int> values_;
    vector<int> positions_;	// size is n+1, first element is not used
    vector<bool> directions_;
    int sign_;

    JohnsonTrotterState(int n) : values_(UpTo(n, 1)), positions_(UpTo(n + 1, -1)), directions_(n + 1, false), sign_(1) {}

    int LargestMobile() const	// returns 0 if no mobile integer exists
    {
        for (int r = values_.size(); r > 0; --r)
        {
            const int loc = positions_[r] + (directions_[r] ? 1 : -1);
            if (loc >= 0 && loc < values_.size() && values_[loc] < r)
                return r;
        }
        return 0;
    }

    bool IsComplete() const { return LargestMobile() == 0; }

    void operator++()	// implement Johnson-Trotter algorithm
    {
        const int r = LargestMobile();
        const int rLoc = positions_[r];
        const int lLoc = rLoc + (directions_[r] ? 1 : -1);
        const int l = values_[lLoc];
        // do the swap
        swap(values_[lLoc], values_[rLoc]);
        swap(positions_[l], positions_[r]);
        sign_ = -sign_;
        // change directions
        for (auto pd = directions_.begin() + r + 1; pd != directions_.end(); ++pd)
            *pd = !*pd;
    }

    vector<int> UpTo(int n, int offset = 0)
    {
        vector<int> retval(n);
        for (int ii = 0; ii < n; ++ii)
            retval[ii] = ii + offset;
        return retval;
    }
};

namespace PlanningAlgoritms {

static double projectCost(Matrix costs, Matrix assignments) {
    double cost;

    for (int i = 0; i < assignments.rows(); i++) {
        cost += costs[i][assignments[i][0]];
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


class AssignsIterator {
public:
    AssignsIterator(Matrix &assign)
        : baseAssign(assign),
          assign(assign),
          state(assign.rows())
    {}

    bool next();

private:
    Matrix baseAssign;
    Matrix &assign;
    JohnsonTrotterState state;
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

    inline AssingsEstimation estimate(Matrix &assigns, Matrix &costs, ProjectGraph &graph);

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
