#ifndef FRONTSHIPGEODE_H
#define FRONTSHIPGEODE_H

#include "LegoGeode.h"
#include "FrontShip.h"

class FrontShipGeode : public LegoGeode {

public:
    FrontShipGeode();
    FrontShipGeode(osg::ref_ptr<FrontShip> frontShip);
    FrontShipGeode(const FrontShipGeode& frontShipGeode);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createClassic(void) const;

    virtual FrontShipGeode* cloning(void) const;
};

#endif // FRONTSHIPGEODE_H

