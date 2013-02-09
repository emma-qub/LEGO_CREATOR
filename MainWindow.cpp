#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "MainWindow.h"
#include "GenerateRoadWindow.h"

#include "LegoFactory.h"
#include "BrickDialog.h"
#include "CornerDialog.h"
#include "RoadDialog.h"
#include "SlopDialog.h"
#include "CharacterDialog.h"

#include <QSettings>

#include <osgDB/WriteFile>

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _legoColor(Qt::red),
    _world(),
    _roadPath(),
    _settings(QSettings::UserScope, "Perso", "Lego Creator"),
    _alreadySaved(false),
    _saved(true) {

    // Settings to record save path and other
    _settings.setValue("SavePath", "~/Documents");
    _settings.setValue("OpenPath", "~/Documents");
    _settings.setValue("FileName", "");

    // Register in factories
    initFactories();

    // Init preview element
    initPreview();
    initDialogs();

    // Create menus
    createFileMenu();
    createGenerateMenu();
    createHelpMenu();

    // Create right dock
    createParamsDock();
    createMoveDock();

    // Create scene
    createScene();

    // Create tabs
    _tabs = new QTabWidget(this);
    _tabs->addTab(_sceneFrame, "Construction");
    _tabs->addTab(new QWidget(this), "Interaction");

    // Set tabs mode
    setCentralWidget(_tabs);

    // Connections
    connect(_shapeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(chooseDialog(int)));
    for (int k = 0; k < _shapeComboBox->count(); k++)
        connect(_legoDialog.at(k), SIGNAL(changedLego(LegoGeode*)), this, SLOT(legoUpdated(LegoGeode*)));
    connect(_colorButton, SIGNAL(clicked()), this, SLOT(browseColor()));
    connect(_createButton, SIGNAL(clicked()), this, SLOT(createLego()));

    connect(_fitButton, SIGNAL(clicked()), this, SLOT(fitLego()));
    connect(_xTransSpinBox, SIGNAL(valueChanged(int)), this, SLOT(translate(int)));
    connect(_yTransSpinBox, SIGNAL(valueChanged(int)), this, SLOT(translate(int)));
    connect(_zTransSpinBox, SIGNAL(valueChanged(int)), this, SLOT(translate(int)));
    connect(_leftRotateButton, SIGNAL(clicked()), this, SLOT(rotateLeft()));
    connect(_rightRotateButton, SIGNAL(clicked()), this, SLOT(rotateRight()));

    // Change soft title
    setWindowTitle("LEGO Creator");

    // Maximize window Commented because bugs occured
    //setWindowState(Qt::WindowMaximized);

    // Apply style sheet
    setStyle();
}

MainWindow::~MainWindow() {
    delete _currLego;
}

void MainWindow::initFactories(void) {
    // Register Brick
    LegoFactory<Brick, QString>::registerLego(QString("Brick"), new Brick);
    // Register BrickGeode
    LegoFactory<BrickGeode, QString>::registerLego(QString("BrickGeode"), new BrickGeode);
    // Register BrickDialog
    LegoFactory<BrickDialog, QString>::registerLego(QString("BrickDialog"), new BrickDialog);

    // Register Corner
    LegoFactory<Corner, QString>::registerLego(QString("Corner"), new Corner);
    // Register CornerGeode
    LegoFactory<CornerGeode, QString>::registerLego(QString("CornerGeode"), new CornerGeode);
    // Register CornerDialog
    LegoFactory<CornerDialog, QString>::registerLego(QString("CornerDialog"), new CornerDialog);

    // Register Road
    LegoFactory<Road, QString>::registerLego(QString("Road"), new Road);
    // Register RoadGeode
    LegoFactory<RoadGeode, QString>::registerLego(QString("RoadGeode"), new RoadGeode);
    // Register RoadDialog
    LegoFactory<RoadDialog, QString>::registerLego(QString("RoadDialog"), new RoadDialog);

    // Register Slop
    LegoFactory<Slop, QString>::registerLego(QString("Slop"), new Slop);
    // Register SlopGeode
    LegoFactory<SlopGeode, QString>::registerLego(QString("SlopGeode"), new SlopGeode);
    // Register SlopDialog
    LegoFactory<SlopDialog, QString>::registerLego(QString("SlopDialog"), new SlopDialog);

    // Register Character
    LegoFactory<Character, QString>::registerLego(QString("Character"), new Character);
    // Register CharacterGeode
    LegoFactory<CharacterGeode, QString>::registerLego(QString("CharacterGeode"), new CharacterGeode);
    // Register CharacterDialog
    LegoFactory<CharacterDialog, QString>::registerLego(QString("CharacterDialog"), new CharacterDialog);

    // ENREGISTRER ICI LES AUTRES CLASSES DE PIECE LEGO QUE L'ON CREERA
}

