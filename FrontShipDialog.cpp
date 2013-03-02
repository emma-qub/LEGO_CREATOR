#include "FrontShipDialog.h"

#include "DoorDialog.h"



FrontShipDialog::FrontShipDialog(const FrontShipDialog& frontShipDialog) :
    LegoDialog(frontShipDialog) {
}


FrontShipDialog::FrontShipDialog(QWidget *parent) :
    LegoDialog(parent) {
}

FrontShipDialog* FrontShipDialog::cloning(void) const {
    return new FrontShipDialog(*this);
}
