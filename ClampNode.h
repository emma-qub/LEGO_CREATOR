#ifndef CLAMPNODE_H
#define CLAMPNODE_H

#include "LegoNode.h"
#include "Clamp.h"

class ClampNode : public LegoNode {
    
public:
    ClampNode();
    ClampNode(Clamp* clamp);
    ClampNode(const ClampNode& clamp);
    
    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createBrick(void) const;
    
    virtual ClampNode* cloning(void) const;
};

#endif // CLAMPNODE_H

