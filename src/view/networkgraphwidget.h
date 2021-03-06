#ifndef NETWORKGRAPHWIDGET_H
#define NETWORKGRAPHWIDGET_H

#include <QWidget>
#include <QVector>
#include "eventwidget.h"
#include "workwidget.h"
#include "resourseswidget.h"
#include "eventpropertiesdialog.h"
#include "workpropertiesdialog.h"
#include "../tools/debug.h"
#include "../tools/ini.h"
#include "../math/algorithms.h"
#include "../project.h"

namespace Ui {
class NetworkGraphWidget;
}

class NetworkGraphWidget :
        public QWidget,
        public ProjectEventFactory,
        public ProjectWorkFactory
{
    Q_OBJECT
public:
    explicit NetworkGraphWidget(QWidget *parent = 0);
    ~NetworkGraphWidget();

    void contextMenuEvent(QContextMenuEvent *event);
    void paintEvent(QPaintEvent *evt);
    void keyPressEvent(QKeyEvent *event);
    void wheelEvent(QWheelEvent *event);

    // ProjectWorkFactory interface
    ProjectWork *createWork(Event *firstEvent, Event *secondEvent);

    // ProjectEventFactory interface
    ProjectEvent *createEvent();

public slots:
    void onGraphChanged();

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
    Ui::NetworkGraphWidget *ui;

    EventPropertiesDialog dEvent;
    WorkPropertiesDialog dWork;

    QAction *aNewEvent;
    QAction *aTest;

    EventWidget *firstConnectEvent;
};

#endif // NETWORKGRAPHWIDGET_H
