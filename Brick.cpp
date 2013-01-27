#include "Brick.h"

Brick::Brick(int width, int length, const QColor &color, QObject* parent) :
    Lego(color, parent),
    _width(width),
    _length(length) {
}

Brick::Brick(const Brick& brick) :
    Lego(brick) {

    _width = brick._width;
    _length = brick._length;
}

Brick* Brick::cloning() const {
    return new Brick(*this);
}
