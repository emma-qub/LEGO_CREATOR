#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "MainWindow.h"

#include "GenerateRoadWindow.h"
#include "Commands.h"
#include "SettingsDialog.h"

#include "LegoFactory.h"
#include "BrickDialog.h"
#include "CornerDialog.h"
#include "RoadDialog.h"
#include "CharacterDialog.h"
#include "WindowDialog.h"
#include "DoorDialog.h"
#include "FromFileDialog.h"
#include "WheelDialog.h"
#include "TileDialog.h"
#include "FrontShipDialog.h"

#include "Traffic.h"

#include <QSettings>

#include <osgDB/WriteFile>
#include <osgDB/ReadFile>


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _legoColor(Qt::red),
    _world(),
    _roadPath(),
    _settings(QSettings::UserScope, "Perso", "Lego Creator"),
    _alreadySaved(false),
    _saved(true) {

    // Create undo stack to manage undo/redo actions
    _undoStack = new QUndoStack(this);

    // Create the settings dialog
    _settingsDialog = new SettingsDialog;

    // Settings to record save path and other
    _settings.setValue("SavePath", "../LEGO_CREATOR/OSG/");
    _settings.setValue("OpenPath", "../LEGO_CREATOR/OSG/");
    _settings.setValue("RecordPath", "../LEGO_CREATOR/OSG/RecordPath/");
    _settings.setValue("VehiculesPath", "../LEGO_CREATOR/OSG/Vehicules");
    _settings.setValue("RecordFileName", "traffic.path");
    _settings.setValue("FileName", "");
    _settings.setValue("DefaultViewerColor", QColor(236.0, 236.0, 236.0));
    _settings.setValue("DefaultViewerWidth", 20);
    _settings.setValue("DefaultViewerLength", 30);

    // Register in factories
    initFactories();

    // Init preview element
    initPreview();
    initDialogs();

    // Create menus
    createFileMenu();
    createEditMenu();
    createGenerateMenu();
    createTrafficMenu();
    createHelpMenu();

    // Create tool bar
    createToolBar();

    // Create undo/redo window
    createUndoView();

    // Create right dock
    createParamsDock();

    // Create scene
    createScene();

    // Init Traffic from world scene
    initTraffic();

    // Set tabs mode
    setCentralWidget(_sceneFrame);
    _sceneFrame->setObjectName("CentralWidget");

    // Connections
    connect(_shapeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(chooseDialog(int)));
    for (int k = 0; k < _shapeComboBox->count(); k++)
        connect(_legoDialog.at(k), SIGNAL(changedLego(LegoNode*)), this, SLOT(legoUpdated(LegoNode*)));
    connect(_colorButton, SIGNAL(clicked()), this, SLOT(browseColor()));
    connect(_createButton, SIGNAL(clicked()), this, SLOT(createLego()));

    connect(_undoAction, SIGNAL(triggered()), this, SLOT(freezeFit()));
    connect(_redoAction, SIGNAL(triggered()), this, SLOT(freezeCreate()));

    connect(_settingsDialog, SIGNAL(gridSizeChanged()), this, SLOT(updateWorldGrid()));
    connect(_settingsDialog, SIGNAL(viewerColorChanged(QColor)), this, SLOT(viewerColorUpdate(QColor)));

    // Change soft title
    setWindowTitle("LEGO Creator");

    // Maximize window  // Commented because bugs occured
    //setWindowState(Qt::WindowMaximized);

    // Apply style sheet
    setStyle();
}

MainWindow::~MainWindow() {
    // Delete all factories
    LegoFactory<Brick, QString>::kill();
    LegoFactory<BrickNode, QString>::kill();
    LegoFactory<BrickDialog, QString>::kill();

    LegoFactory<Corner, QString>::kill();
    LegoFactory<CornerNode, QString>::kill();
    LegoFactory<CornerDialog, QString>::kill();

    LegoFactory<Road, QString>::kill();
    LegoFactory<RoadNode, QString>::kill();
    LegoFactory<RoadDialog, QString>::kill();

    LegoFactory<Tile, QString>::kill();
    LegoFactory<TileNode, QString>::kill();
    LegoFactory<TileDialog, QString>::kill();

    LegoFactory<Character, QString>::kill();
    LegoFactory<CharacterNode, QString>::kill();
    LegoFactory<CharacterDialog, QString>::kill();

    LegoFactory<Window, QString>::kill();
    LegoFactory<WindowNode, QString>::kill();
    LegoFactory<WindowDialog, QString>::kill();

    LegoFactory<Door, QString>::kill();
    LegoFactory<DoorNode, QString>::kill();
    LegoFactory<DoorDialog, QString>::kill();

    LegoFactory<FromFile, QString>::kill();
    LegoFactory<FromFileNode, QString>::kill();
    LegoFactory<FromFileDialog, QString>::kill();

    LegoFactory<Wheel, QString>::kill();
    LegoFactory<WheelNode, QString>::kill();
    LegoFactory<WheelDialog, QString>::kill();

    LegoFactory<Character, QString>::kill();
    LegoFactory<CharacterNode, QString>::kill();
    LegoFactory<CharacterDialog, QString>::kill();

    LegoFactory<FrontShip, QString>::kill();
    LegoFactory<FrontShipNode, QString>::kill();
    LegoFactory<FrontShipDialog, QString>::kill();
}

void MainWindow::initFactories(void) {
    // Register Brick
    LegoFactory<Brick, QString>::instance()->registerLego(QString("Brick"), new Brick);
    // Register BrickNode
    LegoFactory<BrickNode, QString>::instance()->registerLego(QString("BrickNode"), new BrickNode);
    // Register BrickDialog
    LegoFactory<BrickDialog, QString>::instance()->registerLego(QString("BrickDialog"), new BrickDialog);

    // Register Corner
    LegoFactory<Corner, QString>::instance()->registerLego(QString("Corner"), new Corner);
    // Register CornerNode
    LegoFactory<CornerNode, QString>::instance()->registerLego(QString("CornerNode"), new CornerNode);
    // Register CornerDialog
    LegoFactory<CornerDialog, QString>::instance()->registerLego(QString("CornerDialog"), new CornerDialog);

    // Register Road
    LegoFactory<Road, QString>::instance()->registerLego(QString("Road"), new Road);
    // Register RoadNode
    LegoFactory<RoadNode, QString>::instance()->registerLego(QString("RoadNode"), new RoadNode);
    // Register RoadDialog
    LegoFactory<RoadDialog, QString>::instance()->registerLego(QString("RoadDialog"), new RoadDialog);

    // Register Tile
    LegoFactory<Tile, QString>::instance()->registerLego(QString("Tile"), new Tile);
    // Register TileNode
    LegoFactory<TileNode, QString>::instance()->registerLego(QString("TileNode"), new TileNode);
    // Register TileDialog
    LegoFactory<TileDialog, QString>::instance()->registerLego(QString("TileDialog"), new TileDialog);

    // Register Window
    LegoFactory<Window, QString>::instance()->registerLego(QString("Window"), new Window);
    // Register WindowNode
    LegoFactory<WindowNode, QString>::instance()->registerLego(QString("WindowNode"), new WindowNode);
    // Register WindowDialog
    LegoFactory<WindowDialog, QString>::instance()->registerLego(QString("WindowDialog"), new WindowDialog);

    // Register Door
    LegoFactory<Door, QString>::instance()->registerLego(QString("Door"), new Door);
    // Register DoorNode
    LegoFactory<DoorNode, QString>::instance()->registerLego(QString("DoorNode"), new DoorNode);
    // Register DoorDialog
    LegoFactory<DoorDialog, QString>::instance()->registerLego(QString("DoorDialog"), new DoorDialog);

    // Register FromFile
    LegoFactory<FromFile, QString>::instance()->registerLego(QString("FromFile"), new FromFile);
    // Register FromFileNode
    LegoFactory<FromFileNode, QString>::instance()->registerLego(QString("FromFileNode"), new FromFileNode);
    // Register FromFileDialog
    LegoFactory<FromFileDialog, QString>::instance()->registerLego(QString("FromFileDialog"), new FromFileDialog);

    // Register Wheel
    LegoFactory<Wheel, QString>::instance()->registerLego(QString("Wheel"), new Wheel);
    // Register WheelNode
    LegoFactory<WheelNode, QString>::instance()->registerLego(QString("WheelNode"), new WheelNode);
    // Register WheelDialog
    LegoFactory<WheelDialog, QString>::instance()->registerLego(QString("WheelDialog"), new WheelDialog);

    // Register Character
    LegoFactory<Character, QString>::instance()->registerLego(QString("Character"), new Character);
    // Register CharacterNode
    LegoFactory<CharacterNode, QString>::instance()->registerLego(QString("CharacterNode"), new CharacterNode);
    // Register CharacterDialog
    LegoFactory<CharacterDialog, QString>::instance()->registerLego(QString("CharacterDialog"), new CharacterDialog);

    // Register FrontShip
    LegoFactory<FrontShip, QString>::instance()->registerLego(QString("FrontShip"), new FrontShip);
    // Register FrontShipNode
    LegoFactory<FrontShipNode, QString>::instance()->registerLego(QString("FrontShipNode"), new FrontShipNode);
    // Register FrontShipDialog
    LegoFactory<FrontShipDialog, QString>::instance()->registerLego(QString("FrontShipDialog"), new FrontShipDialog);

    // ENREGISTRER ICI LES AUTRES CLASSES DE PIECE LEGO QUE L'ON CREERA
}

