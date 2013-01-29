#include "CornerGeode.h"

#include <osg/Geometry>
#include <osg/Material>
#include <osg/BlendFunc>

CornerGeode::CornerGeode() :
    LegoGeode() {
}

CornerGeode::CornerGeode(Corner* corner) :
    LegoGeode(corner) {

    createGeode();
}

CornerGeode::CornerGeode(const CornerGeode& cornerGeode) :
    LegoGeode(cornerGeode._lego) {
}

void CornerGeode::createGeode(void) {
    // Add the brick basis
    removeDrawables(0, getDrawableList().size());
    addDrawable(createBrickLeft());
    addDrawable(createBrickBack());

    int height = 3;

    addDrawable(createPlot(-Lego::length_unit/2, -Lego::length_unit/2, height));
    addDrawable(createPlot(-Lego::length_unit/2, Lego::length_unit/2, height));
    addDrawable(createPlot(Lego::length_unit/2, Lego::length_unit/2, height));

    addDrawable(createCylinder(-Lego::length_unit/2, 0, height, true));
    addDrawable(createCylinder(0, Lego::length_unit/2, height, true));

//    // Distance between two plot center
//    double distPlot = Lego::length_unit;

//    // Get the brick
//    Corner* brick = static_cast<Corner*>(_lego);


}

osg::ref_ptr<osg::Drawable> CornerGeode::createBrickLeft(void) const {
    // Get the brick
    Corner* brick = static_cast<Corner*>(_lego);

    // Get brick color
    QColor color = brick->getColor();

    // Get integer sizes
    int width = 2;
    int length = 2;
    int height = 3;

    // Get real position
    // d : down, u : up, l : left, r : right, f : front, b : back
    double d = -height*Lego::height_unit/2;
    double u = height*Lego::height_unit/2;
    double l = -length*Lego::length_unit/2;
    double r = 0;//length*Lego::length_unit/2;
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

osg::ref_ptr<osg::Drawable> CornerGeode::createBrickBack(void) const {
    // Get the brick
    Corner* brick = static_cast<Corner*>(_lego);

    // Get brick color
    QColor color = brick->getColor();

    // Get integer sizes
    int width = 2;
    int length = 2;
    int height = 3;

    // Get real position
    // d : down, u : up, l : left, r : right, f : front, b : back
    double d = -height*Lego::height_unit/2;
    double u = height*Lego::height_unit/2;
    double l = -length*Lego::length_unit/2;
    double r = length*Lego::length_unit/2;
    double f = 0;//-width*Lego::length_unit/2;
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

CornerGeode* CornerGeode::cloning(void) const {
    return new CornerGeode(*this);
}
