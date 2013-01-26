#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "ViewerWidget.h"
#include "BrickGeode.h"

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow();

    void initPreview(void);

    void createFileMenu(void);
    void createGenerateMenu(void);
    void createHelpMenu(void);
    void createParamsDock(void);

public slots:
    void browseColor(void);
    void createBrick(int);

private:
    QTabWidget* _tabs;

    ViewerWidget* _previewBrick;

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

    QColor _legoColor;

    osg::ref_ptr<osg::Group> _scene;
    osg::ref_ptr<BrickGeode> _brickGeode;
    Brick* _brick;

signals:
    void legoShapeChanged(osg::Geode*);
    void colorChanged(int);
};

#endif // MAINWINDOW_H
