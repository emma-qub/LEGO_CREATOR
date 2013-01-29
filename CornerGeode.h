#ifndef CORNERGEODE_H
#define CORNERGEODE_H

#include "LegoGeode.h"
#include "Corner.h"

class CornerGeode : public LegoGeode {

public:
    CornerGeode();
    CornerGeode(Corner* corner);
    CornerGeode(const CornerGeode& cornerGeode);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createBrickLeft(void) const;
    osg::ref_ptr<osg::Drawable> createBrickBack(void) const;

    virtual CornerGeode* cloning(void) const;
};

#endif // CORNERGEODE_H
