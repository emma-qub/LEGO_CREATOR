#include "MainWindow.h"

MainWindow::MainWindow() {

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
    connect(this, SIGNAL(legoShapeChanged(osg::Node*)), _previewBrick, SLOT(changeScene(osg::Node*)));

    // Change soft title and maximize window
    setWindowTitle("LEGO Creator");
    setWindowState(Qt::WindowMaximized);
}

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

void MainWindow::createParamsDock(void) {
    // ComboBox choose your brick
    _brickComboBox = new QComboBox(this);
    QStringList brickForms;
    brickForms << "-- Choose your LEGO shape --" << "Brick" << "Plate";
    _brickComboBox->addItems(brickForms);

    // Brick Preview
    QFrame* previewFrame = new QFrame(this);
    previewFrame->setFixedSize(250, 250);
    // Create osg viewer widget that displays bricks
    _previewBrick = new ViewerWidget;
    _previewBrick->initView();
    _previewBrick->changeCamera(_previewBrick->createCamera(0, 0, 100, 100));
    _previewBrick->changeScene(osgDB::readNodeFile("../LEGO_Creator/OSG/cow.osg"));
    _previewBrick->initWidget();
    QVBoxLayout* previewLayout = new QVBoxLayout;
    previewLayout->addWidget(_previewBrick);
    previewFrame->setLayout(previewLayout);

    // Brick size SpinBox
    _widthSpinBox = new QSpinBox(this);
    _widthSpinBox->setMinimum(1);
    _widthSpinBox->setMaximum(2);
    QFormLayout* widthLayout = new QFormLayout;
    widthLayout->addRow("Width:", _widthSpinBox);
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setMinimum(1);
    _lengthSpinBox->setMaximum(16);
    QFormLayout* lengthLayout = new QFormLayout;
    lengthLayout->addRow("Length", _lengthSpinBox);

    // Color Button
    _colorButton = new QPushButton("Color", this);
    _colorButton->setFixedWidth(100);
    connect(_colorButton, SIGNAL(clicked()), this, SLOT(browseColor()));

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel("Preview", this));
    mainLayout->addWidget(previewFrame);
    mainLayout->addWidget(_brickComboBox);
    mainLayout->addLayout(widthLayout);
    mainLayout->addLayout(lengthLayout);
    mainLayout->addWidget(_colorButton);

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
    /*QColor colorSelected = */QColorDialog::getColor(Qt::red, this);
}

// Create LEGO brick according to parameters within right dock
void MainWindow::createBrick() const {
    osg::ref_ptr<osg::Node> node;

    emit legoShapeChanged(node.get());
}
