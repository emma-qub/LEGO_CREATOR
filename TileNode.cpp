#include "TileNode.h"

#include <osg/Geometry>
#include <osg/Material>

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
    case Tile::cornerInt:
        geode->addDrawable(createCornerInt());
        // Add right plots
        for (int k = 0; k < length-1; k++) {
            double radiusX = -xmin;
            double radiusY = ymin + k*distPlot;
            geode->addDrawable(createPlot(radiusX, radiusY, height));
        }
        // Add back plots
        for (int k = 0; k < width-1; k++) {
            double radiusX = xmin + k*distPlot;
            double radiusY = -ymin;
            geode->addDrawable(createPlot(radiusX, radiusY, height));
        }
        // Add corner plot
        geode->addDrawable(createPlot((width-1)*Lego::length_unit/2, (length-1)*Lego::length_unit/2, height));
        break;
    case Tile::cornerExt:
        geode->addDrawable(createCornerExt());
        geode->addDrawable(createPlot((width-1)*Lego::length_unit/2, (length-1)*Lego::length_unit/2, height));
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
    double mhm = (-height+1)*Lego::height_unit/2;
    double ph = (height)*Lego::height_unit/2;

    // Create 10 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(mw, ml, mh);
    osg::Vec3 v1(mw, pl, mh);
    osg::Vec3 v2(pw, pl, mh);
    osg::Vec3 v3(pw, ml, mh);
    osg::Vec3 v4(mw, ml, ph);
    osg::Vec3 v5(mw, pl, ph);
    osg::Vec3 v6(mw, ml, mhm);
    osg::Vec3 v7(pw, ml, mhm);
    osg::Vec3 v8(pw, pl, mhm);
    osg::Vec3 v9(mw, pl, mhm);

    // Create 4 faces, 2 faces are quads splitted into two triangles
    // NB: Down face is transparent, we don't even create it

    // Front face t1
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v7);
    // Front face t2
    vertices->push_back(v2);
    vertices->push_back(v7);
    vertices->push_back(v8);

    // Back face t1
    vertices->push_back(v5);
    vertices->push_back(v4);
    vertices->push_back(v1);
    // Back face t2
    vertices->push_back(v4);
    vertices->push_back(v1);
    vertices->push_back(v0);

    // Slop face t1
    vertices->push_back(v7);
    vertices->push_back(v4);
    vertices->push_back(v5);
    // Slop face t2
    vertices->push_back(v8);
    vertices->push_back(v7);
    vertices->push_back(v5);

    // Left triangle
    vertices->push_back(v4);
    vertices->push_back(v7);
    vertices->push_back(v6);
    // Left part t1
    vertices->push_back(v0);
    vertices->push_back(v3);
    vertices->push_back(v6);
    // Left part t2
    vertices->push_back(v7);
    vertices->push_back(v3);
    vertices->push_back(v6);

    // Right triangle face
    vertices->push_back(v9);
    vertices->push_back(v8);
    vertices->push_back(v5);
    // Right part t1
    vertices->push_back(v1);
    vertices->push_back(v9);
    vertices->push_back(v8);
    // Right part t2
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v8);

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
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    double w = pw - mw;
    double h = ph - mhm;
    double norm = std::sqrt(w*w + h*h);
    normals->push_back(osg::Vec3(h/norm, 0, w/norm));
    normals->push_back(osg::Vec3(h/norm, 0, w/norm));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));

    // Match normals
    tileGeometry->setNormalArray(normals);
    tileGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Create 12 GL_TRIANGLES, i.e. 12*3 vertices
    tileGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 12*3));

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
    double w = pw - mwp;
    double h = ph - mhp;
    double norm = std::sqrt(w*w + h*h);
    normals->push_back(osg::Vec3(h/norm, 0, w/norm));
    normals->push_back(osg::Vec3(h/norm, 0, w/norm));
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

    // Define tile 18 GL_TRIANGLES with 18*3 vertices
    tileGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 18*3));

    // Return the tile whithout plot
    return tileGeometry.get();
}

