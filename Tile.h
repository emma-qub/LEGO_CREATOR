#ifndef TILE_H
#define TILE_H

#include "Lego.h"

class Tile : public Lego {

public:
    enum TileType { classic, roof, bigRoof };

    Tile(int width = 3, int length = 2, TileType tileType = classic, const QColor &color = QColor(Qt::red));
    Tile(const Tile& tile);

    int getWidth(void) const { return _width; }
    int getLength(void) const { return _length; }
    TileType getTileType(void) const { return _tileType; }

    void setWidth(int width) { _width = width; }
    void setLength(int length) { _length = length; }
    void setTileType(TileType tileType) { _tileType = tileType; }
    void setTileType(int index);

    virtual void calculateBoundingBox(void);

    virtual Tile* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    int _width;
    int _length;
    TileType _tileType;

};

#endif // TILE_H
