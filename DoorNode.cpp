#include "DoorNode.h"

#include <osg/Geometry>
#include <osgDB/ReadFile>
#include <osg/TextureRectangle>
#include <osg/TexMat>


DoorNode::DoorNode() :
    LegoNode() {
}

DoorNode::DoorNode(osg::ref_ptr<Door> door) :
    LegoNode(door) {

    createGeode();
}

DoorNode::DoorNode(const DoorNode& doorNode) :
    LegoNode(doorNode) {
}

void DoorNode::createGeode(void) {
    removeChildren(0, getNumChildren());

    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);
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
            geode->addDrawable(createPlot(radiusX, radiusY, height));
        }
    }
}

void DoorNode::createDoor(void) {
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

// ///////////////
// DOOR FRAME
// ///////////////

    // Door has a frame
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

// //////////////
// FRONT PART
// //////////////

    double u3 = u2 + 0.1;
    double d3 = d2 - 0.1;

    double l3 = l2 - 0.1;
    double r3 = r2 + 0.1;

    // Front part
    osg::Vec3 l3f2u3(l3, f2, u3);
    osg::Vec3 r3f2u3(r3, f2, u3);
    osg::Vec3 r3f2d3(r3, f2, d3);
    osg::Vec3 l3f2d3(l3, f2, d3);

    // Create door front part
    createRectangle(l3f2u3, r3f2u3, r3f2d3, l3f2d3, osg::Vec3(0, -1, 0), QColor(Qt::white), true);
}

void DoorNode::setColorAndNormal(const osg::Vec3& normal, osg::Geometry* geometry, const QColor& color) {
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

void DoorNode::createRectangle(const osg::Vec3& A, const osg::Vec3& B, const osg::Vec3& C, const osg::Vec3& D,
                                const osg::Vec3& normal, const QColor& color, bool useTex) {

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

    if (useTex) {
        // Create texture coords for door
        osg::ref_ptr<osg::Vec2Array> texCoords = new osg::Vec2Array;
        texCoords->push_back(osg::Vec2(0.0f, 0.0f));
        texCoords->push_back(osg::Vec2(1.0f, 0.0f));
        texCoords->push_back(osg::Vec2(1.0f, 1.0f));
        texCoords->push_back(osg::Vec2(0.0f, 1.0f));
        geometry->setTexCoordArray(0, texCoords);

        // Disable display list to show up the texture
        geometry->setUseDisplayList(false);

        // Create image
        osg::ref_ptr<osg::Image> img = osgDB::readImageFile("../LEGO_CREATOR/IMG/Door.png");

        // Get state
        osg::ref_ptr<osg::StateSet> state = geometry->getOrCreateStateSet();

        // Set texture
        osg::ref_ptr<osg::TextureRectangle> tex = new osg::TextureRectangle(img);

        osg::ref_ptr<osg::TexMat> texMat = new osg::TexMat;
        texMat->setScaleByTextureRectangleSize(true);

        // Set state
        state->setTextureAttributeAndModes(0, tex, osg::StateAttribute::ON);
        state->setTextureAttributeAndModes(0, texMat, osg::StateAttribute::ON);

        // Turn off light for up face...
        state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    }

    // Add drawable
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);

    geode->addDrawable(geometry);
}

DoorNode* DoorNode::cloning(void) const {
    return new DoorNode(*this);
}
