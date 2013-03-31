#ifndef EDGENODE_H
#define EDGENODE_H

#include "LegoNode.h"
#include "Edge.h"

class EdgeNode : public LegoNode {

public:
    EdgeNode();
    EdgeNode(Edge* edge);
    EdgeNode(const EdgeNode& edgeNode);

    virtual void createGeode(void);
    osg::Drawable* createClassicEdge(void);
    osg::Drawable* createCornerEdge(void);

    virtual EdgeNode* cloning(void) const;
};

#endif // EDGENODE_H
