#ifndef FromFileNode_H
#define FromFileNode_H

#include "LegoNode.h"
#include "FromFile.h"

class FromFileNode : public LegoNode {

public:
    FromFileNode();
    FromFileNode(osg::ref_ptr<FromFile> fromFile);
    FromFileNode(const FromFileNode& fromFileNode);

    virtual void createGeode(void);

    virtual FromFileNode* cloning(void) const;
};

#endif // FromFileNode_H
