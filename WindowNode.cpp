#include "WindowNode.h"

#include <osg/Geometry>
#include <osg/Material>
#include <osgUtil/Tessellator>

#include <cmath>

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
    // Remove children if any
    removeChildren(0, getNumChildren());

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);

    // Get the window
    Window* window = static_cast<Window*>(_lego);

    // Get window bounding box
    window->calculateBoundingBox();
    BoundingBox bb = window->getBoundingBox();
    // Get integer sizes
    int width = bb.getWidth();
    int length = bb.getLength();
    int height = bb.getHeight();

    // Create window shape, according to window type
    if (window->getWindowType() == Window::bent) {
        geode->addDrawable(createBentWindow());
    } else {
        geode->addDrawable(createWindow());
    }

    // Create left pannel, if needed
    if (window->isLeftPannelUsed())
        geode->addDrawable(createLeftPannel());

    // Create right pannel, if needed
    if (window->isRightPannelUsed())
        geode->addDrawable(createRightPannel());

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Calculate x max and y max for plots
    double xmin = -(length-1)*Lego::length_unit/2;
    double ymin = -(width-1)*Lego::length_unit/2;

    // According to window type, plots location is different
    if (window->getWindowType() == Window::bent) {
        for (int i = 0; i < length; i++) {
            double radiusX = xmin + i*distPlot;
            double radiusY = -ymin;
            geode->addDrawable(createPlot(radiusX, radiusY, height));
        }
    } else {
        // Add plots iteratively if the brick type is not flat
        for (int i = 0; i < length; i++) {
            for (int j = 0; j < width; j++) {
                double radiusX = xmin + i*distPlot;
                double radiusY = ymin + j*distPlot;
                geode->addDrawable(createPlot(radiusX, radiusY, height));
            }
        }
    }

    // According to window type, bottom cylinders are different
    double xminb = xmin+Lego::length_unit/2;
    double yminb = ymin;
    bool thin = true;
    if (window->getWindowType() == Window::big) {
        yminb = ymin+Lego::length_unit/2;
        thin = false;
    }
    // Add bottom cylinders iteratively
    for (int i = 0; i < length-1; i++) {
        double radiusX = xminb + i*distPlot;
        double radiusY = yminb;
        geode->addDrawable(createCylinder(radiusX, radiusY, 0.5, thin, (-height+0.5)*Lego::height_unit/2));
    }
}

osg::ref_ptr<osg::Drawable> WindowNode::createWindow(void) const {
    // Get the window
    Window* window = static_cast<Window*>(_lego);

    // Get window color
    QColor color = window->getColor();

    // Get window bounding box
    BoundingBox bb = window->getBoundingBox();
    // Get integer sizes
    int width = bb.getWidth();
    int length = bb.getLength();
    int height = bb.getHeight();

// ///////////////
// WINDOW FRAME
// ///////////////

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
    osg::ref_ptr<osg::Geometry> windowGeometry = new osg::Geometry;

    // Match vertices
    windowGeometry->setVertexArray(vertices);

    // Create color
    osg::Vec4 osgColor(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(osgColor);

    // Match color
    windowGeometry->setColorArray(colors);
    windowGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

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
    windowGeometry->setNormalArray(normals);
    windowGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define 15 GL_QUADS with 15*4 vertices
    windowGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 15*4));

    // Return the tile whithout plot
    return windowGeometry.get();
}

