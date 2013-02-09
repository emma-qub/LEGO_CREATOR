#ifndef WINDOWGEODE_H
#define WINDOWGEODE_H

#include "LegoGeode.h"
#include "Window.h"

class WindowGeode : public LegoGeode {
public:
    WindowGeode();
    WindowGeode(Window* window);
    WindowGeode(const WindowGeode& windowGeode);

    virtual void createGeode(void);
    void createWindow(void);
    void setColorAndNormal(const osg::Vec3& normal, osg::Geometry* geometry, const QColor& color);
    void createRectangle(const osg::Vec3& A, const osg::Vec3& B, const osg::Vec3& C,
                         const osg::Vec3& D, const osg::Vec3& normal, const QColor& color);

    virtual WindowGeode* cloning(void) const;
};

#endif // WINDOWGEODE_H
