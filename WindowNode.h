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
    void createWindow(void);
    void setColorAndNormal(const osg::Vec3& normal, osg::Geometry* geometry, const QColor& color);
    void createRectangle(const osg::Vec3& A, const osg::Vec3& B, const osg::Vec3& C,
                         const osg::Vec3& D, const osg::Vec3& normal, const QColor& color);

    virtual WindowNode* cloning(void) const;
};

#endif // WindowNode_H
