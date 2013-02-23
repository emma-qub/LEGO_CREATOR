#include "Road.h"

Road::Road(RoadType roadType, const QColor& color) :
    Lego(color),
    _roadType(roadType) {

    calculateBoundingBox();
}

Road::Road(const Road& road) :
    Lego(road) {

    _roadType = road._roadType;

    calculateBoundingBox();
}

void Road::setRoadType(int index) {
    switch (index) {
    case 0:
        _roadType = straight;
        break;
    case 1:
        _roadType = curve;
        break;
    case 2:
        _roadType = intersection;
        break;
    case 3:
        _roadType = cross;
        break;
    case 4:
        _roadType = none;
        break;
    }
}

void Road::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, 32, 32, 0);
}

Road* Road::cloning(void) const {
    return new Road(*this);
}

QString Road::whoiam(void) const {
    return "I am a Road";
}
