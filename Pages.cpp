#include "Pages.h"

ViewerPage::ViewerPage(QWidget* parent) :
    QWidget(parent) {

    // Get width, length and color settings
    QSettings settings(QSettings::UserScope, "Perso", "Lego Creator");

    // Get grid width defined within settings
    int width;
    if (settings.childKeys().contains("ViewerWidth")) {
        width = settings.value("ViewerWidth").toInt();
    } else {
        width = settings.value("DefaultViewerWidth").toInt();
    }

    // Get grid length defined within settings
    int length;
    if (settings.childKeys().contains("ViewerLength")) {
        length = settings.value("ViewerLength").toInt();
    } else {
        length = settings.value("DefaultViewerLength").toInt();
    }

    // Get viewer color defined within settings
    if (settings.childKeys().contains("ViewerColor")) {
        _color = settings.value("ViewerColor").value<QColor>();
    } else {
        _color = settings.value("DefaultViewerColor").value<QColor>();
    }

    // Get whether grid is visible
    bool gridVisible;
    if (settings.childKeys().contains("ViewerGridVisible")) {
        gridVisible = settings.value("ViewerGridVisible").toBool();
    } else {
        gridVisible = settings.value("DefaultViewerGridVisible").toBool();
    }

    // Get previous value, to be undo with cancel button
    _previousWidth = width;
    _previousLength = length;
    _previousColor = _color;
    _previousGridVisible = gridVisible;

    // Width
    _widthSpinBox = new QSpinBox;
    _widthSpinBox->setValue(width);
    _widthSpinBox->setRange(0, 100);
    _widthSpinBox->setFixedWidth(80);

    // Length
    _lengthSpinBox = new QSpinBox;
    _lengthSpinBox->setValue(length);
    _lengthSpinBox->setRange(0, 100);
    _lengthSpinBox->setFixedWidth(80);

    // Size layout
    QFormLayout* gridSizeLayout = new QFormLayout;
    gridSizeLayout->addRow("Grid width:", _widthSpinBox);
    gridSizeLayout->addRow("Grid length:", _lengthSpinBox);

    // Size group box
    QGroupBox* sizeGroupBox = new QGroupBox("Grid size", this);
    sizeGroupBox->setLayout(gridSizeLayout);

    // Color
    _colorLabel = new QLabel;
    _colorLabel->setFixedSize(50, 20);
    QPixmap colorPixmap(50, 20);
    colorPixmap.fill(_color);
    _colorLabel->setPixmap(colorPixmap);
    _colorButton = new QPushButton("Choose", this);
    // Connection
    connect(_colorButton, SIGNAL(clicked()), this, SLOT(browsePalette()));

    // Color layout
    QHBoxLayout* colorLayout = new QHBoxLayout;
    colorLayout->addWidget(_colorLabel);
    colorLayout->addWidget(_colorButton);

    // Color group box
    QGroupBox* colorGroupBox = new QGroupBox("Viewer background color");
    colorGroupBox->setLayout(colorLayout);

    // Grid visible
    _gridVisibleCheckBox = new QCheckBox("Grid visible", this);
    _gridVisibleCheckBox->setChecked(gridVisible);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addWidget(sizeGroupBox);
    mainLayout->addWidget(_gridVisibleCheckBox);
    mainLayout->addWidget(colorGroupBox);

    // Set layout
    setLayout(mainLayout);
}


void ViewerPage::resetColor(void) {
    QPixmap pixmap;
    pixmap.fill(_previousColor);
    _colorLabel->setPixmap(pixmap);
    setColor(_previousColor);
}

void ViewerPage::setColor(const QColor& color) {
    _color = color;

    QPixmap colorPixmap(50, 20);
    colorPixmap.fill(_color);
    _colorLabel->setPixmap(colorPixmap);
}

void ViewerPage::browsePalette(void) {
    _color = QColorDialog::getColor(_color, this);

    QPixmap colorPixmap(50, 20);
    colorPixmap.fill(_color);
    _colorLabel->setPixmap(colorPixmap);
}
