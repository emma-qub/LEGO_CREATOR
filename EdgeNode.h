#ifndef EDGENODE_H
#define EDGENODE_H

#include "LegoNode.h"
#include "Edge.h"

class EdgeNode : public LegoNode {

public:
    EdgeNode();
    EdgeNode(osg::ref_ptr<Edge> edge);
    EdgeNode(const EdgeNode& edgeNode);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createEdge(void);

    virtual EdgeNode* cloning(void) const;
};

#endif // EDGENODE_H