void MainWindow::initPreview(void) {
    // Create matrix transform
    _currMatTrans = new osg::MatrixTransform;

    // Create a 4x2 red classic brick by default
    _currLego = LegoFactory<Brick, QString>::instance()->create("Brick");
    static_cast<Brick*>(_currLego.get())->setColor(QColor(Qt::red));
    static_cast<Brick*>(_currLego.get())->setWidth(2);
    static_cast<Brick*>(_currLego.get())->setLength(4);

    // Create associated brick geode
    _currLegoNode = LegoFactory<BrickNode, QString>::instance()->create("BrickNode");
    _currLegoNode->setLego(_currLego);
    _currLegoNode->createGeode();

    _currMatTrans->addChild(_currLegoNode);

    // Create root node and add brick geode
    _scene = new osg::Group;
    _scene->addChild(_currMatTrans);
}

void MainWindow::initDialogs(void) {
    // BrickDialog
    if (BrickDialog* brickDialog = dynamic_cast<BrickDialog*>(LegoFactory<BrickDialog, QString>::instance()->create("BrickDialog"))) {
        brickDialog->initLego(_currLego);
        brickDialog->initLegoNode(_currLegoNode);
        _legoDialog << brickDialog;
    } else
        qDebug() << "Cannot create BrickDialog in MainWindow::initDialogs";

    // CornerDialog
    if (CornerDialog* cornerDialog = dynamic_cast<CornerDialog*>(LegoFactory<CornerDialog, QString>::instance()->create("CornerDialog")))
        _legoDialog << cornerDialog;
    else
        qDebug() << "Cannot create CornerDialog in MainWindow::initDialogs";

    // TileDialog
    if (TileDialog* tileDialog = dynamic_cast<TileDialog*>(LegoFactory<TileDialog, QString>::instance()->create("TileDialog")))
        _legoDialog << tileDialog;
    else
        qDebug() << "Cannot create TileDialog in MainWindow::initDialogs";

    // RoadDialog
    if (RoadDialog* roadDialog = dynamic_cast<RoadDialog*>(LegoFactory<RoadDialog, QString>::instance()->create("RoadDialog")))
        _legoDialog << roadDialog;
    else
        qDebug() << "Cannot create RoadDialog in MainWindow::initDialogs";

    // WindowDialog
    if (WindowDialog* windowDialog = dynamic_cast<WindowDialog*>(LegoFactory<WindowDialog, QString>::instance()->create("WindowDialog")))
        _legoDialog << windowDialog;
    else
        qDebug() << "Cannot create WindowDialog in MainWindow::initDialogs";

    // DoorDialog
    if (DoorDialog* doorDialog = dynamic_cast<DoorDialog*>(LegoFactory<DoorDialog, QString>::instance()->create("DoorDialog")))
        _legoDialog << doorDialog;
    else
        qDebug() << "Cannot create DoorDialog in MainWindow::initDialogs";

    // WheelDialog
    if (WheelDialog* wheelDialog = dynamic_cast<WheelDialog*>(LegoFactory<WheelDialog, QString>::instance()->create("WheelDialog")))
        _legoDialog << wheelDialog;
    else
        qDebug() << "Cannot create WheelDialog in MainWindow::initDialogs";

    // CharacterDialog
    if (CharacterDialog* characterDialog = dynamic_cast<CharacterDialog*>(LegoFactory<CharacterDialog, QString>::instance()->create("CharacterDialog")))
        _legoDialog << characterDialog;
    else
        qDebug() << "Cannot create CharacterDialog in MainWindow::initDialogs";

    // FrontShipDialog
    if (FrontShipDialog* frontShipDialog = dynamic_cast<FrontShipDialog*>(LegoFactory<FrontShipDialog, QString>::instance()->create("FrontShipDialog")))
        _legoDialog << frontShipDialog;
    else
        qDebug() << "Cannot create FrontShipDialog in MainWindow::initDialogs";

    for (int k = 1; k < _legoDialog.size(); k++) {
        _legoDialog.at(k)->setVisible(false);
    }
}



// ////////////////////////////////////
// Create Param Dock Widget
// ////////////////////////////////////
void MainWindow::createParamsDock(void) {
    // ComboBox choose your brick
    _shapeComboBox = new QComboBox(this);
    QStringList brickForms;
    brickForms << "Brick" << "Corner" << "Tile" << "Road" << "Window" << "Door" << "Wheel" << "Character" << "FrontShip";
    _shapeComboBox->addItems(brickForms);
    _shapeComboBox->setFixedWidth(100);
    QFormLayout* shapeLayout = new QFormLayout;
    shapeLayout->addRow("LEGO:", _shapeComboBox);

    // Brick Preview
    QFrame* previewFrame = new QFrame(this);
    previewFrame->setFixedSize(250, 250);
    // Create osg viewer widget that displays bricks
    _brickViewer = new ViewerWidget;
    _brickViewer->initView();
    _brickViewer->initManipulators();
    _brickViewer->changeCamera(ViewerWidget::createCamera(osg::Vec4(.1, .1, .1, 1.), 0, 0, 100, 100));
    _brickViewer->changeScene(_scene.get());
    _brickViewer->initWidget();
    QVBoxLayout* previewLayout = new QVBoxLayout;
    previewLayout->addWidget(_brickViewer);
    previewFrame->setLayout(previewLayout);

    // Color Button
    _colorButton = new QPushButton("", this);
    _colorButton->setIcon(QIcon("../LEGO_CREATOR/IMG/icons/color.png"));
    _colorButton->setFixedWidth(40);
    _colorButton->setFocusPolicy(Qt::NoFocus);
    _colorButton->setObjectName("DockWidgetButton");

    // CreateButton
    _createButton = new QPushButton("", this);
    _createButton->setIcon(QIcon("../LEGO_CREATOR/IMG/icons/createLego.png"));
    _createButton->setFixedWidth(40);
    _createButton->setFocusPolicy(Qt::NoFocus);
    _createButton->setObjectName("DockWidgetButton");

    // Buttons Layout
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(_colorButton);
    buttonsLayout->addWidget(_createButton);

    // Global shape Layout
    QHBoxLayout* globalShapeLayout = new QHBoxLayout;
    globalShapeLayout->addLayout(shapeLayout);
    globalShapeLayout->addLayout(buttonsLayout);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(previewFrame);
    mainLayout->addLayout(globalShapeLayout);
    for (int k = 0; k < _legoDialog.size(); k++)
        mainLayout->addWidget(_legoDialog.at(k));
    mainLayout->setAlignment(Qt::AlignTop);

    // Params Widget
    _paramsWidget = new QWidget(this);
    _paramsWidget->setLayout(mainLayout);
    // Set object name to style it
    _paramsWidget->setObjectName("ParamsWidget");

    // Params Tab Widget
    _paramsTabWidget = new QTabWidget(this);
    _paramsTabWidget->addTab(_paramsWidget, "Bricks");
    _paramsTabWidget->addTab(_undoView, "Commands");
    // No focus on tab, it's ugly
    _paramsTabWidget->setFocusPolicy(Qt::NoFocus);

    // Right Dock
    _paramsDock = new QDockWidget("Create your brick", this);
    _paramsDock->setFixedSize(275, 500);
    addDockWidget(Qt::RightDockWidgetArea, _paramsDock);
    _paramsDock->setWidget(_paramsTabWidget);
    _paramsDock->setAllowedAreas(Qt::RightDockWidgetArea);
    _paramsDock->setFloating(true);
    _paramsDock->move(50, 200);
}

