#ifndef GRIDNODE_H
#define GRIDNODE_H

#include "LegoNode.h"
#include "Grid.h"

class GridNode : public LegoNode {

public:
    GridNode();
    GridNode(Grid* grid);
    GridNode(const GridNode& gridNode);

    virtual void createGeode(void);
    osg::Drawable* createGrid(void);

    virtual GridNode* cloning(void) const;
};

#endif // GRIDNODE_H
