#ifndef RoadNode_H
#define RoadNode_H

#include "LegoNode.h"
#include "Road.h"

class RoadNode : public LegoNode {

public:
    RoadNode();
    RoadNode(Road* road);
    RoadNode(const RoadNode& roadNode);

    virtual void createGeode(void);
    virtual void calculatePlots(void);

    virtual RoadNode* cloning(void) const;
};

#endif // RoadNode_H
