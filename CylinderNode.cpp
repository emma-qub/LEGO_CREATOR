#include "CylinderNode.h"

#include <osg/Geometry>
#include <osg/Material>
#include <osg/ShapeDrawable>

#include <QDebug>

CylinderNode::CylinderNode() :
    LegoNode() {
}

CylinderNode::CylinderNode(osg::ref_ptr<Cylinder> cylinder) :
    LegoNode(cylinder) {

    createGeode();
}

CylinderNode::CylinderNode(const CylinderNode& cylinderNode) :
    LegoNode(cylinderNode) {
}

void CylinderNode::createGeode(void) {
    // Remove children
    removeChildren(0, getNumChildren());

    // Create geode
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    addChild(geode);

    // Distance between two plot center
    double distPlot = Lego::length_unit;

    // Get the brick
    Cylinder* cylinder = static_cast<Cylinder*>(_lego);

    // Get brick type
    Cylinder::CylinderType cylinderType = cylinder->getCylinderType();

    // Create cylinder parameters
    double radius = 0.0;
    int height = 0;
    bool isFlat = false;
    bool isBig = false;

    // Get integer sizes
    switch (cylinderType) {
    case Cylinder::big:
        radius = 1;
        height = 3;
        isFlat = false;
        isBig = true;
        break;
    case Cylinder::bigPlate:
        radius = 1;
        height = 1;
        isFlat = false;
        isBig = true;
        break;
    case Cylinder::bigFlat:
        radius = 1;
        height = 1;
        isFlat = true;
        isBig = true;
        break;
    case Cylinder::thin:
        radius = 0.5;
        height = 3;
        isFlat = false;
        isBig = false;
        break;
    case Cylinder::thinPlate:
        radius = 0.5;
        height = 1;
        isFlat = false;
        isBig = false;
        break;
    case Cylinder::thinFlat:
        radius = 0.5;
        height = 1;
        isFlat = true;
        isBig = false;
        break;
    }

    // Get color
    QColor color = _lego->getColor();

    // Create cylinder shape for big ones
    if (isBig) {
        osg::ref_ptr<osg::ShapeDrawable> cylinderShape = new osg::ShapeDrawable(
                                             new osg::Cylinder(
                                                 osg::Vec3(0.0, 0.0, 0.0),
                                                 radius*Lego::length_unit,
                                                 height*Lego::height_unit
                                             )
                                         );
        // Assign color
        cylinderShape->setColor(osg::Vec4(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0));

        // Add cylinder shape
        geode->addDrawable(cylinderShape.get());

    // Create cylinder shapes for thin ones
    } else {
        qDebug() << Lego::plot_top_height/2
                 << height*Lego::height_unit-Lego::plot_top_height
                 << -height*Lego::height_unit/2+Lego::plot_top_height/2
                 << Lego::plot_top_height;
        osg::ref_ptr<osg::ShapeDrawable> cylinderShape = new osg::ShapeDrawable(
                                             new osg::Cylinder(
                                                 osg::Vec3(0.0, 0.0, Lego::plot_top_height/2),
                                                 radius*Lego::length_unit,
                                                 height*Lego::height_unit-Lego::plot_top_height
                                             )
                                         );
        // Assign color
        cylinderShape->setColor(osg::Vec4(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0));

        osg::ref_ptr<osg::ShapeDrawable> bottomCyShape = new osg::ShapeDrawable(
                                             new osg::Cylinder(
                                                 osg::Vec3(0.0, 0.0, -height*Lego::height_unit/2+Lego::plot_top_height/2),
                                                 Lego::plot_top_radius,
                                                 Lego::plot_top_height
                                             )
                                         );
        // Assign color
        bottomCyShape->setColor(osg::Vec4(static_cast<float>(color.red())/255.0, static_cast<float>(color.green())/255.0, static_cast<float>(color.blue())/255.0, 1.0));

        // Add cylinder shape
        geode->addDrawable(cylinderShape.get());
        geode->addDrawable(bottomCyShape.get());
    }

    // Add plots iteratively if the cylinder type is not flat
    if (!isFlat) {
        double xmin = 0;
        double ymin = 0;
        if (isBig) {
            xmin = -1*Lego::length_unit/2;
            ymin = -1*Lego::length_unit/2;
        }
        for (int i = 0; i < 2*radius; i++) {
            for (int j = 0; j < 2*radius; j++) {
                double radiusX = xmin + i*distPlot;
                double radiusY = ymin + j*distPlot;
                geode->addDrawable(createPlot(radiusX, radiusY, height));
            }
        }
    }

//    // Add bottom cylinder if cylinder is big
//    if (isBig)
//        geode->addDrawable(createCylinder(0.0, 0.0, height, false));
}

CylinderNode* CylinderNode::cloning(void) const {
    return new CylinderNode(*this);
}
