#include "GridNode.h"

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Material>

GridNode::GridNode() :
    LegoNode() {
}

GridNode::GridNode(osg::ref_ptr<Grid> grid) :
    LegoNode(grid) {

    createGeode();
}

GridNode::GridNode(const GridNode& gridNode) :
    LegoNode(gridNode) {
}

void GridNode::createGeode(void) {
    // Remove children
    removeChildren(0, getNumChildren());

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);

    // Create Grid
    geode->addDrawable(createGrid());
}

osg::ref_ptr<osg::Drawable> GridNode::createGrid(void) {
    // Get the grid
    Grid* grid = static_cast<Grid*>(_lego);

    // Get brick color
    QColor color = grid->getColor();

    // Get integer sizes
    int width = 1;
    int length = 2;
    int height = 1;

    // Get real position, according to brick size
    double w0 = (-5*width)*Lego::length_unit/10;
    double w1 = (-3*width)*Lego::length_unit/10;
    double w2 = (-1*width)*Lego::length_unit/10;
    double w3 = (1*width)*Lego::length_unit/10;
    double w4 = (3*width)*Lego::length_unit/10;
    double w5 = (5*width)*Lego::length_unit/10;
    double l0 = (-length)*Lego::length_unit/2;
    double l1 = (-length)*Lego::length_unit/2+(Lego::length_unit/2-Lego::plot_top_radius);
    double l2 = -(Lego::length_unit/2-Lego::plot_top_radius);
    double l3 = (Lego::length_unit/2-Lego::plot_top_radius);
    double l4 = (length)*Lego::length_unit/2-(Lego::length_unit/2-Lego::plot_top_radius);
    double l5 = (length)*Lego::length_unit/2;
    double h0 = (-height)*Lego::height_unit/2;
    double h1 = 0;
    double h2 = (height)*Lego::height_unit/2;

    // Create 36 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(l0, w5, h0);
    osg::Vec3 v1(l0, w5, h1);
    osg::Vec3 v2(l0, w0, h1);
    osg::Vec3 v3(l0, w0, h0);
    osg::Vec3 v4(l1, w5, h0);
    osg::Vec3 v5(l1, w5, h1);
    osg::Vec3 v6(l1, w0, h1);
    osg::Vec3 v7(l1, w0, h0);
    osg::Vec3 v10(l0, w5, h2);
    osg::Vec3 v11(l0, w4, h2);
    osg::Vec3 v12(l0, w4, h0);
    osg::Vec3 v13(l0, w3, h1);
    osg::Vec3 v14(l0, w3, h2);
    osg::Vec3 v15(l0, w2, h2);
    osg::Vec3 v16(l0, w2, h1);
    osg::Vec3 v17(l0, w1, h0);
    osg::Vec3 v18(l0, w1, h2);
    osg::Vec3 v19(l0, w0, h2);
    osg::Vec3 v20(l4, w5, h0);
    osg::Vec3 v21(l4, w5, h1);
    osg::Vec3 v22(l4, w0, h1);
    osg::Vec3 v23(l4, w0, h0);
    osg::Vec3 v24(l5, w5, h0);
    osg::Vec3 v25(l5, w5, h1);
    osg::Vec3 v26(l5, w0, h1);
    osg::Vec3 v27(l5, w0, h0);
    osg::Vec3 v30(l5, w5, h2);
    osg::Vec3 v31(l5, w4, h2);
    osg::Vec3 v32(l5, w4, h0);
    osg::Vec3 v33(l5, w3, h1);
    osg::Vec3 v34(l5, w3, h2);
    osg::Vec3 v35(l5, w2, h2);
    osg::Vec3 v36(l5, w2, h1);
    osg::Vec3 v37(l5, w1, h0);
    osg::Vec3 v38(l5, w1, h2);
    osg::Vec3 v39(l5, w0, h2);
    osg::Vec3 v40(l2, w3, h0);
    osg::Vec3 v41(l2, w3, h1);
    osg::Vec3 v42(l2, w2, h1);
    osg::Vec3 v43(l2, w2, h0);
    osg::Vec3 v44(l3, w3, h0);
    osg::Vec3 v45(l3, w3, h1);
    osg::Vec3 v46(l3, w2, h1);
    osg::Vec3 v47(l3, w2, h0);

    // Create 31 faces with 44 vertices
    // NB: Down face is transparent, we don't even create it

    // Left part
    // Left part left
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v3);
    // Left part top
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v6);
    vertices->push_back(v5);
    // Left part right
    vertices->push_back(v4);
    vertices->push_back(v5);
    vertices->push_back(v6);
    vertices->push_back(v7);
    // Left part bottom
    vertices->push_back(v0);
    vertices->push_back(v3);
    vertices->push_back(v7);
    vertices->push_back(v4);

    // Right part
    // Right part left
    vertices->push_back(v20);
    vertices->push_back(v21);
    vertices->push_back(v22);
    vertices->push_back(v23);
    // Right part top
    vertices->push_back(v21);
    vertices->push_back(v22);
    vertices->push_back(v26);
    vertices->push_back(v25);
    // Right part right
    vertices->push_back(v24);
    vertices->push_back(v25);
    vertices->push_back(v26);
    vertices->push_back(v27);
    // Right part bottom
    vertices->push_back(v20);
    vertices->push_back(v23);
    vertices->push_back(v27);
    vertices->push_back(v24);

    // Front part
    // Front part top
    vertices->push_back(v10);
    vertices->push_back(v11);
    vertices->push_back(v31);
    vertices->push_back(v30);
    // Front part back
    vertices->push_back(v11);
    vertices->push_back(v12);
    vertices->push_back(v32);
    vertices->push_back(v31);
    // Front part bottom
    vertices->push_back(v12);
    vertices->push_back(v32);
    vertices->push_back(v24);
    vertices->push_back(v0);
    // Front part front
    vertices->push_back(v0);
    vertices->push_back(v10);
    vertices->push_back(v30);
    vertices->push_back(v24);
    // Front part left
    vertices->push_back(v10);
    vertices->push_back(v11);
    vertices->push_back(v12);
    vertices->push_back(v0);
    // Front part right
    vertices->push_back(v30);
    vertices->push_back(v31);
    vertices->push_back(v32);
    vertices->push_back(v24);

    // Middle part
    // Middle part top
    vertices->push_back(v14);
    vertices->push_back(v15);
    vertices->push_back(v35);
    vertices->push_back(v34);
    // Middle part back
    vertices->push_back(v15);
    vertices->push_back(v16);
    vertices->push_back(v36);
    vertices->push_back(v35);
    // Middle part bottom
    vertices->push_back(v13);
    vertices->push_back(v16);
    vertices->push_back(v36);
    vertices->push_back(v33);
    // Middle part front
    vertices->push_back(v13);
    vertices->push_back(v14);
    vertices->push_back(v34);
    vertices->push_back(v33);
    // Middle part left
    vertices->push_back(v13);
    vertices->push_back(v14);
    vertices->push_back(v15);
    vertices->push_back(v16);
    // Middle part right
    vertices->push_back(v33);
    vertices->push_back(v34);
    vertices->push_back(v35);
    vertices->push_back(v36);

    // Back part
    // Back part top
    vertices->push_back(v18);
    vertices->push_back(v19);
    vertices->push_back(v39);
    vertices->push_back(v38);
    // Back part back
    vertices->push_back(v19);
    vertices->push_back(v3);
    vertices->push_back(v27);
    vertices->push_back(v39);
    // Back part bottom
    vertices->push_back(v3);
    vertices->push_back(v27);
    vertices->push_back(v37);
    vertices->push_back(v17);
    // Back part front
    vertices->push_back(v17);
    vertices->push_back(v18);
    vertices->push_back(v38);
    vertices->push_back(v37);
    // Back part left
    vertices->push_back(v18);
    vertices->push_back(v19);
    vertices->push_back(v3);
    vertices->push_back(v17);
    // Back part right
    vertices->push_back(v38);
    vertices->push_back(v39);
    vertices->push_back(v27);
    vertices->push_back(v37);

    // Bottom part
    // Bottom part back
    vertices->push_back(v42);
    vertices->push_back(v43);
    vertices->push_back(v47);
    vertices->push_back(v46);
    // Bottom part bottom
    vertices->push_back(v40);
    vertices->push_back(v43);
    vertices->push_back(v47);
    vertices->push_back(v44);
    // Bottom part front
    vertices->push_back(v40);
    vertices->push_back(v44);
    vertices->push_back(v45);
    vertices->push_back(v41);
    // Bottom part left
    vertices->push_back(v40);
    vertices->push_back(v41);
    vertices->push_back(v42);
    vertices->push_back(v43);
    // Bottom part right
    vertices->push_back(v44);
    vertices->push_back(v45);
    vertices->push_back(v46);
    vertices->push_back(v47);

    // Create brick geometry
    osg::ref_ptr<osg::Geometry> gridGeometry = new osg::Geometry;

    // Match vertices
    gridGeometry->setVertexArray(vertices);

    // Create colors
    osg::Vec4 colorVec(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(colorVec);

    // Match color
    gridGeometry->setColorArray(colors);
    gridGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(0, 0, -1));

    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(0, 0, -1));

    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, 0, -1));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));

    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, 0, -1));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));

    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, 0, -1));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));

    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, 0, -1));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));

    // Match normals
    gridGeometry->setNormalArray(normals);
    gridGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define 31 GL_QUADS, with 31*4 vertices
    gridGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 31*4));

    // Return the door decoration
    return gridGeometry.get();
}

GridNode* GridNode::cloning(void) const {
    return new GridNode(*this);
}
