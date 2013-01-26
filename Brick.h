#ifndef BRICK_H
#define BRICK_H

#include "Lego.h"

class Brick : public Lego {

public:
    Brick(int width, int length, const QColor &color = QColor(Qt::red), QObject* parent = 0);

    int getWidth(void) const { return _width; }
    int getLength(void) const { return _length; }
    void setWidth(int width) { _width = width; }
    void setLength(int length) { _length = length; }

    virtual QString description() const;

private:
    int _width;
    int _length;

};

#endif // BRICK_H
