#ifndef CORNERGEODE_H
#define CORNERGEODE_H

#include "LegoGeode.h"
#include "Corner.h"

class CornerGeode : public LegoGeode {

public:
    CornerGeode();
    CornerGeode(osg::ref_ptr<Corner> corner);
    CornerGeode(const CornerGeode& cornerGeode);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createCorner(bool isLeftPart) const;

    virtual CornerGeode* cloning(void) const;
};

#endif // CORNERGEODE_H
