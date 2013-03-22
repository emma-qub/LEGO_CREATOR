#ifndef CLAMP_H
#define CLAMP_H

#include "Lego.h"

class Clamp : public Lego {
    
public:
    Clamp(const QColor &color = QColor(Qt::red));
    Clamp(const Clamp& clamp);
    
    virtual void calculateBoundingBox(void);
    
    virtual Clamp* cloning(void) const;
    
    virtual QString whoiam(void) const;
    
private:
    
};

#endif // CLAMP_H

