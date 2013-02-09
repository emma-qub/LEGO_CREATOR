#ifndef WINDOW_H
#define WINDOW_H

#include "Lego.h"

class Window : public Lego {

public:
    Window(const QColor &color = QColor(Qt::red), QObject* parent = 0);
    Window(const Window& window);

    virtual void calculateBoundingBox(void);

    virtual Window* cloning(void) const;

    virtual QString whoiam(void) const;

};

#endif // WINDOW_H
