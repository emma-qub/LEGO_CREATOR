#ifndef CornerNode_H
#define CornerNode_H

#include "LegoNode.h"
#include "Corner.h"

class CornerNode : public LegoNode {

public:
    CornerNode();
    CornerNode(osg::ref_ptr<Corner> corner);
    CornerNode(const CornerNode& cornerNode);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createCorner(bool isLeftPart) const;

    virtual CornerNode* cloning(void) const;
};

#endif // CornerNode_H
