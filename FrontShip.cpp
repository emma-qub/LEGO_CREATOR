#include "FrontShip.h"

FrontShip::FrontShip(const QColor& color) :
    Lego(color) {

    calculateBoundingBox();
}

FrontShip::FrontShip(const FrontShip& frontShip) :
    Lego(frontShip) {

    calculateBoundingBox();
}

void FrontShip::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, 3, 4, 1);
}

FrontShip* FrontShip::cloning(void) const {
    return new FrontShip(*this);
}

QString FrontShip::whoiam(void) const {
    return "FrontShip";
}
