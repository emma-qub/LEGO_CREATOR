#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

    void createFileMenu(void);
    void createGenerateMenu(void);
    void createHelpMenu(void);
    void createParamsDock(void);

public slots:
    void browseColor();

private:
    QTabWidget* _tabs;

    QWidget* _paramsWidget;
    QDockWidget* _paramsDock;

    QComboBox* _brickComboBox;
    QSpinBox* _widthSpinBox;
    QSpinBox* _lengthSpinBox;
    QPushButton* _colorButton;

    QAction* _newAction;
    QAction* _openAction;
    QAction* _saveAction;
    QAction* _saveAsAction;
    QAction* _quitAction;

    QAction* _generateBuildingAction;
    QAction* _generateHouseAction;

    QAction* _helpAction;
    QAction* _aboutAction;

};

#endif // MAINWINDOW_H
