#include "WindowDialog.h"

WindowDialog::WindowDialog(QWidget* parent) :
    LegoDialog(parent) {
}

WindowDialog::WindowDialog(const WindowDialog& windowDialog) :
    LegoDialog(windowDialog) {

    // Window type
    _windowTypeComboBox = new QComboBox(this);
    QStringList windowTypeList;
    windowTypeList << "Classic" << "Tiny" << "Bent" << "Thin" << "Big" << "Giant";
    _windowTypeComboBox->addItems(windowTypeList);
    QFormLayout* windowTypeLayout = new QFormLayout;
    windowTypeLayout->addRow("window type:", _windowTypeComboBox);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(windowTypeLayout);

    // Connections
    connect(_windowTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLego(int)));

    // Set layout
    setLayout(mainLayout);
}

void WindowDialog::reInitComboBox(void) {
    _windowTypeComboBox->setCurrentIndex(0);
}

void WindowDialog::setLego(int) {
    if (Window* window = dynamic_cast<Window*>(_lego)) {
        if (WindowNode* windowNode = dynamic_cast<WindowNode*>(_legoNode)) {
            window->setWindowType(_windowTypeComboBox->currentIndex());

            windowNode->createGeode();

            emit changedLego(windowNode);
        } else {
            qDebug() << "Cannot cast in WindowNode* WindowDialog::setLego";
        }
    } else {
        qDebug() << "Cannot cast in Window* WindowDialog::setLego";
    }
}

WindowDialog* WindowDialog::cloning(void) const {
    return new WindowDialog(*this);
}
