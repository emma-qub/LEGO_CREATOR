#ifndef ROAD_H
#define ROAD_H

#include "Lego.h"

class Road : public Lego {

public:
    enum RoadType { straight, curve, intersection, cross, none };

    Road(RoadType roadType = straight, const QColor &color = QColor(0.0, 112.0, 44.0));
    Road(const Road& road);

    RoadType getRoadType(void) const { return _roadType; }

    void setRoadType(RoadType roadType) { _roadType = roadType; }
    void setRoadType(int index);

    virtual void calculateBoundingBox(void);

    virtual Road* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    RoadType _roadType;
};

#endif // ROAD_H
