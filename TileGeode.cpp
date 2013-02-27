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
    // Get the tile
    Tile* tile = static_cast<Tile*>(_lego);

    // Get tile type
    Tile::TileType tileType = tile->getTileType();

    // Get integer sizes
    int width = tile->getWidth();
    int length = tile->getLength();
    int height = 3;
    if (tileType == Tile::roof || (tileType == Tile::classic && width == 1))
        height = 2;

    // Remove children
    removeChildren(0, getNumChildren());

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Calculate x max and y max for plots
    double xmin = -(width-1)*Lego::length_unit/2;
    double ymin = -(length-1)*Lego::length_unit/2;

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);
    switch (tileType) {
    case Tile::classic:
        // If tile has plots (width > 1)
        if (width > 1) {
            // Add box
            geode->addDrawable(createClassic());

            // Add plots iteratively
            for (int j = 0; j < length; j++) {
                    double radiusX = xmin;
                    double radiusY = ymin + j*distPlot;
                    geode->addDrawable(createPlot(radiusX, radiusY, height));
            }
        } else {
            geode->addDrawable(createTinyClassic());
        }
        break;
    case Tile::roof:
        geode->addDrawable(createRoof());
        break;
    case Tile::bigRoof:
        geode->addDrawable(createRoof());
        break;
    }

    double xminb = xmin+Lego::length_unit/2;
    double yminb = ymin+Lego::length_unit/2;
    // Add bottom cylinders iteratively
    for (int i = 0; i < width-1; i++) {
        for (int j = 0; j < length-1; j++) {
            double radiusX = xminb + i*distPlot;
            double radiusY = yminb + j*distPlot;
            geode->addDrawable(createCylinder(radiusX, radiusY, 0.5, false, (-height+0.5)*Lego::height_unit/2));
        }
    }
}

osg::ref_ptr<osg::Drawable> TileGeode::createTinyClassic(void) const {
    // Get the tile
    Tile* tile = static_cast<Tile*>(_lego);

    // Get tile color
    QColor color = tile->getColor();

    // Get integer sizes
    int width = tile->getWidth();
    int length = tile->getLength();
    int height = 2;

    // Get real position, according to tile size
    double mw = (-width)*Lego::length_unit/2;
    double pw = (width)*Lego::length_unit/2;
    double ml = (-length)*Lego::length_unit/2;
    double pl = (length)*Lego::length_unit/2;
    double mh = (-height)*Lego::height_unit/2;
    double ph = (height)*Lego::height_unit/2;

    // Create 14 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(mw, ml, mh);
    osg::Vec3 v1(mw, pl, mh);
    osg::Vec3 v2(pw, pl, mh);
    osg::Vec3 v3(pw, ml, mh);
    osg::Vec3 v4(mw, pl, ph);
    osg::Vec3 v5(mw, ml, ph);

    // Create 8 faces, 6 faces are quads splitted in two triangles
    // Down face t1
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v2);
    // Down face t2
    vertices->push_back(v0);
    vertices->push_back(v2);
    vertices->push_back(v3);

    // Back face t1
    vertices->push_back(v5);
    vertices->push_back(v4);
    vertices->push_back(v1);
    // Back face t2
    vertices->push_back(v5);
    vertices->push_back(v1);
    vertices->push_back(v0);

    // Slop face t1
    vertices->push_back(v4);
    vertices->push_back(v3);
    vertices->push_back(v2);
    // Slop face t2
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v5);

    // Right triangle face
    vertices->push_back(v4);
    vertices->push_back(v2);
    vertices->push_back(v1);

    // Left triangle face
    vertices->push_back(v5);
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
    // Add color to 6 other faces
    for (int k = 2; k < 8; k++)
        colors->push_back(colorVec);

    // Match color
    tileGeometry->setColorArray(colors);
    tileGeometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define tile 8 GL_TRIANGLES with 8*3 vertices
    tileGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 8*3));

    // Calculate smooth normals
    osgUtil::SmoothingVisitor::smooth(*tileGeometry);

    // Return the tile whithout plot
    return tileGeometry.get();
}

