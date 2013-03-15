#include "DoorDialog.h"


DoorDialog::DoorDialog(QWidget* parent) :
    LegoDialog(parent) {
}

DoorDialog::DoorDialog(const DoorDialog& doorDialog) :
    LegoDialog(doorDialog),
    _doorColor(Qt::white),
    _doorHandleColor(Qt::black) {

    // Door Color Button
    _doorColorButton = new QPushButton("", this);
    _doorColorButton->setIcon(QIcon("../LEGO_CREATOR/IMG/icons/color.png"));
    _doorColorButton->setFixedWidth(40);
    _doorColorButton->setFocusPolicy(Qt::NoFocus);
    _doorColorButton->setObjectName("DockWidgetButton");

    // Door Handle Color Button
    _doorHandleColorButton = new QPushButton("", this);
    _doorHandleColorButton->setIcon(QIcon("../LEGO_CREATOR/IMG/icons/color.png"));
    _doorHandleColorButton->setFixedWidth(40);
    _doorHandleColorButton->setFocusPolicy(Qt::NoFocus);
    _doorHandleColorButton->setObjectName("DockWidgetButton");

    // Main Layout
    QFormLayout* mainLayout = new QFormLayout;
    mainLayout->addRow("Door color:", _doorColorButton);
    mainLayout->addRow("Handle color:", _doorHandleColorButton);

    // Set layout
    setLayout(mainLayout);

    // Connections
    connect(_doorColorButton, SIGNAL(clicked()), this, SLOT(browseDoorColor()));
    connect(_doorHandleColorButton, SIGNAL(clicked()), this, SLOT(browseDoorHandleColor()));
}

void DoorDialog::setLego(int) {
    if (Door* door = dynamic_cast<Door*>(_lego)) {
        if (DoorNode* doorNode = dynamic_cast<DoorNode*>(_legoNode)) {
            door->setDoorColor(_doorColor);
            door->setDoorHandleColor(_doorHandleColor);

            doorNode->createGeode();

            emit changedLego(doorNode);
        } else {
            qDebug() << "Cannot cast in DoorNode* DoorDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Door* DoorDialog::setLego";
    }
}

DoorDialog* DoorDialog::cloning(void) const {
    return new DoorDialog(*this);
}

void DoorDialog::browseDoorColor(void) {
    // Open color dialog
    QColor newDoorColor = QColorDialog::getColor(_doorColor, this);

    // If users did not push cancel button
    if (newDoorColor.isValid()) {
        _doorColor = newDoorColor;
        setLego(0);
    }
}

void DoorDialog::browseDoorHandleColor(void) {
    // Open color dialog
    QColor newDoorHandleColor = QColorDialog::getColor(_doorHandleColor, this);

    // If users did not push cancel button
    if (newDoorHandleColor.isValid()) {
        _doorHandleColor = newDoorHandleColor;
        setLego(0);
    }
}
