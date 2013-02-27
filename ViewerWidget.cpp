#include "ViewerWidget.h"

#include <QSettings>
#include <QDebug>

ViewerWidget::ViewerWidget(osgViewer::ViewerBase::ThreadingModel threadingModel) :
    QWidget() {
    setThreadingModel(threadingModel);

    _view = NULL;
    _camera = NULL;
    _widget = NULL;

    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(10);
}

ViewerWidget::~ViewerWidget() {
}

osg::Camera* ViewerWidget::createCamera(const osg::Vec4& color, int x, int y, int w, int h, const std::string& name, bool windowDecoration) {
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->windowName = name;
    traits->windowDecoration = windowDecoration;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setGraphicsContext( new osgQt::GraphicsWindowQt(traits.get()));

    camera->setClearColor(color);
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    camera->setProjectionMatrixAsPerspective( 30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 10000.0f);
    return camera.release();
}

void ViewerWidget::initView(void) {
    _view = new osgViewer::View;
    addView(_view);
}

void ViewerWidget::initManipulators(void) {
    // Get record path
    QSettings settings(QSettings::UserScope, "Perso", "Lego Creator");
    QString recordPath = settings.value("RecordPath").toString();
    if (!recordPath.endsWith('/'))
        recordPath += '/';

    // Get record file name
    QString recordFileName = settings.value("RecordFileName").toString();

    // Manipulators
    _keyswitchManipulator = new osgGA::KeySwitchMatrixManipulator;
    _keyswitchManipulator->addMatrixManipulator('1', "Trackball", new osgGA::TrackballManipulator());
    _keyswitchManipulator->addMatrixManipulator('2', "Flight", new osgGA::FlightManipulator());
    _keyswitchManipulator->addMatrixManipulator('3', "Drive", new osgGA::DriveManipulator());
    _keyswitchManipulator->addMatrixManipulator('4', "Terrain", new osgGA::TerrainManipulator());
    _keyswitchManipulator->addMatrixManipulator('5', "Animation", new osgGA::AnimationPathManipulator());
    _keyswitchManipulator->selectMatrixManipulator(0);
    _view->setCameraManipulator(_keyswitchManipulator.get());

    // Add the record camera path handler
    _view->addEventHandler(new osgViewer::RecordCameraPathHandler((recordPath+recordFileName).toStdString()));

    // Add the stats information
    _view->addEventHandler(new osgViewer::StatsHandler);
}

void ViewerWidget::changeCamera(osg::Camera* camera) {
    _camera = camera;
    _view->setCamera(_camera);
}

void ViewerWidget::changeScene(osg::Node* scene) {
    _view->setSceneData(scene);
}

void ViewerWidget::initWidget(void) {
    if (osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>(_camera->getGraphicsContext())) {
        _widget = gw->getGLWidget();
        _widget->show();
    } else
        qDebug() << "Cannot set ViewerWidget widget in ViewerWidget::setWidget(QWidget* widget)";

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_widget);
    setLayout(mainLayout);
}
