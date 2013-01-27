#ifndef LEGOGEODE_H
#define LEGOGEODE_H

#include <osg/Geode>

#include "Lego.h"

class LegoGeode : public osg::Geode {

public:
    LegoGeode(Lego* lego = NULL);
    LegoGeode(const LegoGeode& legoGeode);

    virtual void createGeode(void) = 0;
    virtual Lego* getLego(void) { return _lego; }
    virtual void setLego(Lego* lego) { _lego = lego; }

    virtual LegoGeode* cloning(void) const = 0;

protected:
    Lego* _lego;
};

#endif // LEGONODE_H
