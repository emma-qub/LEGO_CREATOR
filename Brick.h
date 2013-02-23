#ifndef BRICK_H
#define BRICK_H

#include "Lego.h"

class Brick : public Lego {

public:
    enum BrickType { classic, plate, flat };

    Brick(int width = 1, int length = 1, BrickType brickType = classic, const QColor &color = QColor(Qt::red));
    Brick(const Brick& brick);

    int getWidth(void) const { return _width; }
    int getLength(void) const { return _length; }
    BrickType getBrickType(void) const { return _brickType; }

    void setWidth(int width) { _width = width; }
    void setLength(int length) { _length = length; }
    void setBrickType(BrickType brickType) { _brickType = brickType; }
    void setBrickType(int index);

    virtual void calculateBoundingBox(void);

    virtual Brick* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    int _width;
    int _length;
    BrickType _brickType;

};

#endif // BRICK_H
