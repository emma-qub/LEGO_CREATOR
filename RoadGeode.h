#ifndef ROADGEODE_H
#define ROADGEODE_H

#include "LegoGeode.h"
#include "Road.h"

class RoadGeode : public LegoGeode {

public:
    RoadGeode();
    RoadGeode(osg::ref_ptr<Road> road);
    RoadGeode(const RoadGeode& roadGeode);

    virtual void createGeode(void);
    virtual void calculatePlots(void);

    virtual RoadGeode* cloning(void) const;
};

#endif // ROADGEODE_H