void MainWindow::initPreview(void) {
    _currLego = LegoFactory<Brick, QString>::create("Brick");
    static_cast<Brick*>(_currLego)->setColor(QColor(Qt::red));
    static_cast<Brick*>(_currLego)->setWidth(2);
    static_cast<Brick*>(_currLego)->setLength(4);
    _currLegoGeode = LegoFactory<BrickGeode, QString>::create("BrickGeode");
    _currLegoGeode->setLego(_currLego);
    _currLegoGeode->createGeode();
    _scene = new osg::Group;
    _scene->addChild(_currLegoGeode);
}

void MainWindow::initDialogs(void) {
    // BrickDialog
    if (BrickDialog* brickDialog = dynamic_cast<BrickDialog*>(LegoFactory<BrickDialog, QString>::create("BrickDialog"))) {
        brickDialog->initLego(_currLego);
        brickDialog->initLegoGeode(_currLegoGeode);
        _legoDialog << brickDialog;
    } else
        qDebug() << "Cannot create BrickDialog in MainWindow::initDialogs";

    // CornerDialog
    if (CornerDialog* cornerDialog = dynamic_cast<CornerDialog*>(LegoFactory<CornerDialog, QString>::create("CornerDialog")))
        _legoDialog << cornerDialog;
    else
        qDebug() << "Cannot create CornerDialog in MainWindow::initDialogs";

    // SlopDialog
    if (SlopDialog* slopDialog = dynamic_cast<SlopDialog*>(LegoFactory<SlopDialog, QString>::create("SlopDialog")))
        _legoDialog << slopDialog;
    else
        qDebug() << "Cannot create SlopDialog in MainWindow::initDialogs";

    // RoadDialog
    if (RoadDialog* roadDialog = dynamic_cast<RoadDialog*>(LegoFactory<RoadDialog, QString>::create("RoadDialog")))
        _legoDialog << roadDialog;
    else
        qDebug() << "Cannot create RoadDialog in MainWindow::initDialogs";

    // CharacterDialog
    if (CharacterDialog* characterDialog = dynamic_cast<CharacterDialog*>(LegoFactory<CharacterDialog, QString>::create("CharacterDialog")))
        _legoDialog << characterDialog;
    else
        qDebug() << "Cannot create CharacterDialog in MainWindow::initDialogs";

    for (int k = 1; k < _legoDialog.size(); k++) {
        _legoDialog.at(k)->setVisible(false);
    }
}



