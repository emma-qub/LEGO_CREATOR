#include "MainWindow.h"

#include "LegoFactory.h"
#include "BrickDialog.h"
#include "PlateDialog.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _legoColor(Qt::red) {

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

    // Create tabs
    _tabs = new QTabWidget(this);
    _tabs->addTab(new QWidget(this), "Construction");
    _tabs->addTab(new QWidget(this), "Interaction");

    // Set tabs mode
    setCentralWidget(_tabs);

    // Connections
    connect(_shapeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(chooseDialog(int)));
    for (int k = 0; k < _shapeComboBox->count(); k++)
        connect(_legoDialog.at(k), SIGNAL(changedLego(LegoGeode*)), this, SLOT(legoUpdated(LegoGeode*)));

    // Change soft title
    setWindowTitle("LEGO Creator");

    // Maximize window
    setWindowState(Qt::WindowMaximized);
}

void MainWindow::initFactories(void) {
    // Register Brick
    LegoFactory<Brick, QString>::registerLego(QString("Brick"), new Brick);
    // Register BrickGeode
    LegoFactory<BrickGeode, QString>::registerLego(QString("BrickGeode"), new BrickGeode);
    // Register BrickDialog
    LegoFactory<BrickDialog, QString>::registerLego(QString("BrickDialog"), new BrickDialog);

    // Register Plate
    LegoFactory<Plate, QString>::registerLego(QString("Plate"), new Plate);
    // Register PlateGeode
    LegoFactory<PlateGeode, QString>::registerLego(QString("PlateGeode"), new PlateGeode);
    //Register PlateDialog
    LegoFactory<PlateDialog, QString>::registerLego(QString("PlateDialog"), new PlateDialog);

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
    if (BrickDialog* brickDialog = dynamic_cast<BrickDialog*>(LegoFactory<BrickDialog, QString>::create("BrickDialog"))) {
        brickDialog->initLego(_currLego);
        brickDialog->initLegoGeode(_currLegoGeode);
        _legoDialog << brickDialog;
    } else
        qDebug() << "Cannot create brickDialog in MainWindow::initDialogs";

    if (PlateDialog* plateDialog = dynamic_cast<PlateDialog*>(LegoFactory<PlateDialog, QString>::create("PlateDialog")))
        _legoDialog << plateDialog;
    else
        qDebug() << "Cannot create plateDialog in MainWindow::initDialogs";

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
    brickForms << "Brick" << "Plate";
    _shapeComboBox->addItems(brickForms);
    QFormLayout* shapeLayout = new QFormLayout;
    shapeLayout->addRow("LEGO shape:", _shapeComboBox);

    // Brick Preview
    QFrame* previewFrame = new QFrame(this);
    previewFrame->setFixedSize(250, 250);
    // Create osg viewer widget that displays bricks
    _previewBrick = new ViewerWidget;
    _previewBrick->initView();
    _previewBrick->changeCamera(_previewBrick->createCamera(0, 0, 100, 100));
    _previewBrick->changeScene(_scene.get());
    _previewBrick->initWidget();
    QVBoxLayout* previewLayout = new QVBoxLayout;
    previewLayout->addWidget(_previewBrick);
    previewFrame->setLayout(previewLayout);

    // Color Button
    _colorButton = new QPushButton("Color", this);
    _colorButton->setFixedWidth(100);
    connect(_colorButton, SIGNAL(clicked()), this, SLOT(browseColor()));

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel("Preview", this));
    mainLayout->addWidget(previewFrame);
    mainLayout->addLayout(shapeLayout);
    for (int k = 0; k < _legoDialog.size(); k++)
        mainLayout->addWidget(_legoDialog.at(k));
    mainLayout->addWidget(_colorButton);
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

MainWindow::~MainWindow() {
    delete _currLego;
}


// Open the color dialog to change our LEGO color
void MainWindow::browseColor() {
    _legoColor = QColorDialog::getColor(Qt::red, this);
    _currLego->setColor(_legoColor);
    _currLegoGeode->createGeode();
}

void MainWindow::chooseDialog(int dialogIndex) {
    for (int k = 0; k < _legoDialog.size(); k++) {
        if (k == dialogIndex)
            _legoDialog.at(k)->setVisible(true);
        else
            _legoDialog.at(k)->setVisible(false);
    }

    delete _currLego;
    switch (dialogIndex) {
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
    case 1:
        if ((_currLego = dynamic_cast<Plate*>(LegoFactory<Plate, QString>::create("Plate")))) {
            PlateDialog* dialog = static_cast<PlateDialog*>(_legoDialog.at(dialogIndex));
            Plate* lego = static_cast<Plate*>(_currLego);
            lego->setColor(_legoColor);
            lego->setWidth(dialog->getWidth());
            lego->setLength(dialog->getLength());
        } else {
            qDebug() << "Cannot cast in Plate* within MainWindow::chooseDialog";
        }
        if (!(_currLegoGeode = dynamic_cast<PlateGeode*>(LegoFactory<PlateGeode, QString>::create("PlateGeode"))))
            qDebug() << "Cannot cast in PlateGeode* within MainWindow::chooseDialog";
        break;
    }

    _currLegoGeode->setLego(_currLego);
    _currLegoGeode->createGeode();
    _scene->setChild(0, _currLegoGeode.get());

    _legoDialog.at(dialogIndex)->initLego(_currLego);
    _legoDialog.at(dialogIndex)->initLegoGeode(_currLegoGeode.get());
}

void MainWindow::legoUpdated(LegoGeode* legoGeode) {
    _currLegoGeode = legoGeode;
    _currLego = _currLegoGeode->getLego();
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

    // Add separator
    fileMenu->addSeparator();

    // Add Open sub menu
    _openAction = fileMenu->addAction("&Open...");
    _openAction->setShortcut(QKeySequence::Open);

    // Add separator
    fileMenu->addSeparator();

    // Add Save sub menu
    _saveAction = fileMenu->addAction("&Save");
    _saveAction->setShortcut(QKeySequence::Save);

    // Add Save as sub menu
    _saveAsAction = fileMenu->addAction("Save &as...");
    _saveAsAction->setShortcut(QKeySequence::SaveAs);

    // Add separator
    fileMenu->addSeparator();

    // Add Quit sub menu
    _quitAction = fileMenu->addAction("&Quit");
    _quitAction->setShortcut(QKeySequence::Quit);
}

void MainWindow::createGenerateMenu(void) {
    // Create Generate menu
    QMenu* generateMenu = menuBar()->addMenu("&Generate");

    // Add Generate building sub menu
    _generateBuildingAction = generateMenu->addAction("Generate &building...");
    _generateBuildingAction->setShortcut(QKeySequence("CTRL+B"));

    // Add Generate house sub menu
    _generateHouseAction = generateMenu->addAction("Generate &house...");
    _generateHouseAction->setShortcut(QKeySequence("CTRL+H"));
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
