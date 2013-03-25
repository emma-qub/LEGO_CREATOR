#include "Clamp.h"

Clamp::Clamp(const QColor &color) :
    Lego(color) {
    
    calculateBoundingBox();
}

Clamp::Clamp(const Clamp& clamp) :
    Lego(clamp) {
    
    calculateBoundingBox();
}

void Clamp::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, 2, 1, 4);
}

Clamp* Clamp::cloning(void) const {
    return new Clamp(*this);
}

QString Clamp::whoiam(void) const {
    return "Clamp";
}
