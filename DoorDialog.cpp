#include "DoorDialog.h"


DoorDialog::DoorDialog(QWidget* parent) :
    LegoDialog(parent) {
}

DoorDialog::DoorDialog(const DoorDialog& doorDialog) :
    LegoDialog(doorDialog) {
}

DoorDialog* DoorDialog::cloning(void) const {
    return new DoorDialog(*this);
}
