#include "Lego.h"
#include <cmath>

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

void Lego::translateBoundingBox(double x, double y, double z) {
    _boundingBox.translateLeftBottomFrontCorner(x, y, z);
    _boundingBox.translateRightTopBackCorner(x, y, z);
}

QString Lego::whoiam(void) const {
    return "I am a Lego";
}
