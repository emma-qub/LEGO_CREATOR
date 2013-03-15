#include "GridDialog.h"


GridDialog::GridDialog(QWidget* parent) :
    LegoDialog(parent) {
}

GridDialog::GridDialog(const GridDialog& gridDialog) :
    LegoDialog(gridDialog) {
}

GridDialog* GridDialog::cloning(void) const {
    return new GridDialog(*this);
}
