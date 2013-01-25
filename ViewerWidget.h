#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QGridLayout>

#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>

#include <osgDB/ReadFile>

#include <osgQt/GraphicsWindowQt>


class ViewerWidget : public QWidget, public osgViewer::CompositeViewer {
    Q_OBJECT

public:
    ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::CompositeViewer::SingleThreaded);

    QWidget* addViewWidget( osg::Camera* camera, osg::Node* scene );
    osg::Camera* createCamera(int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false);

    virtual void paintEvent( QPaintEvent* event ) { frame(); }

protected:
    QTimer _timer;
};

#endif // VIEWERWIDGET_H
