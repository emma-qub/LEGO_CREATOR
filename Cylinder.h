#ifndef CYLINDER_H
#define CYLINDER_H

#include "Lego.h"

class Cylinder : public Lego {

public:
    enum CylinderType { big, bigPlate, bigFlat, thin, thinPlate, thinFlat };

    Cylinder(CylinderType = big, const QColor &color = QColor(Qt::red));
    Cylinder(const Cylinder& cylinder);

    CylinderType getCylinderType(void) const { return _cylinderType; }

    void setCylinderType(CylinderType cylinderType) { _cylinderType = cylinderType; }
    void setCylinderType(int index);

    virtual void calculateBoundingBox(void);

    virtual Cylinder* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    CylinderType _cylinderType;
};

#endif // CYLINDER_H
