#ifndef WHEEL_H
#define WHEEL_H

#include "Lego.h"

class Wheel : public Lego {

public:
    Wheel(const QColor &color = QColor(Qt::red));
    Wheel(const Wheel& wheel);

    virtual void calculateBoundingBox(void);

    virtual Wheel* cloning(void) const;

    virtual QString whoiam(void) const;

};

#endif // WHEEL_H
