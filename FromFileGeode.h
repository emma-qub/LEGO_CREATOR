#ifndef FROMFILEGEODE_H
#define FROMFILEGEODE_H

#include "LegoGeode.h"
#include "FromFile.h"

class FromFileGeode : public LegoGeode {

public:
    FromFileGeode();
    FromFileGeode(osg::ref_ptr<FromFile> fromFile);
    FromFileGeode(const FromFileGeode& fromFileGeode);

    virtual void createGeode(void);

    virtual FromFileGeode* cloning(void) const;
};

#endif // FROMFILEGEODE_H
