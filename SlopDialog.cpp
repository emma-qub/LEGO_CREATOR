#include "SlopDialog.h"

SlopDialog::SlopDialog(QWidget* parent) :
    LegoDialog(parent) {
}

SlopDialog::SlopDialog(const SlopDialog& slopDialog) :
    LegoDialog(slopDialog) {

    // Slop type
    _slopTypeComboBox = new QComboBox(this);
    QStringList slopTypeList;
    slopTypeList << "Simple" << "Renforce";
    _slopTypeComboBox->addItems(slopTypeList);
    QFormLayout* slopTypeLayout = new QFormLayout;
    slopTypeLayout->addRow("Slop type:", _slopTypeComboBox);

    // Slop width
    _widthSpinBox = new QSpinBox(this);
    _widthSpinBox->setMinimum(1);
    _widthSpinBox->setMaximum(3);
    _widthSpinBox->setValue(1);
    QFormLayout* widthLayout = new QFormLayout;
    widthLayout->addRow("Width", _widthSpinBox);

    // Slop length
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setMinimum(1);
    _lengthSpinBox->setMaximum(16);
    _lengthSpinBox->setValue(1);
    QFormLayout* lengthLayout = new QFormLayout;
    lengthLayout->addRow("Length", _lengthSpinBox);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(slopTypeLayout);
    mainLayout->addLayout(widthLayout);
    mainLayout->addLayout(lengthLayout);

    // Connections
    connect(_widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));
    connect(_lengthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));
    connect(_slopTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLego(int)));
//    connect(_slopTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMaxWidth(int)));

    setLayout(mainLayout);
}

void SlopDialog::reInitComboBox(void) {
    _slopTypeComboBox->setCurrentIndex(0);
}

void SlopDialog::setLego(int) {
    if (Slop* slop = dynamic_cast<Slop*>(_lego)) {
        if (SlopGeode* slopGeode = dynamic_cast<SlopGeode*>(_legoGeode)) {
            slop->setLength(_lengthSpinBox->text().toInt());
            slop->setWidth(_widthSpinBox->text().toInt());
            slop->setSlopType(_slopTypeComboBox->currentIndex());

            slopGeode->createGeode();

            emit changedLego(slopGeode);
        } else {
            qDebug() << "Cannot cast in SlopGeode* SlopDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Slop* SlopDialog::setLego";
    }
}


SlopDialog* SlopDialog::cloning(void) const {
    return new SlopDialog(*this);
}
