#include "DoorNode.h"

#include <osg/Geometry>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osgUtil/Tessellator>

#include "CylinderNode.h"

DoorNode::DoorNode() :
    LegoNode() {
}

DoorNode::DoorNode(Door *door) :
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

    // Create door and door frame and door decoration and door handle
    geode->addDrawable(createDoorFrame());
    geode->addDrawable(createDoor());
    geode->addDrawable(createDoorDecoration());
    addDoorHandle();

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Get integer sizes
    int width = 1;
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
            geode->addDrawable(createPlotCylinder(radiusX, radiusY, height));
            geode->addDrawable(createPlotTop(radiusX, radiusY, height));
        }
    }

    double xminb = xmin+Lego::length_unit/2;
    // Add bottom cylinders iteratively
    for (int i = 0; i < length-1; i++) {
        double radiusX = xminb + i*distPlot;
        double radiusY = ymin;
        geode->addDrawable(createCylinder(radiusX, radiusY, 0.5, true, (-height+0.5)*Lego::height_unit/2));
    }
}

osg::Drawable *DoorNode::createDoorFrame(void) {
    // Get the door
    Door* door = static_cast<Door*>(_lego);

    // Get door frame color
    QColor doorFrameColor = door->getColor();

    // Get integer sizes
    int width = 1;
    int length = 4;
    int height = 18;

    // Get real position, according to tile size
    double mw = (-width)*Lego::length_unit/2;
    double pw = (width)*Lego::length_unit/2;
    double ml = (-length)*Lego::length_unit/2;
    double pl = (length)*Lego::length_unit/2;
    double mlm = (-length+0.5)*Lego::length_unit/2;
    double plm = (length-0.5)*Lego::length_unit/2;
    double mh = (-height)*Lego::height_unit/2;
    double ph = (height)*Lego::height_unit/2;
    double mhm = (-height+2)*Lego::height_unit/2;
    double phm = (height-2)*Lego::height_unit/2;

    // Create 24 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(ml, pw, ph);
    osg::Vec3 v1(pl, pw, ph);
    osg::Vec3 v2(pl, pw, mh);
    osg::Vec3 v3(ml, pw, mh);

    osg::Vec3 v4(mlm, pw, phm);
    osg::Vec3 v5(plm, pw, phm);
    osg::Vec3 v6(plm, pw, mhm);
    osg::Vec3 v7(mlm, pw, mhm);

    osg::Vec3 v8(ml, pw, phm);
    osg::Vec3 v9(pl, pw, phm);
    osg::Vec3 v10(pl, pw, mhm);
    osg::Vec3 v11(ml, pw, mhm);

    osg::Vec3 v20(ml, mw, ph);
    osg::Vec3 v21(pl, mw, ph);
    osg::Vec3 v22(pl, mw, mh);
    osg::Vec3 v23(ml, mw, mh);

    osg::Vec3 v24(mlm, mw, phm);
    osg::Vec3 v25(plm, mw, phm);
    osg::Vec3 v26(plm, mw, mhm);
    osg::Vec3 v27(mlm, mw, mhm);

    osg::Vec3 v28(ml, mw, phm);
    osg::Vec3 v29(pl, mw, phm);
    osg::Vec3 v30(pl, mw, mhm);
    osg::Vec3 v31(ml, mw, mhm);

    // Create 15 faces with 24 vertices
    // NB: Down face is transparent, we don't even create it

    // Front part
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v9);
    vertices->push_back(v8);

    vertices->push_back(v8);
    vertices->push_back(v4);
    vertices->push_back(v7);
    vertices->push_back(v11);

    vertices->push_back(v5);
    vertices->push_back(v9);
    vertices->push_back(v10);
    vertices->push_back(v6);

    vertices->push_back(v11);
    vertices->push_back(v10);
    vertices->push_back(v2);
    vertices->push_back(v3);

    // Back part
    vertices->push_back(v20);
    vertices->push_back(v21);
    vertices->push_back(v29);
    vertices->push_back(v28);

    vertices->push_back(v28);
    vertices->push_back(v24);
    vertices->push_back(v27);
    vertices->push_back(v31);

    vertices->push_back(v25);
    vertices->push_back(v29);
    vertices->push_back(v30);
    vertices->push_back(v26);

    vertices->push_back(v31);
    vertices->push_back(v30);
    vertices->push_back(v22);
    vertices->push_back(v23);

    // Left part
    vertices->push_back(v0);
    vertices->push_back(v3);
    vertices->push_back(v23);
    vertices->push_back(v20);

    vertices->push_back(v24);
    vertices->push_back(v27);
    vertices->push_back(v7);
    vertices->push_back(v4);

    // Right part
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v22);
    vertices->push_back(v21);

    vertices->push_back(v25);
    vertices->push_back(v26);
    vertices->push_back(v6);
    vertices->push_back(v5);

    // Top part
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v21);
    vertices->push_back(v20);

    vertices->push_back(v28);
    vertices->push_back(v29);
    vertices->push_back(v9);
    vertices->push_back(v8);

    // Bottom part
    vertices->push_back(v10);
    vertices->push_back(v11);
    vertices->push_back(v31);
    vertices->push_back(v30);

    // Create tile geometry
    osg::ref_ptr<osg::Geometry> doorGeometry = new osg::Geometry;

    // Match vertices
    doorGeometry->setVertexArray(vertices);

    // Create color
    osg::Vec4 osgColor(static_cast<float>(doorFrameColor.red())/255.0, static_cast<float>(doorFrameColor.green())/255.0, static_cast<float>(doorFrameColor.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(osgColor);

    // Match color
    doorGeometry->setColorArray(colors);
    doorGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, 1, 0));

    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));

    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));

    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));

    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 0, -1));

    normals->push_back(osg::Vec3(0, 0, 1));

    // Match normals
    doorGeometry->setNormalArray(normals);
    doorGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define 15 GL_QUADS with 15*4 vertices
    doorGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 15*4));

    // Return the tile whithout plot
    return doorGeometry.release();
}

