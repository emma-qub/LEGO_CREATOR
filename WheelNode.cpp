#include "WheelNode.h"

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Material>

WheelNode::WheelNode() :
    LegoNode() {
}

WheelNode::WheelNode(Wheel *wheel) :
    LegoNode(wheel) {

    createGeode();
}

WheelNode::WheelNode(const WheelNode& wheelNode) :
    LegoNode(wheelNode) {
}

void WheelNode::createGeode(void) {
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
            geode->addDrawable(createPlotCylinder(radiusX, radiusY, height));
            geode->addDrawable(createPlotTop(radiusX, radiusY, height));
        }
    }

    // Add bottom cylinder
    geode->addDrawable(createCylinder(0, 0, height, false));
}

void WheelNode::createWheel(bool isLeftWheel) {
    // Wheels are composed by two concentric cylinders, a black and a white center one
    // Shift value, depending on the left or right side, enable to create right and left wheel in only one method
    int leftShift = 1;
    if (isLeftWheel)
        leftShift = -1;

    // Create matrix to handle transformation
    osg::Matrix m;
    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;

    // Create wheel black cylinder
    osg::ref_ptr<osg::ShapeDrawable> wheelPart = new osg::ShapeDrawable(
                                                     new osg::Cylinder(
                                                         osg::Vec3(0, 0, 0),
                                                         Lego::length_unit,
                                                         Lego::length_unit-2*EPS
                                                )
                                            );
    // Paint it black
    wheelPart->setColor(osg::Vec4(.0, .0, .0, 1.));

    // Add drawable created to geode
    geode->addDrawable(wheelPart.get());

    // Create white center cylinder and shift it to be seen
    osg::ref_ptr<osg::ShapeDrawable> wheelcenter = new osg::ShapeDrawable(
                                                       new osg::Cylinder(
                                                           osg::Vec3(0, 0, 0),
                                                           Lego::length_unit/2,
                                                           Lego::length_unit-EPS
                                                  )
                                              );
    // Paint it white
    wheelcenter->setColor(osg::Vec4(1., 1., 1., 1.));

    // Add drawable created to geode
    geode->addDrawable(wheelcenter.get());

    // Create matrix transform
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    mt->addChild(geode);

    // Rotate the cylinder, because cylinder basis is on the floor
    m.makeRotate(osg::Quat(M_PI/2, osg::Vec3(0, 1, 0)));
    mt->setMatrix(m*mt->getMatrix());

    // Translate according to side value
    m.makeTranslate(leftShift*1.5*Lego::length_unit, 0, 0);
    mt->setMatrix(mt->getMatrix()*m);

    // Add wheel
    addChild(mt);
}

void WheelNode::createPlate(void) {
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
    // NB: Down face is transparent, we don't even create it

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

    // Match vertices
    brickGeometry->setVertexArray(vertices);

    // Create color
    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(colorVec);

    // Match color
    brickGeometry->setColorArray(colors);
    brickGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));

    // Match normals
    brickGeometry->setNormalArray(normals);
    brickGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Create 5 GL_QUADS, i.e. 5*4 vertices
    brickGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 5*4));

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    // Add drawable to geode
    geode->addDrawable(brickGeometry);
    // Add geode
    addChild(geode);
}

WheelNode* WheelNode::cloning(void) const {
    return new WheelNode(*this);
}
