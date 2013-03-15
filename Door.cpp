#include "Door.h"

Door::Door(const QColor& doorColor, const QColor& doorHandleColor, const QColor& color) :
    Lego(color),
    _doorColor(doorColor),
    _doorHandleColor(doorHandleColor) {

    calculateBoundingBox();
}

Door::Door(const Door& door) :
    Lego(door) {

    _doorColor = door._doorColor;
    _doorHandleColor = door._doorHandleColor;

    calculateBoundingBox();
}

void Door::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, 4, 1, 18);
}

Door* Door::cloning(void) const {
    return new Door(*this);
}

QString Door::whoiam(void) const {
    return "Door";
}
