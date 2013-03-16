#ifndef CONE_H
#define CONE_H

#include "Lego.h"

class Cone : public Lego {

public:
    Cone(const QColor& color = QColor(Qt::red));
    Cone(const Cone& cone);

    virtual void calculateBoundingBox(void);

    virtual Cone* cloning(void) const;

    virtual QString whoiam(void) const;
};

#endif // CONE_H
