#include "ViewerWidget.h"

#include <osg/MatrixTransform>

#include <QSettings>
#include <QDebug>
#include <QDir>

#include "PickHandler.h"

ViewerWidget::ViewerWidget(bool isWorld, osgViewer::ViewerBase::ThreadingModel threadingModel) :
    QWidget(),
    _isWorld(isWorld) {
    setThreadingModel(threadingModel);

    // Init pointers
    _view = NULL;
    _camera = NULL;
    _widget = NULL;
    _picker = NULL;

    // If the viewer refers to the world, we create a real one
    if (_isWorld) {
        // Create the pick event handler
        _picker = new PickHandler;
    }

    // Update view
    connect(&_timer, SIGNAL(timeout()), this, SLOT(update()));
    _timer.start(10);
}

ViewerWidget::~ViewerWidget() {
}

osg::Camera* ViewerWidget::createCamera(const osg::Vec4& color, int x, int y, int w, int h) {
    // Set all traits
    osg::DisplaySettings* ds = osg::DisplaySettings::instance().get();
    osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits;
    traits->x = x;
    traits->y = y;
    traits->width = w;
    traits->height = h;
    traits->doubleBuffer = true;
    traits->alpha = ds->getMinimumNumAlphaBits();
    traits->stencil = ds->getMinimumNumStencilBits();
    traits->sampleBuffers = ds->getMultiSamples();
    traits->samples = ds->getNumMultiSamples();

    // Create camera and graphic context from traits
    osg::ref_ptr<osg::Camera> camera = new osg::Camera;
    camera->setGraphicsContext(new osgQt::GraphicsWindowQt(traits.get()));

    // Create color, viewport and projection values
    camera->setClearColor(color);
    camera->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
    camera->setProjectionMatrixAsPerspective( 30.0f, static_cast<double>(traits->width)/static_cast<double>(traits->height), 1.0f, 100.0f);

    // Return camera
    return camera.release();
}

void ViewerWidget::initView(void) {
    // Init view
    _view = new osgViewer::View;
    addView(_view);

    qDebug() << "Creating viewer:" << _view;
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

    // Get how many path are records in the file
    QDir recordDir(recordPath);
    QString nbPath = QString::number(recordDir.entryList(QDir::Files | QDir::NoDotAndDotDot).size());

    // Add the record camera path handler
    _view->addEventHandler(new osgViewer::RecordCameraPathHandler((recordPath+recordFileName+nbPath).toStdString()));

    // Add the stats information
    _view->addEventHandler(new osgViewer::StatsHandler);

    // If the viewer refers to the world, we add picker handler
    if (_isWorld) {
        // Add the pick event handler
        _view->addEventHandler(_picker.get());
    }
}

void ViewerWidget::changeCamera(osg::Camera* camera) {
    // Set camera
    _camera = camera;
    _view->setCamera(_camera);
}

void ViewerWidget::changeScene(osg::Node* scene) {
    // Set scene
    _view->setSceneData(scene);

    // If the viewer refers to the world, we add picker handler
    if (_isWorld) {
        // Add picker to scene
        scene->asGroup()->addChild(_picker->getOrCreateSelectionBox());
        qDebug() << "In viewer" << _view << "adding picker to scene" << scene;
    }
}

void ViewerWidget::initWidget(void) {
    // Create widget according to camera graphic context
    if (osgQt::GraphicsWindowQt* gw = dynamic_cast<osgQt::GraphicsWindowQt*>(_camera->getGraphicsContext())) {
        _widget = gw->getGLWidget();
        _widget->show();
    } else
        qDebug() << "Cannot set ViewerWidget widget in ViewerWidget::setWidget(QWidget* widget)";

    // Set layout to add widget
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(_widget);
    setLayout(mainLayout);
}
