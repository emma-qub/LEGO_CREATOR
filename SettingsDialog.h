#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QStackedWidget>

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    SettingsDialog();

public slots:
    void changePage(QListWidgetItem* current, QListWidgetItem* previous);
    void okAction(void);
    void applyAction(void);
    void cancelAction(void);
    void resetAction(void);

private:
    void createMenu(void);

    QListWidget* _contentsWidget;
    QStackedWidget* _pagesWidget;
    QPushButton* _okButton;
    QPushButton* _applyButton;
    QPushButton* _cancelButton;
    QPushButton* _resetButton;

signals:
    void gridSizeChanged(void);
    void viewerColorChanged(QColor);
    void gridVisible(bool);
};

#endif // SETTINGSDIALOG_H