osg::ref_ptr<osg::Drawable> TileNode::createCornerInt(void) const {
    // Get the tile
    Tile* tile = static_cast<Tile*>(_lego);

    // Get tile color
    QColor color = tile->getColor();

    // Get integer sizes
    int width = tile->getWidth();
    int length = tile->getLength();
    int height = 3;

    // Get real position, according to tile size
    double pw = (-width)*Lego::length_unit/2;
    double mwp = (width-2)*Lego::length_unit/2;
    double mw = (width)*Lego::length_unit/2;
    double ml = (-length)*Lego::length_unit/2;
    double pl = (length)*Lego::length_unit/2;
    double plm = (length-2)*Lego::length_unit/2;
    double mh = (-height)*Lego::height_unit/2;
    double mhp = (-height+1)*Lego::height_unit/2;
    double ph = (height)*Lego::height_unit/2;

    // Create 14 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(ml, mw, mh);
    osg::Vec3 v1(pl, mw, mh);
    osg::Vec3 v2(pl, pw, mh);
    osg::Vec3 v3(ml, pw, mh);
    osg::Vec3 v4(ml, pw, mhp);
    osg::Vec3 v5(ml, mwp, ph);
    osg::Vec3 v6(ml, mw, ph);
    osg::Vec3 v7(pl, mw, ph);
    osg::Vec3 v8(plm, mwp, ph);
    osg::Vec3 v9(plm, pw, ph);
    osg::Vec3 v10(pl, pw, ph);
    osg::Vec3 v11(plm, pw, mhp);
    osg::Vec3 v12(ml, mwp, mhp);

    // Create 20 faces, 8 faces are quads splitted in two triangles
    // NB: Down face is transparent, we don't even create it

    // Front face t1
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v11);
    // Front face t2
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v11);
    // Front face t3
    vertices->push_back(v2);
    vertices->push_back(v10);
    vertices->push_back(v11);
    // Front face t4
    vertices->push_back(v9);
    vertices->push_back(v10);
    vertices->push_back(v11);
    // Front face t5
    vertices->push_back(v4);
    vertices->push_back(v11);
    vertices->push_back(v9);

    // Back face t1
    vertices->push_back(v7);
    vertices->push_back(v6);
    vertices->push_back(v1);
    // Back face t2
    vertices->push_back(v6);
    vertices->push_back(v1);
    vertices->push_back(v0);

    // Top face t1
    vertices->push_back(v5);
    vertices->push_back(v6);
    vertices->push_back(v7);
    // Top face t2
    vertices->push_back(v5);
    vertices->push_back(v8);
    vertices->push_back(v7);
    // Top face t3
    vertices->push_back(v7);
    vertices->push_back(v8);
    vertices->push_back(v9);
    // Top face t4
    vertices->push_back(v7);
    vertices->push_back(v10);
    vertices->push_back(v9);

    // Slop face t1
    vertices->push_back(v4);
    vertices->push_back(v5);
    vertices->push_back(v8);
    // Slop face t2
    vertices->push_back(v4);
    vertices->push_back(v9);
    vertices->push_back(v8);

    // Right face t1
    vertices->push_back(v1);
    vertices->push_back(v10);
    vertices->push_back(v2);
    // Right face t2
    vertices->push_back(v1);
    vertices->push_back(v10);
    vertices->push_back(v7);

    // Left t1
    vertices->push_back(v0);
    vertices->push_back(v12);
    vertices->push_back(v6);
    // Left t2
    vertices->push_back(v5);
    vertices->push_back(v12);
    vertices->push_back(v6);
    // Left t3
    vertices->push_back(v0);
    vertices->push_back(v12);
    vertices->push_back(v3);
    // Left t4
    vertices->push_back(v12);
    vertices->push_back(v4);
    vertices->push_back(v3);
    // Left t5
    vertices->push_back(v12);
    vertices->push_back(v4);
    vertices->push_back(v5);

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
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 0, 1));
    double w = mwp - pw;
    double h = ph - mhp;
    double l = plm - ml;
    double normW = std::sqrt(w*w + h*h);
    double normL = std::sqrt(l*l + h*h);
    normals->push_back(osg::Vec3(0, -h/normL, l/normL));
    normals->push_back(osg::Vec3(-h/normW, 0, w/normW));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));

    // Match normals
    tileGeometry->setNormalArray(normals);
    tileGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define tile 20 GL_TRIANGLES with 20*3 vertices
    tileGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 20*3));

    // Return the tile whithout plot
    return tileGeometry.get();
}

