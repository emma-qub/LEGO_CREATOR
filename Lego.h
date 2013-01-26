#ifndef LEGO_H
#define LEGO_H

#include <QColor>

class Lego {
public:
    Lego(const QColor &color);
    virtual ~Lego();

    virtual QString description(void) = 0;

private:
    QColor _color;
};

#endif // LEGO_H
