#ifndef CONENODE_H
#define CONENODE_H

#include "LegoNode.h"
#include "Cone.h"

class ConeNode : public LegoNode {

public:
    ConeNode();
    ConeNode(osg::ref_ptr<Cone> cone);
    ConeNode(const ConeNode& coneNode);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Geometry> createTruncatedCone(double startRadius, double endRadius, double center, double length, int numberSegments);

    virtual ConeNode* cloning(void) const;
};

#endif // CONENODE_H