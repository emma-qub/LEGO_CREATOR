#include "Lego.h"

double Lego::height_unit = 3.2;
double Lego::length_unit = 8;
double Lego::plot_radius = 2.5;
double Lego::plot_top = 1.7;


Lego::Lego(const QColor &color, QObject* parent) :
    QObject(parent),
    _color(color) {
}

Lego::Lego(const Lego& lego) :
    QObject() {

    _color = lego._color;
}

QString Lego::whoiam(void) const {
    return "I am a Lego";
}
