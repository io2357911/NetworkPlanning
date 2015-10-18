#ifndef STUCTURES
#define STUCTURES

#include <climits>
#include <QVector>

#define INF INT_MAX

class Matrix {
public:
    Matrix(int n=0, int m=0);
    Matrix(const double *a, int n, int m);

    int rows();
    int cols();

    QVector<double>& operator[](uint row)       { return _m[row]; }
    const QVector<double>& operator[](uint row) const { return _m[row]; }

    // увеличивает размерность матрицы на 1
    // и вставки нулевой строки в начало матрицы
    // путем вставки нулевых элементов в начало строк
    Matrix extend();

private:
    QVector<QVector<double> > _m;
};

class Graph {
public:
    Graph(Matrix adjacencyMatrix) { this->adjacencyMatrix = adjacencyMatrix; }

    QVector<int> vertices();
    QVector<int> neighbours(int vertex);
    double edgeWeight(int v1, int v2);

private:
    Matrix adjacencyMatrix;
};

#endif // STUCTURES

