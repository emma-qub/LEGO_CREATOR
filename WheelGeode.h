#ifndef WHEELGEODE_H
#define WHEELGEODE_H

#include "LegoGeode.h"
#include "Wheel.h"

class WheelGeode : public LegoGeode {

public:
    WheelGeode();
    WheelGeode(Wheel* wheel);
    WheelGeode(const WheelGeode& wheelGeode);

    virtual void createGeode(void);
    void createWheel(bool isLeftWheel);
    void createPlate(void);

    virtual WheelGeode* cloning(void) const;
};

#endif // WHEELGEODE_H
