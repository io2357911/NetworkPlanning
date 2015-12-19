#ifndef NETWORKGRAPHWIDGET_H
#define NETWORKGRAPHWIDGET_H

#include <QWidget>
#include <QVector>
#include "eventwidget.h"
#include "workwidget.h"
#include "eventpropertiesdialog.h"
#include "workpropertiesdialog.h"
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


public slots:
    void newGraph();
    void openGraph(QString path);
    void saveGraph(QString path);

    void computeNetworkGraph();
    void computeCriticalPath();

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

private:
    EventWidget *createEventWidget();
    WorkWidget *createWorkWidget(EventWidget *firstEvent, EventWidget *secondEvent);

private:
    Ui::NetworkGraphWidget *ui;

    NetworkGraph graph;

    QVector<EventWidget*> events;
    QVector<WorkWidget*> works;

    QAction *aNewEvent;
    QAction *aTest;

    EventWidget *firstConnectEvent;

    int eventsCounter, worksCounter;
};

#endif // NETWORKGRAPHWIDGET_H