// ////////////////////////////////////
// Create Right Dock
// ////////////////////////////////////
void MainWindow::createParamsDock(void) {
    // ComboBox choose your brick
    _shapeComboBox = new QComboBox(this);
    QStringList brickForms;
    brickForms << "Brick" << "Corner" << "Slop" << "Road" << "Character";
    _shapeComboBox->addItems(brickForms);
    QFormLayout* shapeLayout = new QFormLayout;
    shapeLayout->addRow("LEGO shape:", _shapeComboBox);

    // Brick Preview
    QFrame* previewFrame = new QFrame(this);
    previewFrame->setFixedSize(250, 250);
    // Create osg viewer widget that displays bricks
    _brickViewer = new ViewerWidget;
    _brickViewer->initView();
    _brickViewer->changeCamera(_brickViewer->createCamera(osg::Vec4(.1, .1, .1, 1.), 0, 0, 100, 100));
    _brickViewer->changeScene(_scene.get());
    _brickViewer->initWidget();
    QVBoxLayout* previewLayout = new QVBoxLayout;
    previewLayout->addWidget(_brickViewer);
    previewFrame->setLayout(previewLayout);

    // Color Button
    _colorButton = new QPushButton("Color", this);
    _colorButton->setFixedWidth(100);

    // CreateButton
    _createButton = new QPushButton("Create", this);
    _createButton->setFixedWidth(100);


    // Buttons Layout
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(_colorButton);
    buttonsLayout->addWidget(_createButton);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel("Preview", this));
    mainLayout->addWidget(previewFrame);
    mainLayout->addLayout(shapeLayout);
    for (int k = 0; k < _legoDialog.size(); k++)
        mainLayout->addWidget(_legoDialog.at(k));
    mainLayout->addLayout(buttonsLayout);
    mainLayout->setAlignment(Qt::AlignTop);

    // Right Dock's Widget
    _paramsWidget = new QWidget(this);
    _paramsWidget->setLayout(mainLayout);

    // Right Dock
    _paramsDock = new QDockWidget("Create your brick", this);
    _paramsDock->setFixedSize(250, 500);
    addDockWidget(Qt::RightDockWidgetArea, _paramsDock);
    _paramsDock->setWidget(_paramsWidget);
    _paramsDock->setAllowedAreas(Qt::RightDockWidgetArea);
}

void MainWindow::createMoveDock(void) {
    // Translation according to x
    _xTransSpinBox = new QSpinBox;
    _xTransSpinBox->setMinimum(World::minLength);
    _xTransSpinBox->setMaximum(World::maxLength);
    _xTransSpinBox->setValue(0);

    // Translation according to y
    _yTransSpinBox = new QSpinBox;
    _yTransSpinBox->setMinimum(World::minWidth);
    _yTransSpinBox->setMaximum(World::maxWidth);
    _yTransSpinBox->setValue(0);

    // Translation according to z
    _zTransSpinBox = new QSpinBox;
    _zTransSpinBox->setMinimum(World::minHeight);
    _zTransSpinBox->setMaximum(World::maxHeight);
    _zTransSpinBox->setValue(0);

    QFormLayout* transLayout = new QFormLayout;
    transLayout->addRow("According to x:", _xTransSpinBox);
    transLayout->addRow("According to y:", _yTransSpinBox);
    transLayout->addRow("According to z:", _zTransSpinBox);

    QGroupBox* transGroupBox = new QGroupBox("Translation", this);
    transGroupBox->setLayout(transLayout);

    // Left Rotation Button
    _leftRotateButton = new QPushButton("Left", this);
    _leftRotateButton->setFixedWidth(100);

    // Right Rotation Button
    _rightRotateButton = new QPushButton("Right", this);
    _rightRotateButton->setFixedWidth(100);

    QHBoxLayout* rotationLayout = new QHBoxLayout;
    rotationLayout->addWidget(_leftRotateButton);
    rotationLayout->addWidget(_rightRotateButton);

    QGroupBox* rotationGroupBox = new QGroupBox("Rotation", this);
    rotationGroupBox->setLayout(rotationLayout);

    // Fit Button
    _fitButton = new QPushButton("Fit", this);
    _fitButton->setFixedWidth(100);

    // Delete Button
    _deleteButton = new QPushButton("Delete", this);
    _deleteButton->setFixedWidth(100);

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(_fitButton);
    buttonsLayout->addWidget(_deleteButton);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(transGroupBox);
    mainLayout->addWidget(rotationGroupBox);
    mainLayout->addLayout(buttonsLayout);
    mainLayout->setAlignment(Qt::AlignTop);

    // Right Dock's Widget
    _moveWidget = new QWidget(this);
    _moveWidget->setLayout(mainLayout);

    // Right Dock
    _moveDock = new QDockWidget("Move your brick", this);
    //_moveDock->setFixedSize(250, 500);
    addDockWidget(Qt::RightDockWidgetArea, _moveDock);
    _moveDock->setWidget(_moveWidget);
    _moveDock->setAllowedAreas(Qt::RightDockWidgetArea);
    _moveDock->setEnabled(false);
}

