#include "Cone.h"

Cone::Cone(const QColor& color) :
    Lego(color) {

    calculateBoundingBox();
}

Cone::Cone(const Cone& cone) :
    Lego(cone) {

    calculateBoundingBox();
}

void Cone::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, 1, 1, 3);
}

Cone* Cone::cloning(void) const {
    return new Cone(*this);
}

QString Cone::whoiam(void) const {
    return "Cone";
}
