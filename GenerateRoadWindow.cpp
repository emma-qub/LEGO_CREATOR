#include "GenerateRoadWindow.h"

GenerateRoadWindow::GenerateRoadWindow(QWidget *parent) :
    QDialog(parent) {

    // Width
    _widthSpinBox = new QSpinBox(this);
    _widthSpinBox->setValue(2);
    _widthSpinBox->setMinimum(1);

    // Length
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setValue(2);
    _lengthSpinBox->setMinimum(1);

    QFormLayout* dimensionLayout = new QFormLayout;
    dimensionLayout->addRow("Width:", _widthSpinBox);
    dimensionLayout->addRow("Length:", _lengthSpinBox);

    // Buttons
    _okButton = new QPushButton("Ok", this);
    connect(_okButton, SIGNAL(clicked()), this, SLOT(accept()));
    _cancelButton = new QPushButton("Cancel", this);
    connect(_cancelButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(_okButton);
    buttonsLayout->addWidget(_cancelButton);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(dimensionLayout);
    mainLayout->addLayout(buttonsLayout);

    setLayout(mainLayout);
}
