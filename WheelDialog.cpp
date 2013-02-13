#include "WheelDialog.h"


WheelDialog::WheelDialog(QWidget* parent) :
    LegoDialog(parent) {
}

WheelDialog::WheelDialog(const WheelDialog& wheelDialog) :
    LegoDialog(wheelDialog) {
}

WheelDialog* WheelDialog::cloning(void) const {
    return new WheelDialog(*this);
}
