#include "Edge.h"

#include "QDebug"

Edge::Edge(int length, EdgeType edgeType, const QColor &color) :
    Lego(color),
    _length(length),
    _edgeType(edgeType) {

    calculateBoundingBox();
}

Edge::Edge(const Edge& edge) :
    Lego(edge) {

    _length = edge._length;
    _edgeType = edge._edgeType;

    calculateBoundingBox();
}

void Edge::setEdgeType(int index) {
    switch (index) {
    case 0 :
        _edgeType = classic;
        break;
    case 1 :
        _edgeType = corner;
        break;
    }
}

void Edge::calculateBoundingBox(void) {
    // According to Edge type, height is different
    switch (_edgeType) {
    case classic:
        _boundingBox = BoundingBox(0, 0, 0, _length, 1, 3);
        break;
    case corner:
        _boundingBox = BoundingBox(0, 0, 0, 1, 1, 3);
        break;
    }
}

Edge* Edge::cloning(void) const {
    return new Edge(*this);
}

QString Edge::whoiam(void) const {
    return "Edge";
}
