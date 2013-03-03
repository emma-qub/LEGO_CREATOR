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
    void createDoor(void);
    void setColorAndNormal(const osg::Vec3& normal, osg::Geometry* geometry, const QColor& color);
    void createRectangle(const osg::Vec3& A, const osg::Vec3& B, const osg::Vec3& C, const osg::Vec3& D,
                         const osg::Vec3& normal, const QColor& color, bool useTex = false);

    virtual DoorNode* cloning(void) const;
};

#endif // DoorNode_H
