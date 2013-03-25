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

    osg::ref_ptr<osg::Drawable> makeDisk(double x, double y, double z, double height, double radius, int numberSegments = 50) const;
    osg::ref_ptr<osg::Drawable> makeCylinder(double x, double y, double z, double height, double radius, int numberSegments = 50) const;
    osg::ref_ptr<osg::Drawable> makeTrimmedCylinder(/*double xExt, double yExt, double zExt, double xInt, double yInt, double zInt, double height, double radiusExt, double radiusInt, int numberSegments = 50*/) const;

    osg::ref_ptr<osg::Drawable> createPlot(double radiusX, double radiusY, int height) const;
    osg::ref_ptr<osg::Drawable> createCylinder(double radiusX, double radiusY, double height, bool thin, double center = 0) const;

    virtual LegoNode* cloning(void) const { return new LegoNode(*this); }

protected:
    Lego* _lego;
};

#endif // LEGONODE_H
