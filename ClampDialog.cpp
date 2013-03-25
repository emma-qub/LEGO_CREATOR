#include "ClampDialog.h"

ClampDialog::ClampDialog(QWidget* parent) :
    LegoDialog(parent) {
}

ClampDialog::ClampDialog(const ClampDialog& clampDialog) :
    LegoDialog(clampDialog) {
    
    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    
    // Connections
    
    // Set layout
    setLayout(mainLayout);
}

ClampDialog* ClampDialog::cloning(void) const {
    return new ClampDialog(*this);
}

