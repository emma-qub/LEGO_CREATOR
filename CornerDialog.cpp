#include "CornerDialog.h"

CornerDialog::CornerDialog(QWidget* parent) :
    LegoDialog(parent) {
}

CornerDialog::CornerDialog(const CornerDialog& brickDialog) :
    LegoDialog(brickDialog) {

}

void CornerDialog::setLego(int) {
    //if (Corner* brick = dynamic_cast<Corner*>(_lego)) {
        if (CornerGeode* brickGeode = dynamic_cast<CornerGeode*>(_legoGeode)) {

            brickGeode->createGeode();

            emit changedLego(brickGeode);
        } else {
            qDebug() << "Cannot cast in BrickGeode* BrickDialog::setLego";
        }
//    } else {
//        qDebug() << "Cannot cast in Brick* BrickDialog::setLego";
//    }
}

CornerDialog* CornerDialog::cloning(void) const {
    return new CornerDialog(*this);
}
