#ifndef LEGOGEODE_H
#define LEGOGEODE_H

#include <osg/Geode>
#include <osg/ShapeDrawable>

#include "Lego.h"

class LegoGeode : public osg::Geode {

public:
    LegoGeode(Lego* lego = NULL);
    LegoGeode(const LegoGeode& legoGeode);

    virtual void createGeode(void) = 0;
    virtual Lego* getLego(void) { return _lego; }
    virtual void setLego(Lego* lego) { _lego = lego; }

    osg::ref_ptr<osg::Drawable> createPlot(double radiusX, double radiusY, int height) const;

    virtual LegoGeode* cloning(void) const = 0;

protected:
    Lego* _lego;
};

#endif // LEGONODE_H
