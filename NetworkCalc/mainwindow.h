#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "math/networkgraph.h"
#include "math/interfaces.h"
#include "project.h"

using namespace Math;
using namespace Math::Planning;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void keyPressEvent(QKeyEvent *event);

private slots:
    void onOpenGraph();
    void onSaveGraph();

    void on_pbCompute_clicked();
    void on_cbAlgorithm_currentTextChanged(const QString &arg1);

private:
    IAlgorithm *currentAlgorithm(NetworkGraph* graph);
    IRandomFactory *currentWorkTimeRandom();

    void logReport();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
