#include "Tile.h"

Tile::Tile(int width, int length, TileType tileType, const QColor& color) :
    Lego(color),
    _width(width),
    _length(length),
    _tileType(tileType) {

    calculateBoundingBox();
}

Tile::Tile(const Tile& tile) :
    Lego(tile) {

    _width = tile._width;
    _length = tile._length;
    _tileType = tile._tileType;

    calculateBoundingBox();
}

void Tile::setTileType(int index) {
    switch (index) {
    case 0 :
        _tileType = classic;
        break;
    case 1 :
        _tileType = roof;
        break;
    case 2:
        _tileType = bigRoof;
    }
}

void Tile::calculateBoundingBox(void) {
    if (_tileType == roof)
        _boundingBox = BoundingBox(0, 0, 0, _width, _length, 2);
    else
        _boundingBox = BoundingBox(0, 0, 0, _width, _length, 3);
}

Tile* Tile::cloning(void) const {
    return new Tile(*this);
}

QString Tile::whoiam(void) const {
    return "Tile";
}
