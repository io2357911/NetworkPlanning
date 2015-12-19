#ifndef STRUCTURES
#define STRUCTURES

#include <QVector>

#include "matrix.h"

/*template <typename V, typename E>
class IGraphVertex;

template <typename V, typename E>
class IGraphEdge {
public:
    IGraphEdge(E *value = NULL) : m_value(value) {}
    IGraphEdge(E *value, IGraphVertex<V,E> *vertex1, IGraphVertex<V,E> *vertex2)
        : m_value(value),
        m_vertex1(vertex1),
        m_vertex2(vertex2) {
      vertex1->addNextEdge(this);
      vertex2->addPreviousEdge(this);
    }

    E *value() { return m_value; }
    void setValue(E *value) { m_value = value; }

    virtual IGraphVertex<V,E> *vertex1() { return m_vertex1; }
    virtual IGraphVertex<V,E> *vertex2() { return m_vertex2; }

private:
    E *m_value;
    IGraphVertex<V,E> *m_vertex1;
    IGraphVertex<V,E> *m_vertex2;
};

template <typename V, typename E>
class IGraphVertex {
public:
    IGraphVertex(V *value = NULL) : m_value(value) {}

    V *value() { return m_value; }

    void addNextEdge(IGraphEdge<V,E> *edge) { m_nextEdges.append(edge); }
    void addPreviousEdge(IGraphEdge<V,E> *edge) { m_previousEdges.append(edge); }

    QVector<IGraphEdge<V,E>*> nextEdges() { return m_nextEdges; }
    QVector<IGraphEdge<V,E>*> previousEdges() { return m_previousEdges; }

    QVector<IGraphVertex*> nextVertices() {
        QVector<IGraphVertex*> next;
        for (int i = 0; i < m_nextEdges.size(); i++) next.append(m_nextEdges[i]->vertex2());
        return next;
    }

    QVector<IGraphVertex*> previousVertices() {
        QVector<IGraphVertex*> prev;
        for (int i = 0; i < m_previousEdges.size(); i++) prev.append(m_previousEdges[i]->vertex1());
        return prev;
    }

    bool isFirst() { return m_previousEdges.isEmpty(); }
    bool isLast() { return m_nextEdges.isEmpty(); }

private:
    V *m_value;
    QVector<IGraphEdge<V,E>*> m_nextEdges;
    QVector<IGraphEdge<V,E>*> m_previousEdges;
};*/

template <typename V, typename E>
class Graph {
public:
    class Edge;
    class Vertex;

    Graph() {}
    ~Graph() {
        //for (int i = 0; i < m_vertices.size(); i++) delete m_vertices[i];
        //for (int i = 0; i < m_edges.size(); i++) delete m_edges[i];
    }

    Graph(QVector<Vertex*> vertices, QVector<Edge*> edges)
        : m_vertices(vertices),
          m_edges(edges) {

    }

    Graph(QVector<V*> verticesValues, QVector<E*> edgesValues, Matrix<int> &adjacencyMatrix) {
        if (adjacencyMatrix.cols() != adjacencyMatrix.rows()) return;
        if (verticesValues.size() != adjacencyMatrix.rows()) return;

        for (int i = 0; i < verticesValues.size(); i++) {
            m_vertices.append(new Vertex(verticesValues[i]));
        }

        for (int i = 0; i < adjacencyMatrix.rows(); i++) {
            for (int j = 0; j < adjacencyMatrix.cols(); j++) {
                int edgeNum = adjacencyMatrix[i][j];
                if (edgeNum >= 0 && edgeNum < edgesValues.size()) {
                    m_edges.append(new Edge(edgesValues[edgeNum], m_vertices[i], m_vertices[j]));
                }
            }
        }
    }

    void addVertex(Vertex* vertex) {
        m_vertices.append(vertex);
    }

    void deleteVertex(Vertex* vertex) {
        QVector<Edge*> nextEdges = vertex->nextEdges();
        for (int i = 0; i < nextEdges.size(); i++) {
            m_edges.removeAll(nextEdges[i]);
        }

        QVector<Edge*> previousEdges = vertex->previousEdges();
        for (int i = 0; i < previousEdges.size(); i++) {
            m_edges.removeAll(previousEdges[i]);
        }

        m_vertices.removeAll(vertex);
    }

