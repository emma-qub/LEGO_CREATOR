#include "ClampNode.h"

#include <osg/Geometry>
#include <osg/Material>
#include <osg/MatrixTransform>
#include <osgUtil/Tessellator>

#include <cmath>

ClampNode::ClampNode() :
    LegoNode() {
}

ClampNode::ClampNode(Clamp* clamp) :
    LegoNode(clamp) {
    
    createGeode();
}

ClampNode::ClampNode(const ClampNode& clampNode) :
    LegoNode(clampNode) {
}

void ClampNode::createGeode(void) {
    // Remove children
    removeChildren(0, getNumChildren());
    
    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);
    geode->addDrawable(createBrick());

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Get the brick
    Clamp* clamp = static_cast<Clamp*>(_lego);

    // Get clamp bounding box
    clamp->calculateBoundingBox();
    BoundingBox bb = clamp->getBoundingBox();
    // Get integer sizes
    int width = bb.getWidth();
    int length = bb.getLength();
    int height = bb.getHeight();

    // Calculate x max and y max for plots
    double xminb = -(length-2)*Lego::length_unit/2;
    double yminb = -(width-1)*Lego::length_unit/2;

    double w = (-width)*Lego::length_unit/2+Lego::height_unit/2;
    double h = (height)*Lego::height_unit/2-Lego::height_unit/2;

    // Add cylinder parts

    // Create geode
    osg::ref_ptr<osg::Geode> cylinderGeode = new osg::Geode;

    // Create drawable cylinder
    osg::ref_ptr<osg::Drawable> cylinderPart = makeCylinder(0, w, h, length*Lego::length_unit, Lego::height_unit/2);
    // Add drawable to geode
    cylinderGeode->addDrawable(cylinderPart.get());

    // Create top and bottom cylinder
    osg::ref_ptr<osg::Drawable> topBottomPart = makeDisk(0, w, h, length*Lego::length_unit, Lego::height_unit/2);
    // Add drawable to geode
    cylinderGeode->addDrawable(topBottomPart.get());

    // Create matrix transform to rotate cylinder
    osg::ref_ptr<osg::MatrixTransform> mt = new osg::MatrixTransform;
    // Create matrix to handle rotation
    osg::Matrix matRot, matTrans;
    // Make PI/2 rotation according to y axis
    matRot.makeRotate(M_PI_2, osg::Vec3(0.f, 1.f, 0.f));
    // Translate
    matTrans.makeTranslate(-length*Lego::length_unit/4-Lego::height_unit/4, 0, 3*height*Lego::height_unit/8);
    // Set matrix transform matrix
    mt->setMatrix(matRot*matTrans);
    // Add geode to matrix transform
    mt->addChild(cylinderGeode.get());
    // Add matrix transform
    addChild(mt);

    // Add bottom cylinders iteratively
    for (int i = 0; i < length-1; i++) {
        for (int j = 0; j < width; j++) {
            double radiusX = xminb + i*distPlot;
            double radiusY = yminb + j*distPlot;

            geode->addDrawable(createCylinder(radiusX, radiusY, 0.5, true, (-height+0.5)*Lego::height_unit/2));
        }
    }
}

