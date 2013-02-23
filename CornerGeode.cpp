#include "CornerGeode.h"

#include <osg/Geometry>
#include <osg/Material>
#include <osg/BlendFunc>

CornerGeode::CornerGeode() :
    LegoGeode() {
}

CornerGeode::CornerGeode(osg::ref_ptr<Corner> corner) :
    LegoGeode(corner) {

    createGeode();
}

CornerGeode::CornerGeode(const CornerGeode& cornerGeode) :
    LegoGeode(cornerGeode) {
}

void CornerGeode::createGeode(void) {
    // Add the corner basis
    removeChildren(0, getNumChildren());

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);
    geode->addDrawable(createCorner(true));
    geode->addDrawable(createCorner(false));

    // Get the corner
    Corner* corner = static_cast<Corner*>(_lego);

    // Calculate height according to corner type
    int height = corner->calculateHeight();

    geode->addDrawable(createPlot(-Lego::length_unit/2, -Lego::length_unit/2, height));
    geode->addDrawable(createPlot(-Lego::length_unit/2, Lego::length_unit/2, height));
    geode->addDrawable(createPlot(Lego::length_unit/2, Lego::length_unit/2, height));

    geode->addDrawable(createCylinder(-Lego::length_unit/2, 0, height, true));
    geode->addDrawable(createCylinder(0, Lego::length_unit/2, height, true));
}

osg::ref_ptr<osg::Drawable> CornerGeode::createCorner(bool isLeftPart) const {
    // Get the corner
    Corner* corner = static_cast<Corner*>(_lego);

    // Get corner color
    QColor color = corner->getColor();

    // Get integer sizes
    int width = 2;
    int length = 2;

    // Calculate height according to corner type
    int height = corner->calculateHeight();

    // Get real position
    // d : down, u : up, l : left, r : right, f : front, b : back
    double d = -height*Lego::height_unit/2;
    double u = height*Lego::height_unit/2;
    double l = -length*Lego::length_unit/2;
    double r = 0;
    if (!isLeftPart)
        r = length*Lego::length_unit/2;
    double f = 0;
    if (isLeftPart)
        f = -width*Lego::length_unit/2;
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

    // Create corner geometry
    osg::ref_ptr<osg::Geometry> cornerGeometry = new osg::Geometry;

    // Handle transparency
    double alpha = 0.1;
    osg::StateSet* state = cornerGeometry->getOrCreateStateSet();
    state->setMode(GL_BLEND,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    osg::Material* mat = new osg::Material;
    mat->setAlpha(osg::Material::FRONT_AND_BACK, alpha);
    state->setAttributeAndModes(mat,osg::StateAttribute::ON |
    osg::StateAttribute::OVERRIDE);
    osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
    state->setAttributeAndModes(bf);
    state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    cornerGeometry->setStateSet(state);

    // Match vertices
    cornerGeometry->setVertexArray(vertices);

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
    cornerGeometry->setColorArray(colors);
    cornerGeometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

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
    cornerGeometry->setNormalArray(normals);
    cornerGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define corner GL_QUADS with 24 vertices
    cornerGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 24));

    // Return the corner whithout plot
    return cornerGeometry.get();
}

CornerGeode* CornerGeode::cloning(void) const {
    return new CornerGeode(*this);
}
