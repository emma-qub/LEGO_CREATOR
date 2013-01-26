#include "BrickGeode.h"

#include <osg/Geometry>
#include <osg/ShapeDrawable>

#include <QDebug>

#define RECT_HEIGHT 3.2
#define RECT_SIZE   8
#define PLOT_RADIUS 2.5
#define PLOT_TOP    1.7

BrickGeode::BrickGeode(Brick* brick) :
    LegoGeode(brick) {

    createGeode();
}

void BrickGeode::createGeode(void) {
    // Add the brick basis
    removeDrawables(0, getDrawableList().size());
    addDrawable(createBrick());

    // Add plots according to the brick dimensions

    // Distance between two plot center
    double distPlot = RECT_SIZE;

    // Get the brick
    Brick* brick = static_cast<Brick*>(_lego);

    // Get integer sizes
    int width = brick->getWidth();
    int length = brick->getLength();

    // Calculate x max et y max
    double xmin = -(length-1)*RECT_SIZE/2;
    double ymin = -(width-1)*RECT_SIZE/2;

    // Add plots iteratively
    for (int i = 0; i < length; i++) {
        for (int j = 0; j < width; j++) {
            double radiusX = xmin + i*distPlot;
            double radiusY = ymin + j*distPlot;
            addDrawable(createPlot(radiusX, radiusY));
        }
    }

}

osg::ref_ptr<osg::Drawable> BrickGeode::createBrick(void) const {
    // Get the brick
    Brick* brick = static_cast<Brick*>(_lego);

    // Get brick color
    QColor color = brick->getColor();

    // Get integer sizes
    int width = brick->getWidth();
    int length = brick->getLength();
    int height = 3;

    // Get real position, according to brick size
    // d : down, u : up, l : left, r : right, f : front, b : back
    double d = -height*RECT_HEIGHT/2;
    double u = height*RECT_HEIGHT/2;
    double l = -length*RECT_SIZE/2;
    double r = length*RECT_SIZE/2;
    double f = -width*RECT_SIZE/2;
    double b = width*RECT_SIZE/2;

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

    // Match vertices
    brickGeometry->setVertexArray(vertices);

    // Add color (each rectangle has the same color except for the down one which is transparent)
    osg::Vec4 colorVec(color.red()/255, color.green()/255, color.blue()/255, 1.0);
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

    return brickGeometry.get();
}

osg::ref_ptr<osg::Drawable> BrickGeode::createPlot(double radiusX, double radiusY) const {
    // Get the brick
    Brick* brick = static_cast<Brick*>(_lego);

    // Get brick color
    QColor color = brick->getColor();

    // Get integer sizes
    int height = 3;

    // The plots are cylinders that start at the brick bottom and above the brick top
    // Since the brick z-middle is 0, the middle of the cylinder equals to the half of the part above the brick
    osg::ref_ptr<osg::ShapeDrawable> plot = new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(radiusX, radiusY, PLOT_TOP/2), PLOT_RADIUS, height*RECT_HEIGHT+PLOT_TOP));

    plot->setColor(osg::Vec4(color.red()/255, color.green()/255, color.blue()/255, 1.0));

    return plot.get();
}
