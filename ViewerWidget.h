#ifndef VIEWERWIDGET_H
#define VIEWERWIDGET_H

#include <QtCore/QTimer>
#include <QtGui/QApplication>
#include <QtGui/QGridLayout>

#include <osgViewer/CompositeViewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>
#include <osgGA/FlightManipulator>
#include <osgGA/DriveManipulator>
#include <osgGA/KeySwitchMatrixManipulator>
#include <osgGA/AnimationPathManipulator>
#include <osgGA/TerrainManipulator>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>

#include <osgDB/ReadFile>

#include <osgQt/GraphicsWindowQt>


class ViewerWidget : public QWidget, public osgViewer::CompositeViewer {
    Q_OBJECT

public:
    ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel = osgViewer::CompositeViewer::SingleThreaded);
    virtual ~ViewerWidget();

    osg::Camera* createCamera(const osg::Vec4 &color, int x, int y, int w, int h, const std::string& name="", bool windowDecoration=false);

    virtual void paintEvent(QPaintEvent*) { frame(); }

public slots:
    void initView(void);
    void initManipulators(void);
    void changeCamera(osg::Camera* camera);
    void changeScene(osg::Node* scene);
    void initWidget(void);

protected:
    QTimer _timer;
    osg::ref_ptr<osgViewer::View> _view;
    osg::ref_ptr<osg::Camera> _camera;
    QWidget* _widget;
    osg::ref_ptr<osgGA::KeySwitchMatrixManipulator> _keyswitchManipulator;
};

#endif // VIEWERWIDGET_H