osg::ref_ptr<osg::Drawable> WindowNode::createBentWindow(void) const {
    // Get the window
    Window* window = static_cast<Window*>(_lego);

    // Get window color
    QColor color = window->getColor();

    // Get window bounding box
    BoundingBox bb = window->getBoundingBox();
    // Get integer sizes
    int width = bb.getWidth();
    int length = bb.getLength();
    int height = bb.getHeight();

// ///////////////
// WINDOW FRAME
// ///////////////

    // Get real position, according to tile size
    double mw = (-width)*Lego::length_unit/2;
    double mwm = (-width+2)*Lego::length_unit/2;
    double pw = (width)*Lego::length_unit/2;
    double pwm = (width-2)*Lego::length_unit/2;
    double ml = (-length)*Lego::length_unit/2;
    double pl = (length)*Lego::length_unit/2;
    double mlm = (-length+0.5)*Lego::length_unit/2;
    double plm = (length-0.5)*Lego::length_unit/2;
    double mh = (-height)*Lego::height_unit/2;
    double ph = (height)*Lego::height_unit/2;
    double mhm = (-height+2)*Lego::height_unit/2;
    double phm = (height-2)*Lego::height_unit/2;

    double correction = Lego::height_unit/2.256;

    // Create 24 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(ml, pw, ph);
    osg::Vec3 v1(pl, pw, ph);
    osg::Vec3 v2(pl, mwm, mh);
    osg::Vec3 v3(ml, mwm, mh);

    osg::Vec3 v4(mlm, pw, ph);
    osg::Vec3 v5(plm, pw, ph);
    osg::Vec3 v6(plm, mwm, mhm);
    osg::Vec3 v7(mlm, mwm, mhm);

    osg::Vec3 v8(ml, pw-correction, phm);
    osg::Vec3 v9(pl, pw-correction, phm);
    osg::Vec3 v10(pl, mwm, mhm);
    osg::Vec3 v11(ml, mwm, mhm);

    osg::Vec3 v20(ml, pwm, ph);
    osg::Vec3 v21(pl, pwm, ph);
    osg::Vec3 v22(pl, mw, mh);
    osg::Vec3 v23(ml, mw, mh);

    osg::Vec3 v24(mlm, pwm, ph);
    osg::Vec3 v25(plm, pwm, ph);
    osg::Vec3 v26(plm, mw, mhm);
    osg::Vec3 v27(mlm, mw, mhm);

    osg::Vec3 v28(ml, pwm-correction, phm);
    osg::Vec3 v29(pl, pwm-correction, phm);
    osg::Vec3 v30(pl, mw, mhm);
    osg::Vec3 v31(ml, mw, mhm);

    // Create 17 faces with 24 vertices
    // NB: Down face is transparent, we don't even create it

    // Front part
    // Front part top
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v9);
    vertices->push_back(v8);
    // Front part left
    vertices->push_back(v0);
    vertices->push_back(v4);
    vertices->push_back(v7);
    vertices->push_back(v11);
    // Front part right
    vertices->push_back(v5);
    vertices->push_back(v1);
    vertices->push_back(v10);
    vertices->push_back(v6);
    // Front part bottom
    vertices->push_back(v11);
    vertices->push_back(v10);
    vertices->push_back(v2);
    vertices->push_back(v3);

    // Back part
    // Back part top
    vertices->push_back(v20);
    vertices->push_back(v21);
    vertices->push_back(v29);
    vertices->push_back(v28);
    // Back part left
    vertices->push_back(v20);
    vertices->push_back(v24);
    vertices->push_back(v27);
    vertices->push_back(v31);
    // Back part right
    vertices->push_back(v25);
    vertices->push_back(v21);
    vertices->push_back(v30);
    vertices->push_back(v26);
    // Back part bottom
    vertices->push_back(v31);
    vertices->push_back(v30);
    vertices->push_back(v22);
    vertices->push_back(v23);

    // Left part
    // Left part middle
    vertices->push_back(v0);
    vertices->push_back(v20);
    vertices->push_back(v31);
    vertices->push_back(v11);
    // Left part bottom
    vertices->push_back(v3);
    vertices->push_back(v11);
    vertices->push_back(v31);
    vertices->push_back(v23);
    // Left part middle back
    vertices->push_back(v24);
    vertices->push_back(v27);
    vertices->push_back(v7);
    vertices->push_back(v4);

    // Right part
    // Right part middle
    vertices->push_back(v1);
    vertices->push_back(v10);
    vertices->push_back(v30);
    vertices->push_back(v21);
    // Right part bottom
    vertices->push_back(v2);
    vertices->push_back(v10);
    vertices->push_back(v30);
    vertices->push_back(v22);
    // Right part middle back
    vertices->push_back(v25);
    vertices->push_back(v26);
    vertices->push_back(v6);
    vertices->push_back(v5);

    // Top part
    // Top part top
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v21);
    vertices->push_back(v20);
    // Top part bottom
    vertices->push_back(v28);
    vertices->push_back(v29);
    vertices->push_back(v9);
    vertices->push_back(v8);

    // Bottom part
    // Bottom part top
    vertices->push_back(v10);
    vertices->push_back(v11);
    vertices->push_back(v31);
    vertices->push_back(v30);

    // Create tile geometry
    osg::ref_ptr<osg::Geometry> windowGeometry = new osg::Geometry;

    // Match vertices
    windowGeometry->setVertexArray(vertices);

    // Create color
    osg::Vec4 osgColor(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(osgColor);

    // Match color
    windowGeometry->setColorArray(colors);
    windowGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    // Front
    normals->push_back(osg::Vec3(0, 3.0/5.0, -4.0/5.0));
    normals->push_back(osg::Vec3(0, 3.0/5.0, -4.0/5.0));
    normals->push_back(osg::Vec3(0, 3.0/5.0, -4.0/5.0));
    normals->push_back(osg::Vec3(0, 1, 0));

    // Back
    normals->push_back(osg::Vec3(0, -3.0/5.0, 4.0/5.0));
    normals->push_back(osg::Vec3(0, -3.0/5.0, 4.0/5.0));
    normals->push_back(osg::Vec3(0, -3.0/5.0, 4.0/5.0));
    normals->push_back(osg::Vec3(0, -1, 0));

    // Left
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));

    // Right
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));

    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 0, -1));

    normals->push_back(osg::Vec3(0, 0, 1));

    // Match normals
    windowGeometry->setNormalArray(normals);
    windowGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define 17 GL_QUADS with 17*4 vertices
    windowGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 17*4));

    // Return the tile whithout plot
    return windowGeometry.get();
}

