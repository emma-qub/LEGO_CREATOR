#include "ReverseTileDialog.h"

ReverseTileDialog::ReverseTileDialog(QWidget *parent) :
    LegoDialog(parent) {
}

ReverseTileDialog::ReverseTileDialog(const ReverseTileDialog& reverseTileDialog) :
    LegoDialog(reverseTileDialog) {

    // Reverse tile width
    _widthSpinBox = new QSpinBox(this);
    _widthSpinBox->setRange(2, 3);
    _widthSpinBox->setValue(2);
    _widthSpinBox->setFixedWidth(50);
    QFormLayout* widthLayout = new QFormLayout;
    widthLayout->addRow("Width:", _widthSpinBox);

    // Reverse tile length
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setRange(1, 2);
    _lengthSpinBox->setValue(1);
    _lengthSpinBox->setFixedWidth(50);
    QFormLayout* lengthLayout = new QFormLayout;
    lengthLayout->addRow("Length", _lengthSpinBox);

    // Size layout
    QHBoxLayout* sizeLayout = new QHBoxLayout;
    sizeLayout->addLayout(widthLayout);
    sizeLayout->addLayout(lengthLayout);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(sizeLayout);

    // Connections
    connect(_widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));
    connect(_lengthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));

    setLayout(mainLayout);
}

void ReverseTileDialog::setLego(int) {
    if (ReverseTile* reverseTile = dynamic_cast<ReverseTile*>(_lego)) {
        if (ReverseTileNode* reverseTileNode = dynamic_cast<ReverseTileNode*>(_legoNode)) {
            reverseTile->setWidth(_widthSpinBox->text().toInt());
            reverseTile->setLength(_lengthSpinBox->text().toInt());

            reverseTileNode->createGeode();

            emit changedLego(reverseTileNode);
        } else {
            qDebug() << "Cannot cast in ReverseTileNode* whithin TileDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in ReverseTile* whithin TileDialog::setLego";
    }
}

ReverseTileDialog* ReverseTileDialog::cloning(void) const {
    return new ReverseTileDialog(*this);
}
