#ifndef STUCTURES
#define STUCTURES

#include <climits>
#include <QVector>
#include <QMap>

#define INF INT_MAX

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

// оценка назначений
class AssingsEstimation {
public:
    AssingsEstimation() {}
    AssingsEstimation(Matrix &assigns, double cost, double time)
        : assigns(assigns),
          cost(cost),
          time(time)
    {}

    Matrix assigns;

    double cost;
    double time;
};

// относительная оценка назначений
class RelativeAssingsEstimation {
public:
    RelativeAssingsEstimation()
        : dcost(0.0), dtime(0.0)
    {}
    RelativeAssingsEstimation(AssingsEstimation &estBase, AssingsEstimation &estNew)
        : baseEstimation(estBase),
          newEstimation(estNew),
          dcost(estNew.cost - estBase.cost),
          dtime(estNew.time - estBase.time)
    {}

    AssingsEstimation baseEstimation;
    AssingsEstimation newEstimation;


    bool isNull() { return dtime == 0 && dcost == 0; }

    bool isBetter(RelativeAssingsEstimation &other) {
        if (other.isNull()) return true;

        if (dcost == 0 || other.dcost == 0) {
            return dtime <= other.dtime;

        } else {
            return (dtime / dcost) < (other.dtime / other.dcost);
        }

        //return dtime < other.dtime;
    }

private:
    double dcost;
    double dtime;
};

#endif // STUCTURES

