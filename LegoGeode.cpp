#include "LegoGeode.h"

LegoGeode::LegoGeode(Lego* lego) :
    osg::Geode() {
    _lego = lego;
}

LegoGeode::LegoGeode(const LegoGeode& legoGeode) :
    osg::Geode(legoGeode) {
}

osg::ref_ptr<osg::Drawable> LegoGeode::createPlot(double radiusX, double radiusY, int height) const {
    // Get plate color
    QColor color = _lego->getColor();

    // The plots are cylinders that start at the plate bottom and above the plate top
    // Since the plate z-middle is 0, the middle of the cylinder equals to the half of the part above the plate
    osg::ref_ptr<osg::ShapeDrawable> plot = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(radiusX, radiusY, Lego::plot_top/2), Lego::plot_radius, height*Lego::height_unit+Lego::plot_top));

    // Set color
    plot->setColor(osg::Vec4(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0));

    return plot.get();
}