void MainWindow::createScene(void) {
    // Scene frame, contains LEGO bricks
    _sceneFrame = new QFrame(this);
    _sceneFrame->setFixedSize(1150, 750);

    // Scene viewer
    _sceneViewer = new ViewerWidget;
    _sceneViewer->initView();
    _sceneViewer->changeCamera(_brickViewer->createCamera(osg::Vec4(77.0/255.0, 188.0/255.0, 233.0/255.0, 1.), 0, 0, 100, 100));
    _sceneViewer->changeScene(_world.getScene().get());
    _sceneViewer->initWidget();

    // Layout
    QVBoxLayout* previewLayout = new QVBoxLayout;
    previewLayout->addWidget(_sceneViewer);
    _sceneFrame->setLayout(previewLayout);
}

// Open the color dialog to change our LEGO color
void MainWindow::browseColor() {
    _legoColor = QColorDialog::getColor(Qt::red, this);
    _currLego->setColor(_legoColor);
    _currLegoGeode->createGeode();
}

void MainWindow::chooseDialog(int dialogIndex) {
    // Dialogs are created, but only the current LEGO dialog is  visible
    for (int k = 0; k < _legoDialog.size(); k++) {
        if (k == dialogIndex)
            _legoDialog.at(k)->setVisible(true);
        else
            _legoDialog.at(k)->setVisible(false);
    }

    delete _currLego;

    // Create dialog, according to
    switch (dialogIndex) {
    // Brick dialog
    case 0:
        if ((_currLego = dynamic_cast<Brick*>(LegoFactory<Brick, QString>::create("Brick")))) {
            BrickDialog* dialog = static_cast<BrickDialog*>(_legoDialog.at(dialogIndex));
            Brick* lego = static_cast<Brick*>(_currLego);
            lego->setColor(_legoColor);
            lego->setWidth(dialog->getWidth());
            lego->setLength(dialog->getLength());
        } else {
            qDebug() << "Cannot cast in Brick* within MainWindow::chooseDialog";
        }
        if (!(_currLegoGeode = dynamic_cast<BrickGeode*>(LegoFactory<BrickGeode, QString>::create("BrickGeode"))))
            qDebug() << "Cannot cast in BrickGeode* within MainWindow::chooseDialog";
        break;
    // Corner dialog
    case 1:
        if ((_currLego = dynamic_cast<Corner*>(LegoFactory<Corner, QString>::create("Corner")))) {
            Corner* lego = static_cast<Corner*>(_currLego);
            lego->setColor(_legoColor);
        } else {
            qDebug() << "Cannot cast in Corner* within MainWindow::chooseDialog";
        }
        if (!(_currLegoGeode = dynamic_cast<CornerGeode*>(LegoFactory<CornerGeode, QString>::create("CornerGeode"))))
            qDebug() << "Cannot cast in CornerGeode* within MainWindow::chooseDialog";
        break;
    // Slop dialog
    case 2:
        if ((_currLego = dynamic_cast<Slop*>(LegoFactory<Slop, QString>::create("Slop")))) {
            Slop* lego = static_cast<Slop*>(_currLego);
            lego->setColor(_legoColor);
        } else {
            qDebug() << "Cannot cast in Slop* within MainWindow::chooseDialog";
        }
        if (!(_currLegoGeode = dynamic_cast<SlopGeode*>(LegoFactory<SlopGeode, QString>::create("SlopGeode"))))
            qDebug() << "Cannot cast in SlopGeode* within MainWindow::chooseDialog";
        break;
    // Road dialog
    case 3:
        if ((_currLego = dynamic_cast<Road*>(LegoFactory<Road, QString>::create("Road")))) {
            Road* lego = static_cast<Road*>(_currLego);
            lego->setColor(QColor(0, 112, 44));
        } else {
            qDebug() << "Cannot cast in Road* within MainWindow::chooseDialog";
        }
        if (!(_currLegoGeode = dynamic_cast<RoadGeode*>(LegoFactory<RoadGeode, QString>::create("RoadGeode"))))
            qDebug() << "Cannot cast in RoadGeode* within MainWindow::chooseDialog";
        break;
    // Character dialog
    case 4:
        if ((_currLego = dynamic_cast<Character*>(LegoFactory<Character, QString>::create("Character")))) {
            Character* lego = static_cast<Character*>(_currLego);
            lego->setColor(QColor(0, 112, 44));
        } else {
            qDebug() << "Cannot cast in Character* within MainWindow::chooseDialog";
        }
        if (!(_currLegoGeode = dynamic_cast<CharacterGeode*>(LegoFactory<CharacterGeode, QString>::create("CharacterGeode"))))
            qDebug() << "Cannot cast in CharacterGeode* within MainWindow::chooseDialog";
        break;
    }

    // Set current objects
    _currLegoGeode->setLego(_currLego);
    _currLegoGeode->createGeode();
    _scene->setChild(0, _currLegoGeode.get());

    // Initialize current objects
    _legoDialog.at(dialogIndex)->initLego(_currLego);
    _legoDialog.at(dialogIndex)->initLegoGeode(_currLegoGeode.get());
    _legoDialog.at(dialogIndex)->reInitComboBox();
}

