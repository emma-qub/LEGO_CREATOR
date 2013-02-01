#include "Corner.h"

Corner::Corner(CornerType cornerType, const QColor &color, QObject *parent) :
    Lego(color, parent),
    _cornerType(cornerType) {

    calculateBoundingBox();
}

Corner::Corner(const Corner& corner) :
    Lego(corner) {

    _cornerType = corner._cornerType;
}

void Corner::calculateBoundingBox(void) {
    switch (_cornerType) {
    case 0:
        _boundingBox = BoundingBox(0, 0, 0, 2, 2, 3);
        break;
    case 1:
        _boundingBox = BoundingBox(0, 0, 0, 2, 2, 1);
        break;
    }
}

Corner* Corner::cloning(void) const {
    return new Corner(*this);
}