    void addEdge(Edge* edge) {
        m_edges.append(edge);
    }

    void deleteEdge(Edge* edge) {
        edge->vertex1()->deleteNextEdge(edge);
        edge->vertex2()->deletePreviousEdge(edge);
        m_edges.removeAll(edge);
    }

    void clear() {
        m_vertices.clear();
        m_edges.clear();
    }

    QVector<Vertex*> vertices() { return m_vertices; }
    QVector<Edge*> edges() { return m_edges; }

    Vertex *firstVertex() {
        for (int i = 0; i < m_vertices.size(); i++) {
            if (m_vertices[i]->isFirst())
                return m_vertices[i];
        }
        return NULL;
    }

    Vertex *lastVertex() {
        for (int i = 0; i < m_vertices.size(); i++) {
            if (m_vertices[i]->isLast())
                return m_vertices[i];
        }
        return NULL;
    }

    bool isNull() { return m_vertices.isEmpty() && m_edges.isEmpty(); }

private:
    QVector<Vertex*> m_vertices;
    QVector<Edge*> m_edges;
};

template <typename V, typename E>
class Graph<V,E>::Vertex {
public:
    Vertex(V *value = NULL) : m_value(value) {}

    V *value() { return m_value; }
    void setValue(V *value) { m_value = value; }

    void addNextEdge(Graph<V,E>::Edge *edge) { m_nextEdges.append(edge); }
    void deleteNextEdge(Graph<V,E>::Edge *edge) { m_nextEdges.removeAll(edge); }
    void addPreviousEdge(Graph<V,E>::Edge *edge) { m_previousEdges.append(edge); }
    void deletePreviousEdge(Graph<V,E>::Edge *edge) { m_previousEdges.removeAll(edge); }

    QVector<Graph<V,E>::Edge*> nextEdges() { return m_nextEdges; }
    QVector<Graph<V,E>::Edge*> previousEdges() { return m_previousEdges; }

    QVector<Graph<V,E>::Vertex*> nextVertices() {
        QVector<Graph<V,E>::Vertex*> next;
        for (int i = 0; i < m_nextEdges.size(); i++) next.append(m_nextEdges[i]->vertex2());
        return next;
    }

    QVector<Graph<V,E>::Vertex*> previousVertices() {
        QVector<Graph<V,E>::Vertex*> prev;
        for (int i = 0; i < m_previousEdges.size(); i++) prev.append(m_previousEdges[i]->vertex1());
        return prev;
    }

    bool isFirst() { return m_previousEdges.isEmpty(); }
    bool isLast() { return m_nextEdges.isEmpty(); }

private:
    V *m_value;
    QVector<Graph<V,E>::Edge*> m_nextEdges;
    QVector<Graph<V,E>::Edge*> m_previousEdges;
};

template <typename V, typename E>
class Graph<V,E>::Edge {
public:
    Edge(E *value = NULL) : m_value(value) {}
    Edge(E *value, Graph<V,E>::Vertex *vertex1, Graph<V,E>::Vertex *vertex2)
        : m_value(value),
          m_vertex1(vertex1),
          m_vertex2(vertex2) {
        vertex1->addNextEdge(this);
        vertex2->addPreviousEdge(this);
    }

    E *value() { return m_value; }
    void setValue(E *value) { m_value = value; }

    Graph<V,E>::Vertex *vertex1() { return m_vertex1; }
    void setVertex1(Graph<V,E>::Vertex *vertex) {
        m_vertex1 = vertex;
        m_vertex1->addNextEdge(this);
    }
    Graph<V,E>::Vertex *vertex2() { return m_vertex2; }
    void setVertex2(Graph<V,E>::Vertex *vertex) {
        m_vertex2 = vertex;
        m_vertex2->addPreviousEdge(this);
    }

private:
    E *m_value;
    Graph<V,E>::Vertex *m_vertex1;
    Graph<V,E>::Vertex *m_vertex2;
};

#endif // STRUCTURES

