#include "Wheel.h"

Wheel::Wheel(const QColor& color, QObject* parent) :
    Lego(color, parent) {
}

Wheel::Wheel(const Wheel& wheel) :
    Lego(wheel) {
}

void Wheel::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, 4, 2, 7);
}

Wheel* Wheel::cloning(void) const {
    return new Wheel(*this);
}

QString Wheel::whoiam(void) const {
    return "I am a Wheel";
}
