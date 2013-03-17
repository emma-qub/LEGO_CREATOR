#ifndef EDGE_H
#define EDGE_H

#include "Lego.h"

class Edge : public Lego {

public:
    enum EdgeType { classic, corner };

    Edge(int length = 2, EdgeType edgeType = classic, const QColor& color = QColor(Qt::red));
    Edge(const Edge& edge);

    int getLength(void) const { return _length; }
    EdgeType getEdgeType(void) const { return _edgeType; }

    void setLength(int length) { _length = length; }
    void setEdgeType(EdgeType edgeType) { _edgeType = edgeType; }
    void setEdgeType(int index);

    virtual void calculateBoundingBox(void);

    virtual Edge* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    int _length;
    EdgeType _edgeType;
};

#endif // EDGE_H
