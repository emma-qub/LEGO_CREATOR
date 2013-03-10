#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>

#include "ViewerWidget.h"
#include "SettingsDialog.h"
#include "LegoDialog.h"
#include "World.h"

#include "Traffic.h"


class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    virtual ~MainWindow();

    void initFactories(void);
    void initPreview(void);
    void initDialogs(void);

    void setStyle(void);

    void createFileMenu(void);
    void createEditMenu(void);
    void createGenerateMenu(void);
    void createTrafficMenu(void);
    void createHelpMenu(void);
    void createToolBar(void);
    void createUndoView(void);
    void createParamsDock(void);
    void createScene(void);

    void initTraffic(void);
    void removeTraffic(void);
    void createLight(void);
    void removeLight(void);

    void chooseRoad(int i, int j, int width, int length, bool roadTop, bool roadRight);

    void openFromFile(const QString& fileName);
    void writeFile(const QString& fileName);

public slots:
    void browseColor(void);
    void chooseDialog(int dialogIndex);
    void legoUpdated(LegoNode* legoNode);

    void createLego(void);
    void fitLego(void);
    void deleteLego(void);

    void translate(int);
    void rotateLeft(void);
    void rotateRight(void);

    void generateRoad(void);
    void generateHouse(void);
    void generateCity(void);
    void generateFormule1(void);

    void eraseScene(void);
    void newFile(void);
    void openFile(void);
    void saveFile(void);
    void saveAsFile(void);
    void quitSoft(void);

    void openSettings(void);
    void updateWorldGrid(void);
    void viewerColorUpdate(QColor color);
    void setGridVisible(bool b);

    void checkExistence(QString fileName);

    void switchTraffic(bool b);

    void freezeFit(void);
    void freezeCreate(void);

private:
    //QTabWidget* _tabs;
    Traffic* _traffic;

    ViewerWidget* _brickViewer;
    ViewerWidget* _sceneViewer;
    QFrame* _sceneFrame;

    QTabWidget* _paramsTabWidget;
    QWidget* _paramsWidget;
    QDockWidget* _paramsDock;

    QComboBox* _shapeComboBox;
    QPushButton* _colorButton;
    QPushButton* _createButton;

    QSpinBox* _xTransSpinBox;
    QSpinBox* _yTransSpinBox;
    QSpinBox* _zTransSpinBox;
    QAction* _leftRotateAction;
    QAction* _rightRotateAction;
    QAction* _fitAction;
    QAction* _deleteAction;

    QAction* _newAction;
    QAction* _openAction;
    QAction* _saveAction;
    QAction* _saveAsAction;
    QAction* _quitAction;

    QAction* _undoAction;
    QAction* _redoAction;
    QAction* _settingsAction;

    QAction* _generateRoadAction;
    QAction* _generateHouseAction;
    QAction* _generateCityAction;
    QAction* _generateFormule1Action;

    QAction* _viewTrafficAction;

    QAction* _helpAction;
    QAction* _aboutAction;

    QColor _legoColor;

    osg::ref_ptr<osg::Group> _scene;
    osg::ref_ptr<osg::MatrixTransform> _currMatTrans;
    osg::ref_ptr<LegoNode> _currLegoNode;
    osg::ref_ptr<Lego> _currLego;

    QVector<LegoDialog*> _legoDialog;

    World _world;

    QVector<QVector<QVector<bool> > > _roadPath;

    QSettings _settings;
    SettingsDialog* _settingsDialog;

    bool _alreadySaved;
    bool _saved;

    QUndoStack* _undoStack;
    QUndoView* _undoView;

    QToolBar* _moveToolBar;

signals:
    void fileAlreadyExists(bool);
};

#endif // MAINWINDOW_H
