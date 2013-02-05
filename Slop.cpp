#include "Slop.h"

#include <QDebug>

Slop::Slop(int length, int width, const QColor &color, SlopType slopType, QObject* parent) :
    Lego(color, parent),
    _length(length),
    _width(width),
    _slopType(slopType) {

    calculateBoundingBox();
}

Slop::Slop(const Slop& slop) : Lego(slop) {
    _length = slop._length;
    _width = slop._width;
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
    qDebug() << "Bouh";

    _boundingBox = BoundingBox(0, 0, 0, _length, _width, 3);
}
