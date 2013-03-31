#include "FrontShipNode.h"

#include <osg/Geometry>
#include <osg/Material>

#include <cmath>

FrontShipNode::FrontShipNode() {
    LegoNode();
}

FrontShipNode::FrontShipNode(FrontShip* frontShip) :
    LegoNode(frontShip) {

    createGeode();
}

FrontShipNode::FrontShipNode(const FrontShipNode& frontShipNode) :
    LegoNode(frontShipNode) {
}

void FrontShipNode::createGeode(void) {
    // Get integer sizes
    int width = 3;
    int length = 4;
    int height = 1;

    // Remove children
    removeChildren(0, getNumChildren());

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Calculate x max and y max for plots
    double xmin = -(width+1)*Lego::length_unit/2;
    double ymin = -(length-1)*Lego::length_unit/2;

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);
    geode->addDrawable(createClassic());


    // Add plots iteratively
    for (int j = 1; j < 3; j++) {
        for (int i = 1; i < 3; i++) {
            double radiusX = xmin + i*distPlot;
            double radiusY = ymin + j*distPlot;
            addChild(createPlotCylinderAndTop(radiusX, radiusY, height));
            //geode->addDrawable(createPlotTop(radiusX, radiusY, height));
        }
    }

    double radiusX = (width-4)*Lego::length_unit/2;
    double radiusY = (length-4)*Lego::length_unit/2;
    addChild(createBottomCylinder(radiusX, radiusY, 0.5, false, (-height+0.5)*Lego::height_unit/2));
}


osg::Drawable* FrontShipNode::createClassic(void) const {
    // Get the frontShip
    FrontShip* frontShip = static_cast<FrontShip*>(_lego);

    // Get frontShip color
    QColor color = frontShip->getColor();

    // Get integer sizes
    int width = 3; // 3
    int length = 4; // 4
    int height = 1;

    // Get real position, according to frontShip size
    // d : down, u : up, l : left, r : right, f : front, b : back
    double f = (-width)*Lego::length_unit/2; // f
    double b = (width)*Lego::length_unit/2; // b
    double m = (1)*Lego::length_unit/2; // b

    double lx = (-length)*Lego::length_unit/2; // l
    double rx = (length)*Lego::length_unit/2; // r
    double l = (-length+2)*Lego::length_unit/2; // l
    double r = (length-2)*Lego::length_unit/2; // r

    double d = (-height)*Lego::height_unit/2; // d
    double u = (height)*Lego::height_unit/2; // u

    // Create 16 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    // Up vertices
    osg::Vec3 v0(b, lx, u);
    osg::Vec3 v1(f, l, u);
    osg::Vec3 v2(f, r, u);
    osg::Vec3 v3(b, rx, u);
    osg::Vec3 v4(b, r, u);
    osg::Vec3 v5(m, r, u);
    osg::Vec3 v6(m, l, u);
    osg::Vec3 v7(b, l, u);
    // Down vertices
    osg::Vec3 v0d(b, lx, d);
    osg::Vec3 v1d(f, l, d);
    osg::Vec3 v2d(f, r, d);
    osg::Vec3 v3d(b, rx, d);
    osg::Vec3 v4d(b, r, d);
    osg::Vec3 v5d(m, r, d);
    osg::Vec3 v6d(m, l, d);
    osg::Vec3 v7d(b, l, d);


    // Create 10 faces, 8 faces are quads splitted in two triangles
    // NB: Down face is transparent, we don't even create it

    // UP
    // Up face t1
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v7);
    // Up face t2
    vertices->push_back(v6);
    vertices->push_back(v1);
    vertices->push_back(v2);

    // Up face t3
    vertices->push_back(v6);
    vertices->push_back(v2);
    vertices->push_back(v5);
    // Up face t4
    vertices->push_back(v4);
    vertices->push_back(v2);
    vertices->push_back(v3);

    // Sides
    // 2
    vertices->push_back(v1);
    vertices->push_back(v1d);
    vertices->push_back(v2d);
    // 2b
    vertices->push_back(v1);
    vertices->push_back(v2d);
    vertices->push_back(v2);

    // 2
    vertices->push_back(v2);
    vertices->push_back(v2d);
    vertices->push_back(v3d);
    // 2b
    vertices->push_back(v2);
    vertices->push_back(v3d);
    vertices->push_back(v3);

    // 3
    vertices->push_back(v3);
    vertices->push_back(v3d);
    vertices->push_back(v4d);
    // 3b
    vertices->push_back(v3);
    vertices->push_back(v4d);
    vertices->push_back(v4);

    // 4
    vertices->push_back(v4);
    vertices->push_back(v4d);
    vertices->push_back(v5d);
    // 4b
    vertices->push_back(v4);
    vertices->push_back(v5d);
    vertices->push_back(v5);

    // 5
    vertices->push_back(v5);
    vertices->push_back(v5d);
    vertices->push_back(v6d);
    // 5b
    vertices->push_back(v5);
    vertices->push_back(v6d);
    vertices->push_back(v6);

    // 6
    vertices->push_back(v6);
    vertices->push_back(v6d);
    vertices->push_back(v7d);
    // 6b
    vertices->push_back(v6);
    vertices->push_back(v7d);
    vertices->push_back(v7);

    // 7
    vertices->push_back(v7);
    vertices->push_back(v7d);
    vertices->push_back(v0d);
    // 7b
    vertices->push_back(v7);
    vertices->push_back(v0d);
    vertices->push_back(v0);

    // 8
    vertices->push_back(v0);
    vertices->push_back(v0d);
    vertices->push_back(v1d);
    // 8b
    vertices->push_back(v0);
    vertices->push_back(v1d);
    vertices->push_back(v1);

    // Create frontShip geometry
    osg::ref_ptr<osg::Geometry> frontShipGeometry = new osg::Geometry;

    // Match vertices
    frontShipGeometry->setVertexArray(vertices);

    // Add color (each rectangle has the same color except for the down one which is transparent)
    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(colorVec);

    // Match color
    frontShipGeometry->setColorArray(colors);
    frontShipGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1/std::sqrt(10), 3/std::sqrt(10), 0));
    normals->push_back(osg::Vec3(1/std::sqrt(10), 3/std::sqrt(10), 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1/std::sqrt(10), -3/std::sqrt(10), 0));
    normals->push_back(osg::Vec3(1/std::sqrt(10), -3/std::sqrt(10), 0));

    // Match normals
    frontShipGeometry->setNormalArray(normals);
    frontShipGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define frontShip 20 GL_TRIANGLES with 20*3 vertices
    frontShipGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 20*3));

    // Return the frontShip whithout plot
    return frontShipGeometry.release();
}


FrontShipNode* FrontShipNode::cloning(void) const {
    return new FrontShipNode(*this);
}
