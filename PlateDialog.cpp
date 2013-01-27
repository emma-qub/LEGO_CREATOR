#include "PlateDialog.h"

PlateDialog::PlateDialog(QWidget* parent) :
    LegoDialog(parent) {

    // Plate width
    _widthSpinBox = new QSpinBox(this);
    _widthSpinBox->setMinimum(1);
    _widthSpinBox->setMaximum(16);
    _widthSpinBox->setValue(2);
    QFormLayout* widthLayout = new QFormLayout;
    widthLayout->addRow("Width:", _widthSpinBox);

    // Plate length
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setMinimum(1);
    _lengthSpinBox->setMaximum(16);
    _lengthSpinBox->setValue(2);
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

PlateDialog::PlateDialog(const PlateDialog& plateDialog) :
    LegoDialog(plateDialog) {

    // Plate width
    _widthSpinBox = new QSpinBox(this);
    _widthSpinBox->setMinimum(1);
    _widthSpinBox->setMaximum(16);
    _widthSpinBox->setValue(2);
    QFormLayout* widthLayout = new QFormLayout;
    widthLayout->addRow("Width:", _widthSpinBox);

    // Plate length
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setMinimum(1);
    _lengthSpinBox->setMaximum(16);
    _lengthSpinBox->setValue(2);
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

void PlateDialog::setLego(int) {
    if (Plate* plate = dynamic_cast<Plate*>(_lego)) {
        if (PlateGeode* plateGeode = dynamic_cast<PlateGeode*>(_legoGeode)) {
            plate->setWidth(_widthSpinBox->text().toInt());
            plate->setLength(_lengthSpinBox->text().toInt());

            plateGeode->createGeode();

            emit changedLego(plateGeode);
        } else {
            qDebug() << "Cannot cast in PlateGeode* PlateDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Plate* PlateDialog::setLego";
    }
}

PlateDialog* PlateDialog::cloning(void) const {
    return new PlateDialog(*this);
}
