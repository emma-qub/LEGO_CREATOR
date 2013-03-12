#include "CylinderDialog.h"

CylinderDialog::CylinderDialog(QWidget* parent) :
    LegoDialog(parent) {
}

CylinderDialog::CylinderDialog(const CylinderDialog& cylinderDialog) :
    LegoDialog(cylinderDialog) {

    // Cylinder type
    _cylinderTypeComboBox = new QComboBox(this);
    QStringList cylinderTypeList;
    cylinderTypeList << "Big" << "Big plate" << "Big flat" << "Thin" << "Thin plate" << "Thin flat";
    _cylinderTypeComboBox->addItems(cylinderTypeList);
    QFormLayout* cylinderTypeLayout = new QFormLayout;
    cylinderTypeLayout->addRow("Cylinder type:", _cylinderTypeComboBox);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(cylinderTypeLayout);

    // Connections
    connect(_cylinderTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLego(int)));

    // Set layout
    setLayout(mainLayout);
}

void CylinderDialog::reInitComboBox() {
    _cylinderTypeComboBox->setCurrentIndex(0);
}

void CylinderDialog::setLego(int) {
    if (Cylinder* cylinder = dynamic_cast<Cylinder*>(_lego)) {
        if (CylinderNode* cylinderNode = dynamic_cast<CylinderNode*>(_legoNode)) {
            cylinder->setCylinderType(_cylinderTypeComboBox->currentIndex());

            cylinderNode->createGeode();

            emit changedLego(cylinderNode);
        } else {
            qDebug() << "Cannot cast in CylinderNode* CylinderDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Cylinder* CylinderDialog::setLego";
    }
}

CylinderDialog* CylinderDialog::cloning(void) const {
    return new CylinderDialog(*this);
}
