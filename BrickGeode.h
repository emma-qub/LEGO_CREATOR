#ifndef BRICKNODE_H
#define BRICKNODE_H

#include "LegoGeode.h"
#include "Brick.h"

class BrickGeode : public LegoGeode {

public:
    BrickGeode();
    BrickGeode(Brick* brick);
    BrickGeode(const BrickGeode& brickGeode);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createBrick(void) const;

    virtual BrickGeode* cloning(void) const;
};

#endif // BRICKNODE_H
