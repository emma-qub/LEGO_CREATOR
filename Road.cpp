#include "Road.h"

Road::Road(RoadType roadType, const QColor& color, QObject* parent) :
    Lego(color, parent),
    _roadType(roadType) {
}

Road::Road(const Road& road) :
    Lego(road) {

    _roadType = road._roadType;
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
    }
}

Road* Road::cloning(void) const {
    return new Road(*this);
}
