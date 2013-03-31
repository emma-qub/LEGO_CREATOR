#ifndef FrontShipNode_H
#define FrontShipNode_H

#include "LegoNode.h"
#include "FrontShip.h"

class FrontShipNode : public LegoNode {

public:
    FrontShipNode();
    FrontShipNode(FrontShip* frontShip);
    FrontShipNode(const FrontShipNode& frontShipNode);

    virtual void createGeode(void);
    osg::Drawable* createClassic(void) const;

    virtual FrontShipNode* cloning(void) const;
};

#endif // FrontShipNode_H