osg::Drawable *DoorNode::createDoor(void) {
    // Get the door
    Door* door = static_cast<Door*>(_lego);

    // Get door color
    QColor doorColor = door->getDoorColor();

    // Get integer sizes
    int width = 1;
    int length = 4;
    int height = 18;

    // Get real position, according to tile size
    double w = (-width+0.5)*Lego::length_unit/2;
    double l0 = (-length+0.5)*Lego::length_unit/2;
    double l1 = (length-1)*Lego::length_unit/2;
    double l2 = (0.25)*Lego::length_unit/2;
    double l3 = (-0.25)*Lego::length_unit/4;
    double l4 = (-length+1)*Lego::length_unit/2;
    double l5 = (length-0.5)*Lego::length_unit/2;
    double h0 = (-height+2)*Lego::height_unit/2;
    double h3 = (3)*Lego::height_unit/2;
    double h4 = (height-1)*Lego::height_unit/4;
    double h5 = (height+1)*Lego::height_unit/4;
    double h6 = (height-3)*Lego::height_unit/2;
    double h7 = (height-2)*Lego::height_unit/2;

    // Create 20 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(l0, w, h0);
    osg::Vec3 v1(l5, w, h0);
    osg::Vec3 v2(l5, w, h7);
    osg::Vec3 v3(l0, w, h7);

    osg::Vec3 v4(l1, w, h6);
    osg::Vec3 v5(l2, w, h6);
    osg::Vec3 v6(l2, w, h5);
    osg::Vec3 v7(l1, w, h5);

    osg::Vec3 v8(l3, w, h6);
    osg::Vec3 v9(l4, w, h6);
    osg::Vec3 v10(l4, w, h5);
    osg::Vec3 v11(l3, w, h5);

    osg::Vec3 v12(l1, w, h4);
    osg::Vec3 v13(l2, w, h4);
    osg::Vec3 v14(l2, w, h3);
    osg::Vec3 v15(l1, w, h3);

    osg::Vec3 v16(l3, w, h4);
    osg::Vec3 v17(l4, w, h4);
    osg::Vec3 v18(l4, w, h3);
    osg::Vec3 v19(l3, w, h3);

    // Create a face with 4 holes
    // Front part
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v3);

    vertices->push_back(v7);
    vertices->push_back(v6);
    vertices->push_back(v5);
    vertices->push_back(v4);

    vertices->push_back(v8);
    vertices->push_back(v9);
    vertices->push_back(v10);
    vertices->push_back(v11);

    vertices->push_back(v12);
    vertices->push_back(v13);
    vertices->push_back(v14);
    vertices->push_back(v15);

    vertices->push_back(v16);
    vertices->push_back(v17);
    vertices->push_back(v18);
    vertices->push_back(v19);

    // Create door geometry
    osg::ref_ptr<osg::Geometry> doorGeometry = new osg::Geometry;

    // Match vertices
    doorGeometry->setVertexArray(vertices);

    // Create color
    osg::Vec4 osgColor(static_cast<float>(doorColor.red())/255.0, static_cast<float>(doorColor.green())/255.0, static_cast<float>(doorColor.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(osgColor);

    // Match color
    doorGeometry->setColorArray(colors);
    doorGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0, -1, 0));

    // Match normals
    doorGeometry->setNormalArray(normals);
    doorGeometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

    // Define 1 GL_QUADS with 1*4 vertices, corresponding to door main quad
    doorGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

    // Define 4 GL_QUADS with 1*4 vertices per quad, corresponding to 4 holes in door
    doorGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 4, 4));
    doorGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 8, 4));
    doorGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 12, 4));
    doorGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 16, 4));

    // Retesslate to create holes
    osgUtil::Tessellator tesslator;
    tesslator.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
    tesslator.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
    tesslator.retessellatePolygons(*doorGeometry);

    // Return the door with four holes
    return doorGeometry.release();
}

