#ifndef STUCTURES
#define STUCTURES

#include <climits>
#include <QVector>
#include <QMap>

#define INF INT_MAX

#include <QVector>

template <typename T>
class Matrix {
public:
    Matrix(int n=0, int m=0) {
        for (int i = 0; i < n; i++) {
            QVector<T> row(m);
            row.resize(m);
            _m.append(row);
        }
    }
    Matrix(const T *a, int n, int m) {
        for (int i = 0; i < n; i++) {
            QVector<T> row(m);
            for (int j = 0; j < m; j++) {
                row[j] = *(a + i*m+j);
            }
            _m.append(row);
        }
    }

    int rows() { return _m.size(); }
    int cols() { return rows() ? _m[0].size() : 0; }

    T get(int row, int col) { return _m[row][col]; }

    QVector<T>& operator[](uint row) { return _m[row]; }
    const QVector<T>& operator[](uint row) const { return _m[row]; }

    // увеличивает размерность матрицы на 1
    // и вставки нулевой строки в начало матрицы
    // путем вставки нулевых элементов в начало строк
    Matrix extend() {
        Matrix a(rows()+1, cols()+1);
        for (int i = 0; i < _m.size(); i++) {
            for (int j = 0; j < _m[0].size(); j++) {
                a[i+1][j+1] = _m[i][j];
            }
        }
        return a;
    }

    bool isNull() { return rows() == 0; }

private:
    QVector<QVector<T> > _m;
};

typedef Matrix<int> Assigns;

// оценка назначений
class AssingsEstimation {
public:
    AssingsEstimation() {}
    AssingsEstimation(Assigns &assigns, double cost, double time)
        : assigns(assigns),
          cost(cost),
          time(time)
    {}

    Assigns assigns;

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

