#include "ClampNode.h"

#include <osg/Geometry>
#include <osg/Material>

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
    
    // Get the clamp
    Clamp* clamp = static_cast<Clamp*>(_lego);
    
    // Get integer sizes
    int width = 2;
    int length = 3;
    int height = 3;
    
    // Calculate x max and y max for plots
    double xmin = -(length-1)*Lego::length_unit/2;
    double ymin = -(width-1)*Lego::length_unit/2;
}

osg::Drawable* ClampNode::createBrick(void) const {
    // Get the brick
    Clamp* clamp = static_cast<Clamp*>(_lego);
    
    // Get brick color
    QColor color = clamp->getColor();
    
    // Get integer sizes
    int width = 2;
    int length = 3;
    int height = 3;
    
    // Get real position, according to tile size
    double mw = (-width)*Lego::length_unit/2;
    double pw = (width)*Lego::length_unit/2;
    double ml = (-length)*Lego::length_unit/2;
    double pl = (length)*Lego::length_unit/2;
    double mh = (-height)*Lego::height_unit/2;
    double ph = (height)*Lego::height_unit/2;
    
    // Create 3 vertices
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::Vec3 v0(mw, ml, mh);
    osg::Vec3 v1(mw, pl, mh);
    osg::Vec3 v2(pw, pl, ph);
    
    // Create 1 faces, 0 faces are quads splitted into two triangles
    // NB: Down face is transparent, we don't even create it
    
    // Front face t1
    vertices->push_back(v0);
    vertices->push_back(v1);
    vertices->push_back(v2);
    
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
    normals->push_back(osg::Vec3(1, 0, 0));
    
    // Match normals
    clampGeometry->setNormalArray(normals);
    clampGeometry->setNormalBinding(osg::Geometry::BIND_PER_PRIMITIVE);
    
    // Create 1 GL_TRIANGLES, i.e. 1*3 vertices
    clampGeometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 1*3));
    
    // Return the tile whithout plot
    return clampGeometry.get();
}

ClampNode* ClampNode::cloning(void) const {
    return new ClampNode(*this);
}

