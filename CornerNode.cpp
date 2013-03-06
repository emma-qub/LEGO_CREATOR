#include "CornerNode.h"

#include <osg/Geometry>
#include <osg/Material>
#include <osg/BlendFunc>
#include <osgUtil/SmoothingVisitor>

CornerNode::CornerNode() :
    LegoNode() {
}

CornerNode::CornerNode(osg::ref_ptr<Corner> corner) :
    LegoNode(corner) {

    createGeode();
}

CornerNode::CornerNode(const CornerNode& cornerNode) :
    LegoNode(cornerNode) {
}

void CornerNode::createGeode(void) {
    // Remove children
    removeChildren(0, getNumChildren());

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);
    geode->addDrawable(createCorner());

    // Get the corner
    Corner* corner = static_cast<Corner*>(_lego);

    // Calculate height according to corner type
    int height = corner->calculateHeight();

    // Create plots
    geode->addDrawable(createPlot(Lego::length_unit/2, Lego::length_unit/2, height));
    geode->addDrawable(createPlot(Lego::length_unit/2, -Lego::length_unit/2, height));
    geode->addDrawable(createPlot(-Lego::length_unit/2, -Lego::length_unit/2, height));

    // Create bottom cylinders
    geode->addDrawable(createCylinder(Lego::length_unit/2, 0, height, true));
    geode->addDrawable(createCylinder(0, -Lego::length_unit/2, height, true));
}

osg::ref_ptr<osg::Drawable> CornerNode::createCorner() const {
    // Get the corner
    Corner* corner = static_cast<Corner*>(_lego);

    // Get corner color
    QColor color = corner->getColor();

    // Get integer sizes
    int width = 2;
    int length = 2;
    // Calculate height according to corner type
    int height = corner->calculateHeight();

    // Get real position, according to tile size
    double mw = (-width)*Lego::length_unit/2;
    double pw = (width)*Lego::length_unit/2;
    double ml = (-length)*Lego::length_unit/2;
    double pl = (length)*Lego::length_unit/2;
    double mh = (-height)*Lego::height_unit/2;
    double ph = (height)*Lego::height_unit/2;

    // Create 13 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(ml, mw, ph);
    osg::Vec3 v1(pl, mw, ph);
    osg::Vec3 v2(pl, 0, ph);
    osg::Vec3 v3(pl, pw, ph);
    osg::Vec3 v4(0, pw, ph);
    osg::Vec3 v5(0, 0, ph);
    osg::Vec3 v6(ml, 0, ph);
    osg::Vec3 v7(pl, pw, mh);
    osg::Vec3 v8(0, pw, mh);
    osg::Vec3 v9(0, 0, mh);
    osg::Vec3 v10(ml, 0, mh);
    osg::Vec3 v11(ml, mw, mh);
    osg::Vec3 v12(pl, mw, mh);

    // Create faces
    // Up face q1
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v6);

    // Up face q2
    vertices->push_back(v5);
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v4);

    // Front face q1
    vertices->push_back(v4);
    vertices->push_back(v3);
    vertices->push_back(v7);
    vertices->push_back(v8);

    // Left face q1
    vertices->push_back(v4);
    vertices->push_back(v8);
    vertices->push_back(v9);
    vertices->push_back(v5);

    // Front face q2
    vertices->push_back(v5);
    vertices->push_back(v9);
    vertices->push_back(v10);
    vertices->push_back(v6);

    // Left face q2
    vertices->push_back(v6);
    vertices->push_back(v10);
    vertices->push_back(v11);
    vertices->push_back(v0);

    // Back face
    vertices->push_back(v0);
    vertices->push_back(v11);
    vertices->push_back(v12);
    vertices->push_back(v1);

    // Right face
    vertices->push_back(v3);
    vertices->push_back(v1);
    vertices->push_back(v12);
    vertices->push_back(v7);

    // Create corner geometry
    osg::ref_ptr<osg::Geometry> cornerGeometry = new osg::Geometry;

    // Match vertices
    cornerGeometry->setVertexArray(vertices);

    // Create colors
    osg::Vec4 osgColor(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(osgColor);

    // Macth color
    cornerGeometry->setColorArray(colors);
    cornerGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(1, 0, 0));

    // Match normals
    cornerGeometry->setNormalArray(normals);
    cornerGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Create 8 GL_QUADS, i.e. 8*4 vertices
    cornerGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 8*4));

    // Return corner geometry
    return cornerGeometry.get();
}

CornerNode* CornerNode::cloning(void) const {
    return new CornerNode(*this);
}
