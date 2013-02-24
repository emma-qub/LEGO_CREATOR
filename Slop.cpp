#include "Slop.h"

#include <QDebug>

Slop::Slop(int width, int length, SlopType slopType, const QColor &color) :
    Lego(color),
    _width(width),
    _length(length),
    _slopType(slopType) {

    calculateBoundingBox();
}

Slop::Slop(const Slop& slop) : Lego(slop) {
    _width = slop._width;
    _length = slop._length;
    _slopType = slop._slopType;

    calculateBoundingBox();
}

void Slop::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, _length, _width, 3);
}

Slop* Slop::cloning(void) const {
    return new Slop(*this);
}

QString Slop::whoiam(void) const {
    return "Slop";
}
