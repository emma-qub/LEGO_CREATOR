#include "TileDialog.h"

TileDialog::TileDialog(QWidget *parent) :
    LegoDialog(parent) {
}

TileDialog::TileDialog(const TileDialog& tileDialog) :
    LegoDialog(tileDialog) {

    // Tile type
    _tileTypeComboBox = new QComboBox(this);
    QStringList tileTypeList;
    tileTypeList << "Classic" << "Roof" << "Big Roof";
    _tileTypeComboBox->addItems(tileTypeList);
    QFormLayout* tileTypeLayout = new QFormLayout;
    tileTypeLayout->addRow("Tile type:", _tileTypeComboBox);

    // Tile width
    _widthSpinBox = new QSpinBox(this);
    _widthSpinBox->setRange(1, 4);
    _widthSpinBox->setValue(3);
    _widthSpinBox->setFixedWidth(50);
    QFormLayout* widthLayout = new QFormLayout;
    widthLayout->addRow("Width:", _widthSpinBox);

    // Tile length
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setRange(1, 6);
    _lengthSpinBox->setValue(2);
    _lengthSpinBox->setFixedWidth(50);
    QFormLayout* lengthLayout = new QFormLayout;
    lengthLayout->addRow("Length", _lengthSpinBox);

    // Size layout
    QHBoxLayout* sizeLayout = new QHBoxLayout;
    sizeLayout->addLayout(widthLayout);
    sizeLayout->addLayout(lengthLayout);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(tileTypeLayout);
    mainLayout->addLayout(sizeLayout);

    // Connections
    connect(_widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));
    connect(_lengthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));
    connect(_tileTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLego(int)));
    connect(_tileTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateMaxWidth(int)));

    setLayout(mainLayout);
}

void TileDialog::reInitComboBox(void) {
    _tileTypeComboBox->setCurrentIndex(0);
}

void TileDialog::setLego(int) {
    if (Tile* tile = dynamic_cast<Tile*>(_lego)) {
        if (TileNode* tileNode = dynamic_cast<TileNode*>(_legoNode)) {
            tile->setWidth(_widthSpinBox->text().toInt());
            tile->setLength(_lengthSpinBox->text().toInt());
            tile->setTileType(_tileTypeComboBox->currentIndex());

            tileNode->createGeode();

            emit changedLego(tileNode);
        } else {
            qDebug() << "Cannot cast in TileNode* whithin TileDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Tile* whithin TileDialog::setLego";
    }
}

void TileDialog::updateMaxWidth(int tileType) {
    if (tileType == Tile::classic) {
        _widthSpinBox->setRange(1, 4);
    } else {
        _widthSpinBox->setRange(2, 2);
    }
}

TileDialog* TileDialog::cloning(void) const {
    return new TileDialog(*this);
}
