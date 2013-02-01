#ifndef ROAD_H
#define ROAD_H

#include "Lego.h"

class Road : public Lego {

public:
    enum RoadType { straight, curve, intersection, cross };

    Road(RoadType roadType = straight, const QColor &color = QColor(Qt::red), QObject* parent = 0);
    Road(const Road& road);

    RoadType getRoadType(void) const { return _roadType; }

    void setRoadType(RoadType roadType) { _roadType = roadType; }
    void setRoadType(int index);

    virtual void calculateBoundingBox(void);

    virtual Road* cloning(void) const;

private:
    RoadType _roadType;
};

#endif // ROAD_H
