#ifndef PICKHANDLER_H
#define PICKHANDLER_H

#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osg/PolygonMode>
#include <osgDB/ReadFile>
#include <osgUtil/LineSegmentIntersector>
#include "ViewerWidget.h"


class PickHandler : public osgGA::GUIEventHandler {

public:
    osg::Node* getOrCreateSelectionBox(void);
    virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);
    void initSelectionBox(void);

protected:
    osg::ref_ptr<osg::MatrixTransform> _selectionBox;

};

#endif // PICKHANDLER_H
