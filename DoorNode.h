#ifndef DoorNode_H
#define DoorNode_H

#include "LegoNode.h"
#include "Door.h"

class DoorNode : public LegoNode {
public:
    DoorNode();
    DoorNode(osg::ref_ptr<Door> door);
    DoorNode(const DoorNode& doorNode);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createDoor(void);

    virtual DoorNode* cloning(void) const;
};

#endif // DoorNode_H
