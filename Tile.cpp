#include "Tile.h"

Tile::Tile(int width, int length, const QColor& color) :
    Lego(color),
    _width(width),
    _length(length) {

    calculateBoundingBox();
}

Tile::Tile(const Tile& tile) :
    Lego(tile) {

    _width = tile._width;
    _length = tile._length;

    calculateBoundingBox();
}

void Tile::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, _length, _width, 3);
}

Tile* Tile::cloning(void) const {
    return new Tile(*this);
}

QString Tile::whoiam(void) const {
    return "Tile";
}
