#include "LegoGeode.h"

#include <QDebug>


LegoGeode::LegoGeode(osg::ref_ptr<Lego> lego) :
    osg::Group() {
    _lego = lego;
}

LegoGeode::LegoGeode(const LegoGeode& legoGeode) :
    osg::Group(legoGeode) {

    _lego = legoGeode._lego;
}

LegoGeode::~LegoGeode() {
}

osg::ref_ptr<osg::Drawable> LegoGeode::createPlot(double radiusX, double radiusY, int height) const {
    // Get plate color
    QColor color = _lego->getColor();

    // The plots are cylinders that start at the plate bottom and above the plate top
    // Since the plate z-middle is 0, the middle of the cylinder equals to the half of the part above the plate
    osg::ref_ptr<osg::ShapeDrawable> plot = new osg::ShapeDrawable(
                                                new osg::Cylinder(
                                                    osg::Vec3(radiusX, radiusY, height*Lego::height_unit/2+Lego::plot_top_height/2),
                                                    Lego::plot_top_radius,
                                                    Lego::plot_top_height-EPS
                                                )
                                            );

    // Set color
    plot->setColor(osg::Vec4(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0));

    return plot.get();
}


osg::ref_ptr<osg::Drawable> LegoGeode::createCylinder(double radiusX, double radiusY, double height, bool thin, double center) const {
    // Get plate color
    QColor color = _lego->getColor();

    double radius;
    if (thin)
        radius = Lego::plot_bottom_thin_radius;
    else
        radius = Lego::plot_bottom_radius;

    // The plots are cylinders that start at the plate bottom and above the plate top
    // Since the plate z-middle is 0, the middle of the cylinder equals to the half of the part above the plate
    osg::ref_ptr<osg::ShapeDrawable> cyl = new osg::ShapeDrawable(
                                                new osg::Cylinder(
                                                    osg::Vec3(radiusX, radiusY, center),
                                                    radius,
                                                    height*Lego::plot_bottom_height
                                                )
                                            );

    // Set color
    cyl->setColor(osg::Vec4(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0));

    return cyl.get();
}
