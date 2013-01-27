#include "BrickDialog.h"

BrickDialog::BrickDialog(QWidget* parent) :
    LegoDialog(parent) {

    // Brick width
    _widthSpinBox = new QSpinBox(this);
    _widthSpinBox->setMinimum(1);
    _widthSpinBox->setMaximum(2);
    QFormLayout* widthLayout = new QFormLayout;
    widthLayout->addRow("Width:", _widthSpinBox);

    // Brick length
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setMinimum(1);
    _lengthSpinBox->setMaximum(16);
    QFormLayout* lengthLayout = new QFormLayout;
    lengthLayout->addRow("Length", _lengthSpinBox);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(widthLayout);
    mainLayout->addLayout(lengthLayout);

    setLayout(mainLayout);
}

BrickDialog::BrickDialog(const BrickDialog& brickDialog) :
    LegoDialog(brickDialog) {

    // Brick width
    _widthSpinBox = new QSpinBox(this);
    _widthSpinBox->setMinimum(1);
    _widthSpinBox->setMaximum(2);
    QFormLayout* widthLayout = new QFormLayout;
    widthLayout->addRow("Width:", _widthSpinBox);

    // Brick length
    _lengthSpinBox = new QSpinBox(this);
    _lengthSpinBox->setMinimum(1);
    _lengthSpinBox->setMaximum(16);
    QFormLayout* lengthLayout = new QFormLayout;
    lengthLayout->addRow("Length", _lengthSpinBox);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(widthLayout);
    mainLayout->addLayout(lengthLayout);

    setLayout(mainLayout);
}

//void BrickDialog::setLego(Lego* lego, LegoGeode* legoGeode) {
//    if (Brick* brick = dynamic_cast<Brick*>(lego)) {
//        if (BrickGeode* brickGeode = dynamic_cast<BrickGeode*>(legoGeode)) {
//            brick->setWidth(_widthSpinBox->text().toInt());
//            brick->setLength(_lengthSpinBox->text().toInt());

//            brickGeode->createGeode();

//            //emit legoChanged(brickGeode);
//        }
//    } else {
//        qDebug() << "erreur in BrickDialog::setLego";
//    }
//}

BrickDialog* BrickDialog::cloning(void) const {
    return new BrickDialog(*this);
}
