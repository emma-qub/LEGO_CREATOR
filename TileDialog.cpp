#include "TileDialog.h"

TileDialog::TileDialog(QWidget *parent) :
    LegoDialog(parent) {
}

TileDialog::TileDialog(const TileDialog& tileDialog) :
    LegoDialog(tileDialog) {

    // Tile width
    _widthSpinBox = new QSpinBox(this);
    _widthSpinBox->setMinimum(2);
    _widthSpinBox->setMaximum(4);
    _widthSpinBox->setValue(3);
    QFormLayout* widthLayout = new QFormLayout;
    widthLayout->addRow("Width:", _widthSpinBox);

    // Tile length
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setMinimum(1);
    _lengthSpinBox->setMaximum(6);
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

void TileDialog::setLego(int) {
    if (Tile* tile = dynamic_cast<Tile*>(_lego)) {
        if (TileGeode* tileGeode = dynamic_cast<TileGeode*>(_legoGeode)) {
            tile->setWidth(_widthSpinBox->text().toInt());
            tile->setLength(_lengthSpinBox->text().toInt());

            tileGeode->createGeode();

            emit changedLego(tileGeode);
        } else {
            qDebug() << "Cannot cast in TileGeode* whithin TileDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Tile* whithin TileDialog::setLego";
    }
}

TileDialog* TileDialog::cloning(void) const {
    return new TileDialog(*this);
}
