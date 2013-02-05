#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "ViewerWidget.h"
#include "LegoDialog.h"
#include "World.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    virtual ~MainWindow();

    void initFactories(void);
    void initPreview(void);
    void initDialogs(void);

    void setStyle(void) ;

    void createFileMenu(void);
    void createGenerateMenu(void);
    void createHelpMenu(void);
    void createParamsDock(void);
    void createMoveDock(void);
    void createScene(void);

public slots:
    void browseColor(void);
    void chooseDialog(int dialogIndex);
    void legoUpdated(LegoGeode* legoGeode);

    void createLego(void);
    void fitLego(void);

    void translate(int);
    void rotateLeft(void);
    void rotateRight(void);

    void generateRoad(void);

private:
    QTabWidget* _tabs;

    ViewerWidget* _brickViewer;
    ViewerWidget* _sceneViewer;
    QFrame* _sceneFrame;

    QWidget* _paramsWidget;
    QDockWidget* _paramsDock;

    QComboBox* _shapeComboBox;
    QPushButton* _colorButton;
    QPushButton* _createButton;

    QWidget* _moveWidget;
    QDockWidget* _moveDock;

    QSpinBox* _xTransSpinBox;
    QSpinBox* _yTransSpinBox;
    QSpinBox* _zTransSpinBox;
    QPushButton* _leftRotateButton;
    QPushButton* _rightRotateButton;
    QPushButton* _fitButton;
    QPushButton* _deleteButton;

    QAction* _newAction;
    QAction* _openAction;
    QAction* _saveAction;
    QAction* _saveAsAction;
    QAction* _quitAction;

    QAction* _generateRoadAction;
    QAction* _generateBuildingAction;
    QAction* _generateHouseAction;

    QAction* _helpAction;
    QAction* _aboutAction;

    QColor _legoColor;

    osg::ref_ptr<osg::Group> _scene;
    osg::ref_ptr<LegoGeode> _currLegoGeode;
    Lego* _currLego;

    QVector<LegoDialog*> _legoDialog;

    World _world;

};

#endif // MAINWINDOW_H
