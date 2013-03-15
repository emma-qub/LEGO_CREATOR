#ifndef GRID_H
#define GRID_H

#include "Lego.h"

class Grid : public Lego {

public:
    Grid(const QColor& color = QColor(Qt::red));
    Grid(const Grid& grid);

    virtual void calculateBoundingBox(void);

    virtual Grid* cloning(void) const;

    virtual QString whoiam(void) const;
};

#endif // GRID_H
