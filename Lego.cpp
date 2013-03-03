#include "Lego.h"
#include <cmath>

// Define Lego units
double Lego::height_unit = 3.2;
double Lego::length_unit = 8;
double Lego::plot_top_radius = 2.5;
double Lego::plot_bottom_radius = std::sqrt(2)/2*Lego::length_unit - Lego::plot_top_radius;
double Lego::plot_bottom_thin_radius = (Lego::length_unit-2*Lego::plot_top_radius)/2;
double Lego::plot_top_height = 1.7;
double Lego::plot_bottom_height = Lego::height_unit-EPS;


Lego::Lego(const QColor &color) :
    osg::Referenced(),
    _color(color) {
}

Lego::Lego(const Lego& lego) :
    osg::Referenced() {

    _color = lego._color;
}

QString Lego::whoiam(void) const {
    return "Lego";
}