osg::ref_ptr<osg::Drawable> WindowNode::createLeftPannel(void) const {
    // Get door color
    QColor color = QColor(Qt::white);

    // Get integer sizes
    int width = 1;
    int length = 4;
    int height = 9;

    // Get real position, according to tile size
    double w = (-width+0.5)*Lego::length_unit/2;
    double l0 = (-length+0.5)*Lego::length_unit/2;
    double l1 = (-length+1)*Lego::length_unit/2;
    double l2 = (-0.6)*Lego::length_unit/2;
    double l3 = (-0.1)*Lego::length_unit/2;
    double h0 = (-height+2)*Lego::height_unit/2;
    double h1 = (-height+3)*Lego::height_unit/2;
    double h2 = (-0.5)*Lego::height_unit/2;
    double h3 = (0.5)*Lego::height_unit/2;
    double h4 = (height-3)*Lego::height_unit/2;
    double h5 = (height-2)*Lego::height_unit/2;

    // Create 12 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(l0, w, h0);
    osg::Vec3 v1(l3, w, h0);
    osg::Vec3 v2(l3, w, h5);
    osg::Vec3 v3(l0, w, h5);

    osg::Vec3 v4(l1, w, h4);
    osg::Vec3 v5(l2, w, h4);
    osg::Vec3 v6(l2, w, h3);
    osg::Vec3 v7(l1, w, h3);

    osg::Vec3 v8(l1, w, h2);
    osg::Vec3 v9(l2, w, h2);
    osg::Vec3 v10(l2, w, h1);
    osg::Vec3 v11(l1, w, h1);

    // Create a face with 2 holes
    // Front part
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v3);

    vertices->push_back(v4);
    vertices->push_back(v5);
    vertices->push_back(v6);
    vertices->push_back(v7);

    vertices->push_back(v8);
    vertices->push_back(v9);
    vertices->push_back(v10);
    vertices->push_back(v11);

    // Create door geometry
    osg::ref_ptr<osg::Geometry> pannelGeometry = new osg::Geometry;

    // Match vertices
    pannelGeometry->setVertexArray(vertices);

    // Create color
    osg::Vec4 osgColor(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(osgColor);

    // Match color
    pannelGeometry->setColorArray(colors);
    pannelGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0, -1, 0));

    // Match normals
    pannelGeometry->setNormalArray(normals);
    pannelGeometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

    // Define 1 GL_QUADS with 1*4 vertices, corresponding to door main quad
    pannelGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

    // Define 4 GL_QUADS with 1*4 vertices per quad, corresponding to 4 holes in door
    pannelGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 4, 4));
    pannelGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 8, 4));

    // Retesslate to create holes
    osgUtil::Tessellator tesslator;
    tesslator.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
    tesslator.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
    tesslator.retessellatePolygons(*pannelGeometry);

    // Return the door with four holes
    return pannelGeometry.get();
}

