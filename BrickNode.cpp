#include "BrickNode.h"

#include <osg/Geometry>
#include <osg/Material>
#include <osgUtil/Tessellator>

BrickNode::BrickNode() :
    LegoNode() {
}

BrickNode::BrickNode(Brick* brick) :
    LegoNode(brick) {

    createGeode();
}

BrickNode::BrickNode(const BrickNode& brickNode) :
    LegoNode(brickNode) {
}

void BrickNode::createGeode(void) {
    // Remove children
    removeChildren(0, getNumChildren());

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);
    geode->addDrawable(createBrick());

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Get the brick
    Brick* brick = static_cast<Brick*>(_lego);

    // Get brick type
    Brick::BrickType brickType = brick->getBrickType();

    // Get integer sizes
    int width = brick->getWidth();
    int length = brick->getLength();
    int height = 1;
    if (brickType == Brick::classic)
        height = 3;

    // Calculate x max and y max for plots
    double xmin = -(length-1)*Lego::length_unit/2;
    double ymin = -(width-1)*Lego::length_unit/2;

    // Calculate x max and y max for bottom cylinder
    bool thinw = (brick->getWidth() == 1);
    bool thinl = (brick->getLength() == 1);
    bool thin = (thinw || thinl);
    double xminb = xmin;
    if (!thinl)
        xminb = xmin+Lego::length_unit/2;
    double yminb = ymin;
    if (!thinw)
        yminb = ymin+Lego::length_unit/2;

    // Add plots iteratively if the brick type is not flat
    if (brickType != Brick::flat) {
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < width; j++) {
                double radiusX = xmin + i*distPlot;
                double radiusY = ymin + j*distPlot;
                addChild(createPlotCylinderAndTop(radiusX, radiusY, height));
                //geode->addDrawable(createPlotTop(radiusX, radiusY, height));
            }
        }
    }

    if (thinw && !thinl)
        width = 2;
    if (thinl && !thinw)
        length = 2;

    // Add bottom cylinders iteratively
    for (int i = 0; i < length-1; i++) {
        for (int j = 0; j < width-1; j++) {
            double radiusX = xminb + i*distPlot;
            double radiusY = yminb + j*distPlot;
            addChild(createBottomCylinder(radiusX, radiusY, height, thin));
        }
    }
}

osg::Drawable *BrickNode::createBrick(void) const {
    // Get the brick
    Brick* brick = static_cast<Brick*>(_lego);

    // Get brick color
    QColor color = brick->getColor();

    // Get brick type
    Brick::BrickType brickType = brick->getBrickType();

    // Get integer sizes
    int width = brick->getWidth();
    int length = brick->getLength();
    int height = 1;
    if (brickType == Brick::classic)
        height = 3;

    // Get real position, according to brick size
    // d : down, u : up, l : left, r : right, f : front, b : back
    double d = -height*Lego::height_unit/2;
    double u = height*Lego::height_unit/2;
    double l = -length*Lego::length_unit/2;
    double r = length*Lego::length_unit/2;
    double f = -width*Lego::length_unit/2;
    double b = width*Lego::length_unit/2;
    double mwpm = (-width)*Lego::length_unit/2+Lego::height_unit/2;
    double pwm = (width)*Lego::length_unit/2-Lego::height_unit/2;
    double mlp = (-length+0.5)*Lego::length_unit/2;
    double plm = (length-0.5)*Lego::length_unit/2;
    double mh = (-height)*Lego::height_unit/2;
    double mhpm = (height)*Lego::height_unit/2-Lego::plot_top_height;

    // Create 8 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 lfd(l, f, d);
    osg::Vec3 rfd(r, f, d);
    osg::Vec3 rfu(r, f, u);
    osg::Vec3 lfu(l, f, u);
    osg::Vec3 lbd(l, b, d);
    osg::Vec3 rbd(r, b, d);
    osg::Vec3 rbu(r, b, u);
    osg::Vec3 lbu(l, b, u);
    osg::Vec3 v20(mlp, mwpm, mh);
    osg::Vec3 v21(plm, mwpm, mh);
    osg::Vec3 v22(plm, pwm, mh);
    osg::Vec3 v23(mlp, pwm, mh);
    osg::Vec3 v24(mlp, mwpm, mhpm);
    osg::Vec3 v25(plm, mwpm, mhpm);
    osg::Vec3 v26(plm, pwm, mhpm);
    osg::Vec3 v27(mlp, pwm, mhpm);

    // Create faces
    // NB: Down face is transparent, we don't even create it

    // Bottom
    vertices->push_back(lfd);
    vertices->push_back(rfd);
    vertices->push_back(rbd);
    vertices->push_back(lbd);
    // Bottom hole
    vertices->push_back(v20);
    vertices->push_back(v21);
    vertices->push_back(v22);
    vertices->push_back(v23);
    // Bottom far
    vertices->push_back(v24);
    vertices->push_back(v25);
    vertices->push_back(v26);
    vertices->push_back(v27);

    // Up face
    vertices->push_back(rfu);
    vertices->push_back(rbu);
    vertices->push_back(lbu);
    vertices->push_back(lfu);

    // Front face
    vertices->push_back(rfd);
    vertices->push_back(rfu);
    vertices->push_back(lfu);
    vertices->push_back(lfd);

    // Back face
    vertices->push_back(rbd);
    vertices->push_back(rbu);
    vertices->push_back(lbu);
    vertices->push_back(lbd);

    // Left face
    vertices->push_back(lfd);
    vertices->push_back(lfu);
    vertices->push_back(lbu);
    vertices->push_back(lbd);

    // Right face
    vertices->push_back(rfd);
    vertices->push_back(rfu);
    vertices->push_back(rbu);
    vertices->push_back(rbd);

    // Bottom front
    vertices->push_back(v20);
    vertices->push_back(v21);
    vertices->push_back(v25);
    vertices->push_back(v24);

    // Bottom right
    vertices->push_back(v21);
    vertices->push_back(v22);
    vertices->push_back(v26);
    vertices->push_back(v25);

    // Bottom back
    vertices->push_back(v22);
    vertices->push_back(v23);
    vertices->push_back(v27);
    vertices->push_back(v26);

    // Bottom left
    vertices->push_back(v23);
    vertices->push_back(v20);
    vertices->push_back(v24);
    vertices->push_back(v27);

    // Create brick geometry
    osg::ref_ptr<osg::Geometry> brickGeometry = new osg::Geometry;

    // Match vertices
    brickGeometry->setVertexArray(vertices);

    // Create colors
    osg::Vec4 osgColor(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(osgColor);

    // Match colors
    brickGeometry->setColorArray(colors);
    brickGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0, 0, -1));
    normals->push_back(osg::Vec3(0, 0, -1));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(1, 0, 0));

    // Match normals
    brickGeometry->setNormalArray(normals);
    brickGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define 1 GL_QUADS with 1*4 vertices, corresponding to bottom part
    brickGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0*4, 4));

    // Define 1 GL_QUADS with 1*4 vertices, corresponding to 1 hole in bottom part
    brickGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 1*4, 4));

    // Retesslate to create hole
    osgUtil::Tessellator tesslator;
    tesslator.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
    tesslator.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
    tesslator.retessellatePolygons(*brickGeometry);

    // Create 9 GL_QUADS, i.e. 9*4 vertices
    brickGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 2*4, 9*4));

    // return brick geometry
    return brickGeometry.release();
}

BrickNode* BrickNode::cloning(void) const {
    return new BrickNode(*this);
}
