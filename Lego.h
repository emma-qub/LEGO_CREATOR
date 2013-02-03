#ifndef LEGO_H
#define LEGO_H

#include <QColor>
#include <QObject>

#include "BoundingBox.h"

// Avoid graphical default
#define EPS 0.01


class Lego : public QObject {

public:
    Lego(const QColor& color = QColor(Qt::red), QObject* parent = 0);
    Lego(const Lego& lego);

    virtual QColor getColor(void) const { return _color; }
    virtual void setColor(const QColor& color) { _color = color; }
    virtual void calculateBoundingBox(void) = 0;
    BoundingBox getBoundingBox(void) const { return _boundingBox; }

    virtual Lego* cloning(void) const = 0;

    virtual QString whoiam(void) const;

    static double height_unit;
    static double length_unit;
    static double plot_top_radius;
    static double plot_bottom_radius;
    static double plot_bottom_thin_radius;
    static double plot_top_height;
    static double plot_bottom_height;

protected:
    QColor _color;
    BoundingBox _boundingBox;
};

#endif // LEGO_H
