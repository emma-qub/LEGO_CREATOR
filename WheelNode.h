#ifndef WheelNode_H
#define WheelNode_H

#include "LegoNode.h"
#include "Wheel.h"

class WheelNode : public LegoNode {

public:
    WheelNode();
    WheelNode(Wheel* wheel);
    WheelNode(const WheelNode& wheelNode);

    virtual void createGeode(void);
    void createWheel(bool isLeftWheel);
    void createPlate(void);

    virtual WheelNode* cloning(void) const;
};

#endif // WheelNode_H
