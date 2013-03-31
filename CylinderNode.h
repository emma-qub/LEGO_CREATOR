#ifndef CYLINDERNODE_H
#define CYLINDERNODE_H

#include "LegoNode.h"
#include "Cylinder.h"

class CylinderNode : public LegoNode {

public:
    CylinderNode();
    CylinderNode(Cylinder* cylinder);
    CylinderNode(const CylinderNode& cylinderNode);

    virtual void createGeode(void);

    virtual CylinderNode* cloning(void) const;
};

#endif // CYLINDERNODE_H
