#include "WindowNode.h"

#include <osg/Geometry>


WindowNode::WindowNode() :
    LegoNode() {
}

WindowNode::WindowNode(osg::ref_ptr<Window> window) :
    LegoNode(window) {

    createGeode();
}

WindowNode::WindowNode(const WindowNode& windowNode) :
    LegoNode(windowNode) {
}

void WindowNode::createGeode(void) {
    removeChildren(0, getNumChildren());

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);
    createWindow();

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Get integer sizes
    int width = 2;
    int length = 4;
    int height = 9;

    // Calculate x max and y max for plots
    double xmin = -(length-1)*Lego::length_unit/2;
    double ymin = -(width-1)*Lego::length_unit/2;

    // Add plots iteratively if the brick type is not flat
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            double radiusX = xmin + i*distPlot;
            double radiusY = ymin + j*distPlot;
            geode->addDrawable(createPlot(radiusX, radiusY, height));
        }
    }
}

void WindowNode::createWindow(void) {
    // Get the brick
    Window* window = static_cast<Window*>(_lego);

    // Get brick color
    QColor color = window->getColor();

    // Get integer sizes
    int width = 2;
    int length = 4;
    int height = 9;

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

// ///////////////
// WINDOW FRAME
// ///////////////

    // Window has a frame
    double d2 = d+2;
    double u2 = u-2;

    double l2 = l+2;
    double r2 = r-2;

    // left side frame
    //osg::Vec3 lfd(l, f, d);
    osg::Vec3 l2fd(l2, f, d);
    osg::Vec3 l2fu(l2, f, u);
    //osg::Vec3 lfu(l, f, u);

    // create left side frame
    createRectangle(lfd, l2fd, l2fu, lfu, osg::Vec3(0, -1, 0), color);

    // right side frame
    //osg::Vec3 rfd(r, f, d);
    osg::Vec3 r2fd(r2, f, d);
    osg::Vec3 r2fu(r2, f, u);
    //osg::Vec3 rfu(r, f, u);

    // create right side frame
    createRectangle(rfd, r2fd, r2fu, rfu, osg::Vec3(0, -1, 0), color);

    // up side frame
    //osg::Vec3 lfu(l, f, u);
    //osg::Vec3 rfu(r, f, u);
    osg::Vec3 rfu2(r, f, u2);
    osg::Vec3 lfu2(l, f, u2);

    // create up side frame
    createRectangle(lfu, rfu, rfu2, lfu2, osg::Vec3(0, -1, 0), color);

    // down side frame
    //osg::Vec3 lfd(l, f, d);
    //osg::Vec3 rfd(r, f, d);
    osg::Vec3 rfd2(r, f, d2);
    osg::Vec3 lfd2(l, f, d2);

    // create down side frame
    createRectangle(lfd, rfd, rfd2, lfd2, osg::Vec3(0, -1, 0), color);

// ////////////////
// CROSS PART
// ////////////////

    // A window has also a cross part...
    double d4 = -2;
    double u4 = 2;

    double l4 = -2;
    double r4 = 2;

    double f2 = f + 1;

    // Horizontal cross part
    osg::Vec3 lf2d4(l, f2, d4);
    osg::Vec3 rf2d4(r, f2, d4);
    osg::Vec3 rf2u4(r, f2, u4);
    osg::Vec3 lf2u4(l, f2, u4);

    // Create horizontal cross part
    createRectangle(lf2d4, rf2d4, rf2u4, lf2u4, osg::Vec3(0, -1, 0), QColor(Qt::white));

    // Vertical cross part
    osg::Vec3 l4f2d(l4, f2, d);
    osg::Vec3 r4f2d(r4, f2, d);
    osg::Vec3 r4f2u(r4, f2, u);
    osg::Vec3 l4f2u(l4, f2, u);

    // Create vertical cross part
    createRectangle(l4f2d, r4f2d, r4f2u, l4f2u, osg::Vec3(0, -1, 0), QColor(Qt::white));

// ////////////////
// CROSS FRAME
// ////////////////

    // ...and a frame
    double l3 = l2 + 2;
    double r3 = r2 - 2;

    double u3 = u2 - 2;
    double d3 = d2 + 2;

    // left side frame
    osg::Vec3 lf2d(l, f2, d);
    osg::Vec3 l3f2d(l3, f2, d);
    osg::Vec3 l3f2u(l3, f2, u);
    osg::Vec3 lf2u(l, f2, u);

    // create left side frame
    createRectangle(lf2d, l3f2d, l3f2u, lf2u, osg::Vec3(0, -1, 0), QColor(Qt::white));

    // right side frame
    osg::Vec3 rf2d(r, f2, d);
    osg::Vec3 r3f2d(r3, f2, d);
    osg::Vec3 r3f2u(r3, f2, u);
    osg::Vec3 rf2u(r, f2, u);

    // create right side frame
    createRectangle(rf2d, r3f2d, r3f2u, rf2u, osg::Vec3(0, -1, 0), QColor(Qt::white));

    // up side frame
    //osg::Vec3 lf2u(l, f2, u);
    //osg::Vec3 rf2u(r, f2, u);
    osg::Vec3 rf2u3(r, f2, u3);
    osg::Vec3 lf2u3(l, f2, u3);

    // create up side frame
    createRectangle(lf2u, rf2u, rf2u3, lf2u3, osg::Vec3(0, -1, 0), QColor(Qt::white));

    // down side frame
    //osg::Vec3 lf2d(l, f2, d);
    //osg::Vec3 rf2d(r, f2, d);
    osg::Vec3 rf2d3(r, f2, d3);
    osg::Vec3 lf2d3(l, f2, d3);

    // create down side frame
    createRectangle(lf2d, rf2d, rf2d3, lf2d3, osg::Vec3(0, -1, 0), QColor(Qt::white));
}

void WindowNode::setColorAndNormal(const osg::Vec3& normal, osg::Geometry* geometry, const QColor& color) {
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

void WindowNode::createRectangle(const osg::Vec3& A, const osg::Vec3& B, const osg::Vec3& C,
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
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);

    geode->addDrawable(geometry);
}

WindowNode* WindowNode::cloning(void) const {
    return new WindowNode(*this);
}