void MainWindow::createScene(void) {
    // Scene frame, contains LEGO bricks
    _sceneFrame = new QFrame(this);
    _sceneFrame->setFixedSize(1440, 770);

    // Get background color defined within settings
    QColor color;
    if (_settings.childKeys().contains("ViewerColor")) {
        color = _settings.value("ViewerColor").value<QColor>();
    } else {
        color = _settings.value("DefaultViewerColor").value<QColor>();
    }

    double r = static_cast<double>(color.red());
    double g = static_cast<double>(color.green());
    double b = static_cast<double>(color.blue());

    // Scene viewer
    _sceneViewer = new ViewerWidget;
    _sceneViewer->initView();
    _sceneViewer->initManipulators();
    _sceneViewer->changeCamera(ViewerWidget::createCamera(osg::Vec4(r/255.0, g/255.0, b/255.0, 1.), 0.0, 0.0, 1440.0, 770.0));
    _sceneViewer->changeScene(_world.getScene().get());
    _sceneViewer->initWidget();

    // Layout
    QVBoxLayout* previewLayout = new QVBoxLayout;
    previewLayout->addWidget(_sceneViewer);
    _sceneFrame->setLayout(previewLayout);
}

void MainWindow::initTraffic(void) {
   // Create a new traffic
    _traffic = new Traffic;

    // Add traffic root to the scene
    _world.getScene()->addChild(_traffic->getRoot());
}

// Open the color dialog to change our LEGO color
void MainWindow::browseColor() {
    _legoColor = QColorDialog::getColor(Qt::red, this);
    _currLego->setColor(_legoColor);
    _currLegoNode->createGeode();
}

void MainWindow::chooseDialog(int dialogIndex) {
    // Dialogs are created, but only the current LEGO dialog is  visible
    for (int k = 0; k < _legoDialog.size(); k++) {
        if (k == dialogIndex)
            _legoDialog.at(k)->setVisible(true);
        else
            _legoDialog.at(k)->setVisible(false);
    }

    // Create dialog, according to
    switch (dialogIndex) {
    // Brick dialog
    case 0:
        if ((_currLego = dynamic_cast<Brick*>(LegoFactory<Brick, QString>::instance()->create("Brick")))) {
            BrickDialog* dialog = static_cast<BrickDialog*>(_legoDialog.at(dialogIndex));
            Brick* lego = static_cast<Brick*>(_currLego.get());
            lego->setColor(_legoColor);
            lego->setWidth(dialog->getWidth());
            lego->setLength(dialog->getLength());
        } else {
            qDebug() << "Cannot cast in Brick* within MainWindow::chooseDialog";
        }
        if (!(_currLegoNode = dynamic_cast<BrickNode*>(LegoFactory<BrickNode, QString>::instance()->create("BrickNode"))))
            qDebug() << "Cannot cast in BrickNode* within MainWindow::chooseDialog";
        break;
    // Corner dialog
    case 1:
        if ((_currLego = dynamic_cast<Corner*>(LegoFactory<Corner, QString>::instance()->create("Corner")))) {
            Corner* lego = static_cast<Corner*>(_currLego.get());
            lego->setColor(_legoColor);
        } else {
            qDebug() << "Cannot cast in Corner* within MainWindow::chooseDialog";
        }
        if (!(_currLegoNode = dynamic_cast<CornerNode*>(LegoFactory<CornerNode, QString>::instance()->create("CornerNode"))))
            qDebug() << "Cannot cast in CornerNode* within MainWindow::chooseDialog";
        break;
    // Tile dialog
    case 2:
        if ((_currLego = dynamic_cast<Tile*>(LegoFactory<Tile, QString>::instance()->create("Tile")))) {
            TileDialog* dialog = static_cast<TileDialog*>(_legoDialog.at(dialogIndex));
            Tile* lego = static_cast<Tile*>(_currLego.get());
            lego->setColor(_legoColor);
            lego->setWidth(dialog->getWidth());
            lego->setLength(dialog->getLength());
        } else {
            qDebug() << "Cannot cast in Tile* within MainWindow::chooseDialog";
        }
        if (!(_currLegoNode = dynamic_cast<TileNode*>(LegoFactory<TileNode, QString>::instance()->create("TileNode"))))
            qDebug() << "Cannot cast in TileNode* within MainWindow::chooseDialog";
        break;
    // Road dialog
    case 3:
        if ((_currLego = dynamic_cast<Road*>(LegoFactory<Road, QString>::instance()->create("Road")))) {
            Road* lego = static_cast<Road*>(_currLego.get());
            lego->setColor(QColor(0, 112, 44));
        } else {
            qDebug() << "Cannot cast in Road* within MainWindow::chooseDialog";
        }
        if (!(_currLegoNode = dynamic_cast<RoadNode*>(LegoFactory<RoadNode, QString>::instance()->create("RoadNode"))))
            qDebug() << "Cannot cast in RoadNode* within MainWindow::chooseDialog";
        break;
    // Window dialog
    case 4:
        if ((_currLego = dynamic_cast<Window*>(LegoFactory<Window, QString>::instance()->create("Window")))) {
            Window* lego = static_cast<Window*>(_currLego.get());
            lego->setColor(_legoColor);
        } else {
            qDebug() << "Cannot cast in Window* within MainWindow::chooseDialog";
        }
        if (!(_currLegoNode = dynamic_cast<WindowNode*>(LegoFactory<WindowNode, QString>::instance()->create("WindowNode"))))
            qDebug() << "Cannot cast in WindowNode* within MainWindow::chooseDialog";
        break;
    case 5:
        if ((_currLego = dynamic_cast<Door*>(LegoFactory<Door, QString>::instance()->create("Door")))) {
            Door* lego = static_cast<Door*>(_currLego.get());
            lego->setColor(_legoColor);
        } else {
            qDebug() << "Cannot cast in Door* within MainWindow::chooseDialog";
        }
        if (!(_currLegoNode = dynamic_cast<DoorNode*>(LegoFactory<DoorNode, QString>::instance()->create("DoorNode"))))
            qDebug() << "Cannot cast in DoorNode* within MainWindow::chooseDialog";
        break;
    case 6:
        if ((_currLego = dynamic_cast<Wheel*>(LegoFactory<Wheel, QString>::instance()->create("Wheel")))) {
            Wheel* lego = static_cast<Wheel*>(_currLego.get());
            lego->setColor(_legoColor);
        } else {
            qDebug() << "Cannot cast in Wheel* within MainWindow::chooseDialog";
        }
        if (!(_currLegoNode = dynamic_cast<WheelNode*>(LegoFactory<WheelNode, QString>::instance()->create("WheelNode"))))
            qDebug() << "Cannot cast in WheelNode* within MainWindow::chooseDialog";

        break;
    // Character dialog
    case 7:
        if ((_currLego = dynamic_cast<Character*>(LegoFactory<Character, QString>::instance()->create("Character")))) {
            Character* lego = static_cast<Character*>(_currLego.get());
            lego->setColor(QColor(0, 112, 44));
        } else {
            qDebug() << "Cannot cast in Character* within MainWindow::chooseDialog";
        }
        if (!(_currLegoNode = dynamic_cast<CharacterNode*>(LegoFactory<CharacterNode, QString>::instance()->create("CharacterNode"))))
            qDebug() << "Cannot cast in CharacterNode* within MainWindow::chooseDialog";
        break;
    // FrontShip dialog
    case 8:
        if ((_currLego = dynamic_cast<FrontShip*>(LegoFactory<FrontShip, QString>::instance()->create("FrontShip")))) {
            FrontShip* lego = static_cast<FrontShip*>(_currLego.get());
            lego->setColor(_legoColor);
        } else {
            qDebug() << "Cannot cast in FrontShip* within MainWindow::chooseDialog";
        }
        if (!(_currLegoNode = dynamic_cast<FrontShipNode*>(LegoFactory<FrontShipNode, QString>::instance()->create("FrontShipNode"))))
            qDebug() << "Cannot cast in FrontShipNode* within MainWindow::chooseDialog";
        break;
    }

    // Set current objects
    _currMatTrans = new osg::MatrixTransform;
    _currLegoNode->setLego(_currLego);
    _currLegoNode->createGeode();
    _currMatTrans->addChild(_currLegoNode);
    _scene->setChild(0, _currMatTrans.get());

    // Initialize current objects
    _legoDialog.at(dialogIndex)->initLego(_currLego);
    _legoDialog.at(dialogIndex)->initLegoNode(_currLegoNode.get());
    _legoDialog.at(dialogIndex)->reInitComboBox();
}

