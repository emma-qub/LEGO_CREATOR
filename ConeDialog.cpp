#include "ConeDialog.h"


ConeDialog::ConeDialog(QWidget* parent) :
    LegoDialog(parent) {
}

ConeDialog::ConeDialog(const ConeDialog& coneDialog) :
    LegoDialog(coneDialog) {
}

ConeDialog* ConeDialog::cloning(void) const {
    return new ConeDialog(*this);
}
