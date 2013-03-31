#include "ReverseTileNode.h"

#include <osg/Geometry>
#include <osg/Material>

#include <cmath>

ReverseTileNode::ReverseTileNode() :
    LegoNode() {
}

ReverseTileNode::ReverseTileNode(ReverseTile *reverseTile) :
    LegoNode(reverseTile) {

    createGeode();
}

ReverseTileNode::ReverseTileNode(const ReverseTileNode& reverseTileNode) :
    LegoNode(reverseTileNode) {
}

void ReverseTileNode::createGeode(void) {
    // Remove children
    removeChildren(0, getNumChildren());

    // Get the tile
    ReverseTile* tile = static_cast<ReverseTile*>(_lego);

    // Get integer sizes
    int width = tile->getWidth();
    int length = tile->getLength();
    int height = 3;

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Calculate x max and y max for plots
    double xmin = -(width-1)*Lego::length_unit/2;
    double ymin = -(length-1)*Lego::length_unit/2;

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);
    geode->addDrawable(createReverseTile());

    // Add plots iteratively
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < length; j++) {
            double radiusX = xmin + i*distPlot;
            double radiusY = ymin + j*distPlot;
            geode->addDrawable(createPlotCylinder(radiusX, radiusY, height));
            geode->addDrawable(createPlotTop(radiusX, radiusY, height));
        }
    }
}

osg::Drawable *ReverseTileNode::createReverseTile(void) const {
    // Get the tile
    ReverseTile* tile = static_cast<ReverseTile*>(_lego);

    // Get tile color
    QColor color = tile->getColor();

    // Get integer sizes
    int width = tile->getWidth();
    int length = tile->getLength();
    int height = 3;

    // Get real position, according to tile size
    double mw = (-width)*Lego::length_unit/2;
    double pw = (width)*Lego::length_unit/2;
    double mwp = (-width+2)*Lego::length_unit/2;
    double ml = (-length)*Lego::length_unit/2;
    double pl = (length)*Lego::length_unit/2;
    double mh = (-height)*Lego::height_unit/2;
    double ph = (height)*Lego::height_unit/2;
    double phm = (height-1)*Lego::height_unit/2;

    // Create 14 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(mw, ml, mh);
    osg::Vec3 v1(mw, pl, mh);
    osg::Vec3 v2(mwp, pl, mh);
    osg::Vec3 v3(mwp, ml, mh);
    osg::Vec3 v4(pw, ml, phm);
    osg::Vec3 v5(pw, pl, phm);
    osg::Vec3 v6(pw, pl, ph);
    osg::Vec3 v7(pw, ml, ph);
    osg::Vec3 v8(mw, ml, ph);
    osg::Vec3 v9(mw, pl, ph);
    osg::Vec3 v10(mwp, ml, phm);
    osg::Vec3 v11(mwp, ml, ph);
    osg::Vec3 v12(mwp, pl, ph);
    osg::Vec3 v13(mwp, pl, phm);

    // Create 10 faces, 8 faces are quads splitted into two triangles
    // NB: Down face is transparent, we don't even create it

    // Front face t1
    vertices->push_back(v4);
    vertices->push_back(v5);
    vertices->push_back(v6);
    // Front face t2
    vertices->push_back(v4);
    vertices->push_back(v6);
    vertices->push_back(v7);

    // Back face t1
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v8);
    // Back face t2
    vertices->push_back(v1);
    vertices->push_back(v8);
    vertices->push_back(v9);

    // Top face t1
    vertices->push_back(v6);
    vertices->push_back(v7);
    vertices->push_back(v9);
    // Top face t2
    vertices->push_back(v7);
    vertices->push_back(v8);
    vertices->push_back(v9);

    // Slop face t1
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v5);
    // Slop face t2
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v5);

    // Right triangle face
    vertices->push_back(v2);
    vertices->push_back(v13);
    vertices->push_back(v5);

    // Right quad face t1
    vertices->push_back(v13);
    vertices->push_back(v12);
    vertices->push_back(v6);
    // Right quad face t2
    vertices->push_back(v13);
    vertices->push_back(v6);
    vertices->push_back(v5);

    // Right quad face down t1
    vertices->push_back(v1);
    vertices->push_back(v9);
    vertices->push_back(v12);
    // Right quad face down t2
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v12);

    // Left triangle face
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v10);

    // Left quad face t1
    vertices->push_back(v4);
    vertices->push_back(v10);
    vertices->push_back(v11);
    // Left quad face t2
    vertices->push_back(v4);
    vertices->push_back(v7);
    vertices->push_back(v11);

    // Left quad face down t1
    vertices->push_back(v0);
    vertices->push_back(v3);
    vertices->push_back(v8);
    // Left quad face down t2
    vertices->push_back(v3);
    vertices->push_back(v8);
    vertices->push_back(v11);

    // Create tile geometry
    osg::ref_ptr<osg::Geometry> tileGeometry = new osg::Geometry;

    // Match vertices
    tileGeometry->setVertexArray(vertices);

    // Add color (each rectangle has the same color except for the down one which is transparent)
    osg::Vec4 osgColor(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(osgColor);

    // Match color
    tileGeometry->setColorArray(colors);
    tileGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 0, 1));
    double w = pw - mwp;
    double h = phm - mh;
    double norm = std::sqrt(w*w + h*h);
    normals->push_back(osg::Vec3(h/norm, 0, -w/norm));
    normals->push_back(osg::Vec3(h/norm, 0, -w/norm));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));

    // Match normals
    tileGeometry->setNormalArray(normals);
    tileGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define tile 18 GL_TRIANGLES with 20*3 vertices
    tileGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 18*3));

    // Return the tile whithout plot
    return tileGeometry.release();
}

ReverseTileNode* ReverseTileNode::cloning(void) const {
    return new ReverseTileNode(*this);
}
