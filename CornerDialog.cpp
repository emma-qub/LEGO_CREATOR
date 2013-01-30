#include "CornerDialog.h"

CornerDialog::CornerDialog(QWidget* parent) :
    LegoDialog(parent) {
}

CornerDialog::CornerDialog(const CornerDialog& cornerDialog) :
    LegoDialog(cornerDialog) {

    // Corner type
    _cornerTypeComboBox = new QComboBox(this);
    QStringList cornerTypeList;
    cornerTypeList << "Classic" << "Plate";
    _cornerTypeComboBox->addItems(cornerTypeList);
    QFormLayout* cornerTypeLayout = new QFormLayout;
    cornerTypeLayout->addRow("Corner type:", _cornerTypeComboBox);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(cornerTypeLayout);

    // Connections
    connect(_cornerTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLego(int)));

    setLayout(mainLayout);
}

void CornerDialog::reInitComboBox() {
    _cornerTypeComboBox->setCurrentIndex(0);
}

void CornerDialog::setLego(int) {
    if (Corner* corner = dynamic_cast<Corner*>(_lego)) {
        if (CornerGeode* cornerGeode = dynamic_cast<CornerGeode*>(_legoGeode)) {
            corner->setCornerType(_cornerTypeComboBox->currentIndex());

            cornerGeode->createGeode();

            emit changedLego(cornerGeode);
        } else {
            qDebug() << "Cannot cast in CornerGeode* CornerDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Corner* CornerDialog::setLego";
    }
}

CornerDialog* CornerDialog::cloning(void) const {
    return new CornerDialog(*this);
}
