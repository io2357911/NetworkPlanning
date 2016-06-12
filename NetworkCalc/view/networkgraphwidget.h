#ifndef NETWORKGRAPHWIDGET_H
#define NETWORKGRAPHWIDGET_H

#include <QWidget>
#include <QVector>
#include "eventwidget.h"
#include "workwidget.h"
#include "eventpropertiesdialog.h"
#include "workpropertiesdialog.h"
#include "workerswidget.h"
#include "assignswidget.h"
#include "../tools/debug.h"
#include "../tools/ini.h"
#include "../math/algorithms.h"

namespace Ui {
class NetworkGraphWidget;
}

class NetworkGraphWidget : public QWidget
{
    Q_OBJECT
public:
    explicit NetworkGraphWidget(QWidget *parent = 0);
    ~NetworkGraphWidget();

    void contextMenuEvent(QContextMenuEvent *event);
    void paintEvent(QPaintEvent *evt);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    void setAssingsWidget(AssignsWidget *w);
    void setWorkersWidget(WorkersWidget *w);

signals:
    void showWorkers();
    void showAssigns();

    void workersChanged();
    void worksChanged();

    void graphComputed(NetworkGraph *graph);

public slots:
    void newGraph();
    void openGraph(QString path);
    void saveGraph(QString path);

    void computeNetworkGraph();
    void computeAssigns();

private slots:
    void onNewEvent();
    void onTest();

    void onEventClicked(EventWidget *widget);
    void onEventMoved(EventWidget *widget);
    void onEventStartConnect(EventWidget *widget);
    void onEventProperties(EventWidget *widget);
    void onEventDelete(EventWidget *widget);

    void onWorkProperties(WorkWidget *widget);
    void onWorkDelete(WorkWidget *widget);

    void newWorker();
    void deleteWorker(IWorker*);

private:
    EventWidget *createEventWidget();
    WorkWidget *createWorkWidget(EventWidget *firstEvent, EventWidget *secondEvent);

private:
    Ui::NetworkGraphWidget *ui;

    EventPropertiesDialog dEvent;
    WorkPropertiesDialog dWork;
//    WorkersWidget dWorkers;
//    AssignsWidget dAssigns;

    NetworkGraph graph;

    QVector<EventWidget*> events;
    QVector<WorkWidget*> works;
    QVector<IWorker*> workers;

    QAction *aNewEvent;
    QAction *aTest;

    EventWidget *firstConnectEvent;

    int eventsCounter, worksCounter, workerCounter;
};

#endif // NETWORKGRAPHWIDGET_H
