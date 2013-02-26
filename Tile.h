#ifndef TILE_H
#define TILE_H

#include "Lego.h"

class Tile : public Lego {

public:
    Tile(int width = 3, int length = 2, const QColor &color = QColor(Qt::red));
    Tile(const Tile& tile);

    int getWidth(void) const { return _width; }
    int getLength(void) const { return _length; }

    void setWidth(int width) { _width = width; }
    void setLength(int length) { _length = length; }

    virtual void calculateBoundingBox(void);

    virtual Tile* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    int _width;
    int _length;
};

#endif // TILE_H
