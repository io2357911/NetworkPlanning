#include "algoritms.h"

using namespace std;

Matrix PlanningAlgoritms::HungarianAlgorithm::compute(Matrix &ma)
{
    Matrix result(ma.cols(), 1);

    int n = ma.rows(); // кол-во рабочих
    if (!n) return result;

    int m = ma.cols(); // кол-во заданий
    if (!m) return result;

    if (!(n <= m)) return result;

    Matrix a = ma.extend();

    vector<int> u (n+1), v (m+1), p (m+1), way (m+1);
    for (int i=1; i<=n; ++i) {
        p[0] = i;
        int j0 = 0;
        vector<int> minv (m+1, INF);
        vector<char> used (m+1, false);
        do {
            used[j0] = true;
            int i0 = p[j0],  delta = INF,  j1;
            for (int j=1; j<=m; ++j)
                if (!used[j]) {
                    int cur = a[i0][j]-u[i0]-v[j];
                    if (cur < minv[j])
                        minv[j] = cur,  way[j] = j0;
                    if (minv[j] < delta)
                        delta = minv[j],  j1 = j;
                }
            for (int j=0; j<=m; ++j)
                if (used[j])
                    u[p[j]] += delta,  v[j] -= delta;
                else
                    minv[j] -= delta;
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = way[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }

    for (int j=1; j <= m; j++)
        result[j-1][0] = p[j]-1;

    return result;
}

double PlanningAlgoritms::DPCPAlgoritm::compute(Graph &graph)
{
    B.clear();
    this->graph = &graph;

    QVector<int> vertices = graph.vertices();
    if (!vertices.size()) return -1;

    B[vertices.last()] = 0;
    for (int i = vertices.size() - 2; i > -1; i--) {

        int vertex = vertices[i];
        QVector<int> neighbours = graph.neighbours(vertex);

        if (!neighbours.size()) return -1;

        B[vertex] = maxLength(vertex, neighbours);
        //qDebug("B(%d) = %f", vertex, B[vertex]);
    }

    return B[vertices[0]];
}

double PlanningAlgoritms::DPCPAlgoritm::maxLength(int vertex, QVector<int> neighbours)
{
    double maxLen = 0;
    for (int i = 0; i < neighbours.size(); i++) {
        int nextVertex = neighbours[i];
        double curLen = graph->edgeWeight(vertex, nextVertex) + B[nextVertex];
        if (curLen > maxLen) {
            maxLen = curLen;
        }
    }
    return maxLen;
}


Matrix PlanningAlgoritms::NetworkPlanningAlgorithm::compute(int workersCount, double maxTime, Matrix costs, Matrix times, Matrix netAdjecency)
{
    if (!isCorrectInput(workersCount, maxTime, costs, times, netAdjecency)) return Matrix();

    // решение классической задачи о назначениях
    Matrix assigns = algAssigns->compute(costs);

    ProjectGraph graph(netAdjecency, &assigns, &times);

    // базовая оценка назначений
    AssingsEstimation estBase = estimate(assigns, costs, graph);
    if (estBase.time <= maxTime)
        return estBase.assigns;

    // сравнительная оценка назначений
    RelativeAssingsEstimation relEstBest;

    // выполняем последовательные переназначения
    AssignsIterator assingsIter(assigns);
    while (assingsIter.next()) {
        AssingsEstimation estNew = estimate(assigns, costs, graph);

        RelativeAssingsEstimation relEstNew(estBase, estNew);
        if (relEstNew.isBetter(relEstBest)) {
            relEstBest = relEstNew;
        }
    }

    if (relEstBest.newEstimation.time <= maxTime)
        return relEstBest.newEstimation.assigns;
    else
        return Matrix();
}

AssingsEstimation PlanningAlgoritms::NetworkPlanningAlgorithm::estimate(Matrix &assigns, Matrix &costs, ProjectGraph &graph)
{
    return AssingsEstimation(assigns,
                             projectCost(costs, assigns),
                             algCritPath->compute(graph));
}

bool PlanningAlgoritms::NetworkPlanningAlgorithm::isCorrectInput(int workersCount, double/* maxTime*/, Matrix &costs,
                                                                 Matrix &times, Matrix &/*netAdjecency*/)
{
    bool correct = true;

    correct &= workersCount == costs.rows();
    correct &= workersCount == costs.cols();
    correct &= workersCount == times.rows();
    correct &= workersCount == times.rows();

    return correct;
}

bool PlanningAlgoritms::AssignsIterator::next()
{
    if (!state.IsComplete()) {
        ++state;

        for (int i = 0; i < state.values_.size(); i++) {
            int index = state.values_[i] - 1;
            assign[i][0] = baseAssign[index][0];
        }

        return true;

    } else {
        return false;
    }
}