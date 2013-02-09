#ifndef DOOR_H
#define DOOR_H

#include "Lego.h"

class Door : public Lego {

public:
    Door(const QColor &color = QColor(Qt::red), QObject* parent = 0);
    Door(const Door& door);

    virtual void calculateBoundingBox(void);

    virtual Door* cloning(void) const;

    virtual QString whoiam(void) const;

};

#endif // DOOR_H
