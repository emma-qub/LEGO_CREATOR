#ifndef BRICKNODE_H
#define BRICKNODE_H

#include "LegoNode.h"
#include "Brick.h"

class BrickNode : public LegoNode {

public:
    BrickNode();
    BrickNode(osg::ref_ptr<Brick> brick);
    BrickNode(const BrickNode& brickNode);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createBrick(void) const;

    virtual BrickNode* cloning(void) const;
};

#endif // BRICKNODE_H
