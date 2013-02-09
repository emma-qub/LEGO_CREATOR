#ifndef DOORGEODE_H
#define DOORGEODE_H

#include "LegoGeode.h"
#include "Door.h"

class DoorGeode : public LegoGeode {
public:
    DoorGeode();
    DoorGeode(Door* door);
    DoorGeode(const DoorGeode& doorGeode);

    virtual void createGeode(void);
    void createDoor(void);
    void setColorAndNormal(const osg::Vec3& normal, osg::Geometry* geometry, const QColor& color);
    void createRectangle(const osg::Vec3& A, const osg::Vec3& B, const osg::Vec3& C,
                         const osg::Vec3& D, const osg::Vec3& normal, const QColor& color);

    virtual DoorGeode* cloning(void) const;
};

#endif // DOORGEODE_H
