#include "LegoNode.h"

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Material>
#include <osgUtil/SmoothingVisitor>
#include <osgUtil/Tessellator>

#include <QDebug>

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

osg::Drawable* LegoNode::makeDisk(double xExt, double yExt, double z, double radiusExt, double height, bool isTop, bool hasHole, double xInt, double yInt, double radiusInt, int numberSegments) const {

    return new osg::Geometry;


//    // Calculate real z value, according to whether it's top or bottom disk
//    if (isTop)
//        z += height/2;
//    else
//        z -= height/2;

//    // Get plate color
//    QColor color = _lego->getColor();

//    // Create angle values
//    float angle = 0.0f;
//    float angleDelta = 2.0f * osg::PI/(float)numberSegments;

//    // Create arrays to hold the X & Y coeffiecients
//    int size = numberSegments+1;
//    if (hasHole)
//        size = 2*(numberSegments+1);
//    std::vector<float> xCoords = std::vector<float>(size);
//    std::vector<float> yCoords = std::vector<float>(size);

//    // Fill x and y point vectors
//    for (int i = 0; i < numberSegments; i++, angle -= angleDelta) {
//        // Compute the cos/sin of the current angle as we rotate around the cylinder
//        float cosAngle = cosf(angle);
//        float sinAngle = sinf(angle);

//        // Compute the top/bottom extern locations
//        xCoords[i] = xExt + cosAngle * radiusExt;
//        yCoords[i] = yExt + sinAngle * radiusExt;

//        // Compute the top/bottom intern locations if needed
//        if (hasHole) {
//            xCoords[i+numberSegments+1] = xInt + cosAngle * radiusInt;
//            yCoords[i+numberSegments+1] = yInt + sinAngle * radiusInt;
//        }
//    }

//    // Put the last point equal to the first point
//    // so the disk is complete
//    xCoords[numberSegments] = xCoords[0];
//    yCoords[numberSegments] = yCoords[0];

//    // Idem for intern disk
//    xCoords[2*numberSegments+1] = xCoords[numberSegments+1];
//    yCoords[2*numberSegments+1] = yCoords[numberSegments+1];

//    // Create an array to hold the cylinder vertices
//    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

//    // Add disk center
//    vertices->push_back(osg::Vec3(xExt, yExt, z));
//    // Add top extern vertices
//    for (int k = 0; k < size; k++) {
//        vertices->push_back(osg::Vec3(xCoords[k], yCoords[k], z));
//    }

//    // If there is a whole, we add intern vertices
//    if (hasHole) {
//        // Add top disk center
//        vertices->push_back(osg::Vec3(xInt, yInt, z));
//        // Add top intern vertices (watch the wise)
//        for (int k = size-1; k >= numberSegments+1; k--) {
//            vertices->push_back(osg::Vec3(xCoords[k], yCoords[k], z));
//        }
//    }

//    // Create cylinder geometry
//    osg::ref_ptr<osg::Geometry> cylinderGeometry = new osg::Geometry;

//    // Set the vertices on the cylinder
//    cylinderGeometry->setVertexArray(vertices);

//    // Create colors
//    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
//    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
//    // Every face has the same color, so there is only one color
//    colors->push_back(colorVec);

//    // Match colors
//    cylinderGeometry->setColorArray(colors.get());
//    cylinderGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

//    // Create normals
//    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
//    if (isTop)
//        normals->push_back(osg::Vec3(0, 0, 1));
//    else
//        normals->push_back(osg::Vec3(0, 0, -1));

//    // Match normals
//    cylinderGeometry->setNormalArray(normals);
//    cylinderGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

//    // Create numberSegments GL_QUADS, i.e. numberSegments*4 vertices
//    cylinderGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN, 0, size+1));

//    // Retesslate to create hole if needed
//    if (hasHole) {
//        osgUtil::Tessellator tesslator;
//        tesslator.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
//        tesslator.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
//        tesslator.retessellatePolygons(*cylinderGeometry);
//    }

//    // Return cone geometry
//    return cylinderGeometry.release();
}

osg::Drawable *LegoNode::makeCylinder(double x, double y, double z, double height, double radius, int numberSegments) const {
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
    for (int k = 0; k < numberSegments; k++) {
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

osg::Drawable* LegoNode::createPlotCylinder(double radiusX, double radiusY, int height) const {
//    // Get plate color
//    QColor color = _lego->getColor();

    // The plots are cylinders that start at the plate bottom and above the plate top
    // Since the plate z-middle is 0, the middle of the cylinder equals to the half of the part above the plate
    osg::ref_ptr<osg::Drawable> plotCylinder = makeCylinder(radiusX, radiusY, height*Lego::height_unit/2+Lego::plot_top_height/2,
                                               Lego::plot_top_height-EPS, Lego::plot_top_radius);
//    osg::ref_ptr<osg::ShapeDrawable> plot = new osg::ShapeDrawable(
//                                                new osg::Cylinder(
//                                                    osg::Vec3(radiusX, radiusY, height*Lego::height_unit/2+Lego::plot_top_height/2),
//                                                    Lego::plot_top_radius,
//                                                    Lego::plot_top_height-EPS
//                                                )
//                                            );

//    // Set color
//    plot->setColor(osg::Vec4(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0));

    // Because LEGO bricks don't move
    plotCylinder->setDataVariance(osg::Object::STATIC);

//    osg::ref_ptr<osg::Drawable> plotTop = makeDisk(radiusX, radiusY, height*Lego::height_unit/2+Lego::plot_top_height/2,
//                                                   Lego::plot_top_radius, Lego::plot_top_height-EPS, true);

//    osg::ref_ptr<osg::Geode> plot = new osg::Geode;
//    plot->addDrawable(plotCylinder);
//    plot->addDrawable(plotTop);

    return plotCylinder.release();
}

osg::Drawable *LegoNode::createPlotTop(double radiusX, double radiusY, int height) const {
    osg::ref_ptr<osg::Drawable> plotTop = makeDisk(radiusX, radiusY, height*Lego::height_unit/2+Lego::plot_top_height/2,
                                                   Lego::plot_top_radius, Lego::plot_top_height-EPS, true);

    plotTop->setDataVariance(osg::Object::STATIC);

    return plotTop.release();
}

osg::Drawable* LegoNode::createCylinder(double radiusX, double radiusY, double height, bool thin, double center) const {
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

    return cyl.release();
}
