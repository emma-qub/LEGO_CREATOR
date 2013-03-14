#ifndef WindowNode_H
#define WindowNode_H

#include "LegoNode.h"
#include "Window.h"

class WindowNode : public LegoNode {
public:
    WindowNode();
    WindowNode(osg::ref_ptr<Window> window);
    WindowNode(const WindowNode& windowNode);

    virtual void createGeode(void);
    osg::ref_ptr<osg::Drawable> createWindow(void) const;
    osg::ref_ptr<osg::Drawable> createBentWindow(void) const;

    virtual WindowNode* cloning(void) const;
};

#endif // WindowNode_H
