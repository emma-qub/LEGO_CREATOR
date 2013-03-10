#include "LegoNode.h"

LegoNode::LegoNode(osg::ref_ptr<Lego> lego) :
    osg::Group() {

    _lego = lego;

    // Because LEGO bricks don't move
    setDataVariance(osg::Object::STATIC);
}

LegoNode::LegoNode(const LegoNode& legoNode) :
    osg::Group(legoNode) {

    _lego = legoNode._lego;

    // Because LEGO bricks don't move
    setDataVariance(osg::Object::STATIC);
}

LegoNode::~LegoNode() {
}

osg::ref_ptr<osg::Drawable> LegoNode::createPlot(double radiusX, double radiusY, int height) const {
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

    // Because LEGO bricks don't move
    plot->setDataVariance(osg::Object::STATIC);

    return plot.get();
}


osg::ref_ptr<osg::Drawable> LegoNode::createCylinder(double radiusX, double radiusY, double height, bool thin, double center) const {
    // Get plate color
    QColor color = _lego->getColor();

    // Create bottom cylinder radius according to oddwise width
    // Lego bricks with a width = 1 have thin bottom cylinders
    // Lego bricks with a width > 1 have wide bottom cylinders
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

    // Because LEGO bricks don't move
    cyl->setDataVariance(osg::Object::STATIC);

    return cyl.get();
}