void MainWindow::legoUpdated(LegoNode* legoNode) {
    // Setting new current objects
    _currLegoNode = legoNode;
    _currLego = _currLegoNode->getLego();

    // The file has changed
    _saved = false;
}

void MainWindow::createLego(void) {
    // z spin box is disabled when roads are created, so first, it has to be enabled again
    _zTransSpinBox->setEnabled(true);

    // When a new brick has been created, users have to fit it before being able to create another one
    freezeCreate();

    // Create current Matrix Transform
    _currMatTrans = new osg::MatrixTransform;
    _currMatTrans->addChild(_currLegoNode);

    // Create AddCommand to handle undo/redo action
    QUndoCommand* addLegoCommand = new AddLegoCommand(&_world, _currLegoNode);
    _undoStack->push(addLegoCommand);

    // Reinit dialog, because a new brick is always put at the origin of the scene
    _xTransSpinBox->setValue(0);
    _yTransSpinBox->setValue(0);
    _zTransSpinBox->setValue(0);

    // Check whether the LEGO is a road, and disable z spin box after having set it at minHeight world specification
    if (dynamic_cast<Road*>(_currLego.get())) {
        _zTransSpinBox->setValue(World::minHeight);
        _zTransSpinBox->setEnabled(false);
    }

    // The scene has changed
    _saved = false;

    _sceneViewer->getCamera()->setViewMatrixAsLookAt(osg::Vec3(0.0f, -5.0f, 5.0f), osg::Vec3(), osg::Vec3(0.0f, 1.0f, 1.0f));
}

void MainWindow::fitLego(void) {
    // For the moment, users can fit LEGO wherever they want. So it only disables the move GUI and enables the construction GUI again.
    freezeFit();

    // The file has changed
    _saved = false;
}

void MainWindow::deleteLego(void) {
//    // Get current matrix index whithin world scene
//    osg::Group* root = _world.getScene();
//    for (unsigned int k = 0; k < root->getNumChildren(); k++) {
//        qDebug() << QString::fromStdString(root->getChild(k)->getName());
//    }
//    qDebug() << QString::number(root->getChildIndex(_currMatTrans.get()));
//    //qDebug() << QString::fromStdString(root->getChild(root->getChildIndex(_currMatTrans.get()))->getName());
//    qDebug() << QString::fromStdString(_currMatTrans->getName());
//    std::string matrixName = root->getChild(root->getChildIndex(_currMatTrans.get()))->getName();
//    qDebug() << QString::fromStdString(matrixName);

//    // Create DeleteCommand to handle undo/redo action
//    QUndoCommand* delLegoCommand = new DeleteLegoCommand(&_world, _currLegoNode, matrixName);
//    _undoStack->push(delLegoCommand);

    // Delete brick
    _world.deleteLego();

    // Users can create another brick
    freezeFit();

    // The file has changed
    _saved = false;
}

void MainWindow::translate(int) {
    // As soon as users have change one of the x, y, or z brick coordinate, we translate it
    _world.translationXYZ(_xTransSpinBox->text().toInt(), _yTransSpinBox->text().toInt(), _zTransSpinBox->text().toInt());

    // The file has changed
    _saved = false;
}

void MainWindow::rotateLeft(void) {
    // Rotate clock wise
    _world.rotation(true);

    // The file has changed
    _saved = false;
}

void MainWindow::rotateRight(void) {
    // Rotate counter clock wise
    _world.rotation(false);
}

void MainWindow::writeFile(const QString& fileName) {
    // Try to write the scene in fileName file
    if (osgDB::writeNodeFile(*(_world.getScene().get()), fileName.toStdString())) {
        _saved = true;
        _alreadySaved = true;
    // Fail to write
    } else {
        QMessageBox::critical(this, "The document has not been saved", "An error occured while tempting to save your construction.");
    }
}

void MainWindow::openFromFile(const QString& fileName) {
    if ((_currLego = dynamic_cast<FromFile*>(LegoFactory<FromFile, QString>::instance()->create("FromFile")))) {
        if (_currLegoNode = dynamic_cast<FromFileNode*>(LegoFactory<FromFileNode, QString>::instance()->create("FromFileNode"))) {
            _currLegoNode->setLego(_currLego);
            FromFile* fromFile = static_cast<FromFile*>(_currLego.get());
            fromFile->setFileName(fileName);
            _currLegoNode->createGeode();

            createLego();
        } else
            qDebug() << "Cannot cast in FromFileNode* within MainWindow::openFromFile";
    } else
        qDebug() << "Cannot cast in FromFile* within MainWindow::openFromFile";

    // Force dialog to refresh and avoid segfault
    int currDialogIndex = _shapeComboBox->currentIndex();
    int nextDialogIndex = (currDialogIndex+1)%(_shapeComboBox->count());
    _shapeComboBox->setCurrentIndex(nextDialogIndex);
    _shapeComboBox->setCurrentIndex(currDialogIndex);
}