void MainWindow::legoUpdated(LegoGeode* legoGeode) {
    // Setting new current objects
    _currLegoGeode = legoGeode;
    _currLego = _currLegoGeode->getLego();

    // The file has changed
    _saved = false;
}

void MainWindow::createLego(void) {
    // z spin box is disabled when roads are created, so first, it has to be enabled again
    _zTransSpinBox->setEnabled(true);

    // When a new brick has been created, users have to fit it before being able to create another one
    _paramsDock->setEnabled(false);
    _moveDock->setEnabled(true);

    // Add brick geode in world
    _world.addBrick(_currLegoGeode.get());

    // Reinit dialog, because a new brick is always put at the origin of the scene
    _xTransSpinBox->setValue(0);
    _yTransSpinBox->setValue(0);
    _zTransSpinBox->setValue(0);

    // Check whether the LEGO is a road, and disable z spin box after having set it at minHeight world specification
    if (dynamic_cast<Road*>(_currLego)) {
        _zTransSpinBox->setValue(World::minHeight);
        _zTransSpinBox->setEnabled(false);
    }

    // The scene has changed
    _saved = false;
}

void MainWindow::fitLego(void) {
    // For the moment, users can fit LEGO wherever they want. So it only disables the move GUI and enables the construction GUI again.
    _moveDock->setEnabled(false);
    _paramsDock->setEnabled(true);

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
    // Rotate counter clock wise
    _world.rotation(true);

    // The file has changed
    _saved = false;
}

void MainWindow::rotateRight(void) {
    // Rotate clock wise
    _world.rotation(false);
}

