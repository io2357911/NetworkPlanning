#include "debug.h"

Debug::Debug()
{
}

void Debug::printGraph(NetworkGraph &graph) {
    if (graph.isNull()) {
        qDebug("printGraphInfo(): Graph is null!");
        return;
    }

    QVector<Event*> events = graph.vertices();

    for (int i = 0; i < events.size(); i++) {

        qDebug("\nVertex: %s first: %d last: %d",
               events[i]->name().toStdString().c_str(),
               events[i]->isFirst(),
               events[i]->isLast());

        qDebug("-------NextEdges-------");
        printEdges(events[i]->nextEdges());

        qDebug("-------PriveousEdges-------");
        printEdges(events[i]->previousEdges());
    }
}

void Debug::printEdges(QVector<Work *> edges) {
    for (int j = 0; j < edges.size(); j++) {
        qDebug("Edge: %s v1: %s v2: %s, reserve: %f",
               edges[j]->name().toStdString().c_str(),
               edges[j]->vertex1()->name().toStdString().c_str(),
               edges[j]->vertex2()->name().toStdString().c_str(),
               edges[j]->fullReserve());
    }
}

void Debug::printEvents(NetworkGraph &graph) {
    if (graph.isNull()) {
        qDebug("printEventsInfo(): Graph is null!");
        return;
    }

    QVector<Event*> events = graph.vertices();

    for (int i = 0; i < events.size(); i++) {
        Event *event = events[i];
        qDebug("Event: %s earlyTime: %f lateTime: %f reserve: %f",
               event->name().toStdString().c_str(),
               event->earlyTime(),
               event->lateTime(),
               event->reserve());
    }
}

void Debug::printCriticalPath(const QVector<Work *> &path) {
    QString sPath = "Critical path: ";
    for (int i = 0; i < path.size(); i++) {
        sPath += QString("%1-").arg(path[i]->name());
    }
    qDebug(sPath.toStdString().c_str());
}

