#include "tools.h"

Debug::Debug()
{
}

void Debug::printEdgesInfo(QVector<NetworkGraph::Edge *> edges) {
    for (int j = 0; j < edges.size(); j++) {
        qDebug("Edge: %d v1: %d v2: %d",
               edges[j]->value()->getID(),
               edges[j]->vertex1()->value()->getID(),
               edges[j]->vertex2()->value()->getID());
    }
}

void Debug::printGraph(NetworkGraph &graph) {
    if (graph.isNull()) {
        qDebug("printGraphInfo(): Graph is null!");
        return;
    }

    QVector<NetworkGraph::Vertex*> vertices = graph.vertices();

    for (int i = 0; i < vertices.size(); i++) {

        qDebug("\nVertex: %d first: %d last: %d",
               vertices[i]->value()->getID(),
               vertices[i]->isFirst(),
               vertices[i]->isLast());

        qDebug("-------NextEdges-------");
        printEdgesInfo(vertices[i]->nextEdges());

        qDebug("-------PriveousEdges-------");
        printEdgesInfo(vertices[i]->previousEdges());
    }
}

void Debug::printEvents(NetworkGraph &graph) {
    if (graph.isNull()) {
        qDebug("printEventsInfo(): Graph is null!");
        return;
    }

    QVector<NetworkGraph::Vertex*> vertices = graph.vertices();

    for (int i = 0; i < vertices.size(); i++) {
        IEvent *event = vertices[i]->value();
        qDebug("Event: %d earlyTime: %d lateTime: %d reserve: %d",
               event->getID(),
               event->getEarlyTime(),
               event->getLateTime(),
               event->getReserve());
    }
}

void Debug::printCriticalPath(const QVector<NetworkGraph::Edge *> &path) {
    QString sPath = "Critical path: ";
    for (int i = 0; i < path.size(); i++) {
        sPath += QString("%1-").arg(path[i]->value()->getID());
    }
    qDebug(sPath.toStdString().c_str());
}

