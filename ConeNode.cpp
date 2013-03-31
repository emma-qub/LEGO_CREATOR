#include "ConeNode.h"

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Material>
#include <osgUtil/SmoothingVisitor>

#include <vector>

ConeNode::ConeNode() :
    LegoNode() {
}

ConeNode::ConeNode(Cone *cone) :
    LegoNode(cone) {

    createGeode();
}

ConeNode::ConeNode(const ConeNode& coneNode) :
    LegoNode(coneNode) {
}

void ConeNode::createGeode(void) {
    // Remove children
    removeChildren(0, getNumChildren());

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);

    // Get the Cone
    Cone* cone = static_cast<Cone*>(_lego);

    // Get brick color
    QColor color = cone->getColor();

    // Get integer sizes
    int height = 3;

    // Set double values
    double zCenter = Lego::plot_bottom_height/4;
    double length = height*Lego::height_unit-2*zCenter;

    // Create Cone
    geode->addDrawable(createTruncatedCone(Lego::length_unit/2,
                                           Lego::plot_top_radius,
                                           zCenter,
                                           length,
                                           50));

    // Add plot
    addChild(createPlotCylinderAndTop(0, 0, height));
    ////geode->addDrawable(createPlotTop(0, 0, height));

    // Add bottom cylinder
    addChild(createBottomCylinder(0, 0, 0.5, false, (-height+0.5)*Lego::height_unit/2));

    // Add bottom cache
    osg::ref_ptr<osg::ShapeDrawable> cache = new osg::ShapeDrawable(
                new osg::Cylinder(
                    osg::Vec3(0, 0, zCenter-length/2),
                    Lego::length_unit/2,
                    0.1));
    cache->setColor(osg::Vec4(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0));
    geode->addDrawable(cache);
}

osg::Geometry* ConeNode::createTruncatedCone(double startRadius, double endRadius, double center, double length, int numberSegments) {
    // Get the Cone
    Cone* cone = static_cast<Cone*>(_lego);

    // Get brick color
    QColor color = cone->getColor();

    // Create angle values
    float angle = 0.0f;
    float angleDelta = 2.0f * osg::PI/(float)numberSegments;

    // Create arrays to hold the X & Y coeffiecients
    std::vector<float> xTop = std::vector<float>(numberSegments+1);
    std::vector<float> yTop = std::vector<float>(numberSegments+1);
    std::vector<float> xBottom = std::vector<float>(numberSegments+1);
    std::vector<float> yBottom = std::vector<float>(numberSegments+1);

    // Fill top and bottom point vectors
    for (int i = 0; i < numberSegments; i++, angle -= angleDelta)
    {
        // Compute the cos/sin of the current angle as we rotate around the cylinder
        float cosAngle = cosf(angle);
        float sinAngle = sinf(angle);

        // Compute the top/bottom locations
        xTop[i] = cosAngle * endRadius;
        yTop[i] = sinAngle * endRadius;

        xBottom[i] = cosAngle * startRadius;
        yBottom[i] = sinAngle * startRadius;
    }

    // Put the last point equal to the first point so the cylinder
    // is complete
    xTop[numberSegments] = xTop[0];
    yTop[numberSegments] = yTop[0];
    xBottom[numberSegments] = xBottom[0];
    yBottom[numberSegments] = yBottom[0];

    // Create an array to hold the cylinder vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;

    // Create surrounding vertices and fill normal array
    for (int k = 0; k <= numberSegments; k++) {
        // Set the vertex location
        vertices->push_back(osg::Vec3(xBottom[k], yBottom[k], center-length/2));
        vertices->push_back(osg::Vec3(xTop[k], yTop[k], center+length/2));
        vertices->push_back(osg::Vec3(xTop[k+1], yTop[k+1], center+length/2));
        vertices->push_back(osg::Vec3(xBottom[k+1], yBottom[k+1], center-length/2));
    }

    // Create cone geometry
    osg::ref_ptr<osg::Geometry> coneGeometry = new osg::Geometry;

    // Set the vertices on the cone
    coneGeometry->setVertexArray(vertices);

    // Create colors
    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(colorVec);

    // Match colors
    coneGeometry->setColorArray(colors.get());
    coneGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create numberSegments GL_QUADS, i.e. numberSegments*4 vertices
    coneGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, numberSegments*4));

    // Smooth visitor to create normals automagically
    osgUtil::SmoothingVisitor::smooth(*coneGeometry);

    // Return cone geometry
    return coneGeometry.release();
}

ConeNode* ConeNode::cloning(void) const {
    return new ConeNode(*this);
}
