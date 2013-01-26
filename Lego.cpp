#include "Lego.h"

Lego::Lego(const QColor &color, QObject* parent) :
    QObject(parent),
    _color(color) {
}

Lego::~Lego() {
}
