#ifndef LegoNode_H
#define LegoNode_H

#include <osg/Node>
#include <osg/ShapeDrawable>
#include <osg/Geode>

#include "Lego.h"

class LegoNode : public osg::Group {

public:
    LegoNode(osg::ref_ptr<Lego> lego = NULL);
    LegoNode(const LegoNode& legoNode);
    virtual ~LegoNode();

    virtual void createGeode(void) {}
    virtual Lego* getLego(void) { return _lego; }
    virtual void setLego(Lego* lego) { _lego = lego; }

    osg::Drawable* makeDisk(double xExt, double yExt, double z,
                                         double radiusExt, double height,
                                         bool isTop,
                                         bool hasHole = false,
                                         double xInt = 0.0, double yInt = 0.0,
                                         double radiusInt = 0.0,
                                         int numberSegments = 20) const;
    osg::Drawable* makeCylinder(double x, double y, double z,
                                             double height, double radius,
                                             int numberSegments = 20) const;

    osg::Drawable* createPlotCylinder(double radiusX, double radiusY, int height) const;
    osg::Drawable* createPlotTop(double radiusX, double radiusY, int height) const;
    osg::Drawable* createCylinder(double radiusX, double radiusY, double height, bool thin, double center = 0) const;

    virtual LegoNode* cloning(void) const { return new LegoNode(*this); }

protected:
    Lego* _lego;
};

#endif // LEGONODE_H
