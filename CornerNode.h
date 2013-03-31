#ifndef CornerNode_H
#define CornerNode_H

#include "LegoNode.h"
#include "Corner.h"

class CornerNode : public LegoNode {

public:
    CornerNode();
    CornerNode(Corner* corner);
    CornerNode(const CornerNode& cornerNode);

    virtual void createGeode(void);
    osg::Drawable* createCorner() const;

    virtual CornerNode* cloning(void) const;
};

#endif // CornerNode_H
