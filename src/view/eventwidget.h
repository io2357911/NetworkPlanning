#ifndef EVENTWIDGET_H
#define EVENTWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include "../tools/ini.h"
#include "../math/networkgraph.h"
#include "../project.h"

using namespace Math::Planning;

namespace Ui {
class EventWidget;
}

class EventWidget : public QWidget, public ProjectEvent
{
    Q_OBJECT

public:
    explicit EventWidget(QWidget *parent = 0);
    ~EventWidget();

    // IPersistent interface
    void store(QString fileName);
    void restore(QString fileName);

    // Event interface
    void setName(QString value);
    void setEarlyTime(double value);
    void setLateTime(double value);
    void setReserve(double value);

    // IRelease interface
    void release();

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent * event);
    void paintEvent(QPaintEvent *evt);
    void contextMenuEvent(QContextMenuEvent *event);

    QPoint center();

signals:
    void clicked(EventWidget *widget);
    void moved(EventWidget *widget);

    void connectMe(EventWidget *widget);
    void properties(EventWidget *widget);
    void deleteMe(EventWidget *widget);

private slots:
    void onConnection() { emit connectMe(this); }
    void onProperties() { emit properties(this); }
    void onDelete() { emit deleteMe(this); }

    void onEventChanged();

private:
    Ui::EventWidget *ui;

    //Event m_event;

    QAction *aConnection;
    QAction *aProperties;
    QAction *aDelete;

    QPoint offset;

    bool isCalculated;
};

#endif // EVENTWIDGET_H
