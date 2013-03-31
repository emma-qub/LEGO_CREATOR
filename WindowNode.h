#ifndef WindowNode_H
#define WindowNode_H

#include "LegoNode.h"
#include "Window.h"

class WindowNode : public LegoNode {
public:
    WindowNode();
    WindowNode(Window* window);
    WindowNode(const WindowNode& windowNode);

    virtual void createGeode(void);
    osg::Drawable* createWindow(void) const;
    osg::Drawable* createBentWindow(void) const;
    osg::Drawable* createLeftPannel(void) const;
    osg::Drawable* createRightPannel(void) const;

    virtual WindowNode* cloning(void) const;
};

#endif // WindowNode_H