osg::Drawable *DoorNode::createDoorDecoration(void) {
    // Get integer sizes
    int width = 1;
    int length = 4;
    int height = 18;

    // Get real position, according to tile size
    double w = (-width+0.4)*Lego::length_unit/2;
    double l1 = (length-1)*Lego::length_unit/2;
    double l4 = (-length+1)*Lego::length_unit/2;
    double h1 = (-height+3)*Lego::height_unit/2;
    double h2 = (-3)*Lego::height_unit/2;
    double epsShift = 0.1*Lego::length_unit;

    // Create 4 vertices to decorate
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(l1, w, h1);
    osg::Vec3 v1(l4, w, h1);
    osg::Vec3 v2(l4, w, h2);
    osg::Vec3 v3(l1, w, h2);
    osg::Vec3 v4(l1-epsShift, w, h1+epsShift);
    osg::Vec3 v5(l4+epsShift, w, h1+epsShift);
    osg::Vec3 v6(l4+epsShift, w, h2-epsShift);
    osg::Vec3 v7(l1-epsShift, w, h2-epsShift);

    // Add vertices to create lines
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v5);
    vertices->push_back(v6);
    vertices->push_back(v7);

    // Create door decoration geometry
    osg::ref_ptr<osg::Geometry> decorationGeometry = new osg::Geometry;

    // Match vertices
    decorationGeometry->setVertexArray(vertices);

    // Create color
    osg::Vec4 osgColor(0.5, 0.5, 0.5, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(osgColor);

    // Match color
    decorationGeometry->setColorArray(colors);
    decorationGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0, -1, 0));

    // Match normals
    decorationGeometry->setNormalArray(normals);
    decorationGeometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

    // Define 2 GL_QUADS with 2*4 vertices, corresponding to door decoration
    decorationGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 0, 4));
    decorationGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP, 4, 4));

    // Return the door decoration
    return decorationGeometry.release();
}

void DoorNode::addDoorHandle(void) {
    // Get the door
    Door* door = static_cast<Door*>(_lego);

    // Get door handle color
    QColor doorHandleColor = door->getDoorHandleColor();

    // Get integer sizes
    int width = 1;
    int length = 4;

    // Create door handle
    osg::ref_ptr<Cylinder> cyl = new Cylinder(Cylinder::thinPlate, doorHandleColor);
    osg::ref_ptr<CylinderNode> doorHandle = new CylinderNode(cyl.get());

    // Create matrix transform to set door handle position
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    osg::Matrix matTrans, matRot;
    // Translate
    matTrans.makeTranslate((length-1.5)*Lego::length_unit/2, (-width)*Lego::length_unit/2, 0);
    mt->preMult(matTrans);
    // Rotate
    matRot.makeRotate(osg::Quat(M_PI/2, osg::Vec3(1, 0, 0)));
    mt->preMult(matRot);
    // Add door handle to matrix
    mt->addChild(doorHandle.get());

    // Add it to door
    addChild(mt.get());
}

DoorNode* DoorNode::cloning(void) const {
    return new DoorNode(*this);
}
