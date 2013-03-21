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
    case 1:
        _tileType = cornerInt;
        break;
    case 2:
        _tileType = cornerExt;
        break;
    case 3 :
        _tileType = roof;
        break;
    case 4:
        _tileType = bigRoof;
        break;
    }
}

void Tile::calculateBoundingBox(void) {
    // According to roof type, height is different
    switch (_tileType) {
    case classic:
        _boundingBox = BoundingBox(0, 0, 0, _width, _length, 3);
        break;
    case cornerInt:
        _boundingBox = BoundingBox(0, 0, 0, _width, _length, 3);
        break;
    case cornerExt:
        _boundingBox = BoundingBox(0, 0, 0, _width, _length, 3);
        break;
    case roof:
        if (_tileType == classic && _width == 1)
            _boundingBox = BoundingBox(0, 0, 0, _width, _length, 2);
        else
            _boundingBox = BoundingBox(0, 0, 0, _width, _length, 3);
        break;
    case bigRoof:
        _boundingBox = BoundingBox(0, 0, 0, _width, _length, 3);
        break;
    }
}

Tile* Tile::cloning(void) const {
    return new Tile(*this);
}

QString Tile::whoiam(void) const {
    return "Tile";
}
