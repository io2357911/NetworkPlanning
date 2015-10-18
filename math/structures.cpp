#include "structures.h"

Matrix::Matrix(int n, int m)
{
    for (int i = 0; i < n; i++) {
        QVector<double> row(m);
        row.resize(m);
        _m.append(row);
    }
}

Matrix::Matrix(const double *a, int n, int m)
{
    for (int i = 0; i < n; i++) {
        QVector<double> row(m);
        for (int j = 0; j < m; j++) {
            row[j] = *(a + i*m+j);
        }
        _m.append(row);
    }
}

int Matrix::rows()
{
    return _m.size();
}

int Matrix::cols()
{
    return rows() ? _m[0].size() : 0;
}

Matrix Matrix::extend()
{
    Matrix a(rows()+1, cols()+1);

    for (int i = 0; i < _m.size(); i++) {
        for (int j = 0; j < _m[0].size(); j++) {
            a[i+1][j+1] = _m[i][j];
        }
    }

    return a;
}


double Graph::edgeWeight(int v1, int v2)
{
    return adjacencyMatrix[v1][v2];
}

QVector<int> Graph::neighbours(int vertex)
{
    QVector<int> neigs;
    for (int j = 0; j < adjacencyMatrix.cols(); j++) {
        if (adjacencyMatrix[vertex][j] > 0)
        neigs.append(j);
    }
    return neigs;
}

QVector<int> Graph::vertices()
{
    QVector<int> vert;
    for (int i = 0; i < adjacencyMatrix.rows(); i++) {
        vert.append(i);
    }
    return vert;
}
