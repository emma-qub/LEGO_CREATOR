#include "TileDialog.h"

TileDialog::TileDialog(QWidget *parent) :
    LegoDialog(parent) {
}

TileDialog::TileDialog(const TileDialog& tileDialog) :
    LegoDialog(tileDialog) {

    // Tile type
    _tileTypeComboBox = new QComboBox(this);
    QStringList tileTypeList;
    tileTypeList << "Classic" << "Intern Corner" << "Extern Corner" << "Roof" << "Big Roof";
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
    _lengthSpinBox->setRange(1, 8);
    _lengthSpinBox->setValue(2);
    _lengthSpinBox->setFixedWidth(50);
    QFormLayout* lengthLayout = new QFormLayout;
    lengthLayout->addRow("Length", _lengthSpinBox);

    // Width length layout
    QHBoxLayout* widthLengthLayout = new QHBoxLayout;
    widthLengthLayout->addLayout(widthLayout);
    widthLengthLayout->addLayout(lengthLayout);

    // Width length group box
    _widthLengthGroupBox = new QGroupBox(this);
    _widthLengthGroupBox->setLayout(widthLengthLayout);

    // Tile size layout
    _sizeSpinBox = new QSpinBox(this);
    _sizeSpinBox->setRange(2, 4);
    _sizeSpinBox->setValue(2);
    _sizeSpinBox->setFixedWidth(50);
    QFormLayout* sizeLayout = new QFormLayout;
    sizeLayout->addRow("Size", _sizeSpinBox);

    _sizeGroupBox = new QGroupBox(this);
    _sizeGroupBox->setLayout(sizeLayout);
    _sizeGroupBox->setVisible(false);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(tileTypeLayout);
    mainLayout->addWidget(_widthLengthGroupBox);
    mainLayout->addWidget(_sizeGroupBox);

    // Connections
    connect(_widthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));
    connect(_lengthSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));
    connect(_sizeSpinBox, SIGNAL(valueChanged(int)), this, SLOT(setLego(int)));
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
            tile->setTileType(_tileTypeComboBox->currentIndex());
            if (tile->getTileType() == Tile::cornerInt || tile->getTileType() == Tile::cornerExt) {
                tile->setWidth(_sizeSpinBox->text().toInt());
                tile->setLength(_sizeSpinBox->text().toInt());
            } else {
                tile->setWidth(_widthSpinBox->text().toInt());
                tile->setLength(_lengthSpinBox->text().toInt());
            }

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
    if (tileType == Tile::cornerInt || tileType == Tile::cornerExt) {
        _widthLengthGroupBox->setVisible(false);
        _sizeGroupBox->setVisible(true);
    } else {
        _widthLengthGroupBox->setVisible(true);
        _sizeGroupBox->setVisible(false);
        if (tileType == Tile::classic) {
            _widthSpinBox->setRange(1, 4);
        } else {
            _widthSpinBox->setRange(2, 2);
        }
    }
}

TileDialog* TileDialog::cloning(void) const {
    return new TileDialog(*this);
}
