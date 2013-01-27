#ifndef PLATEGEODE_H
#define PLATEGEODE_H

#include "LegoGeode.h"
#include "Plate.h"

class PlateGeode : public LegoGeode {

public:
    PlateGeode();
    PlateGeode(Plate* plate);
    PlateGeode(const PlateGeode& plateGeode);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createPlate(void) const;
    osg::ref_ptr<osg::Drawable> createPlot(double radiusX, double radiusY) const;

    virtual PlateGeode* cloning(void) const;
};

#endif // PLATEGEODE_H
