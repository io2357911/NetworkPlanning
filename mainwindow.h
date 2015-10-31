#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "math/algoritms.h"
#include <QMainWindow>
#include <QTableWidget>
#include <QMessageBox>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    Matrix values(QTableWidget *table, bool zeroFirst = false, bool greaterZero = true);
    void setValues(QTableWidget *table, Matrix values, bool zeroFirst = true);

    void resizeTable(QTableWidget *table,
                     int rowsCount,
                     int colsCount,
                     QString rowHeader,
                     QString colHeader,
                     double defVal = 0);

    QStringList createTableHeader(QString name, int count);

private slots:
    void on_sbWorkersCount_valueChanged(int arg1);
    void on_sbStageCount_valueChanged(int arg1);
    void on_pbCalcExpenses_clicked();

    void countAsignments(bool);
    void countAsignmentsWithTime(bool);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
