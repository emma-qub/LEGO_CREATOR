#ifndef REVERSETILENODE_H
#define REVERSETILENODE_H

#include "LegoNode.h"
#include "ReverseTile.h"

class ReverseTileNode : public LegoNode {
public:
    ReverseTileNode();
    ReverseTileNode(osg::ref_ptr<ReverseTile> reverseTile);
    ReverseTileNode(const ReverseTileNode& reverseTileNode);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createReverseTile(void) const;

    virtual ReverseTileNode* cloning(void) const;
};

#endif // REVERSETILENODE_H