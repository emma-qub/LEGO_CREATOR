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

    // Using pannels
    _useLeftPannel = new QCheckBox("Use left pannel", this);
    _useLeftPannel->setChecked(true);
    _useRightPannel = new QCheckBox("Use right pannel", this);
    _useRightPannel->setChecked(true);
    // Pannel layout
    QVBoxLayout* pannelLayout = new QVBoxLayout;
    pannelLayout->addWidget(_useLeftPannel);
    pannelLayout->addWidget(_useRightPannel);

    // Main Layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(windowTypeLayout);
    mainLayout->addWidget(_useLeftPannel);
    mainLayout->addWidget(_useRightPannel);

    // Connections
    connect(_windowTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setLego(int)));
    connect(_windowTypeComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(pannelVisible(int)));
    connect(_useLeftPannel, SIGNAL(clicked()), this, SLOT(isPannelUsed()));
    connect(_useRightPannel, SIGNAL(clicked()), this, SLOT(isPannelUsed()));

    // Set layout
    setLayout(mainLayout);
}

void WindowDialog::reInitComboBox(void) {
    _windowTypeComboBox->setCurrentIndex(0);
}

void WindowDialog::pannelVisible(int) {
    if (Window* window = dynamic_cast<Window*>(_lego)) {
        _useLeftPannel->setVisible(window->getWindowType() == Window::classic);
        _useRightPannel->setVisible(window->getWindowType() == Window::classic);
    } else {
        qDebug() << "Cannot cast in Window* WindowDialog::pannelVisible";
    }
}

void WindowDialog::isPannelUsed(void) {
    setLego(0);
}

void WindowDialog::setLego(int) {
    if (Window* window = dynamic_cast<Window*>(_lego)) {
        if (WindowNode* windowNode = dynamic_cast<WindowNode*>(_legoNode)) {
            window->setWindowType(_windowTypeComboBox->currentIndex());
            // If window is classic, we may have to create pannels
            if (window->getWindowType() == Window::classic) {
                window->setUseLeftPanel(_useLeftPannel->isChecked());
                window->setUseRightPanel(_useRightPannel->isChecked());
            } else {
                window->setUseLeftPanel(false);
                window->setUseRightPanel(false);
            }

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
