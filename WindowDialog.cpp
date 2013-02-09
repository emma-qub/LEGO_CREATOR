#include "WindowDialog.h"


WindowDialog::WindowDialog(QWidget* parent) :
    LegoDialog(parent) {
}

WindowDialog::WindowDialog(const WindowDialog& windowDialog) :
    LegoDialog(windowDialog) {
}

WindowDialog* WindowDialog::cloning(void) const {
    return new WindowDialog(*this);
}