void MainWindow::chooseRoad(int i, int j, int width, int length, bool roadTop, bool roadRight) {
    // Create a road part, according to its top and left neighbour.
    // The algorithm could (might?) be improved

    // The new road
    osg::ref_ptr<Road> road = new Road;

    // Road might have to be rotated several times
    int nbRotations = 0;
    // Road type will be get randomly
    int roadType = -1;

    // There are a road coming from the top and the left
    if (roadTop && roadRight) {
        // There are four roads possible in this particular configuration
        roadType = round((rand()/(double)RAND_MAX) * 3);
        switch (roadType) {
        case 0:
            road->setRoadType(Road::curve);
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 0;
            _roadPath[i][j][2] = 0;
            _roadPath[i][j][3] = 1;
            break;
        case 1:
            road->setRoadType(Road::intersection);
            nbRotations = 2;
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 0;
            _roadPath[i][j][3] = 1;
            break;
        case 2:
            road->setRoadType(Road::intersection);
            nbRotations = 1;
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 0;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 1;
            break;
        case 3:
            road->setRoadType(Road::cross);
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 1;
            break;
        }
    // There is only a road coming from the top
    } else if (roadTop && !roadRight) {
        // There are three roads possible in this particular configuration
        roadType = round((rand()/(double)RAND_MAX) * 2);
        switch (roadType) {
        case 0:
            road->setRoadType(Road::curve);
            nbRotations = 3;
            _roadPath[i][j][0] = 0;
            _roadPath[i][j][1] = 0;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 1;
            break;
        case 1:
            road->setRoadType(Road::straight);
            _roadPath[i][j][0] = 0;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 0;
            _roadPath[i][j][3] = 1;
            break;
        case 2:
            road->setRoadType(Road::intersection);
            _roadPath[i][j][0] = 0;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 1;
            break;
        }
    // There is only a road coming from the left
    } else if (!roadTop && roadRight) {
        // There are three roads possible in this particular configuration
        roadType = round((rand()/(double)RAND_MAX) * 2);
        switch (roadType) {
        case 0:
            road->setRoadType(Road::curve);
            nbRotations = 1;
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 0;
            _roadPath[i][j][3] = 0;
            break;
        case 1:
            road->setRoadType(Road::straight);
            nbRotations = 1;
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 0;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 0;
            break;
        case 2:
            road->setRoadType(Road::intersection);
            nbRotations = 3;
            _roadPath[i][j][0] = 1;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 0;
            break;
        }
    // There is no road coming from the top nor the left
    } else {
        roadType = round((rand()/(double)RAND_MAX));
        // There are two roads possible in this particular configuration
        switch (roadType) {
        case 0:
            road->setRoadType(Road::curve);
            nbRotations = 2;
            _roadPath[i][j][0] = 0;
            _roadPath[i][j][1] = 1;
            _roadPath[i][j][2] = 1;
            _roadPath[i][j][3] = 0;
            break;
        case 1:
            road->setRoadType(Road::none);
            _roadPath[i][j][0] = 0;
            _roadPath[i][j][1] = 0;
            _roadPath[i][j][2] = 0;
            _roadPath[i][j][3] = 0;
            break;
        }
    }

    // Create the road geode thanks to the brand new road
    osg::ref_ptr<RoadNode> roadNode = new RoadNode(road);

    // Create matrix transform
    osg::ref_ptr<osg::MatrixTransform> matTrans = new osg::MatrixTransform;
    // Add road geode
    matTrans->addChild(roadNode);

    // Add brick to the world
    _world.addBrick(roadNode.get(), road.get());
    // Translate the road
    _world.translation(-32*floor(length/2)-16 + 32*i, -32*floor(width/2)+16 + 32*j, World::minHeight);
    // Apply rotation
    for (int k = 0; k < nbRotations; k++) {
        _world.rotation(true);
    }
}

void MainWindow::generateRoad(void) {
    // Call the road dialog to ask users the width and length of their road circuit
    GenerateRoadWindow* roadWindow = new GenerateRoadWindow(this);

    // If users entered OK button
    if (roadWindow->exec() == QDialog::Accepted) {
        // Get width and length entered by users
        int width = roadWindow->getWidth();
        int length = roadWindow->getLength();

        // 4 sides: left top right bottom; true = road, false = no road.
        _roadPath.clear();
        // Record four bool values, one for each road, in order
        _roadPath = QVector<QVector<QVector<bool> > >(width, QVector<QVector<bool> >(length, QVector <bool>(4, false)));

        // Bool values to know wheter there are roads coming from top and left road
        bool roadTop;
        bool roadLeft;

        // Create road iteratively, from a corner
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < length; j++) {
                // First road, road may or may not come from top or left
                if (i == 0 && j == 0) {
                    roadTop = round((rand()/(double)RAND_MAX));
                    roadLeft = round((rand()/(double)RAND_MAX));
                // On left side, top road coming depends on previous road, road may or may not come from left
                } else if (i == 0 && j != 0) {
                    roadTop = _roadPath[i][j-1][1];
                    roadLeft = round((rand()/(double)RAND_MAX));
                // On top side, left road coming depends on left road previously created, road may or may not come from top
                } else if (i != 0 && j == 0) {
                    roadTop = round((rand()/(double)RAND_MAX));
                    roadLeft = _roadPath[i-1][j][2];
                // Wherever else, roads coming from top and left depend on previously added roads
                } else {
                    roadTop = _roadPath[i][j-1][1];
                    roadLeft = _roadPath[i-1][j][2];
                }
                // Create a random road, according to roads coming or not from top and left
                chooseRoad(i, j, width, length, roadTop, roadLeft);
            }
        }
    }

    // The file has changed
    _saved = false;
}

void MainWindow::generateHouse(void) {
    // Load saved house
    openFromFile("../LEGO_CREATOR/OSG/house.osg");
}

void MainWindow::generateCity(void) {
    // Load saved city
    openFromFile("../LEGO_CREATOR/OSG/town.osg");
}

void MainWindow::generateFormule1(void) {
    // Load saved formule1
    openFromFile("../LEGO_CREATOR/OSG/formule1.osg");
}

void MainWindow::switchTraffic(bool b) {
    // Switch traffic to on or off, according to traffic toggle action state
    _traffic->switchTraffic(b);
}

void MainWindow::eraseScene(void) {
    // remove everything from scene
    _world.getScene()->removeChildren(0, _world.getScene()->getNumChildren());

    // There is no file associated anymore
    _settings.setValue("FileName", "");

    // No object = no modification, the scene is considered as saved...
    _saved = true;

    // ... and has never been saved before
    _alreadySaved = false;

    // We can create LEGO pieces, and therefore not move/fit them
    freezeFit();

    // Traffic has been removed, we regenerate it
    _traffic->createTraffic();
    _world.getScene()->addChild(_traffic->getRoot());

    // To avoid any bug, traffic toggle action is set to off
    _viewTrafficAction->setChecked(false);

    // Guide lines has been removed too, we recreate it
    _world.createGuideLines();

    // Empty undo/redo stack, ofc
    _undoStack->clear();
}

