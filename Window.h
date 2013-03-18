#ifndef WINDOW_H
#define WINDOW_H

#include "Lego.h"

class Window : public Lego {

public:
    enum WindowType { classic, tiny, bent, thin, big, giant };

    Window(WindowType windowType = classic, bool useLeftPanel = true, bool useRightPanel = true, const QColor &color = QColor(Qt::red));
    Window(const Window& window);

    WindowType getWindowType(void) const { return _windowType; }
    bool isLeftPannelUsed(void) const { return _useLeftPanel; }
    bool isRightPannelUsed(void) const { return _useRightPanel; }

    void setWindowType(WindowType windowType) { _windowType = windowType; }
    void setWindowType(int index);
    void setUseLeftPanel(bool useLeftPanel) { _useLeftPanel = useLeftPanel; }
    void setUseRightPanel(bool useRightPanel) { _useRightPanel = useRightPanel; }

    virtual void calculateBoundingBox(void);

    virtual Window* cloning(void) const;

    virtual QString whoiam(void) const;

private:
    WindowType _windowType;
    bool _useLeftPanel;
    bool _useRightPanel;
};

#endif // WINDOW_H
