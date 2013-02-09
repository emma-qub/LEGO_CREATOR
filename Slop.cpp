#include "Slop.h"

#include <QDebug>

Slop::Slop(int width, int length, SlopType slopType, const QColor &color, QObject* parent) :
    Lego(color, parent),
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

Slop* Slop::cloning(void) const {
    return new Slop(*this);
}

QString Slop::whoiam(void) const {
    return "I am a Slop";
}

void Slop::calculateBoundingBox(void) {
    _boundingBox = BoundingBox(0, 0, 0, _length, _width, 3);
}
