#include "Door.h"

Door::Door(const QColor &color) :
    Lego(color) {

    calculateBoundingBox();
}

Door::Door(const Door& door) :
    Lego(door) {

    calculateBoundingBox();
}

void Door::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, 4, 2, 18);
}

Door* Door::cloning(void) const {
    return new Door(*this);
}

QString Door::whoiam(void) const {
    return "I am a Door";
}