osg::ref_ptr<osg::Drawable> ClampNode::createBrick(void) const {
    // Get the brick
    Clamp* clamp = static_cast<Clamp*>(_lego);
    
    // Get brick color
    QColor color = clamp->getColor();

    // Get clamp bounding box
    clamp->calculateBoundingBox();
    BoundingBox bb = clamp->getBoundingBox();
    // Get integer sizes
    int width = bb.getWidth();
    int length = bb.getLength();
    int height = bb.getHeight();

    // Get real position, according to tile size
    double mw = (-width)*Lego::length_unit/2;
    double mwpm = (-width)*Lego::length_unit/2+Lego::height_unit/2;
    double mwp = (-width)*Lego::length_unit/2+0.93*Lego::height_unit;
    double pw = (width)*Lego::length_unit/2;
    double pwm = (width)*Lego::length_unit/2-Lego::height_unit/2;
    double ml = (-length)*Lego::length_unit/2;
    double mlp = (-length+0.5)*Lego::length_unit/2;
    double pl = (length)*Lego::length_unit/2;
    double plm = (length-0.5)*Lego::length_unit/2;
    double mh = (-height)*Lego::height_unit/2;
    double mhp = (-height)*Lego::height_unit/2+2*Lego::plot_top_height;
    double mhpm = (-height)*Lego::height_unit/2+Lego::plot_top_height;
    double phm = (height)*Lego::height_unit/2-Lego::height_unit/2;
    double phmp = (height)*Lego::height_unit/2-0.5*Lego::height_unit/2;
    
    // Create 3 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(ml, mw, mh);
    osg::Vec3 v1(pl, mw, mh);
    osg::Vec3 v2(pl, pw, mh);
    osg::Vec3 v3(ml, pw, mh);
    osg::Vec3 v4(ml, pw, mhp);
    osg::Vec3 v5(pl, pw, mhp);
    osg::Vec3 v6(pl, mw, mhp);
    osg::Vec3 v7(ml, mw, mhp);
    osg::Vec3 v8(mlp, mw, mhp);
    osg::Vec3 v9(mlp, mw, phm);
    osg::Vec3 v10(ml, mw, phm);
    osg::Vec3 v11(ml, mwp, phmp);
    osg::Vec3 v12(mlp, mwp, phmp);
    osg::Vec3 v13(mlp, pw, mhp);
    osg::Vec3 v14(plm, mw, mhp);
    osg::Vec3 v15(plm, mw, phm);
    osg::Vec3 v16(pl, mw, phm);
    osg::Vec3 v17(pl, mwp, phmp);
    osg::Vec3 v18(plm, mwp, phmp);
    osg::Vec3 v19(plm, pw, mhp);
    osg::Vec3 v20(mlp, mwpm, mh);
    osg::Vec3 v21(plm, mwpm, mh);
    osg::Vec3 v22(plm, pwm, mh);
    osg::Vec3 v23(mlp, pwm, mh);
    osg::Vec3 v24(mlp, mwpm, mhpm);
    osg::Vec3 v25(plm, mwpm, mhpm);
    osg::Vec3 v26(plm, pwm, mhpm);
    osg::Vec3 v27(mlp, pwm, mhpm);
    
    // Create 1 faces, 0 faces are quads splitted into two triangles
    // NB: Down face is transparent, we don't even create it

    // Bottom
    vertices->push_back(v3);
    vertices->push_back(v2);
    vertices->push_back(v1);
    vertices->push_back(v0);
    // Bottom hole
    vertices->push_back(v20);
    vertices->push_back(v21);
    vertices->push_back(v22);
    vertices->push_back(v23);
    // Bottom far
    vertices->push_back(v24);
    vertices->push_back(v25);
    vertices->push_back(v26);
    vertices->push_back(v27);

    // Front face
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v5);

    // Back face
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v6);
    vertices->push_back(v7);

    // Left bottom face
    vertices->push_back(v0);
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v7);

    // Right bottom face
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v5);
    vertices->push_back(v6);

    // Top face
    vertices->push_back(v4);
    vertices->push_back(v5);
    vertices->push_back(v6);
    vertices->push_back(v7);

    // Left part back
    vertices->push_back(v7);
    vertices->push_back(v8);
    vertices->push_back(v9);
    vertices->push_back(v10);

    // Left part left ext
    vertices->push_back(v4);
    vertices->push_back(v7);
    vertices->push_back(v10);
    vertices->push_back(v11);

    // Left part front
    vertices->push_back(v4);
    vertices->push_back(v11);
    vertices->push_back(v12);
    vertices->push_back(v13);

    // Left part left int
    vertices->push_back(v8);
    vertices->push_back(v9);
    vertices->push_back(v12);
    vertices->push_back(v13);

    // Right part back
    vertices->push_back(v6);
    vertices->push_back(v14);
    vertices->push_back(v15);
    vertices->push_back(v16);

    // Left part left ext
    vertices->push_back(v5);
    vertices->push_back(v6);
    vertices->push_back(v16);
    vertices->push_back(v17);

    // Left part front
    vertices->push_back(v5);
    vertices->push_back(v17);
    vertices->push_back(v18);
    vertices->push_back(v19);

    // Left part left int
    vertices->push_back(v14);
    vertices->push_back(v15);
    vertices->push_back(v18);
    vertices->push_back(v19);

    // Bottom front
    vertices->push_back(v20);
    vertices->push_back(v21);
    vertices->push_back(v25);
    vertices->push_back(v24);

    // Bottom right
    vertices->push_back(v21);
    vertices->push_back(v22);
    vertices->push_back(v26);
    vertices->push_back(v25);

    // Bottom back
    vertices->push_back(v22);
    vertices->push_back(v23);
    vertices->push_back(v27);
    vertices->push_back(v26);

    // Bottom left
    vertices->push_back(v23);
    vertices->push_back(v20);
    vertices->push_back(v24);
    vertices->push_back(v27);

    // Create tile geometry
    osg::ref_ptr<osg::Geometry> clampGeometry = new osg::Geometry;
    
    // Match vertices
    clampGeometry->setVertexArray(vertices);
    
    // Create colors
    osg::Vec4 osgColor(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(osgColor);
    
    // Match color
    clampGeometry->setColorArray(colors);
    clampGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);
    
    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0, 0, -1));
    normals->push_back(osg::Vec3(0, 0, -1));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    double w = pw - mwp;
    double h = phmp - mhp;
    double norm = std::sqrt(w*w + h*h);
    normals->push_back(osg::Vec3(0, h/norm, w/norm));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(0, h/norm, w/norm));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    
    // Match normals
    clampGeometry->setNormalArray(normals);
    clampGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define 1 GL_QUADS with 1*4 vertices, corresponding to bottom part
    clampGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0*4, 4));

    // Define 1 GL_QUADS with 1*4 vertices, corresponding to 1 hole in bottom part
    clampGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 1*4, 4));

    // Retesslate to create hole
    osgUtil::Tessellator tesslator;
    tesslator.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
    tesslator.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
    tesslator.retessellatePolygons(*clampGeometry);

    // Create 17 GL_QUADS, i.e. 18*4 vertices
    clampGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 2*4, 18*4));

    // Return the tile whithout plot
    return clampGeometry.get();
}

ClampNode* ClampNode::cloning(void) const {
    return new ClampNode(*this);
}

