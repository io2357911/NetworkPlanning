#include "algoritms.h"

using namespace std;

bool PlanningAlgoritms::HungarianAlgorithm::compute(Matrix &ma, QVector<int> &result)
{
    int n = ma.rows(); // кол-во рабочих
    if (!n) return false;

    int m = ma.cols(); // кол-во заданий
    if (!m) return false;

    if (!(n <= m)) return false;

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

    result.resize(n);
    for (int j=1; j <= m; j++)
        result[p[j]-1] = j-1;

    return true;
}

bool PlanningAlgoritms::DPCPAlgoritm::compute(Graph &graph, double &pathLength)
{
    B.clear();
    this->graph = &graph;

    QVector<int> vertices = graph.vertices();
    if (!vertices.size()) return false;

    B[vertices.last()] = 0;
    for (int i = vertices.size() - 2; i > -1; i--) {

        int vertex = vertices[i];
        QVector<int> neighbours = graph.neighbours(vertex);

        if (!neighbours.size()) return false;

        B[vertex] = maxLength(vertex, neighbours);
        //qDebug("B(%d) = %f", vertex, B(vertex));
    }

    pathLength = B[vertices[0]];
    return true;
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

