#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void resizeTable(QTableWidget *table,
                     int rowsColsCount,
                     QString horHeader = "Работа",
                     QString vertHeader = "Исполнитель",
                     double defVal = 0);

private slots:
    void on_sbWorkersCount_valueChanged(int arg1);
    void on_sbStageCount_valueChanged(int arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
