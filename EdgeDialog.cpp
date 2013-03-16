#include "EdgeDialog.h"

EdgeDialog::EdgeDialog(QWidget* parent) :
    LegoDialog(parent) {
}

EdgeDialog::EdgeDialog(const EdgeDialog& edgeDialog) :
    LegoDialog(edgeDialog) {

    // Edge type
    _edgeTypeComboBox = new QComboBox(this);
    QStringList edgeTypeList;
    edgeTypeList << "Classic" << "Corner";
    _edgeTypeComboBox->addItems(edgeTypeList);
    QFormLayout* edgeTypeLayout = new QFormLayout;
    edgeTypeLayout->addRow("Edge type:", _edgeTypeComboBox);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(edgeTypeLayout);

    // Connections
    connect(_edgeTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLego(int)));

    // Set layout
    setLayout(mainLayout);
}

void EdgeDialog::reInitComboBox() {
    _edgeTypeComboBox->setCurrentIndex(0);
}

void EdgeDialog::setLego(int) {
    if (Edge* edge = dynamic_cast<Edge*>(_lego)) {
        if (EdgeNode* edgeNode = dynamic_cast<EdgeNode*>(_legoNode)) {
            edge->setEdgeType(_edgeTypeComboBox->currentIndex());

            edgeNode->createGeode();

            emit changedLego(edgeNode);
        } else {
            qDebug() << "Cannot cast in EdgeNode* EdgeDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Edge* EdgeDialog::setLego";
    }
}

EdgeDialog* EdgeDialog::cloning(void) const {
    return new EdgeDialog(*this);
}
