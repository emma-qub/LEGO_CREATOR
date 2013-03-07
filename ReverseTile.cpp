#include "ReverseTile.h"

ReverseTile::ReverseTile(int width, int length, const QColor& color) :
    Lego(color),
    _width(width),
    _length(length) {

    calculateBoundingBox();
}

ReverseTile::ReverseTile(const ReverseTile& reverseTile) :
    Lego(reverseTile) {

    _width = reverseTile._width;
    _length = reverseTile._length;

    calculateBoundingBox();
}

void ReverseTile::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, _width, _length, 3);
}

ReverseTile* ReverseTile::cloning(void) const {
    return new ReverseTile(*this);
}

QString ReverseTile::whoiam(void) const {
    return "Reverse Tile";
}
