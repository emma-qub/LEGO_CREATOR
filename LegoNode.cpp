#include "LegoNode.h"

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Material>
#include <osgUtil/SmoothingVisitor>

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

osg::ref_ptr<osg::Drawable> LegoNode::makeDisk(double x, double y, double z, double height, double radius, int numberSegments) const {
    // Get plate color
    QColor color = _lego->getColor();

    // Create angle values
    float angle = 0.0f;
    float angleDelta = 2.0f * osg::PI/(float)numberSegments;

    // Create arrays to hold the X & Y coeffiecients
    std::vector<float> xCoords = std::vector<float>(numberSegments+1);
    std::vector<float> yCoords = std::vector<float>(numberSegments+1);

    // Fill top and bottom point vectors
    for (int i = 0; i < numberSegments; i++, angle -= angleDelta)
    {
        // Compute the cos/sin of the current angle as we rotate around the cylinder
        float cosAngle = cosf(angle);
        float sinAngle = sinf(angle);

        // Compute the top/bottom locations
        xCoords[i] = x + cosAngle * radius;
        yCoords[i] = y + sinAngle * radius;
    }

    // Put the last point equal to the first point so the cylinder
    // is complete
    xCoords[numberSegments] = xCoords[0];
    yCoords[numberSegments] = yCoords[0];

    // Create an array to hold the cylinder vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

    // Add top disk center
    vertices->push_back(osg::Vec3(x, y, z+height/2));
    // Create surrounding vertices and fill normal array
    for (int k = 0; k <= numberSegments; k++) {
        vertices->push_back(osg::Vec3(xCoords[k], yCoords[k], z+height/2));
    }

    // Add bottom disk center
    vertices->push_back(osg::Vec3(x, y, z-height/2));
    // Create surrounding vertices and fill normal array
    for (int k = 0; k <= numberSegments; k++) {
        vertices->push_back(osg::Vec3(xCoords[k], yCoords[k], z-height/2));
    }

    // Create cylinder geometry
    osg::ref_ptr<osg::Geometry> cylinderGeometry = new osg::Geometry;

    // Set the vertices on the cylinder
    cylinderGeometry->setVertexArray(vertices);

    // Create colors
    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(colorVec);

    // Match colors
    cylinderGeometry->setColorArray(colors.get());
    cylinderGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 0, -1));

    // Match normals
    cylinderGeometry->setNormalArray(normals);
    cylinderGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Create numberSegments GL_QUADS, i.e. numberSegments*4 vertices
    cylinderGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN, 0, numberSegments+1));
    cylinderGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN, numberSegments+2, numberSegments+1));

    // Return cone geometry
    return cylinderGeometry.release();
}

osg::ref_ptr<osg::Drawable> LegoNode::makeCylinder(double x, double y, double z, double height, double radius, int numberSegments) const {
    // Get plate color
    QColor color = _lego->getColor();

    // Create angle values
    float angle = 0.0f;
    float angleDelta = 2.0f * osg::PI/(float)numberSegments;

    // Create arrays to hold the X & Y coeffiecients
    std::vector<float> xCoords = std::vector<float>(numberSegments+1);
    std::vector<float> yCoords = std::vector<float>(numberSegments+1);

    // Fill top and bottom point vectors
    for (int i = 0; i < numberSegments; i++, angle -= angleDelta)
    {
        // Compute the cos/sin of the current angle as we rotate around the cylinder
        float cosAngle = cosf(angle);
        float sinAngle = sinf(angle);

        // Compute the top/bottom locations
        xCoords[i] = x + cosAngle * radius;
        yCoords[i] = y + sinAngle * radius;
    }

    // Put the last point equal to the first point so the cylinder
    // is complete
    xCoords[numberSegments] = xCoords[0];
    yCoords[numberSegments] = yCoords[0];

    // Create an array to hold the cylinder vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

    // Create surrounding vertices and fill normal array
    for (int k = 0; k <= numberSegments; k++) {
        vertices->push_back(osg::Vec3(xCoords[k], yCoords[k], z-height/2));
        vertices->push_back(osg::Vec3(xCoords[k], yCoords[k], z+height/2));
        vertices->push_back(osg::Vec3(xCoords[k+1], yCoords[k+1], z+height/2));
        vertices->push_back(osg::Vec3(xCoords[k+1], yCoords[k+1], z-height/2));
    }

    // Create cylinder geometry
    osg::ref_ptr<osg::Geometry> cylinderGeometry = new osg::Geometry;

    // Set the vertices on the cylinder
    cylinderGeometry->setVertexArray(vertices);

    // Create colors
    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(colorVec);

    // Match colors
    cylinderGeometry->setColorArray(colors.get());
    cylinderGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create numberSegments GL_QUADS, i.e. numberSegments*4 vertices
    cylinderGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, numberSegments*4));

    // Smooth visitor to create normals automagically
    osgUtil::SmoothingVisitor::smooth(*cylinderGeometry);

    // Return cone geometry
    return cylinderGeometry.release();
}

osg::ref_ptr<osg::Drawable> LegoNode::makeTrimmedCylinder(/*double xExt, double yExt, double zExt, double xInt, double yInt, double zInt, double height, double radiusExt, double radiusInt, int numberSegments*/) const {
    return NULL;
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
