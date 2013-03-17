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

    // Edge length
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setRange(2, 4);
    _lengthSpinBox->setValue(2);
    _lengthSpinBox->setFixedWidth(50);
    QFormLayout* lengthLayout = new QFormLayout;
    lengthLayout->addRow("Length:", _lengthSpinBox);
    _lengthGroupBox = new QGroupBox;
    _lengthGroupBox->setLayout(lengthLayout);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(edgeTypeLayout);
    mainLayout->addWidget(_lengthGroupBox);

    // Connections
    connect(_lengthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));
    connect(_edgeTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLego(int)));
    connect(_edgeTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMaxLength(int)));

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
            edge->setLength(_lengthSpinBox->value());

            edgeNode->createGeode();

            emit changedLego(edgeNode);
        } else {
            qDebug() << "Cannot cast in EdgeNode* EdgeDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Edge* EdgeDialog::setLego";
    }
}

void EdgeDialog::updateMaxLength(int edgeType) {
    // According to brick type, length max is different
    if (edgeType == Edge::corner)
        _lengthGroupBox->setVisible(false);
    else
        _lengthGroupBox->setVisible(true);
}

EdgeDialog* EdgeDialog::cloning(void) const {
    return new EdgeDialog(*this);
}