osg::ref_ptr<osg::Drawable> TileNode::createCornerExt(void) const {
    // Get the tile
    Tile* tile = static_cast<Tile*>(_lego);

    // Get tile color
    QColor color = tile->getColor();

    // Get integer sizes
    int width = tile->getWidth();
    int length = tile->getLength();
    int height = 3;

    // Get real position, according to tile size
    double pw = (-width)*Lego::length_unit/2;
    double mwp = (width-2)*Lego::length_unit/2;
    double mw = (width)*Lego::length_unit/2;
    double ml = (-length)*Lego::length_unit/2;
    double pl = (length)*Lego::length_unit/2;
    double plm = (length-2)*Lego::length_unit/2;
    double mh = (-height)*Lego::height_unit/2;
    double mhp = (-height+1)*Lego::height_unit/2;
    double ph = (height)*Lego::height_unit/2;

    // Create 14 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(ml, mw, mh);
    osg::Vec3 v1(pl, mw, mh);
    osg::Vec3 v2(pl, pw, mh);
    osg::Vec3 v3(ml, pw, mh);
    osg::Vec3 v4(ml, pw, mhp);
    osg::Vec3 v5(ml, mw, mhp);
    osg::Vec3 v6(plm, mw, ph);
    osg::Vec3 v7(plm, mwp, ph);
    osg::Vec3 v8(pl, mw, ph);
    osg::Vec3 v9(pl, mwp, ph);
    osg::Vec3 v10(pl, pw, mhp);
    osg::Vec3 v11(plm, pw, mhp);
    osg::Vec3 v12(ml, mwp, mhp);
    osg::Vec3 v13(plm, mw, mhp);
    osg::Vec3 v14(pl, mwp, mhp);

    // Create 20 faces, 8 faces are quads splitted in two triangles
    // NB: Down face is transparent, we don't even create it

    // Front face t1
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v10);
    // Front face t2
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v10);
    // Front face t3
    vertices->push_back(v9);
    vertices->push_back(v7);
    vertices->push_back(v11);
    // Front face t4
    vertices->push_back(v9);
    vertices->push_back(v10);
    vertices->push_back(v11);
    // Front face t5 (real triangle)
    vertices->push_back(v4);
    vertices->push_back(v11);
    vertices->push_back(v7);

    // Back face t1
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v5);
    // Back face t2
    vertices->push_back(v5);
    vertices->push_back(v13);
    vertices->push_back(v1);
    // Back face t3
    vertices->push_back(v6);
    vertices->push_back(v13);
    vertices->push_back(v1);
    // Back face t4
    vertices->push_back(v8);
    vertices->push_back(v6);
    vertices->push_back(v1);
    // Back face t5 (real triangle)
    vertices->push_back(v5);
    vertices->push_back(v13);
    vertices->push_back(v6);

    // Top face t1
    vertices->push_back(v6);
    vertices->push_back(v7);
    vertices->push_back(v8);
    // Top face t2
    vertices->push_back(v7);
    vertices->push_back(v8);
    vertices->push_back(v9);

    // Right face t1
    vertices->push_back(v1);
    vertices->push_back(v8);
    vertices->push_back(v9);
    // Right face t2
    vertices->push_back(v1);
    vertices->push_back(v9);
    vertices->push_back(v14);
    // Right face t3
    vertices->push_back(v1);
    vertices->push_back(v14);
    vertices->push_back(v10);
    // Right face t4
    vertices->push_back(v1);
    vertices->push_back(v10);
    vertices->push_back(v2);
    // Right face t5 (real triangle)
    vertices->push_back(v9);
    vertices->push_back(v10);
    vertices->push_back(v14);

    // Left t1
    vertices->push_back(v0);
    vertices->push_back(v3);
    vertices->push_back(v5);
    // Left t2
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v5);
    // Left t3
    vertices->push_back(v5);
    vertices->push_back(v6);
    vertices->push_back(v12);
    // Left t4
    vertices->push_back(v6);
    vertices->push_back(v7);
    vertices->push_back(v12);
    // Left t5 (real triangle)
    vertices->push_back(v12);
    vertices->push_back(v4);
    vertices->push_back(v7);

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

    // Calculate slop normals
    double h = ph - mhp;
    double w = mwp - pw;
    double l = plm - ml;
    double normW = std::sqrt(w*w + h*h);
    double normL = std::sqrt(l*l + h*h);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -h/normW, w/normW));
    normals->push_back(osg::Vec3(0, -h/normW, w/normW));
    normals->push_back(osg::Vec3(0, -h/normW, w/normW));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-h/normL, 0, l/normL));
    normals->push_back(osg::Vec3(-h/normL, 0, l/normL));
    normals->push_back(osg::Vec3(-h/normL, 0, l/normL));

    // Match normals
    tileGeometry->setNormalArray(normals);
    tileGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define tile 22 GL_TRIANGLES with 22*3 vertices
    tileGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 22*3));

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

    // Create slop normals
    double w = pw;
    double h = ph - mhp;
    double norm = std::sqrt(w*w + h*h);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(h/norm, 0, w/norm));
    normals->push_back(osg::Vec3(h/norm, 0, w/norm));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-h/norm, 0, w/norm));
    normals->push_back(osg::Vec3(-h/norm, 0, w/norm));
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
