#ifndef LEGOGEODE_H
#define LEGOGEODE_H

#include <osg/Geode>

#include "Lego.h"

class LegoGeode : public osg::Geode {

public:
    LegoGeode(Lego* lego);
    virtual ~LegoGeode();

    virtual void createGeode(void) = 0;

protected:
    Lego* _lego;
};

#endif // LEGONODE_H
