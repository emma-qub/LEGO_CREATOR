#ifndef FRONTSHIP_H
#define FRONTSHIP_H

#include "Lego.h"

class FrontShip : public Lego {

public:

    FrontShip(const QColor &color = QColor(Qt::red));
    FrontShip(const FrontShip& frontShip);

    virtual void calculateBoundingBox(void);

    virtual FrontShip* cloning(void) const;

    virtual QString whoiam(void) const;
};

#endif // FRONTSHIP_H
