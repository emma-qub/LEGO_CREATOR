#include "MainWindow.h"

MainWindow::MainWindow() {

    createFileMenu();
    createGenerateMenu();
    createHelpMenu();
    createParamsDock();

    _tabs = new QTabWidget(this);
    _tabs->addTab(new QWidget(this), "Construction");
    _tabs->addTab(new QWidget(this), "Interaction");

    setCentralWidget(_tabs);

    setWindowTitle("LEGO Creator");
    setWindowState(Qt::WindowMaximized);
}

void MainWindow::createFileMenu(void) {
    QMenu* fileMenu = menuBar()->addMenu("&File");

    _newAction = fileMenu->addAction("&New...");
    _newAction->setShortcut(QKeySequence::New);

    fileMenu->addSeparator();

    _openAction = fileMenu->addAction("&Open...");
    _openAction->setShortcut(QKeySequence::Open);

    fileMenu->addSeparator();

    _saveAction = fileMenu->addAction("&Save");
    _saveAction->setShortcut(QKeySequence::Save);

    _saveAsAction = fileMenu->addAction("Save &as...");
    _saveAsAction->setShortcut(QKeySequence::SaveAs);

    fileMenu->addSeparator();

    _quitAction = fileMenu->addAction("&Quit");
    _quitAction->setShortcut(QKeySequence::Quit);
}

void MainWindow::createGenerateMenu(void) {
    QMenu* generateMenu = menuBar()->addMenu("&Generate");

    _generateBuildingAction = generateMenu->addAction("Generate &building...");
    _generateBuildingAction->setShortcut(QKeySequence("CTRL+B"));

    _generateHouseAction = generateMenu->addAction("Genera &house...");
    _generateHouseAction->setShortcut(QKeySequence("CTRL+H"));
}

void MainWindow::createHelpMenu(void) {
    QMenu* helpMenu = menuBar()->addMenu("&?");

    _helpAction = helpMenu->addAction("&Help");
    _helpAction->setShortcut(QKeySequence::HelpContents);

    helpMenu->addSeparator();

    _aboutAction = helpMenu->addAction("&About");
    _aboutAction->setShortcut(QKeySequence("Alt+F1"));
}

void MainWindow::createParamsDock(void) {
    _paramsWidget = new QWidget(this);

    _brickComboBox = new QComboBox(this);
    QStringList brickForms;
    brickForms << "-- Choose your LEGO shape --" << "Brick" << "Plate";
    _brickComboBox->addItems(brickForms);

    QTextEdit* preview = new QTextEdit("Image Render", this);
    preview->setFixedHeight(200);

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

    _colorButton = new QPushButton("Color", this);
    _colorButton->setFixedWidth(100);
    connect(_colorButton, SIGNAL(clicked()), this, SLOT(browseColor()));

    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(new QLabel("Preview", this));
    mainLayout->addWidget(preview);
    mainLayout->addWidget(_brickComboBox);
    mainLayout->addLayout(widthLayout);
    mainLayout->addLayout(lengthLayout);
    mainLayout->addWidget(_colorButton);

    _paramsWidget->setLayout(mainLayout);

    _paramsDock = new QDockWidget("Create your brick", this);
    _paramsDock->setFixedSize(250, 500);
    addDockWidget(Qt::RightDockWidgetArea, _paramsDock);
    _paramsDock->setWidget(_paramsWidget);
    _paramsDock->setAllowedAreas(Qt::RightDockWidgetArea);
}

void MainWindow::browseColor() {
    /*QColor colorSelected = */QColorDialog::getColor(Qt::red, this);
}