osg::ref_ptr<osg::Drawable> TileGeode::createClassic(void) const {
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
    // Add color to 18 other faces
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

osg::ref_ptr<osg::Drawable> TileGeode::createRoof(void) const {
    // Get the tile
    Tile* tile = static_cast<Tile*>(_lego);

    // Get tile color
    QColor color = tile->getColor();

    // Get tile type
    Tile::TileType tileType = tile->getTileType();

    // Get integer sizes
    int width = tile->getWidth();
    int length = tile->getLength();
    int height = 2;
    if (tileType == Tile::bigRoof)
        height = 3;

    // Get real position, according to tile size
    double mw = (-width)*Lego::length_unit/2;
    double pw = (width)*Lego::length_unit/2;
    double ml = (-length)*Lego::length_unit/2;
    double pl = (length)*Lego::length_unit/2;
    double mh = (-height)*Lego::height_unit/2;
    double ph = (height)*Lego::height_unit/2;
    double mhp = (-height+1)*Lego::height_unit/2;

    // Create 12 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(mw, ml, mh);
    osg::Vec3 v1(mw, pl, mh);
    osg::Vec3 v2(pw, pl, mh);
    osg::Vec3 v3(pw, ml, mh);
    osg::Vec3 v4(pw, ml, mhp);
    osg::Vec3 v5(pw, pl, mhp);
    osg::Vec3 v6(0, pl, ph);
    osg::Vec3 v7(0, ml, ph);
    osg::Vec3 v8(mw, pl, mhp);
    osg::Vec3 v9(mw, ml, mhp);
    osg::Vec3 v10(0, ml, mhp);
    osg::Vec3 v11(0, pl, mhp);

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
    vertices->push_back(v3);
    vertices->push_back(v2);
    vertices->push_back(v5);
    // Front face t2
    vertices->push_back(v3);
    vertices->push_back(v5);
    vertices->push_back(v4);

    // Slop front t1
    vertices->push_back(v4);
    vertices->push_back(v6);
    vertices->push_back(v7);
    // Slop front t2
    vertices->push_back(v4);
    vertices->push_back(v5);
    vertices->push_back(v6);

    // Back face t1
    vertices->push_back(v9);
    vertices->push_back(v1);
    vertices->push_back(v0);
    // Back face t2
    vertices->push_back(v9);
    vertices->push_back(v8);
    vertices->push_back(v1);

    // Slop back t1
    vertices->push_back(v8);
    vertices->push_back(v9);
    vertices->push_back(v7);
    // Slop back t2
    vertices->push_back(v8);
    vertices->push_back(v7);
    vertices->push_back(v6);

    // Left quad t1
    vertices->push_back(v0);
    vertices->push_back(v3);
    vertices->push_back(v9);
    // Left quad t2
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v9);

    // Left triangle t1
    vertices->push_back(v4);
    vertices->push_back(v7);
    vertices->push_back(v10);
    // Left triangle t2
    vertices->push_back(v7);
    vertices->push_back(v9);
    vertices->push_back(v10);

    // Right quad t1
    vertices->push_back(v1);
    vertices->push_back(v5);
    vertices->push_back(v2);
    // Right quad t2
    vertices->push_back(v8);
    vertices->push_back(v5);
    vertices->push_back(v1);

    // Right triangle t1
    vertices->push_back(v8);
    vertices->push_back(v6);
    vertices->push_back(v11);
    // Right triangle t2
    vertices->push_back(v11);
    vertices->push_back(v6);
    vertices->push_back(v5);

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
    // Add color to 16 other faces
    for (int k = 2; k < 18; k++)
        colors->push_back(colorVec);

    // Match color
    tileGeometry->setColorArray(colors);
    tileGeometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define tile 18 GL_TRIANGLES with 18*3 vertices
    tileGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 18*3));

    // Calculate smooth normals
    osgUtil::SmoothingVisitor::smooth(*tileGeometry);

    // Return the tile whithout plot
    return tileGeometry.get();
}

TileGeode* TileGeode::cloning(void) const {
    return new TileGeode(*this);
}
