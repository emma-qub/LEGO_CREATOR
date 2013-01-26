#ifndef BRICKNODE_H
#define BRICKNODE_H

#include "LegoGeode.h"
#include "Brick.h"

class BrickGeode : public LegoGeode {

public:
    BrickGeode(Brick* brick);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createBrick(void) const;
    osg::ref_ptr<osg::Drawable> createPlot(double radiusX, double radiusY) const;
};

#endif // BRICKNODE_H