void MainWindow::newFile(void) {
    // File modified?
    if (!_saved) {
        int ret = QMessageBox::warning(this, "The document has been modified", "Do you want to save your changes?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);

        switch (ret) {
        // Users want to save
        case QMessageBox::Save:
            saveFile();
            // If users canceled saving operation, we assume they wanted to cancel new file action too
            if (_saved)
                eraseScene();
            break;
        // Users don't give a ****
        case QMessageBox::Discard:
            eraseScene();
            break;
        // Users change their mind
        case QMessageBox::Cancel:
            break;
        }
    } else {
        // File saved? let's go!
        eraseScene();
    }
}

void MainWindow::openFile(void) {
    // Create a poor dialog, because rendering in the main thread avoid fancy open dialog to be painted
    QDialog* dialog = new QDialog(this);

    // Get the open path
    QString openPath = _settings.value("OpenPath").toString();
    if (!openPath.endsWith('/'))
        openPath += "/";

    // Get all file in directory
    QDir directory(openPath);
    QStringList allFiles = directory.entryList(QDir::Files | QDir::NoDotAndDotDot);
    QComboBox* filesComboBox = new QComboBox;
    // Add file names to combo box
    filesComboBox->addItems(allFiles);
    QFormLayout* fileNameLayout = new QFormLayout;
    fileNameLayout->addRow("File name:", filesComboBox);

    // Create cancel button
    QPushButton* cancelButton = new QPushButton("Cancel", dialog);
    connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(reject()));

    // Create save button
    QPushButton* saveButton = new QPushButton("Open", dialog);
    connect(saveButton, SIGNAL(clicked()), dialog, SLOT(accept()));

    // Button layout
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(saveButton);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(fileNameLayout);
    mainLayout->addLayout(buttonsLayout);

    // Set layout
    dialog->setLayout(mainLayout);

    // Get data from dialog
    if (dialog->exec() == QDialog::Accepted) {
        QString fileName = filesComboBox->currentText();
        if (fileName.split(".").last() != "osg") {
            QMessageBox::critical(this, "Not an OSG file", "The selected file is not an OSG file.\nPlease retry with a correct file.");
        } else {
            openFromFile(openPath+fileName);
        }
    }

    // Here is the old code that create a beautiful but unusable openFile pop up
    // Maybe one day I'll be able to render OSG scene in another thread and use this snipet
//    // Get open path
//    QString openPath = _settings.value("SavePath").toString();

//    // Open dialog because users know what they want to open
//    QString fileName = QFileDialog::getOpenFileName(this, "Open object", openPath);

//    // Future node, or not
//    osg::ref_ptr<osg::Node> newLego = NULL;

//    // If users didn't cancel
//    if (fileName != "") {
//        // Read file to create a node
//        newLego = osgDB::readNodeFile(fileName.toStdString());
//        // If the new Lego has been correctly got
//        if (newLego) {
//            _world.getScene()->addChild(newLego);
//            _saved = false;
//        // Else, users have no luck, but they can retry!
//        } else {
//            int ret = QMessageBox::critical(this, "Your file could not have been read", "An error occured while tempting to open your file. =(\nPlease retry (crossing fingers), or abort if you want to give up.", QMessageBox::Retry | QMessageBox::Abort, QMessageBox::Retry);
//            // If users are fighters
//            if (ret == QMessageBox::Retry)
//                openFile();
//        }
//    }
}

void MainWindow::saveFile(void) {
    // File modified?
    if (!_saved) {
        // First time users save?
        if (!_alreadySaved) {
            saveAsFile();
        // Not the first time? so we already know the file to write data
        } else {
            // Get the current save path
            QString savePath = _settings.value("SavePath").toString();
            if (!savePath.endsWith('/'))
                savePath += "/";

            // Get the current file name, recorded whithin QSettings
            QString fileName = _settings.value("FileName").toString();

            // Time to really save the OSG file
            writeFile(savePath+fileName);
        }
    }
}

void MainWindow::checkExistence(QString fileName) {
    // Get save path
    QString savePath = _settings.value("SavePath").toString();
    if (!savePath.endsWith('/'))
        savePath += "/";

    // File path
    QFile file(savePath+fileName);

    // If users have not put osg, we check that one too!
    QFile fileWhisoutOSG(savePath+fileName+".osg");

    // Emit signal exist
    emit fileAlreadyExists(file.exists() || fileWhisoutOSG.exists());
}

void MainWindow::saveAsFile(void) {
    // Create a poor dialog, because rendering in the main thread avoid fancy save dialog to be painted
    QDialog* dialog = new QDialog(this);

    // Label to be displayed when users enter an already existing file name
    QLabel* warningFileAlreadyExists = new QLabel("Warning: the specified file already exists!");
    warningFileAlreadyExists->setStyleSheet("color: #FF7700");
    warningFileAlreadyExists->setVisible(false);

    // Create a line edit for file name input
    QLineEdit* fileNameLineEdit = new QLineEdit(this);
    QFormLayout* fileNameLayout = new QFormLayout;
    fileNameLayout->addRow("File name:", fileNameLineEdit);

    // Create cancel button
    QPushButton* cancelButton = new QPushButton("Cancel", dialog);
    connect(cancelButton, SIGNAL(clicked()), dialog, SLOT(reject()));

    // Create save button
    QPushButton* saveButton = new QPushButton("Save", dialog);
    connect(saveButton, SIGNAL(clicked()), dialog, SLOT(accept()));

    // Buttons layout
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(cancelButton);
    buttonsLayout->addWidget(saveButton);
    buttonsLayout->setAlignment(Qt::AlignTop);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(warningFileAlreadyExists);
    mainLayout->addLayout(fileNameLayout);
    mainLayout->addLayout(buttonsLayout);

    // Connections to handle warning message on already exist file name
    connect(fileNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(checkExistence(QString)));
    connect(this, SIGNAL(fileAlreadyExists(bool)), warningFileAlreadyExists, SLOT(setVisible(bool)));

    // Set main layout
    dialog->setLayout(mainLayout);

    // Get data from dialog
    if (dialog->exec() == QDialog::Accepted) {
        // Get the file name entered by users
        QString fileName = fileNameLineEdit->text();
        if (fileName.split(".").last() != "osg") {
            fileName.append(".osg");
        }

        // Get the current save path
        QString savePath = _settings.value("SavePath").toString();
        if (!savePath.endsWith('/'))
            savePath += "/";

        // Set current file name
        _settings.setValue("FileName", fileName);

        // Write scene into OSG file
        writeFile(savePath+fileName);
    // If users canceled,
    } else {
        _saved = false;
        _alreadySaved = false;
    }

    // Same issue: rendering OSG in the main thread avoid to paint fancy saveFile dialog
    // But I keep that code for the day I can fix it
//    // Get the save path, according to last users directory visit
//    QString savePath = _settings.value("SavePath").toString();

//    // Pop up to ask user where they want to save their file
//    QString fileName = QFileDialog::getSaveFileName(this, "Save your construction", savePath+"/untitled.osg", "OSG Files (*.osg)");

//    // If users have not canceled
//    if (fileName != "") {
//        // Get the directory in order to record it and open it directly next time users want to save
//        _settings.setValue("SavePath", QDir(fileName).absolutePath());
//        // If users missed to specify .osg at the end, we do for them because we are so nice!
//        if (fileName.split(".").last() != "osg") {
//            fileName.append(".osg");
//        }
//        _settings.setValue("FileName", fileName);
//        // Time to really save the osg file
//        writeFile(fileName);
//    }
}

