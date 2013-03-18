#include "Window.h"

Window::Window(WindowType windowType, bool useLeftPanel, bool useRightPanel, const QColor& color) :
    Lego(color),
    _windowType(windowType),
    _useLeftPanel(useLeftPanel),
    _useRightPanel(useRightPanel) {

    calculateBoundingBox();
}

Window::Window(const Window& window) :
    Lego(window) {

    _windowType = window._windowType;
    _useLeftPanel = window._useLeftPanel;
    _useRightPanel = window._useRightPanel;

    calculateBoundingBox();
}

void Window::setWindowType(int index) {
    switch (index) {
    case 0 :
        _windowType = classic;
        break;
    case 1 :
        _windowType = tiny;
        break;
    case 2 :
        _windowType = bent;
        break;
    case 3 :
        _windowType = thin;
        break;
    case 4 :
        _windowType = big;
        break;
    case 5 :
        _windowType = giant;
        break;
    }
}

void Window::calculateBoundingBox(void) {
    // According to brick type, width, length and height are different
    switch (_windowType) {
    case classic:
        _boundingBox = BoundingBox(0, 0, 0, 4, 1, 9);
        break;
    case tiny:
        _boundingBox = BoundingBox(0, 0, 0, 2, 1, 6);
        break;
    case bent:
        _boundingBox = BoundingBox(0, 0, 0, 4, 4, 9);
        break;
    case thin:
        _boundingBox = BoundingBox(0, 0, 0, 2, 1, 9);
        break;
    case big:
        _boundingBox = BoundingBox(0, 0, 0, 4, 2, 9);
        break;
    case giant:
        _boundingBox = BoundingBox(0, 0, 0, 4, 1, 18);
        break;
    }
}

Window* Window::cloning(void) const {
    return new Window(*this);
}

QString Window::whoiam(void) const {
    return "Window";
}