void MainWindow::chooseRoad(int i, int j, int width, int length, bool roadTop, bool roadRight) {
    // Create a road part, according to its top and left neighbour.
    // The algorithm could (might?) be improved

    // The new road
    Road* road = new Road;

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
    RoadGeode* roadGeode = new RoadGeode(road);

    // Add brick to the world
    _world.addBrick(roadGeode);
    // Translate the
    _world.translation(-32*floor(length/2)-16 + 32*i, -32*floor(width/2)+16 + 32*j, -10);
    for (int k = 0; k < nbRotations; k++)     {
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

void MainWindow::eraseScene(void) {
    // remove everything from scene
    _scene->removeChildren(0, _scene->getNumChildren());
    _saved = true;
    _alreadySaved = false;
}

void MainWindow::writeFile(const QString& fileName) {
    // Try to write the scene in fileName file
    if (osgDB::writeNodeFile(*(_scene.get()), fileName.toStdString())) {
        QMessageBox::information(this, "The document has been saved", "Your construction is safe!");
        _saved = true;
        _alreadySaved = true;
    // Fail! Users can retry or not
    } else {
        int ret = QMessageBox::critical(this, "The document has not been saved", "An error occured while tempting to save your construction. =(\nPlease retry (crossing fingers), or abort if you want to give up.", QMessageBox::Retry | QMessageBox::Abort, QMessageBox::Retry);
        // If users are fighters
        if (ret == QMessageBox::Retry)
            writeFile(fileName);
        // If users are loosers
        _saved = false;
        _alreadySaved = false;
    }
}

void MainWindow::newFile(void) {
    // File modified?
    if (!_saved) {
        int ret = QMessageBox::warning(this, "The document has been modified", "Do you want to save your changes?", QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);

        switch (ret) {
        // Users want to save
        case 0:
            saveFile();
            eraseScene();
            break;
        // Users don't give a ****
        case 1:
            eraseScene();
            break;
        // Users change their mind
        case 2:
            break;
        }
    } else {
        // File saved? let's go!
        eraseScene();
    }
}

void MainWindow::openFile(void) {
    // Get open path
    QString openPath = _settings.value("SavePath").toString();

    // Open dialog because users know what they want to open
    QString fileName = QFileDialog::getOpenFileName(this, "Open object", openPath);

    // Future node, or not
    osg::ref_ptr<osg::Node> newLego = NULL;

    // If users didn't cancel
    if (fileName != "") {
        // Read file to create a node
        newLego = osgDB::readNodeFile(fileName.toStdString());
        // If the new Lego has been correctly got
        if (newLego) {
            _scene->addChild(newLego);
            _saved = false;
        // Else, users have no luck, but they can retry!
        } else {
            int ret = QMessageBox::critical(this, "Your file could not have been read", "An error occured while tempting to open your file. =(\nPlease retry (crossing fingers), or abort if you want to give up.", QMessageBox::Retry | QMessageBox::Abort, QMessageBox::Retry);
            // If users are fighters
            if (ret == QMessageBox::Retry)
                openFile();
        }
    }
}

void MainWindow::saveFile(void) {
    // First time users save?
    if (!_alreadySaved)
        saveAsFile();
    // File modified?
    else if (!_saved) {
        // Get the current file name, recorded whithin QSettings
        QString fileName = _settings.value("FileName").toString();

        // Time to really save the osg file
        writeFile(fileName);
    }
}

void MainWindow::saveAsFile(void) {
    // Get the save path, according to last users directory visit
    QString savePath = _settings.value("SavePath").toString();

    // Pop up to ask user where they want to save their file
    QString fileName = QFileDialog::getSaveFileName(this, "Save your construction", savePath+"/untitled.osg", "OSG Files (*.osg)");

    // If users have not canceled
    if (fileName != "") {
        // Get the directory in order to record it and open it directly next time users want to save
        _settings.setValue("SavePath", QDir(fileName).absolutePath());
        // If users missed to specify .osg at the end, we do for them because we are so nice!
        if (fileName.split(".").last() != "osg") {
            fileName.append(".osg");
        }
        _settings.setValue("FileName", fileName);
        // Time to really save the osg file
        writeFile(fileName);
    }
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



// ////////////////////////////////////
// Create menus
// ////////////////////////////////////

void MainWindow::createFileMenu(void) {
    // Create File menu
    QMenu* fileMenu = menuBar()->addMenu("&File");

    // Add New sub menu
    _newAction = fileMenu->addAction("&New...");
    _newAction->setShortcut(QKeySequence::New);
    // Connect action
    connect(_newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    // Add separator
    fileMenu->addSeparator();

    // Add Open sub menu
    _openAction = fileMenu->addAction("&Open...");
    _openAction->setShortcut(QKeySequence::Open);
    // Connect action
    connect(_openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    // Add separator
    fileMenu->addSeparator();

    // Add Save sub menu
    _saveAction = fileMenu->addAction("&Save");
    _saveAction->setShortcut(QKeySequence::Save);
    // Connect action
    connect(_saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    // Add Save as sub menu
    _saveAsAction = fileMenu->addAction("Save &as...");
    _saveAsAction->setShortcut(QKeySequence::SaveAs);
    // Connect action
    connect(_saveAction, SIGNAL(triggered()), this, SLOT(saveAsFile()));

    // Add separator
    fileMenu->addSeparator();

    // Add Quit sub menu
    _quitAction = fileMenu->addAction("&Quit");
    _quitAction->setShortcut(QKeySequence::Quit);
    // Connect action
    connect(_quitAction, SIGNAL(triggered()), this, SLOT(quitSoft()));
}

void MainWindow::createGenerateMenu(void) {
    // Create Generate menu
    QMenu* generateMenu = menuBar()->addMenu("&Generate");

    // Add Generate road sub menu
    _generateRoadAction = generateMenu->addAction("Generate &road...");
    _generateRoadAction->setShortcut(QKeySequence("CTRL+SHIFT+R"));
    // Connect action
    connect(_generateRoadAction, SIGNAL(triggered()), this, SLOT(generateRoad()));

    // Add Generate building sub menu
    _generateBuildingAction = generateMenu->addAction("Generate &building...");
    _generateBuildingAction->setShortcut(QKeySequence("CTRL+SHIFT+B"));

    // Add Generate house sub menu
    _generateHouseAction = generateMenu->addAction("Generate &house...");
    _generateHouseAction->setShortcut(QKeySequence("CTRL+SHIFT+H"));
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



// ////////////////////////////////////
// STYLE SHEETS
// ////////////////////////////////////
void MainWindow::setStyle(void) {
    //_paramsWidget->setStyleSheet("background-image: url(../LEGO_GIT/IMG/tileBrick.png);");
    //_paramsWidget->setStyleSheet("background: yellow;");

    QString dockWidgetStyle = "";
    dockWidgetStyle += "QDockWidget { ";
    dockWidgetStyle += "    font-size: 10pt;";
    dockWidgetStyle += "    font-weight: bold;";
    dockWidgetStyle += "    font-style: italic;";
    dockWidgetStyle += "    font-family: 'KG Lego House';";
    dockWidgetStyle += "    border: solid 3px black;";
    dockWidgetStyle += "    color: #ffffff;";
    dockWidgetStyle += "}";

    QString dockWidgetTitleStyle;
    dockWidgetTitleStyle += "QDockWidget::title {";
    dockWidgetTitleStyle += "    text-align: center;";
    dockWidgetTitleStyle += "    padding: 3px;";
    dockWidgetTitleStyle += "    background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,";
    dockWidgetTitleStyle += "                stop: 0 #6a6a6a, stop: 0.4 #444444,";
    dockWidgetTitleStyle += "                stop: 0.5 #272727, stop: 1.0 #4a4a4a);";
    //dockWidgetTitleStyle += "    background: red;";
    dockWidgetTitleStyle += "}";

    QString dockWidgetButtonsStyle;
    dockWidgetButtonsStyle += "QDockWidget {";
    dockWidgetButtonsStyle += "    titlebar-close-icon: url(\"./icones/closeIcon.png\");";
    dockWidgetButtonsStyle += "    titlebar-normal-icon: url(\"./icones/reduceIcon.png\");";
    dockWidgetButtonsStyle += "}";
    dockWidgetButtonsStyle += "QDockWidget::close-button, QDockWidget::float-button {";
    dockWidgetButtonsStyle += "    padding: 0px;";
    dockWidgetButtonsStyle += "}";
    dockWidgetButtonsStyle += "QDockWidget::close-button:pressed, QDockWidget::float-button:pressed {";
    dockWidgetButtonsStyle += "    padding: 1px -1px -1px 1px;";
    dockWidgetButtonsStyle += "}";

    QString style = dockWidgetStyle + dockWidgetTitleStyle + dockWidgetButtonsStyle;
    setStyleSheet(style);
}
