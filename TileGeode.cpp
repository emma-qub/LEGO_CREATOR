#include "TileGeode.h"

#include <osg/Geometry>
#include <osg/Material>
#include <osg/BlendFunc>
#include <osgUtil/SmoothingVisitor>

#include <QDebug>

TileGeode::TileGeode() {
    LegoGeode();
}

TileGeode::TileGeode(osg::ref_ptr<Tile> tile) :
    LegoGeode(tile) {

    createGeode();
}

TileGeode::TileGeode(const TileGeode& tileGeode) :
    LegoGeode(tileGeode) {
}

void TileGeode::createGeode(void) {
    // Remove children
    removeChildren(0, getNumChildren());

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);
    geode->addDrawable(createBrick());

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Get the tile
    Tile* tile = static_cast<Tile*>(_lego);

    // Get integer sizes
    int width = tile->getWidth();
    int length = tile->getLength();
    int height = 3;

    // Calculate x max and y max for plots
    double xmin = -(width-1)*Lego::length_unit/2;
    double ymin = -(length-1)*Lego::length_unit/2;

    // Add plots iteratively
    for (int i = 0; i < length; i++) {
            double radiusX = xmin;
            double radiusY = ymin + i*distPlot;
            geode->addDrawable(createPlot(radiusX, radiusY, height));
    }
}

osg::ref_ptr<osg::Drawable> TileGeode::createBrick(void) const {
    // Get the tile
    Tile* tile = static_cast<Tile*>(_lego);

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
    double mhp = (-height+1)*Lego::height_unit/2;

    // Create 14 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(mw, ml, mh);
    osg::Vec3 v1(mw, pl, mh);
    osg::Vec3 v2(pw, pl, mh);
    osg::Vec3 v3(pw, ml, mh);
    osg::Vec3 v4(pw, ml, mhp);
    osg::Vec3 v5(pw, pl, mhp);
    osg::Vec3 v6(mw, pl, ph);
    osg::Vec3 v7(mw, ml, ph);
    osg::Vec3 v8(mwp, ml, ph);
    osg::Vec3 v9(mwp, pl, ph);
    osg::Vec3 v10(mwp, pl, mhp);
    osg::Vec3 v11(mw, pl, mhp);
    osg::Vec3 v12(mwp, ml, mhp);
    osg::Vec3 v13(mw, ml, mhp);

    // Create 10 faces, 8 faces are quads splitted in two triangles
    // Down face t1
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v2);
    // Down face t2
    vertices->push_back(v0);
    vertices->push_back(v2);
    vertices->push_back(v3);

    // Front face t1
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v5);
    // Front face t2
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v5);

    // Back face t1
    vertices->push_back(v7);
    vertices->push_back(v6);
    vertices->push_back(v1);
    // Back face t2
    vertices->push_back(v7);
    vertices->push_back(v1);
    vertices->push_back(v0);

    // Top face t1
    vertices->push_back(v8);
    vertices->push_back(v7);
    vertices->push_back(v6);
    // Top face t2
    vertices->push_back(v8);
    vertices->push_back(v6);
    vertices->push_back(v9);

    // Slop face t1
    vertices->push_back(v4);
    vertices->push_back(v5);
    vertices->push_back(v9);
    // Slop face t2
    vertices->push_back(v4);
    vertices->push_back(v9);
    vertices->push_back(v8);

    // Right triangle face
    vertices->push_back(v9);
    vertices->push_back(v5);
    vertices->push_back(v10);

    // Right quad face t1
    vertices->push_back(v6);
    vertices->push_back(v9);
    vertices->push_back(v11);
    // Right quad face t2
    vertices->push_back(v11);
    vertices->push_back(v9);
    vertices->push_back(v10);

    // Right quad face down t1
    vertices->push_back(v11);
    vertices->push_back(v5);
    vertices->push_back(v1);
    // Right quad face down t2
    vertices->push_back(v1);
    vertices->push_back(v5);
    vertices->push_back(v2);

    // Left triangle face
    vertices->push_back(v4);
    vertices->push_back(v8);
    vertices->push_back(v12);

    // Left quad face t1
    vertices->push_back(v7);
    vertices->push_back(v8);
    vertices->push_back(v12);
    // Left quad face t2
    vertices->push_back(v7);
    vertices->push_back(v12);
    vertices->push_back(v13);

    // Left quad face down t1
    vertices->push_back(v13);
    vertices->push_back(v4);
    vertices->push_back(v3);
    // Left quad face down t2
    vertices->push_back(v13);
    vertices->push_back(v3);
    vertices->push_back(v0);

    // Create tile geometry
    osg::ref_ptr<osg::Geometry> tileGeometry = new osg::Geometry;

    // Handle transparency
    double alpha = 0.1;
    osg::StateSet* state = tileGeometry->getOrCreateStateSet();
    state->setMode(GL_BLEND,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    osg::Material* mat = new osg::Material;
    mat->setAlpha(osg::Material::FRONT_AND_BACK, alpha);
    state->setAttributeAndModes(mat,osg::StateAttribute::ON |
    osg::StateAttribute::OVERRIDE);
    osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
    state->setAttributeAndModes(bf);
    state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    tileGeometry->setStateSet(state);

    // Match vertices
    tileGeometry->setVertexArray(vertices);

    // Add color (each rectangle has the same color except for the down one which is transparent)
    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::Vec4 transparent(.0f, .0f, .0f, .0f);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Add transparent color
    colors->push_back(transparent);
    colors->push_back(transparent);
    // Add color to 5 other faces
    for (int k = 2; k < 20; k++)
        colors->push_back(colorVec);

    // Match color
    tileGeometry->setColorArray(colors);
    tileGeometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define tile 20 GL_TRIANGLES with 20*3 vertices
    tileGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 20*3));

    // Calculate smooth normals
    osgUtil::SmoothingVisitor::smooth(*tileGeometry);

    // Return the tile whithout plot
    return tileGeometry.get();
}

TileGeode* TileGeode::cloning(void) const {
    return new TileGeode(*this);
}
