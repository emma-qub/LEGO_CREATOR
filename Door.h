#ifndef DOOR_H
#define DOOR_H

#include "Lego.h"

class Door : public Lego {

public:
    Door(const QColor& doorColor = QColor(Qt::white), const QColor& doorHandleColor = QColor(Qt::black), const QColor &color = QColor(Qt::red));
    Door(const Door& door);

    QColor getDoorColor(void) const { return _doorColor; }
    QColor getDoorHandleColor(void) const { return _doorHandleColor; }

    void setDoorColor(const QColor& doorColor) { _doorColor = doorColor; }
    void setDoorHandleColor(const QColor& doorHandleColor) { _doorHandleColor = doorHandleColor; }

    virtual void calculateBoundingBox(void);

    virtual Door* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    QColor _doorColor;
    QColor _doorHandleColor;
};

#endif // DOOR_H
