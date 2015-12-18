#ifndef EVENTWIDGET_H
#define EVENTWIDGET_H

#include <QWidget>
#include <QMouseEvent>

namespace Ui {
class EventWidget;
}

class EventWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EventWidget(QWidget *parent = 0);
    ~EventWidget();

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

private:
    Ui::EventWidget *ui;

    QAction *aConnection;
    QAction *aProperties;
    QAction *aDelete;

    QPoint offset;
};

#endif // EVENTWIDGET_H
