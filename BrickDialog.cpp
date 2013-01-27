#include "BrickDialog.h"

BrickDialog::BrickDialog(QWidget* parent) :
    LegoDialog(parent) {

    // Brick width
    _widthSpinBox = new QSpinBox(this);
    _widthSpinBox->setMinimum(1);
    _widthSpinBox->setMaximum(2);
    _widthSpinBox->setValue(2);
    QFormLayout* widthLayout = new QFormLayout;
    widthLayout->addRow("Width:", _widthSpinBox);

    // Brick length
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setMinimum(1);
    _lengthSpinBox->setMaximum(16);
    _lengthSpinBox->setValue(4);
    QFormLayout* lengthLayout = new QFormLayout;
    lengthLayout->addRow("Length", _lengthSpinBox);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(widthLayout);
    mainLayout->addLayout(lengthLayout);

    // Connections
    connect(_widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));
    connect(_lengthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));

    setLayout(mainLayout);
}

BrickDialog::BrickDialog(const BrickDialog& brickDialog) :
    LegoDialog(brickDialog) {

    // Brick width
    _widthSpinBox = new QSpinBox(this);
    _widthSpinBox->setMinimum(1);
    _widthSpinBox->setMaximum(2);
    _widthSpinBox->setValue(2);
    QFormLayout* widthLayout = new QFormLayout;
    widthLayout->addRow("Width:", _widthSpinBox);

    // Brick length
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setMinimum(1);
    _lengthSpinBox->setMaximum(16);
    _lengthSpinBox->setValue(4);
    QFormLayout* lengthLayout = new QFormLayout;
    lengthLayout->addRow("Length", _lengthSpinBox);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(widthLayout);
    mainLayout->addLayout(lengthLayout);

    // Connections
    connect(_widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));
    connect(_lengthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));

    setLayout(mainLayout);
}

void BrickDialog::setLego(int) {
    if (Brick* brick = dynamic_cast<Brick*>(_lego)) {
        if (BrickGeode* brickGeode = dynamic_cast<BrickGeode*>(_legoGeode)) {
            brick->setWidth(_widthSpinBox->text().toInt());
            brick->setLength(_lengthSpinBox->text().toInt());

            brickGeode->createGeode();

            emit changedLego(brickGeode);
        } else {
            qDebug() << "Cannot cast in BrickGeode* BrickDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Brick* BrickDialog::setLego";
    }
}

BrickDialog* BrickDialog::cloning(void) const {
    return new BrickDialog(*this);
}
