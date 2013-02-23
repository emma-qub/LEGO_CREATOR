#include "WheelGeode.h"

#include <QDebug>

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/BlendFunc>

WheelGeode::WheelGeode() :
    LegoGeode() {
}

WheelGeode::WheelGeode(osg::ref_ptr<Wheel> wheel) :
    LegoGeode(wheel) {

    createGeode();
}

WheelGeode::WheelGeode(const WheelGeode& wheelGeode) :
    LegoGeode(wheelGeode) {
}

void WheelGeode::createGeode(void) {
    // Remove children
    removeChildren(0, getNumChildren());

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);

    // Create wheels
    createWheel(true);
    createWheel(false);

    // Create plate
    createPlate();

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Get integer sizes
    int width = 2;
    int length = 2;
    int height = 1;

    // Calculate x max and y max for plots
    double xmin = -(length-1)*Lego::length_unit/2;
    double ymin = -(width-1)*Lego::length_unit/2;

    // Add plots
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            double radiusX = xmin + i*distPlot;
            double radiusY = ymin + j*distPlot;
            geode->addDrawable(createPlot(radiusX, radiusY, height));
        }
    }

    // Add bottom cylinder
    geode->addDrawable(createCylinder(0, 0, height, false));
}

void WheelGeode::createWheel(bool isLeftWheel) {
    int leftShift = 1;
    if (isLeftWheel)
        leftShift = -1;

    osg::Matrix m;
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    osg::ref_ptr<osg::ShapeDrawable> wheelPart = new osg::ShapeDrawable(
                                                     new osg::Cylinder(
                                                         osg::Vec3(0, 0, 0),
                                                         Lego::length_unit,
                                                         Lego::length_unit-2*EPS
                                                )
                                            );

    wheelPart->setColor(osg::Vec4(.0, .0, .0, 1.));

    geode->addDrawable(wheelPart.get());

    osg::ref_ptr<osg::ShapeDrawable> wheelcenter = new osg::ShapeDrawable(
                                                       new osg::Cylinder(
                                                           osg::Vec3(0, 0, 0),
                                                           Lego::length_unit/2,
                                                           Lego::length_unit-EPS
                                                  )
                                              );

    wheelcenter->setColor(osg::Vec4(1., 1., 1., 1.));

    geode->addDrawable(wheelcenter.get());

    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    mt->addChild(geode);

    m.makeRotate(osg::Quat(M_PI/2, osg::Vec3(0, 1, 0)));
    mt->setMatrix(m*mt->getMatrix());

    m.makeTranslate(leftShift*1.5*Lego::length_unit, 0, 0);
    mt->setMatrix(mt->getMatrix()*m);

    addChild(mt);
}

void WheelGeode::createPlate(void) {
    // Get the wheel
    Wheel* wheel = static_cast<Wheel*>(_lego);

    // Get brick color
    QColor color = wheel->getColor();

    // Get integer sizes
    int width = 2;
    int length = 2;
    int height = 1;

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

    // Match color
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

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(brickGeometry);
    addChild(geode);
}

WheelGeode* WheelGeode::cloning(void) const {
    return new WheelGeode(*this);
}
