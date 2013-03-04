#include "DoorNode.h"

#include <osg/Geometry>
#include <osg/Material>
#include <osg/BlendFunc>
#include <osgUtil/SmoothingVisitor>


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
    // Remove children if any
    removeChildren(0, getNumChildren());

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);

    // Create door shape
    geode->addDrawable(createDoor());

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Get integer sizes
    int width = 1;
    int length = 4;
    int height = 15;

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

osg::ref_ptr<osg::Drawable> DoorNode::createDoor(void) {
    // Get the door
    Door* door = static_cast<Door*>(_lego);

    // Get door color
    QColor color = door->getColor();

    // Get integer sizes
    int width = 1;
    int length = 4;
    int height = 15;

// ///////////////
// DOOR FRAME
// ///////////////

    // Get real position, according to tile size
    double mw = (-width)*Lego::length_unit/2;
    double pw = (width)*Lego::length_unit/2;
    double ml = (-length)*Lego::length_unit/2;
    double pl = (length)*Lego::length_unit/2;
    double mlm = (-length+1)*Lego::length_unit/2;
    double plm = (length-1)*Lego::length_unit/2;
    double mh = (-height)*Lego::height_unit/2;
    double ph = (height)*Lego::height_unit/2;
    double mhm = (-height+2)*Lego::height_unit/2;
    double phm = (height-2)*Lego::height_unit/2;

    // Create 24 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(ml, pw, phm);
    osg::Vec3 v1(pl, pw, phm);
    osg::Vec3 v2(pl, pw, mhm);
    osg::Vec3 v3(ml, pw, mhm);
    osg::Vec3 v4(mlm, pw, phm);
    osg::Vec3 v5(plm, pw, phm);
    osg::Vec3 v6(plm, pw, mhm);
    osg::Vec3 v7(mlm, pw, mhm);
    osg::Vec3 v8(pl, pw, mh);
    osg::Vec3 v9(ml, pw, mh);
    osg::Vec3 v10(ml, mw, mh);
    osg::Vec3 v11(pl, mw, mh);
    osg::Vec3 v12(pl, mw, mhm);
    osg::Vec3 v13(ml, mw, mhm);
    osg::Vec3 v14(ml, pw, ph);
    osg::Vec3 v15(pl, pw, ph);
    osg::Vec3 v16(pl, mw, ph);
    osg::Vec3 v17(ml, mw, ph);
    osg::Vec3 v18(ml, mw, phm);
    osg::Vec3 v19(pl, mw, phm);
    osg::Vec3 v20(mlm, mw, phm);
    osg::Vec3 v21(plm, mw, phm);
    osg::Vec3 v22(plm, mw, phm);
    osg::Vec3 v23(mlm, mw, phm);

    // Create 16 faces with 24 vertices
    // Down part
    vertices->push_back(v8);    // transparent
    vertices->push_back(v9);    // transparent
    vertices->push_back(v10);   // transparent
    vertices->push_back(v11);   // transparent
    vertices->push_back(v11);
    vertices->push_back(v10);
    vertices->push_back(v13);
    vertices->push_back(v12);
    vertices->push_back(v12);
    vertices->push_back(v13);
    vertices->push_back(v3);
    vertices->push_back(v2);
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v9);
    vertices->push_back(v8);

    // Left part
    vertices->push_back(v9);
    vertices->push_back(v14);
    vertices->push_back(v17);
    vertices->push_back(v10);
    vertices->push_back(v13);
    vertices->push_back(v18);
    vertices->push_back(v20);
    vertices->push_back(v23);
    vertices->push_back(v23);
    vertices->push_back(v20);
    vertices->push_back(v4);
    vertices->push_back(v7);
    vertices->push_back(v7);
    vertices->push_back(v4);
    vertices->push_back(v0);
    vertices->push_back(v3);

    // Right part
    vertices->push_back(v15);
    vertices->push_back(v8);
    vertices->push_back(v11);
    vertices->push_back(v16);
    vertices->push_back(v19);
    vertices->push_back(v12);
    vertices->push_back(v22);
    vertices->push_back(v21);
    vertices->push_back(v21);
    vertices->push_back(v22);
    vertices->push_back(v6);
    vertices->push_back(v5);
    vertices->push_back(v5);
    vertices->push_back(v6);
    vertices->push_back(v2);
    vertices->push_back(v1);

    // Top part
    vertices->push_back(v14);
    vertices->push_back(v15);
    vertices->push_back(v16);
    vertices->push_back(v17);
    vertices->push_back(v17);
    vertices->push_back(v16);
    vertices->push_back(v19);
    vertices->push_back(v18);
    vertices->push_back(v18);
    vertices->push_back(v19);
    vertices->push_back(v1);
    vertices->push_back(v0);
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v15);
    vertices->push_back(v14);

    // Create tile geometry
    osg::ref_ptr<osg::Geometry> doorGeometry = new osg::Geometry;

    // Handle transparency
    double alpha = 0.1;
    osg::StateSet* state = doorGeometry->getOrCreateStateSet();
    state->setMode(GL_BLEND,osg::StateAttribute::ON|osg::StateAttribute::OVERRIDE);
    osg::Material* mat = new osg::Material;
    mat->setAlpha(osg::Material::FRONT_AND_BACK, alpha);
    state->setAttributeAndModes(mat,osg::StateAttribute::ON |
    osg::StateAttribute::OVERRIDE);
    osg::BlendFunc* bf = new osg::BlendFunc(osg::BlendFunc::SRC_ALPHA, osg::BlendFunc::ONE_MINUS_SRC_ALPHA);
    state->setAttributeAndModes(bf);
    state->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);
    state->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
    doorGeometry->setStateSet(state);

    // Match vertices
    doorGeometry->setVertexArray(vertices);

    // Add color (each rectangle has the same color except for the down one which is transparent)
    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::Vec4 transparent(.0f, .0f, .0f, .0f);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Add transparent color
    for (int k = 1; k < 2; k++)
        colors->push_back(transparent);
    // Add color to 15 other faces
    for (int k = 2; k <= 16; k++)
        colors->push_back(colorVec);

    // Match color
    doorGeometry->setColorArray(colors);
    doorGeometry->setColorBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define tile 16 GL_QUADS with 16*4 vertices
    doorGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 16*4));

    // Calculate smooth normals
    osgUtil::SmoothingVisitor::smooth(*doorGeometry);

    // Return the tile whithout plot
    return doorGeometry.get();
}

DoorNode* DoorNode::cloning(void) const {
    return new DoorNode(*this);
}
