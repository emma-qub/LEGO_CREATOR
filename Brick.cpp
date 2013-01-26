#include "Brick.h"

Brick::Brick(int width, int length, const QColor &color, QObject* parent) :
    Lego(color, parent),
    _width(width),
    _length(length) {
}

QString Brick::description(void) const {
    return QString("Shape:\tBrick\nWidth:\t%1\nLength:\t%2").arg(_width).arg(_length);
}
