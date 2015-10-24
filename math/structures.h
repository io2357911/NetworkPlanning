#ifndef STUCTURES
#define STUCTURES

#include <climits>
#include <QVector>
#include <QMap>

#define INF INT_MAX

struct Job {
    int id;
};

struct Worker {
    int id;

    double cost(Job job);
    double time(Job job);
};

struct Assinment {
    Worker worker;
    Job job;
};


template <typename F, typename S>
class TwoWayMapping {
public:
    F direct(S s);
    S inverse(F s);
};

typedef TwoWayMapping<Job, Worker> JobWorkerMapping;


class Matrix {
public:
    Matrix(int n=0, int m=0);
    Matrix(const double *a, int n, int m);

    int rows();
    int cols();

    double get(int row, int col);

    QVector<double>& operator[](uint row)       { return _m[row]; }
    const QVector<double>& operator[](uint row) const { return _m[row]; }

    // увеличивает размерность матрицы на 1
    // и вставки нулевой строки в начало матрицы
    // путем вставки нулевых элементов в начало строк
    Matrix extend();

    bool isNull() { return rows() == 0; }

private:
    QVector<QVector<double> > _m;
};

class Graph {
public:
    Graph(Matrix adjacencyMatrix) { this->adjacencyMatrix = adjacencyMatrix; }

    QVector<int> vertices();
    QVector<int> neighbours(int vertex);
    virtual double edgeWeight(int v1, int v2);

protected:
    Matrix adjacencyMatrix;
};

class ProjectGraph : public Graph {
public:
    ProjectGraph(Matrix adjacencyMatrix, Matrix *asignments, Matrix *times)
        : Graph(adjacencyMatrix),
          asignments(asignments),
          times(times)
    {}

    double edgeWeight(int v1, int v2);

private:
    Matrix *asignments;
    Matrix *times;
};

#endif // STUCTURES

