#include "Plate.h"

Plate::Plate(int width, int length, const QColor& color, QObject* parent) :
    Lego(color, parent),
    _width(width),
    _length(length) {
}

Plate::Plate(const Plate& plate) :
    Lego(plate) {

    _width = plate._width;
    _length = plate._length;
}

Plate* Plate::cloning(void) const {
    return new Plate(*this);
}

QString Plate::whoiam(void) const {
    return "I am a Plate";
}
