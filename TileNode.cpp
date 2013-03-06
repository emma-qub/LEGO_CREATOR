#include "TileNode.h"

#include <osg/Geometry>
#include <osg/Material>
#include <osg/BlendFunc>
#include <osgUtil/SmoothingVisitor>

#include <cmath>

TileNode::TileNode() {
    LegoNode();
}

TileNode::TileNode(osg::ref_ptr<Tile> tile) :
    LegoNode(tile) {

    createGeode();
}

TileNode::TileNode(const TileNode& tileNode) :
    LegoNode(tileNode) {
}

void TileNode::createGeode(void) {
    // Remove children
    removeChildren(0, getNumChildren());

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

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Calculate x max and y max for plots
    double xmin = -(width-1)*Lego::length_unit/2;
    double ymin = -(length-1)*Lego::length_unit/2;

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);

    // Create shape according to tile type
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

osg::ref_ptr<osg::Drawable> TileNode::createTinyClassic(void) const {
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

    // Create 6 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(mw, ml, mh);
    osg::Vec3 v1(mw, pl, mh);
    osg::Vec3 v2(pw, pl, mh);
    osg::Vec3 v3(pw, ml, mh);
    osg::Vec3 v4(mw, ml, ph);
    osg::Vec3 v5(mw, pl, ph);

    // Create 4 faces, 2 faces are quads splitted into two triangles
    // NB: Down face is transparent, we don't even create it

    // Back face t1
    vertices->push_back(v5);
    vertices->push_back(v4);
    vertices->push_back(v1);
    // Back face t2
    vertices->push_back(v4);
    vertices->push_back(v1);
    vertices->push_back(v0);

    // Slop face t1
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v5);
    // Slop face t2
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v5);

    // Left triangle face
    vertices->push_back(v4);
    vertices->push_back(v3);
    vertices->push_back(v0);

    // Right triangle face
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v5);

    // Create tile geometry
    osg::ref_ptr<osg::Geometry> tileGeometry = new osg::Geometry;

    // Match vertices
    tileGeometry->setVertexArray(vertices);

    // Create colors
    osg::Vec4 osgColor(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(osgColor);

    // Match color
    tileGeometry->setColorArray(colors);
    tileGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1/std::sqrt(2), 0, 1/std::sqrt(2)));
    normals->push_back(osg::Vec3(1/std::sqrt(2), 0, 1/std::sqrt(2)));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));

    // Match normals
    tileGeometry->setNormalArray(normals);
    tileGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Create 6 GL_TRIANGLES, i.e. 6*3 vertices
    tileGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 6*3));

    // Return the tile whithout plot
    return tileGeometry.get();
}

osg::ref_ptr<osg::Drawable> TileNode::createClassic(void) const {
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
    // NB: Down face is transparent, we don't even create it

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
    double norm = std::sqrt((width-1)*(width-1) + height*height);
    normals->push_back(osg::Vec3((width-1)/norm, 0, height/norm));
    normals->push_back(osg::Vec3((width-1)/norm, 0, height/norm));
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
    return tileGeometry.get();
}

osg::ref_ptr<osg::Drawable> TileNode::createRoof(void) const {
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

    // Create 6, with 16 triangles
    // NB: Down face is transparent, we don't even create it

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

    // Match vertices
    tileGeometry->setVertexArray(vertices);

    // Add color (each rectangle has the same color except for the down one which is transparent)
    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(colorVec);

    // Match color
    tileGeometry->setColorArray(colors);
    tileGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    double norm = std::sqrt((width/2)*(width/2) + height*height);
    normals->push_back(osg::Vec3(width/(2*norm), 0, height/norm));
    normals->push_back(osg::Vec3(width/(2*norm), 0, height/norm));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-width/(2*norm), 0, height/norm));
    normals->push_back(osg::Vec3(-width/(2*norm), 0, height/norm));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));

    // Match normals
    tileGeometry->setNormalArray(normals);
    tileGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define tile 16 GL_TRIANGLES with 18*3 vertices
    tileGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 16*3));

    // Return the tile whithout plot
    return tileGeometry.get();
}

TileNode* TileNode::cloning(void) const {
    return new TileNode(*this);
}
