#include "FromFileDialog.h"


FromFileDialog::FromFileDialog(QWidget* parent) :
    LegoDialog(parent) {
}

FromFileDialog::FromFileDialog(const FromFileDialog& fromFileDialog) :
    LegoDialog(fromFileDialog) {
}

FromFileDialog* FromFileDialog::cloning(void) const {
    return new FromFileDialog(*this);
}
