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

void ClampDialog::setLego(int) {
    if (Clamp* clamp = dynamic_cast<Clamp*>(_lego)) {
        if (ClampNode* clampNode = dynamic_cast<ClampNode*>(_legoNode)) {
            
            clampNode->createGeode();
            
            emit changedLego(clampNode);
        } else {
            qDebug() << "Cannot cast in ClampNode* ClampDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Clamp* ClampDialog::setLego";
    }
}

ClampDialog* ClampDialog::cloning(void) const {
    return new ClampDialog(*this);
}

