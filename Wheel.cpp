#include "Wheel.h"

#include <QDebug>

Wheel::Wheel(const QColor& color, QObject* parent) :
    Lego(color, parent) {

    calculateBoundingBox();
}

Wheel::Wheel(const Wheel& wheel) :
    Lego(wheel) {

    calculateBoundingBox();
}

void Wheel::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, 4, 2, 5);
}

Wheel* Wheel::cloning(void) const {
    return new Wheel(*this);
}

QString Wheel::whoiam(void) const {
    return "I am a Wheel";
}
