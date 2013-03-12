#include "Cylinder.h"

Cylinder::Cylinder(CylinderType cylinderType, const QColor &color) :
    Lego(color),
    _cylinderType(cylinderType) {

    calculateBoundingBox();
}

Cylinder::Cylinder(const Cylinder& cylinder) :
    Lego(cylinder) {

    _cylinderType = cylinder._cylinderType;

    calculateBoundingBox();
}

void Cylinder::setCylinderType(int index) {
    switch (index) {
    case 0:
        _cylinderType = big;
        break;
    case 1:
        _cylinderType = bigPlate;
        break;
    case 2:
        _cylinderType = bigFlat;
        break;
    case 3:
        _cylinderType = thin;
        break;
    case 4:
        _cylinderType = thinPlate;
        break;
    case 5:
        _cylinderType = thinFlat;
        break;
    }
}

void Cylinder::calculateBoundingBox(void) {
    // According to cylinder type, length, width and height are different
    switch (_cylinderType) {
    case big:
        _boundingBox = BoundingBox(0, 0, 0, 2, 2, 3);
        break;
    case bigPlate:
        _boundingBox = BoundingBox(0, 0, 0, 2, 2, 1);
        break;
    case bigFlat:
        _boundingBox = BoundingBox(0, 0, 0, 2, 2, 1);
        break;
    case thin:
        _boundingBox = BoundingBox(0, 0, 0, 1, 1, 3);
        break;
    case thinPlate:
        _boundingBox = BoundingBox(0, 0, 0, 1, 1, 1);
        break;
    case thinFlat:
        _boundingBox = BoundingBox(0, 0, 0, 1, 1, 1);
        break;
    }
}

Cylinder* Cylinder::cloning(void) const {
    return new Cylinder(*this);
}

QString Cylinder::whoiam(void) const {
    return "Cylinder";
}
