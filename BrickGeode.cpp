#include "BrickGeode.h"

#include <osg/Geometry>
#include <osg/Material>
#include <osg/BlendFunc>

#include <QDebug>

BrickGeode::BrickGeode() :
    LegoGeode() {
}

BrickGeode::BrickGeode(Brick* brick) :
    LegoGeode(brick) {

    createGeode();
}

BrickGeode::BrickGeode(const BrickGeode& brickGeode) :
    LegoGeode(brickGeode._lego) {
}

void BrickGeode::createGeode(void) {
    // Add the brick basis
    removeDrawables(0, getDrawableList().size());
    addDrawable(createBrick());

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
                addDrawable(createPlot(radiusX, radiusY, height));
            }
        }
    }

    if (thinw && !thinl)
        width = 2;
    if (thinl && !thinw)
        length = 2;

    // Add bottom cylinder iteratively
    for (int i = 0; i < length-1; i++) {
        for (int j = 0; j < width-1; j++) {
            double radiusX = xminb + i*distPlot;
            double radiusY = yminb + j*distPlot;
            addDrawable(createCylinder(radiusX, radiusY, height, thin));
        }
    }
}

osg::ref_ptr<osg::Drawable> BrickGeode::createBrick(void) const {
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

    // Create faces
    // Down face
    vertices->push_back(rfd);
    vertices->push_back(rbd);
    vertices->push_back(lbd);
    vertices->push_back(lfd);

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

    // Create brick geometry
    osg::ref_ptr<osg::Geometry> brickGeometry = new osg::Geometry;

    // Handle transparency
    double alpha = 0.1;
    osg::StateSet* state = brickGeometry->getOrCreateStateSet();
    state->setMode(GL_BLEND,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    osg::Material* mat = new osg::Material;
    mat->setAlpha(osg::Material::FRONT_AND_BACK, alpha);
    state->setAttributeAndModes(mat,osg::StateAttribute::ON |
    osg::StateAttribute::OVERRIDE);
    osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
    state->setAttributeAndModes(bf);
    state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    brickGeometry->setStateSet(state);

    // Match vertices
    brickGeometry->setVertexArray(vertices);

    // Add color (each rectangle has the same color except for the down one which is transparent)
    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::Vec4 transparent(.0f, .0f, .0f, .0f);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Add transparent color
    colors->push_back(transparent);
    // Add color to 5 other faces
    for (int k = 1; k < 6; k++)
        colors->push_back(colorVec);

    // Macth color
    brickGeometry->setColorArray(colors);
    brickGeometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;

    // Add normals (mind the insert order!)
    normals->push_back(osg::Vec3(0, 0, -1));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));

    // Match normals
    brickGeometry->setNormalArray(normals);
    brickGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define brick GL_QUADS with 24 vertices
    brickGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 24));

    // Return the brick whithout plot
    return brickGeometry.get();
}

BrickGeode* BrickGeode::cloning(void) const {
    return new BrickGeode(*this);
}
