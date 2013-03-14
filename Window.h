#ifndef WINDOW_H
#define WINDOW_H

#include "Lego.h"

class Window : public Lego {

public:
    enum WindowType { classic, tiny, bent, thin, big, giant };

    Window(WindowType windowType = classic, const QColor &color = QColor(Qt::red));
    Window(const Window& window);

    WindowType getWindowType(void) const { return _windowType; }

    void setWindowType(WindowType windowType) { _windowType = windowType; }
    void setWindowType(int index);

    virtual void calculateBoundingBox(void);

    virtual Window* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    WindowType _windowType;
};

#endif // WINDOW_H
