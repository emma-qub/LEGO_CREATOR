#ifndef DoorNode_H
#define DoorNode_H

#include "LegoNode.h"
#include "Door.h"

class DoorNode : public LegoNode {
public:
    DoorNode();
    DoorNode(Door* door);
    DoorNode(const DoorNode& doorNode);

    virtual void createGeode(void);
    osg::Drawable* createDoorFrame(void);
    osg::Drawable* createDoor(void);
    osg::Drawable* createDoorDecoration(void);
    void addDoorHandle(void);

    virtual DoorNode* cloning(void) const;
};

#endif // DoorNode_H
