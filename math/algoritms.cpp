#include "algoritms.h"

using namespace std;

Matrix PlanningAlgoritms::HungarianAlgorithm::compute(Matrix &ma)
{
    Matrix result(ma.rows(), 1);

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
        result[p[j]-1][0] = j-1;

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
        //qDebug("B(%d) = %f", vertex, B(vertex));
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

