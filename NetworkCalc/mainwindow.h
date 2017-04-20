#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "math/networkgraph.h"
#include "project.h"

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

    void onComputePert();
    void onComputeMonteCarlo();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