osg::ref_ptr<osg::Drawable> WindowNode::createRightPannel(void) const {
    // Get door color
    QColor color = QColor(Qt::white);

    // Get integer sizes
    int width = 1;
    int length = 4;
    int height = 9;

    // Get real position, according to tile size
    double w = (-width+0.5)*Lego::length_unit/2;
    double l0 = (0.1)*Lego::length_unit/2;
    double l1 = (0.6)*Lego::length_unit/2;
    double l2 = (length-1)*Lego::length_unit/2;
    double l3 = (length-0.5)*Lego::length_unit/2;
    double h0 = (-height+2)*Lego::height_unit/2;
    double h1 = (-height+3)*Lego::height_unit/2;
    double h2 = (-0.5)*Lego::height_unit/2;
    double h3 = (0.5)*Lego::height_unit/2;
    double h4 = (height-3)*Lego::height_unit/2;
    double h5 = (height-2)*Lego::height_unit/2;

    // Create 12 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(l0, w, h0);
    osg::Vec3 v1(l3, w, h0);
    osg::Vec3 v2(l3, w, h5);
    osg::Vec3 v3(l0, w, h5);

    osg::Vec3 v4(l1, w, h4);
    osg::Vec3 v5(l2, w, h4);
    osg::Vec3 v6(l2, w, h3);
    osg::Vec3 v7(l1, w, h3);

    osg::Vec3 v8(l1, w, h2);
    osg::Vec3 v9(l2, w, h2);
    osg::Vec3 v10(l2, w, h1);
    osg::Vec3 v11(l1, w, h1);

    // Create a face with 2 holes
    // Front part
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v3);

    vertices->push_back(v4);
    vertices->push_back(v5);
    vertices->push_back(v6);
    vertices->push_back(v7);

    vertices->push_back(v8);
    vertices->push_back(v9);
    vertices->push_back(v10);
    vertices->push_back(v11);

    // Create door geometry
    osg::ref_ptr<osg::Geometry> pannelGeometry = new osg::Geometry;

    // Match vertices
    pannelGeometry->setVertexArray(vertices);

    // Create color
    osg::Vec4 osgColor(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(osgColor);

    // Match color
    pannelGeometry->setColorArray(colors);
    pannelGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0, -1, 0));

    // Match normals
    pannelGeometry->setNormalArray(normals);
    pannelGeometry->setNormalBinding(osg::Geometry::BIND_OVERALL);

    // Define 1 GL_QUADS with 1*4 vertices, corresponding to door main quad
    pannelGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

    // Define 4 GL_QUADS with 1*4 vertices per quad, corresponding to 4 holes in door
    pannelGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 4, 4));
    pannelGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 8, 4));

    // Retesslate to create holes
    osgUtil::Tessellator tesslator;
    tesslator.setTessellationType(osgUtil::Tessellator::TESS_TYPE_GEOMETRY);
    tesslator.setWindingType(osgUtil::Tessellator::TESS_WINDING_ODD);
    tesslator.retessellatePolygons(*pannelGeometry);

    // Return the door with four holes
    return pannelGeometry.get();
}

WindowNode* WindowNode::cloning(void) const {
    return new WindowNode(*this);
}
