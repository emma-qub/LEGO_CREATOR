#ifndef BRICK_H
#define BRICK_H

#include "Lego.h"

class Brick : public Lego {

public:
    enum BrickType { brick, plate };

    Brick(int width = 1, int length = 1, const QColor &color = QColor(Qt::red), QObject* parent = 0);
    Brick(const Brick& brick);

    int getWidth(void) const { return _width; }
    int getLength(void) const { return _length; }

    void setWidth(int width) { _width = width; }
    void setLength(int length) { _length = length; }

    virtual Brick* cloning(void) const;

private:
    int _width;
    int _length;

};

#endif // BRICK_H
