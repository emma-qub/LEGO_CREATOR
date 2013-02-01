#include "MainWindow.h"

#include "LegoFactory.h"
#include "BrickDialog.h"
#include "CornerDialog.h"
#include "RoadDialog.h"

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

    // Change soft title
    setWindowTitle("LEGO Creator");

    // Maximize window
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

    // RoadDialog
    if (RoadDialog* roadDialog = dynamic_cast<RoadDialog*>(LegoFactory<RoadDialog, QString>::create("RoadDialog")))
        _legoDialog << roadDialog;
    else
        qDebug() << "Cannot create RoadDialog in MainWindow::initDialogs";

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
    brickForms << "Brick" << "Corner" << "Road";
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

void MainWindow::createScene(void) {
    _sceneFrame = new QFrame(this);
    _sceneFrame->setFixedSize(1150, 750);
    _sceneViewer = new ViewerWidget;
    _sceneViewer->initView();
    _sceneViewer->changeCamera(_brickViewer->createCamera(osg::Vec4(77.0/255.0, 188.0/255.0, 233.0/255.0, 1.), 0, 0, 100, 100));
    _sceneViewer->changeScene(_scene.get());
    _sceneViewer->initWidget();
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
        if ((_currLego = dynamic_cast<Corner*>(LegoFactory<Corner, QString>::create("Corner")))) {
            Corner* lego = static_cast<Corner*>(_currLego);
            lego->setColor(_legoColor);
        } else {
            qDebug() << "Cannot cast in Corner* within MainWindow::chooseDialog";
        }
        if (!(_currLegoGeode = dynamic_cast<CornerGeode*>(LegoFactory<CornerGeode, QString>::create("CornerGeode"))))
            qDebug() << "Cannot cast in CornerGeode* within MainWindow::chooseDialog";
        break;
    case 2:
        if ((_currLego = dynamic_cast<Road*>(LegoFactory<Road, QString>::create("Road")))) {
            Road* lego = static_cast<Road*>(_currLego);
            lego->setColor(QColor(0, 112, 44));
        } else {
            qDebug() << "Cannot cast in Road* within MainWindow::chooseDialog";
        }
        if (!(_currLegoGeode = dynamic_cast<RoadGeode*>(LegoFactory<RoadGeode, QString>::create("RoadGeode"))))
            qDebug() << "Cannot cast in RoadGeode* within MainWindow::chooseDialog";
        break;
    }

    _currLegoGeode->setLego(_currLego);
    _currLegoGeode->createGeode();
    _scene->setChild(0, _currLegoGeode.get());

    _legoDialog.at(dialogIndex)->initLego(_currLego);
    _legoDialog.at(dialogIndex)->initLegoGeode(_currLegoGeode.get());
    _legoDialog.at(dialogIndex)->reInitComboBox();

}

void MainWindow::legoUpdated(LegoGeode* legoGeode) {
    _currLegoGeode = legoGeode;
    _currLego = _currLegoGeode->getLego();
}

void MainWindow::createLego(void) {
    _paramsDock->setEnabled(false);
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



/****************
 * STYLE SHEETS *
 ****************/
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
    this->_paramsDock->setStyleSheet(style);
}
