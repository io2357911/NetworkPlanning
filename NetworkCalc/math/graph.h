#ifndef STRUCTURES
#define STRUCTURES

#include <QVector>

namespace Math {

template <typename V, typename E>
class GraphVertex;

template <typename V, typename E>
class GraphEdge {
public:
    GraphEdge(V *vertex1 = NULL, V *vertex2 = NULL)
        : m_vertex1(vertex1),
          m_vertex2(vertex2)
    {
        if (vertex1) vertex1->addNextEdge((E*)this);
        if (vertex2) vertex2->addPreviousEdge((E*)this);
    }

    V *vertex1() { return m_vertex1; }
    void setVertex1(V *vertex) {
        m_vertex1 = vertex;
        m_vertex1->addNextEdge((E*)this);
    }
    V *vertex2() { return m_vertex2; }
    void setVertex2(V *vertex) {
        m_vertex2 = vertex;
        m_vertex2->addPreviousEdge((E*)this);
    }

private:
    V *m_vertex1;
    V *m_vertex2;
};

template <typename V, typename E>
class GraphVertex {
public:
    GraphVertex() {}

    void addNextEdge(E *edge) { m_nextEdges.append(edge); }
    void deleteNextEdge(E *edge) { m_nextEdges.removeAll(edge); }
    void addPreviousEdge(E *edge) { m_previousEdges.append(edge); }
    void deletePreviousEdge(E *edge) { m_previousEdges.removeAll(edge); }

    QVector<E*> nextEdges() { return m_nextEdges; }
    QVector<E*> previousEdges() { return m_previousEdges; }

    QVector<V*> nextVertices() {
        QVector<V*> next;
        for (int i = 0; i < m_nextEdges.size(); i++) next.append(m_nextEdges[i]->vertex2());
        return next;
    }

    QVector<V*> previousVertices() {
        QVector<V*> prev;
        for (int i = 0; i < m_previousEdges.size(); i++) prev.append(m_previousEdges[i]->vertex1());
        return prev;
    }

    bool isFirst() { return m_previousEdges.isEmpty(); }
    bool isLast() { return m_nextEdges.isEmpty(); }

private:
    QVector<E*> m_nextEdges;
    QVector<E*> m_previousEdges;
};

template <typename V, typename E>
class Graph {
public:
    Graph() {}
    ~Graph() {
    }

    Graph(QVector<V*> vertices, QVector<E*> edges)
        : m_vertices(vertices),
          m_edges(edges) {
    }

    void addVertex(V* vertex) {
        m_vertices.append(vertex);
    }

    void deleteVertex(V* vertex) {
        QVector<E*> nextEdges = vertex->nextEdges();
        for (int i = 0; i < nextEdges.size(); i++) {
            m_edges.removeAll(nextEdges[i]);
        }

        QVector<E*> previousEdges = vertex->previousEdges();
        for (int i = 0; i < previousEdges.size(); i++) {
            m_edges.removeAll(previousEdges[i]);
        }

        m_vertices.removeAll(vertex);
    }

    void addEdge(E* edge) {
        m_edges.append(edge);
    }

    void deleteEdge(E* edge) {
        edge->vertex1()->deleteNextEdge(edge);
        edge->vertex2()->deletePreviousEdge(edge);
        m_edges.removeAll(edge);
    }

    void clear() {
        m_vertices.clear();
        m_edges.clear();
    }

    QVector<V*> vertices() { return m_vertices; }
    QVector<E*> edges() { return m_edges; }

    V *firstVertex() {
        for (int i = 0; i < m_vertices.size(); i++) {
            if (m_vertices[i]->isFirst())
                return m_vertices[i];
        }
        return NULL;
    }

    V *lastVertex() {
        for (int i = 0; i < m_vertices.size(); i++) {
            if (m_vertices[i]->isLast())
                return m_vertices[i];
        }
        return NULL;
    }

    bool isNull() { return m_vertices.isEmpty() && m_edges.isEmpty(); }

private:
    QVector<V*> m_vertices;
    QVector<E*> m_edges;
};

}

#endif // STRUCTURES

