#ifndef EDGE_H
#define EDGE_H

#include "Lego.h"

class Edge : public Lego {

public:
    enum EdgeType { classic, corner };

    Edge(EdgeType edgeType = classic, const QColor& color = QColor(Qt::red));
    Edge(const Edge& edge);

    EdgeType getEdgeType(void) const { return _edgeType; }

    void setEdgeType(EdgeType edgeType) { _edgeType = edgeType; }
    void setEdgeType(int index);

    virtual void calculateBoundingBox(void);

    virtual Edge* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    EdgeType _edgeType;
};

#endif // EDGE_H
