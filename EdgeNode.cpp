#include "EdgeNode.h"

#include <osg/ShapeDrawable>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Material>

EdgeNode::EdgeNode() :
    LegoNode() {
}

EdgeNode::EdgeNode(osg::ref_ptr<Edge> edge) :
    LegoNode(edge) {

    createGeode();
}

EdgeNode::EdgeNode(const EdgeNode& edgeNode) :
    LegoNode(edgeNode) {
}

void EdgeNode::createGeode(void) {
    // Remove children
    removeChildren(0, getNumChildren());

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);

    // Create edge shape
    geode->addDrawable(createEdge());
}

osg::ref_ptr<osg::Drawable> EdgeNode::createEdge(void) {
    // Get the Edge
    Edge* edge = static_cast<Edge*>(_lego);

    // Get brick color
    QColor color = edge->getColor();

    // Get integer sizes
    int width = 1;
    int length = 2;
    if (edge->getEdgeType() == Edge::corner)
        length = 1;
    int height = 3;

    // Get real position, according to tile size
    double pw = (-width)*Lego::length_unit/2;
    double mwm = (width-0.5)*Lego::length_unit/2;
    double mw = (width)*Lego::length_unit/2;
    double ml = (-length)*Lego::length_unit/2;
    double pl = (length)*Lego::length_unit/2;
    double mh = (-height)*Lego::height_unit/2;
    double mhm = (-height+2)*Lego::height_unit/2;
    double ph = (height)*Lego::height_unit/2;

    // Create 14 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(ml, mw, mh);
    osg::Vec3 v1(pl, mw, mh);
    osg::Vec3 v2(pl, pw, mh);
    osg::Vec3 v3(ml, pw, mh);
    osg::Vec3 v4(ml, pw, mhm);
    osg::Vec3 v5(pl, pw, mhm);
    osg::Vec3 v6(pl, mw, mhm);
    osg::Vec3 v7(ml, mw, mhm);

    osg::Vec3 v8(ml, mwm, mhm);
    osg::Vec3 v9(pl, mwm, mhm);
    osg::Vec3 v10(pl, mwm, ph);
    osg::Vec3 v11(ml, mwm, ph);
    osg::Vec3 v12(ml, mw, ph);
    osg::Vec3 v13(pl, mw, ph);

    // Create 10 faces with 14 vertices
    // NB: Down faces are transparent, we don't even create them

    // Part #1
    // Part #1 top
    vertices->push_back(v4);
    vertices->push_back(v5);
    vertices->push_back(v6);
    vertices->push_back(v7);
    // Part #1 back
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v6);
    vertices->push_back(v7);
    // Part #1 left
    vertices->push_back(v0);
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v7);
    // Part #1 right
    vertices->push_back(v1);
    vertices->push_back(v2);
    vertices->push_back(v5);
    vertices->push_back(v6);
    // Part #1 front
    vertices->push_back(v2);
    vertices->push_back(v3);
    vertices->push_back(v4);
    vertices->push_back(v5);

    // Part #2
    // Part #2 top
    vertices->push_back(v10);
    vertices->push_back(v11);
    vertices->push_back(v12);
    vertices->push_back(v13);
    // Part #2 back
    vertices->push_back(v6);
    vertices->push_back(v7);
    vertices->push_back(v12);
    vertices->push_back(v13);
    // Part #2 left
    vertices->push_back(v7);
    vertices->push_back(v8);
    vertices->push_back(v11);
    vertices->push_back(v12);
    // Part #2 right
    vertices->push_back(v6);
    vertices->push_back(v9);
    vertices->push_back(v10);
    vertices->push_back(v13);
    // Part #2 front
    vertices->push_back(v8);
    vertices->push_back(v9);
    vertices->push_back(v10);
    vertices->push_back(v11);

    // Create edge geometry
    osg::ref_ptr<osg::Geometry> edgeGeometry = new osg::Geometry;

    // Match vertices
    edgeGeometry->setVertexArray(vertices);

    // Create color
    osg::Vec4 osgColor(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0);
    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    // Every face has the same color, so there is only one color
    colors->push_back(osgColor);

    // Match color
    edgeGeometry->setColorArray(colors);
    edgeGeometry->setColorBinding(osg::Geometry::BIND_OVERALL);

    // Create normals
    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(0, -1, 0));
    normals->push_back(osg::Vec3(0, 0, 1));
    normals->push_back(osg::Vec3(0, 1, 0));
    normals->push_back(osg::Vec3(-1, 0, 0));
    normals->push_back(osg::Vec3(1, 0, 0));
    normals->push_back(osg::Vec3(0, -1, 0));

    // Match normals
    edgeGeometry->setNormalArray(normals);
    edgeGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);

    // Define 10 GL_QUADS with 10*4 vertices
    edgeGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 10*4));

    // Return edge geometry
    return edgeGeometry;
}

EdgeNode* EdgeNode::cloning(void) const {
    return new EdgeNode(*this);
}
