#include "DoorGeode.h"

#include <osg/Geometry>


DoorGeode::DoorGeode() :
    LegoGeode() {
}

DoorGeode::DoorGeode(Door* door) :
    LegoGeode(door) {

    createGeode();
}

DoorGeode::DoorGeode(const DoorGeode& doorGeode) :
    LegoGeode(doorGeode) {

    _lego = doorGeode._lego;
}

void DoorGeode::createGeode(void) {
    // Add the brick basis
    removeDrawables(0, getDrawableList().size());
    createDoor();

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Get integer sizes
    int width = 2;
    int length = 4;
    int height = 18;

    // Calculate x max and y max for plots
    double xmin = -(length-1)*Lego::length_unit/2;
    double ymin = -(width-1)*Lego::length_unit/2;

    // Add plots iteratively if the brick type is not flat
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            double radiusX = xmin + i*distPlot;
            double radiusY = ymin + j*distPlot;
            addDrawable(createPlot(radiusX, radiusY, height));
        }
    }
}

void DoorGeode::createDoor(void) {
    // Get the brick
    Door* door = static_cast<Door*>(_lego);

    // Get brick color
    QColor color = door->getColor();

    // Get integer sizes
    int width = 2;
    int length = 4;
    int height = 18;

    // Get real position, according to brick size
    // d : down, u : up, l : left, r : right, f : front, b : back
    double d = -height*Lego::height_unit/2;
    double u = height*Lego::height_unit/2;
    double l = -length*Lego::length_unit/2;
    double r = length*Lego::length_unit/2;
    double f = -width*Lego::length_unit/2;
    double b = width*Lego::length_unit/2;

    // Create vertices
    osg::Vec3 lfd(l, f, d);
    osg::Vec3 rfd(r, f, d);
    osg::Vec3 rfu(r, f, u);
    osg::Vec3 lfu(l, f, u);
    osg::Vec3 lbd(l, b, d);
    osg::Vec3 rbd(r, b, d);
    osg::Vec3 rbu(r, b, u);
    osg::Vec3 lbu(l, b, u);

    // Create left side
    createRectangle(lfd, lfu, lbu, lbd, osg::Vec3(-1, 0, 0), color);

    // Create right side
    createRectangle(rfu, rfd, rbd, rbu, osg::Vec3(1, 0, 0), color);

    // Create up side
    createRectangle(lfu, rfu, rbu, lbu, osg::Vec3(0, 0, 1), color);

    // Create down side
    createRectangle(rfd, lfd, lbd, rbd, osg::Vec3(0, 0, -1), color);

    // A door has also a front part
    double f2 = f + 1;

    // Horizontal cross part
    osg::Vec3 lf2u(l, f2, u);
    osg::Vec3 rf2u(r, f2, u);
    osg::Vec3 rf2d(r, f2, d);
    osg::Vec3 lf2d(l, f2, d);

    // Create door front part
    createRectangle(lf2u, rf2u, rf2d, lf2d, osg::Vec3(0, -1, 0), QColor(Qt::white));
}

void DoorGeode::setColorAndNormal(const osg::Vec3& normal, osg::Geometry* geometry, const QColor& color) {
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(normal);
    geometry->setNormalArray(normals);
    geometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back(colorVec);
    geometry->setColorArray(colors);
    geometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);
}

void DoorGeode::createRectangle(const osg::Vec3& A, const osg::Vec3& B, const osg::Vec3& C,
                                  const osg::Vec3& D, const osg::Vec3& normal, const QColor& color) {

    osg::ref_ptr<osg::Geometry> geometry = new osg::Geometry;
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    vertices->push_back(A);
    vertices->push_back(B);
    vertices->push_back(C);
    vertices->push_back(D);
    geometry->setVertexArray(vertices);

    // Add vertices according to order creation
    osg::ref_ptr<osg::DrawElementsUInt> pPrimitiveSet = new osg::DrawElementsUInt(osg::PrimitiveSet::QUADS, 0);
    pPrimitiveSet->push_back(0);
    pPrimitiveSet->push_back(1);
    pPrimitiveSet->push_back(2);
    pPrimitiveSet->push_back(3);
    geometry->addPrimitiveSet(pPrimitiveSet);

    // Assign color and normal to rectangle
    setColorAndNormal(normal, geometry.get(), color);

    // Add drawable
    addDrawable(geometry);
}

DoorGeode* DoorGeode::cloning(void) const {
    return new DoorGeode(*this);
}
