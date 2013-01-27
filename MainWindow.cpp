#include "MainWindow.h"
#include "BrickGeode.h"


MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    _legoColor(Qt::red) {

    // Register in factories
    initFactories();

    // Init preview element
    initPreview();
    initDialogs();

    // Create menus and right dock
    createFileMenu();
    createGenerateMenu();
    createHelpMenu();
    createParamsDock();

    // Create tabs
    _tabs = new QTabWidget(this);
    _tabs->addTab(new QWidget(this), "Construction");
    _tabs->addTab(new QWidget(this), "Interaction");

    // Set tabs mode
    setCentralWidget(_tabs);

    // Connections
    connect(_legoComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(chooseDialog(int)));

    // Change soft title and maximize window
    setWindowTitle("LEGO Creator");
    setWindowState(Qt::WindowMaximized);
}

void MainWindow::initFactories(void) {
    // Register Brick
    LegoFactory<Brick, QString>::registerLego(QString("Brick"), new Brick);

    // Register BrickGeode
    LegoFactory<BrickGeode, QString>::registerLego(QString("BrickGeode"), new BrickGeode);

    // Register BrickDialog
    LegoFactory<BrickDialog, QString>::registerLego(QString("BrickDialog"), new BrickDialog);
}

void MainWindow::initDialogs(void) {
    if (BrickDialog* brickDialog = dynamic_cast<BrickDialog*>(LegoFactory<BrickDialog, QString>::create("BrickDialog")))
        _legoDialog << brickDialog;
    else
        qDebug() << "Cannot create brickDialog in MainWindow::initDialogs";

    for (int k = 0; k < _legoDialog.size(); k++) {
        _legoDialog.at(k)->setVisible(false);
    }
}

void MainWindow::initPreview(void) {
    _scene = new osg::Group;
}



// ////////////////////////////////////
// Create Right Dock
// ////////////////////////////////////
void MainWindow::createParamsDock(void) {
    // ComboBox choose your brick
    _legoComboBox = new QComboBox(this);
    QStringList brickForms;
    brickForms << "-- Choose your LEGO shape --" << "Brick";
    _legoComboBox->addItems(brickForms);

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
    mainLayout->addWidget(_legoComboBox);
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



// Open the color dialog to change our LEGO color
void MainWindow::browseColor() {
    _legoColor = QColorDialog::getColor(Qt::red, this);
    emit colorChanged(0);
}

void MainWindow::chooseDialog(int dialogIndex) {
    for (int k = 0; k < _legoDialog.size(); k++) {
        if (k == dialogIndex-1)
            _legoDialog.at(k)->setVisible(true);
        else
            _legoDialog.at(k)->setVisible(false);
    }
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
