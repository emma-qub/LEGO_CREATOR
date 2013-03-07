#ifndef REVERSETILE_H
#define REVERSETILE_H

#include "Lego.h"

class ReverseTile : public Lego {

public:
    ReverseTile(int width = 1, int length = 2, const QColor &color = QColor(Qt::red));
    ReverseTile(const ReverseTile& reverseTile);

    int getWidth(void) const { return _width; }
    int getLength(void) const { return _length; }

    void setWidth(int width) { _width = width; }
    void setLength(int length) { _length = length; }

    virtual void calculateBoundingBox(void);

    virtual ReverseTile* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    int _width;
    int _length;
};

#endif // REVERSETILE_H
