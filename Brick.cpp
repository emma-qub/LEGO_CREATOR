#include "Brick.h"

#include "QDebug"

Brick::Brick(int width, int length, BrickType brickType, const QColor &color) :
    Lego(color),
    _width(width),
    _length(length),
    _brickType(brickType) {

    calculateBoundingBox();
}

Brick::Brick(const Brick& brick) :
    Lego(brick) {

    _width = brick._width;
    _length = brick._length;
    _brickType = brick._brickType;

    calculateBoundingBox();
}

void Brick::setBrickType(int index) {
    switch (index) {
    case 0 :
        _brickType = classic;
        break;
    case 1 :
        _brickType = plate;
        break;
    case 2 :
        _brickType = flat;
        break;
    }
}

void Brick::calculateBoundingBox(void) {
    switch (_brickType) {
    case classic:
        _boundingBox = BoundingBox(0, 0, 0, _length, _width, 3);
        break;
    case plate:
        _boundingBox = BoundingBox(0, 0, 0, _length, _width, 1);
        break;
    case flat:
        _boundingBox = BoundingBox(0, 0, 0, _length, _width, 1);
        break;
    }
}

Brick* Brick::cloning(void) const {
    return new Brick(*this);
}

QString Brick::whoiam(void) const {
    return "Brick";
}
