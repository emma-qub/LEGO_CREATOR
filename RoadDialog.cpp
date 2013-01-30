#include "RoadDialog.h"

RoadDialog::RoadDialog(QWidget* parent) :
    LegoDialog(parent) {
}

RoadDialog::RoadDialog(const RoadDialog& roadDialog) :
    LegoDialog(roadDialog) {

    // Road type
    _roadTypeComboBox = new QComboBox(this);
    QStringList roadTypeList;
    roadTypeList << "Straight" << "Curve" << "Intersection" << "Cross";
    _roadTypeComboBox->addItems(roadTypeList);
    QFormLayout* roadTypeLayout = new QFormLayout;
    roadTypeLayout->addRow("Road type:", _roadTypeComboBox);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(roadTypeLayout);

    // Connections
    connect(_roadTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLego(int)));

    setLayout(mainLayout);
}

void RoadDialog::reInitComboBox(void) {
    _roadTypeComboBox->setCurrentIndex(0);
}

void RoadDialog::setLego(int) {
    if (Road* road = dynamic_cast<Road*>(_lego)) {
        if (RoadGeode* roadGeode = dynamic_cast<RoadGeode*>(_legoGeode)) {
            road->setRoadType(_roadTypeComboBox->currentIndex());

            roadGeode->createGeode();

            emit changedLego(roadGeode);
        } else {
            qDebug() << "Cannot cast in RoadGeode* RoadDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Road* RoadDialog::setLego";
    }
}

RoadDialog* RoadDialog::cloning(void) const {
    return new RoadDialog(*this);
}
