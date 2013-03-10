#include <QtGui>

#include <QDebug>

#include "SettingsDialog.h"
#include "Pages.h"

SettingsDialog::SettingsDialog() :
    QDialog() {

    // Menu list
    _contentsWidget = new QListWidget;
    _contentsWidget->setViewMode(QListView::ListMode);
    _contentsWidget->setMaximumWidth(128);

    // Widget stack
    _pagesWidget = new QStackedWidget;
    _pagesWidget->addWidget(new ViewerPage);

    // Buttons
    _okButton = new QPushButton("Ok", this);
    _applyButton = new QPushButton("Apply", this);
    _cancelButton = new QPushButton("Cancel", this);
    _resetButton = new QPushButton("Reset", this);

    // Create menu list
    createMenu();

    // Set current widget to first one
    _contentsWidget->setCurrentRow(0);

    // Connections
    connect(_okButton, SIGNAL(clicked()), this, SLOT(okAction()));
    connect(_applyButton, SIGNAL(clicked()), this, SLOT(applyAction()));
    connect(_cancelButton, SIGNAL(clicked()), this, SLOT(cancelAction()));
    connect(_resetButton, SIGNAL(clicked()), this, SLOT(resetAction()));

    // Menu/Page layout
    QHBoxLayout* manuPageLayout = new QHBoxLayout;
    manuPageLayout->addWidget(_contentsWidget);
    manuPageLayout->addWidget(_pagesWidget);

    // Buttons layout
    QHBoxLayout* buttonsLayout = new QHBoxLayout;
    buttonsLayout->addWidget(_okButton);
    buttonsLayout->addWidget(_applyButton);
    buttonsLayout->addWidget(_cancelButton);
    buttonsLayout->addWidget(_resetButton);
    buttonsLayout->setAlignment(Qt::AlignRight);

    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout;
    mainLayout->addLayout(manuPageLayout);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    // Set window title
    setWindowTitle("Settings");
}

void SettingsDialog::okAction(void) {
    applyAction();
    close();
}

void SettingsDialog::applyAction(void) {
    // If the page is about Viewer settings
    ViewerPage* currPage = dynamic_cast<ViewerPage*>(_pagesWidget->currentWidget());
    if (currPage) {
        // Get settings
        QSettings settings(QSettings::UserScope, "Perso", "Lego Creator");

        // Set settings values
        settings.setValue("ViewerWidth", currPage->getWidth());
        settings.setValue("ViewerLength", currPage->getLength());
        settings.setValue("ViewerColor", currPage->getColor());
        settings.setValue("ViewerGridVisible", currPage->isGridVisible());

        // Get dialog values
        emit gridSizeChanged();
        emit viewerColorChanged(currPage->getColor());
        emit gridVisible(currPage->isGridVisible());
    }
}

void SettingsDialog::cancelAction(void) {
    // If the page is about Viewer settings
    ViewerPage* currPage = dynamic_cast<ViewerPage*>(_pagesWidget->currentWidget());
    if (currPage) {
        // Get settings
        QSettings settings(QSettings::UserScope, "Perso", "Lego Creator");

        // Set settings values
        settings.setValue("ViewerWidth", currPage->getPreviousWidth());
        settings.setValue("ViewerLength", currPage->getPreviousLength());
        settings.setValue("ViewerColor", currPage->getPreviousColor());
        settings.setValue("ViewerGridVisible", currPage->isPreviousGridVisible());

        currPage->resetWidth();
        currPage->resetLength();
        currPage->resetColor();
        currPage->resetGridVisible();

        // Get dialog values
        emit gridSizeChanged();
        emit viewerColorChanged(currPage->getPreviousColor());
        emit gridVisible(currPage->isGridVisible());
    }
    close();
}

void SettingsDialog::resetAction(void) {
    // Get settings
    QSettings settings(QSettings::UserScope, "Perso", "Lego Creator");

    // Set viewer settings values
    ViewerPage* currPage = dynamic_cast<ViewerPage*>(_pagesWidget->currentWidget());
    if (currPage) {
        // Get default values
        int defaultWidth = settings.value("DefaultViewerWidth").toInt();
        int defaultLength = settings.value("DefaultViewerLength").toInt();
        QColor defaultColor = settings.value("DefaultViewerColor").value<QColor>();
        bool defaultGridVisible = settings.value("DefaultViewerGridVisible").toBool();

        // Reset dialog values
        currPage->setWidth(defaultWidth);
        currPage->setLength(defaultLength);
        currPage->setColor(defaultColor);
        currPage->toggleGridVisible(defaultGridVisible);
    }
}

void SettingsDialog::createMenu(void) {
    // Add menus
    QListWidgetItem* viewerMenu = new QListWidgetItem(_contentsWidget);
    viewerMenu->setText("Viewer");
    viewerMenu->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);

    // Connection
    connect(_contentsWidget, SIGNAL(currentItemChanged(QListWidgetItem*,QListWidgetItem*)), this, SLOT(changePage(QListWidgetItem*,QListWidgetItem*)));
}

void SettingsDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous) {
    if (!current)
        current = previous;

    _pagesWidget->setCurrentIndex(_contentsWidget->row(current));
}
