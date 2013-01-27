#ifndef PLATE_H
#define PLATE_H

#include "Lego.h"

class Plate : public Lego {
public:
    Plate(int width = 1, int length = 1, const QColor &color = QColor(Qt::red), QObject* parent = 0);
    Plate(const Plate& plate);

    int getWidth(void) const { return _width; }
    int getLength(void) const { return _length; }

    void setWidth(int width) { _width = width; }
    void setLength(int length) { _length = length; }

    virtual Plate* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    int _width;
    int _length;

};

#endif // PLATE_H
