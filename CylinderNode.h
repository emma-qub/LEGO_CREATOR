#ifndef CYLINDERNODE_H
#define CYLINDERNODE_H

#include "LegoNode.h"
#include "Cylinder.h"

class CylinderNode : public LegoNode {

public:
    CylinderNode();
    CylinderNode(osg::ref_ptr<Cylinder> cylinder);
    CylinderNode(const CylinderNode& cylinderNode);

    virtual void createGeode(void);

    virtual CylinderNode* cloning(void) const;
};

#endif // CYLINDERNODE_H