void MainWindow::quitSoft(void) {
    // File modified?
    if (!_saved) {
        int ret = QMessageBox::warning(this, "The document has been modified", "Do you want to save your changes?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);

        switch (ret) {
        // Users want to save
        case 0:
            saveFile();
            qApp->quit();
            break;
        // Users don't give a ****
        case 1:
            qApp->quit();
            break;
        // Users change their mind
        case 2:
            break;
        }
    } else {
        // File saved? let's go!
        qApp->quit();
    }
}

void MainWindow::openSettings(void) {
    _settingsDialog->exec();
}

void MainWindow::updateWorldGrid(void) {
    _world.createGuideLines();
}

void MainWindow::viewerColorUpdate(QColor color) {
    double r = static_cast<double>(color.red());
    double g = static_cast<double>(color.green());
    double b = static_cast<double>(color.blue());

    // Scene viewer
    _sceneViewer->getCamera()->setClearColor(osg::Vec4(r/255.0, g/255.0, b/255.0, 1.));
}

void MainWindow::freezeFit(void) {
    // Piece has been fit, users can create another one
    _moveToolBar->setEnabled(false);
    _paramsTabWidget->find(_paramsWidget->winId())->setEnabled(true);

    // When fit is frozen, users can generate or open file
    _generateCityAction->setEnabled(true);
    _generateFormule1Action->setEnabled(true);
    _generateHouseAction->setEnabled(true);
    _generateRoadAction->setEnabled(true);
    _openAction->setEnabled(true);
}

void MainWindow::freezeCreate(void) {
    // Piece has been created, users can move/translate and then fit it
    _paramsTabWidget->find(_paramsWidget->winId())->setEnabled(false);
    _moveToolBar->setEnabled(true);

    // When create is frozen, users can't generate nor open file
    _generateCityAction->setEnabled(false);
    _generateFormule1Action->setEnabled(false);
    _generateHouseAction->setEnabled(false);
    _generateRoadAction->setEnabled(false);
    _openAction->setEnabled(false);
}


// ////////////////////////////////////
// Create menus
// ////////////////////////////////////

void MainWindow::createFileMenu(void) {
    // Create File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");

    // Add New sub menu
    _newAction = fileMenu->addAction("&New...");
    _newAction->setShortcut(QKeySequence::New);
    // Set icon
    _newAction->setIcon(QIcon("../LEGO_CREATOR/IMG/icons/newFile.png"));
    // Connect action
    connect(_newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    // Add separator
    fileMenu->addSeparator();

    // Add Open sub menu
    _openAction = fileMenu->addAction("&Open...");
    _openAction->setShortcut(QKeySequence::Open);// Set icon
    _openAction->setIcon(QIcon("../LEGO_CREATOR/IMG/icons/openFile.png"));
    // Connect action
    connect(_openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    // Add separator
    fileMenu->addSeparator();

    // Add Save sub menu
    _saveAction = fileMenu->addAction("&Save");
    _saveAction->setShortcut(QKeySequence::Save);// Set icon
    _saveAction->setIcon(QIcon("../LEGO_CREATOR/IMG/icons/saveFile.png"));
    // Connect action
    connect(_saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    // Add Save as sub menu
    _saveAsAction = fileMenu->addAction("Save &as...");
    _saveAsAction->setShortcut(QKeySequence::SaveAs);
    // Set icon
    _saveAsAction->setIcon(QIcon("../LEGO_CREATOR/IMG/icons/saveAsFile.png"));
    // Connect action
    connect(_saveAsAction, SIGNAL(triggered()), this, SLOT(saveAsFile()));

    // Add separator
    fileMenu->addSeparator();

    // Add Quit sub menu
    _quitAction = fileMenu->addAction("&Quit");
    _quitAction->setShortcut(QKeySequence::Quit);
    // Connect action
    connect(_quitAction, SIGNAL(triggered()), this, SLOT(quitSoft()));
}

void MainWindow::createEditMenu(void) {
    // Create Edit menu
    QMenu* editMenu = menuBar()->addMenu("&Edit");

    // Add Undo action
    _undoAction = _undoStack->createUndoAction(this, "&Undo");
    editMenu->addAction(_undoAction);
    _undoAction->setShortcut(QKeySequence::Undo);
    // Set icon
    _undoAction->setIcon(QIcon("../LEGO_CREATOR/IMG/icons/undo.png"));

    // Add Redo action
    _redoAction = _undoStack->createRedoAction(this, "&Redo");
    editMenu->addAction(_redoAction);
    _redoAction->setShortcut(QKeySequence::Redo);
    // Set icon
    _redoAction->setIcon(QIcon("../LEGO_CREATOR/IMG/icons/redo.png"));

    // Add separator
    editMenu->addSeparator();

    // Add Settings action
    _settingsAction = editMenu->addAction("&Settings...");
    _settingsAction->setShortcut(QKeySequence::Preferences);
    // Connect action
    connect(_settingsAction, SIGNAL(triggered()), this, SLOT(openSettings()));
}

void MainWindow::createGenerateMenu(void) {
    // Create Generate menu
    QMenu* generateMenu = menuBar()->addMenu("&Generate");

    // Add Generate road sub menu
    _generateRoadAction = generateMenu->addAction("Generate &road...");
    _generateRoadAction->setShortcut(QKeySequence("CTRL+SHIFT+R"));
    // Connect action
    connect(_generateRoadAction, SIGNAL(triggered()), this, SLOT(generateRoad()));

    // Add Generate house sub menu
    _generateHouseAction = generateMenu->addAction("Generate &house");
    _generateHouseAction->setShortcut(QKeySequence("CTRL+SHIFT+H"));
    // Connect action
    connect(_generateHouseAction, SIGNAL(triggered()), this, SLOT(generateHouse()));

    // Add Generate city sub menu
    _generateCityAction = generateMenu->addAction("Generate &city...");
    _generateCityAction->setShortcut(QKeySequence("CTRL+SHIFT+C"));
    // Connect action
    connect(_generateCityAction, SIGNAL(triggered()), this, SLOT(generateCity()));

    // Add Generate Formule 1 sub menu
    _generateFormule1Action = generateMenu->addAction("Generate Formule1");
    _generateFormule1Action->setShortcut(QKeySequence("CTRL+SHIFT+F"));
    // Connect action
    connect(_generateFormule1Action, SIGNAL(triggered()), this, SLOT(generateFormule1()));
}

void MainWindow::createTrafficMenu(void) {
    // Create Traffic menu
    QMenu* trafficMenu = menuBar()->addMenu("&Traffic");

    // Add View Traffic sub menu
    _viewTrafficAction = trafficMenu->addAction("Toggle traffic");
    _viewTrafficAction->setShortcut(QKeySequence("CTRL+SHIFT+T"));
    _viewTrafficAction->setCheckable(true);
    _viewTrafficAction->setChecked(false);
    connect(_viewTrafficAction, SIGNAL(toggled(bool)), this, SLOT(switchTraffic(bool)));
}

void MainWindow::createHelpMenu(void) {
    // Create ? menu
    QMenu* helpMenu = menuBar()->addMenu("&?");

    // Add Help sub menu
    _helpAction = helpMenu->addAction("&Help");
    _helpAction->setShortcut(QKeySequence::HelpContents);

    // Add separator
    helpMenu->addSeparator();

    // Add About sub menu
    _aboutAction = helpMenu->addAction("&About");
    _aboutAction->setShortcut(QKeySequence("Alt+F1"));
}

void MainWindow::createToolBar(void) {
    // Create classic tool bar, with new, open, save...
    QToolBar* toolBar = addToolBar("ToolBar");
    toolBar->addAction(_newAction);
    toolBar->addAction(_openAction);
    toolBar->addAction(_saveAction);
    toolBar->addAction(_saveAsAction);
    toolBar->addSeparator();
    toolBar->addAction(_undoAction);
    toolBar->addAction(_redoAction);
    // Set tool bar object name to enable access when set style
    toolBar->setObjectName("ToolBar");
    // This tool bar cannot be moved, resized and so on
    toolBar->setMovable(false);

    // Create Move tool bar
    _moveToolBar = addToolBar("MoveToolBar");

    // Set tool bar object name to enable access when set style
    _moveToolBar->setObjectName("MoveToolBar");

    // Create spacer widget to separate these two tool bars
    QWidget* spacerWidget = new QWidget(this);
    spacerWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    spacerWidget->setVisible(true);
    _moveToolBar->addWidget(spacerWidget);

    // Translation according to x
    _xTransSpinBox = new QSpinBox;
    _xTransSpinBox->setMinimum(World::minLength);
    _xTransSpinBox->setMaximum(World::maxLength);
    _xTransSpinBox->setValue(0);
    // Connect
    connect(_xTransSpinBox, SIGNAL(valueChanged(int)), this, SLOT(translate(int)));
    // Add to tool bar
    _moveToolBar->addWidget(new QLabel("X:", this));
    _moveToolBar->addWidget(_xTransSpinBox);

    // Translation according to y
    _yTransSpinBox = new QSpinBox;
    _yTransSpinBox->setMinimum(World::minWidth);
    _yTransSpinBox->setMaximum(World::maxWidth);
    _yTransSpinBox->setValue(0);
    // Connect
    connect(_yTransSpinBox, SIGNAL(valueChanged(int)), this, SLOT(translate(int)));
    // Add to tool bar
    _moveToolBar->addWidget(new QLabel("Y:", this));
    _moveToolBar->addWidget(_yTransSpinBox);

    // Translation according to z
    _zTransSpinBox = new QSpinBox;
    _zTransSpinBox->setMinimum(World::minHeight);
    _zTransSpinBox->setMaximum(World::maxHeight);
    _zTransSpinBox->setValue(0);
    // Connect
    connect(_zTransSpinBox, SIGNAL(valueChanged(int)), this, SLOT(translate(int)));
    // Add to tool bar
    _moveToolBar->addWidget(new QLabel("Z:", this));
    _moveToolBar->addWidget(_zTransSpinBox);

    // Left Rotation Action
    _leftRotateAction = new QAction(QIcon("../LEGO_CREATOR/IMG/icons/rotationLeft.png"), "", this);
    // Add tool tip to explain
    _leftRotateAction->setToolTip("Left rotation");
    // Connect
    connect(_leftRotateAction, SIGNAL(triggered()), this, SLOT(rotateLeft()));
    // Add to tool bar
    _moveToolBar->addAction(_leftRotateAction);

    // Right Rotation Action
    _rightRotateAction = new QAction(QIcon("../LEGO_CREATOR/IMG/icons/rotationRight.png"), "", this);
    // Add tool tip to explain
    _rightRotateAction->setToolTip("Right rotation");
    // Connect
    connect(_rightRotateAction, SIGNAL(triggered()), this, SLOT(rotateRight()));
    // Add to tool bar
    _moveToolBar->addAction(_rightRotateAction);

    // Fit Action
    _fitAction = new QAction(QIcon("../LEGO_CREATOR/IMG/icons/fitLego.png"), "", this);
    // Set shortcut
    _fitAction->setShortcut(QKeySequence("CTRL+DOWN"));
    // Add tool tip to explain
    _fitAction->setToolTip("Fit the current brick");
    // Connect
    connect(_fitAction, SIGNAL(triggered()), this, SLOT(fitLego()));
    // Add to tool bar
    _moveToolBar->addAction(_fitAction);

    // Delete Action
    _deleteAction  = new QAction(QIcon("../LEGO_CREATOR/IMG/icons/deleteLego.png"), "", this);
    // Add tool tip to explain
    _deleteAction->setToolTip("Delete the current brick");
    // Connect
    connect(_deleteAction, SIGNAL(triggered()), this, SLOT(deleteLego()));
    // Add to tool bar
    _moveToolBar->addAction(_deleteAction);

    // This tool bar cannot be moved, resized and so on
    _moveToolBar->setMovable(false);

    // At the beginning, there is no LEGO, so fit actions are disabled
    _moveToolBar->setEnabled(false);
}

void MainWindow::createUndoView(void) {
    // Create stack window
    _undoView = new QUndoView(_undoStack);

    // Set object name to style it
    _undoView->setObjectName("UndoView");
}

// ////////////////////////////////////
// STYLE SHEETS
// ////////////////////////////////////
void MainWindow::setStyle(void) {
    QString centralWidgetStyle = "";
    centralWidgetStyle += "#CentralWidget {";
    centralWidgetStyle += "     background-color: #fff;";
    centralWidgetStyle += "}";

    QString moveToolBarStyle = "";
    moveToolBarStyle += "#MoveToolBar {";
    moveToolBarStyle += "   spacing: 10px;";
    moveToolBarStyle += "}";

    QString comboBoxStyle = "";
    comboBoxStyle += "QComboBox {";
    comboBoxStyle += "  background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,";
    comboBoxStyle += "                stop: 0 #df0, stop: 0.4 #ac0,";
    comboBoxStyle += "                stop: 0.5 #9b0, stop: 1.0 #ac0);";
    comboBoxStyle += "  border: 1px solid #ac0";
    comboBoxStyle += "}";
    comboBoxStyle += "QComboBox::drop-down {";
    comboBoxStyle += "  border: 1px solid #ac0";
    comboBoxStyle += "}";

    QString dockButtonStyle = "";
    dockButtonStyle += "#DockWidgetButton {";
    dockButtonStyle += "    background-color: #fff;";
    dockButtonStyle += "    border: 1px solid #ac0;";
    dockButtonStyle += "    border-radius: 3px;";
    dockButtonStyle += "}";

    QString dockWidgetStyle = "";
    dockWidgetStyle += "QDockWidget { ";
    dockWidgetStyle += "    font-size: 10pt;";
    dockWidgetStyle += "    font-weight: bold;";
    dockWidgetStyle += "    font-style: italic;";
    dockWidgetStyle += "    font-family: 'KG Lego House';";
    dockWidgetStyle += "    border: solid 3px black;";
    dockWidgetStyle += "    color: #000;";
    dockWidgetStyle += "    background-color: #fff;";
    dockWidgetStyle += "    border: 2px solid #ac0;";
    dockWidgetStyle += "}";
    dockWidgetStyle += "QDockWidget::title {";
    dockWidgetStyle += "    text-align: center;";
    dockWidgetStyle += "    padding: 10px;";
    dockWidgetStyle += "    background-color: #ac0;";
    dockWidgetStyle += "}";
    dockWidgetStyle += "QDockWidget {";
    dockWidgetStyle += "    titlebar-close-icon: url(\"\");";
    dockWidgetStyle += "    titlebar-normal-icon: url(\"\");";
    dockWidgetStyle += "}";
    dockWidgetStyle += "QDockWidget::close-button, QDockWidget::float-button {";
    dockWidgetStyle += "    padding: 0px;";
    dockWidgetStyle += "}";
    dockWidgetStyle += "QDockWidget::close-button:pressed, QDockWidget::float-button:pressed {";
    dockWidgetStyle += "    padding: 1px -1px -1px 1px;";
    dockWidgetStyle += "}";

    QString dockTabWidgetStyle = "";
    dockTabWidgetStyle += "#ParamsWidget {";
    dockTabWidgetStyle += "     background-color: #efa;";
    dockTabWidgetStyle += "}";

    QString tabBarStyle = "";
    tabBarStyle += "QTabBar::tab {";
    tabBarStyle += "    padding: 5px;";
    tabBarStyle += "    width: 121px;";
    tabBarStyle += "    border-top-left-radius: 4px;";
    tabBarStyle += "    border-top-right-radius: 4px;";
    tabBarStyle += "}";
    tabBarStyle += "QTabBar::tab:selected {";
    tabBarStyle += "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,";
    tabBarStyle += "                stop: 0 #df0, stop: 0.4 #ac0,";
    tabBarStyle += "                stop: 0.5 #9b0, stop: 1.0 #ac0);";
    tabBarStyle += "    border: 2px solid #ac0;";
    tabBarStyle += "}";
    tabBarStyle += "QTabBar::tab:!selected {";
    tabBarStyle += "    background-color: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,";
    tabBarStyle += "                stop: 0 #fafafa, stop: 0.4 #f4f4f4,";
    tabBarStyle += "                stop: 0.5 #e7e7e7, stop: 1.0 #fafafa);";
    tabBarStyle += "    border: 2px solid #c4c4c4;";
    tabBarStyle += "}";

    QString style = centralWidgetStyle
                  + moveToolBarStyle
                  + dockButtonStyle
                  + comboBoxStyle
                  + dockWidgetStyle
                  + dockTabWidgetStyle
                  + tabBarStyle;
    setStyleSheet(style);
}
